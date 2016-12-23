#pragma once
#include <chrono>
namespace std {
  namespace chrono {
    using days = std::chrono::duration<int64_t, 24 * 3600L>;
    using weeks = std::chrono::duration<int64_t, 7* 24 * 3600L>;
  }
}
