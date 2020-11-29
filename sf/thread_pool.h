#pragma once

#include <jtk/concurrency.h>

#include <atomic>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace jtk
  {

  class thread_pool
    {
    public:
      typedef std::function<void()> t_job_type;

      void _query_loop()
        {
        t_job_type _job;
        while (!_quit)
          {
              {
              std::unique_lock<std::mutex> lock(_queue_mutex);

              _query_cv.wait(lock, [&] {return !_queue.empty() || _quit; });
              if (!_quit)
                {
                _job = _queue.front();
                _queue.pop();
                }
              }
              if (!_quit)
                {
                ++_number_of_jobs_pending;
                _job();
                if (--_number_of_jobs_pending == 0)
                  _completed_all_jobs_cv.notify_one();
                }
          }
        }

    public:
      thread_pool() : _quit(false), _number_of_jobs_pending(0) {}

      ~thread_pool()
        {
        if (!_pool.empty())
          stop();
        }

      size_t size() const
        {
        return _pool.size();
        }

      void init()
        {
        unsigned int number_of_threads = hardware_concurrency()*32;
        _pool.reserve(number_of_threads);
        for (unsigned int i = 0; i < number_of_threads; ++i)
           _pool.push_back(std::thread(&thread_pool::_query_loop, this));
        }

      void stop()
        {
        std::unique_lock<std::mutex> lock(_threadpool_mutex);
        _quit = true;              
        _query_cv.notify_all();
        for (auto& t : _pool)
          t.join();
        _pool.clear();
        }

      /*
      To push an arbitrary function, use
      push(std::bind(my_class::my_method, my_object));
      */
      void push(t_job_type job)
        {
            {
            std::unique_lock<std::mutex> lock(_queue_mutex);
            _queue.push(job);
            }
            _query_cv.notify_one();
        }

      void wait_until_all_jobs_finished()
        {
        std::unique_lock<std::mutex> lock(_completed_all_jobs_mutex);
        _completed_all_jobs_cv.wait(lock);
        }

    private:
      std::vector<std::thread> _pool;
      std::mutex _queue_mutex;
      std::mutex _threadpool_mutex;
      std::mutex _completed_all_jobs_mutex;
      std::condition_variable _query_cv;
      std::condition_variable _completed_all_jobs_cv;
      std::queue<t_job_type> _queue;
      std::atomic<int> _number_of_jobs_pending;
      bool _quit;      
    };



  template <class _Type, class TFunctor>
  void pooled_parallel_for(_Type first, _Type last, TFunctor fun, thread_pool& tp)
    {
    const _Type n_threads = (_Type)tp.size();

    const _Type n = last - first;

    const _Type n_max_tasks_per_thread = (n / n_threads) + (n % n_threads == 0 ? 0 : 1);
    const _Type n_lacking_tasks = n_max_tasks_per_thread * n_threads - n;

    auto inner_loop = [](_Type first, TFunctor fun, const _Type thread_index, const _Type n_threads, const _Type n_max_tasks_per_thread, const _Type n_lacking_tasks)
      {
      const _Type n_lacking_tasks_so_far = n_threads > (thread_index + n_lacking_tasks) ? 0 : (thread_index + n_lacking_tasks) - n_threads;
      const _Type inclusive_start_index = thread_index * n_max_tasks_per_thread - n_lacking_tasks_so_far;
      const _Type exclusive_end_index = inclusive_start_index + n_max_tasks_per_thread - (thread_index + n_lacking_tasks >= n_threads ? 1 : 0);

      for (_Type k = inclusive_start_index; k < exclusive_end_index; ++k)
        {
        fun(k + first);
        }
      };

    for (_Type j = 0; j < n_threads; ++j)
      tp.push(std::bind(inner_loop, first, fun, j, n_threads, n_max_tasks_per_thread, n_lacking_tasks));

    tp.wait_until_all_jobs_finished();
    }
  }