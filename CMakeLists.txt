# CMakeLists.txt
cmake_minimum_required(VERSION 3.21)
project(main VERSION 0.1.0)

# Main target
add_executable(main)
target_sources(main PRIVATE "src/main.cc")

# Strict warning level
target_compile_options(main PRIVATE 
  -Wall 
  -Wextra 
  -Wpedantic 
  -Werror
)
