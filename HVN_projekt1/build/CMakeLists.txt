cmake_minimum_required(VERSION 3.18)
project(hvn_projekt1)
include_directories(../inc)
add_executable(run_hvn_projekt1 ../src/gpiod_utils.c ../src/main.c ../src/semaphore.c)
target_compile_options(hvn_projekt1 PRIVATE -Wall -Werror)
target_link_libraries(hvn_projekt1 gpiod pthread)
set_target_properties(hvn_projekt1 PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../)
