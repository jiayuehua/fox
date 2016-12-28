#pragma once
#include <mutex>
#include <shared_mutex>
namespace fox {
  enum IsRecursive
  {
    NotRecursive,
    Recursive,
  };
  enum IsTimed
  {
    NotTimed,
    Timed,
  };
  enum IsShared {
    NotShared,
    Shared,
  };
  template<IsRecursive R = NotRecursive, IsTimed T = NotTimed, IsShared S = NotShared>
  class Mutex;

  template<>
  class Mutex<NotRecursive, NotTimed, NotShared> : public std::mutex {
  };

  template<>
  class Mutex<Recursive, NotTimed, NotShared> : public std::recursive_mutex {
  };

  template<>
  class Mutex<NotRecursive, Timed, NotShared> : public std::timed_mutex {
  };
  template<>
  class Mutex<Recursive, Timed, NotShared> : public std::recursive_timed_mutex {
  };
  template<>
  class Mutex<NotRecursive, NotTimed, Shared> : public std::shared_mutex {
  };
  template<>
  class Mutex<NotRecursive, Timed, Shared> : public std::shared_timed_mutex {
  };
}
