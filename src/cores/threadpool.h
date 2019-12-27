#pragma once

#include <atomic>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <future>
#include <utility>

template <typename T>
class ThreadSafeQueue
{
public:
	/**
	 * Destructor.
	 */
	~ThreadSafeQueue(void)
	{
		invalidate();
	}

	/**
	 * Attempt to get the first value in the queue.
	 * Returns true if a value was successfully written to the out parameter, false otherwise.
	 */
	bool tryPop(T& out)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		if (m_queue.empty() || !m_valid)
		{
			return false;
		}
		out = std::move(m_queue.front());
		m_queue.pop();
		return true;
	}

	/**
	 * Get the first value in the queue.
	 * Will block until a value is available unless clear is called or the instance is destructed.
	 * Returns true if a value was successfully written to the out parameter, false otherwise.
	 */
	bool waitPop(T& out)
	{
		std::unique_lock<std::mutex> lock{ m_mutex };
		m_condition.wait(lock, [this]()
		{
			return !m_queue.empty() || !m_valid;
		});
		/*
		 * Using the condition in the predicate ensures that spurious wakeups with a valid
		 * but empty queue will not proceed, so only need to check for validity before proceeding.
		 */
		if (!m_valid)
		{
			return false;
		}
		out = std::move(m_queue.front());
		m_queue.pop();
		return true;
	}

	/**
	 * Push a new value onto the queue.
	 */
	void push(T value)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		m_queue.push(std::move(value));
		m_condition.notify_one();
	}

	/**
	 * Check whether or not the queue is empty.
	 */
	bool empty(void) const
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_queue.empty();
	}

	/**
	 * Clear all items from the queue.
	 */
	void clear(void)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		while (!m_queue.empty())
		{
			m_queue.pop();
		}
		m_condition.notify_all();
	}

	/**
	 * Invalidate the queue.
	 * Used to ensure no conditions are being waited on in waitPop when
	 * a thread or the application is trying to exit.
	 * The queue is invalid after calling this method and it is an error
	 * to continue using a queue after this method has been called.
	 */
	void invalidate(void)
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		m_valid = false;
		m_condition.notify_all();
	}

	/**
	 * Returns whether or not this queue is valid.
	 */
	bool isValid(void) const
	{
		std::lock_guard<std::mutex> lock{ m_mutex };
		return m_valid;
	}

private:
	std::atomic_bool m_valid{ true };
	mutable std::mutex m_mutex;
	std::queue<T> m_queue;
	std::condition_variable m_condition;
};

class ThreadPool
{
private:
	class IThreadTask
	{
	public:
		IThreadTask(void) = default;
		virtual ~IThreadTask(void) = default;
		IThreadTask(const IThreadTask& rhs) = delete;
		IThreadTask& operator=(const IThreadTask& rhs) = delete;
		IThreadTask(IThreadTask&& other) = default;
		IThreadTask& operator=(IThreadTask&& other) = default;

		/**
		 * Run the task.
		 */
		virtual void execute() = 0;
	};

	template <typename Func>
	class ThreadTask : public IThreadTask
	{
	public:
		ThreadTask(Func&& func)
			:m_func{ std::move(func) }
		{
		}

		~ThreadTask(void) override = default;
		ThreadTask(const ThreadTask& rhs) = delete;
		ThreadTask& operator=(const ThreadTask& rhs) = delete;
		ThreadTask(ThreadTask&& other) = default;
		ThreadTask& operator=(ThreadTask&& other) = default;

		/**
		 * Run the task.
		 */
		void execute() override
		{
			m_func();
		}

	private:
		Func m_func;
	};

public:
	/**
	 * A wrapper around a std::future that adds the behavior of futures returned from std::async.
	 * Specifically, this object will block and wait for execution to finish before going out of scope.
	 */
	template <typename T>
	class TaskFuture
	{
	public:
		TaskFuture(std::future<T>&& future)
			:m_future{ std::move(future) }
		{
		}

		TaskFuture(const TaskFuture& rhs) = delete;
		TaskFuture& operator=(const TaskFuture& rhs) = delete;
		TaskFuture(TaskFuture&& other) = default;
		TaskFuture& operator=(TaskFuture&& other) = default;
		~TaskFuture(void)
		{
			if (m_future.valid())
			{
				m_future.get();
			}
		}

		auto get(void)
		{
			return m_future.get();
		}


	private:
		std::future<T> m_future;
	};

public:
	/**
	 * Constructor.
	 */
	ThreadPool(void)
		:ThreadPool{ std::max(std::thread::hardware_concurrency(), 2u) - 1u }
	{
		/*
		 * Always create at least one thread.  If hardware_concurrency() returns 0,
		 * subtracting one would turn it to UINT_MAX, so get the maximum of
		 * hardware_concurrency() and 2 before subtracting 1.
		 */
	}

	/**
	 * Constructor.
	 */
	explicit ThreadPool(const std::uint32_t numThreads)
		:m_done{ false },
		m_workQueue{},
		m_threads{}
	{
		try
		{
			for (std::uint32_t i = 0u; i < numThreads; ++i)
			{
				m_threads.emplace_back(&ThreadPool::worker, this);
			}
		}
		catch (...)
		{
			destroy();
			throw;
		}
	}

	/**
	 * Non-copyable.
	 */
	ThreadPool(const ThreadPool& rhs) = delete;

	/**
	 * Non-assignable.
	 */
	ThreadPool& operator=(const ThreadPool& rhs) = delete;

	/**
	 * Destructor.
	 */
	~ThreadPool(void)
	{
		destroy();
	}

	/**
	 * Submit a job to be run by the thread pool.
	 */
	template <typename Func, typename... Args>
	auto submit(Func&& func, Args&&... args)
	{
		auto boundTask = std::bind(std::forward<Func>(func), std::forward<Args>(args)...);
		using ResultType = std::result_of_t<decltype(boundTask)()>;
		using PackagedTask = std::packaged_task<ResultType()>;
		using TaskType = ThreadTask<PackagedTask>;

		PackagedTask task{ std::move(boundTask) };
		TaskFuture<ResultType> result{ task.get_future() };
		m_workQueue.push(std::make_unique<TaskType>(std::move(task)));
		return result;
	}

private:
	/**
	 * Constantly running function each thread uses to acquire work items from the queue.
	 */
	void worker(void)
	{
		while (!m_done)
		{
			std::unique_ptr<IThreadTask> pTask{ nullptr };
			if (m_workQueue.waitPop(pTask))
			{
				pTask->execute();
			}
		}
	}

	/**
	 * Invalidates the queue and joins all running threads.
	 */
	void destroy(void)
	{
		m_done = true;
		m_workQueue.invalidate();
		for (auto& thread : m_threads)
		{
			if (thread.joinable())
			{
				thread.join();
			}
		}
	}

private:
	std::atomic_bool m_done;
	ThreadSafeQueue<std::unique_ptr<IThreadTask>> m_workQueue;
	std::vector<std::thread> m_threads;
};