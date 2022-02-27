# CMakeLists.txt
cmake_minimum_required(VERSION 3.21)
project(main VERSION 0.1.0)

# Dependencies
find_package(SDL2 CONFIG REQUIRED)
find_package(Catch2 CONFIG REQUIRED)

# Main target
add_executable(main)
target_include_directories(main PRIVATE 
  "src"
)
target_sources(main PRIVATE 
  "src/graphics/graphics.cc"
  "src/matrix/matrix.cc"
  "src/mesh/mesh.cc"
  "src/triangle/triangle.cc"
  "src/vector3/vector3.cc"
  "src/main.cc"
)
target_link_libraries(main PRIVATE SDL2::SDL2 SDL2::SDL2main)

# Strict warning level
target_compile_options(main PRIVATE 
  -Wall 
  -Wextra 
  -Wpedantic 
  -Werror
  -Wno-c++98-compat
)

# Tests target
add_executable(tests)
target_sources(tests PRIVATE 
  "src/vector3/vector3.cc"
  "src/vector3/vector3.test.cc"
)
target_link_libraries(tests PRIVATE Catch2::Catch2)

# Add unit tests
include(CTest)
include(Catch)
catch_discover_tests(tests)

# Smoke test
enable_testing()
add_test(NAME "Smoke test" COMMAND main)