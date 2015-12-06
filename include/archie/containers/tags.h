#pragma once

namespace archie {
namespace containers {
  struct enable_small_buffer_optimization final {};
  struct disable_small_buffer_optimization final {};
  using enable_sbo = enable_small_buffer_optimization;
  using disable_sbo = disable_small_buffer_optimization;
}
}
