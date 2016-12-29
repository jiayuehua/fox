#pragma once
#include <thread>
namespace fox {
  enum ThreadWaitWay {
    Join,
    Detach,
  };
  class Thread final {
    ThreadWaitWay way;
    std::thread t1;
  public:

    template <typename... T>
    Thread(ThreadWaitWay w, T&&... var) :way(w), t1(std::forward<T>(var)...)
    {
    }
    template <typename... T>
    Thread(T&&... var) : Thread(ThreadWaitWay::Join, std::forward<T>(var)...)
    {
    }
    Thread(const Thread&) = delete;
    Thread(Thread&& other) = default;
    Thread& operator=(const Thread&) = delete;
    Thread& operator=(Thread&&) = default;
    std::thread::id  get_id()const noexcept
    {
      return t1.get_id();
    }
    void swap(Thread& oth)noexcept
    {
      t1.swap(oth.t1);
    }

    ~Thread()
    {
      if (t1.joinable())
      {
        if (way == ThreadWaitWay::Detach) {
          t1.detach();
        }
        else {
          t1.join();
        }
      }
    }
  };
}
