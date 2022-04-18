include(FetchContent)

project(m2-batch)
cmake_minimum_required(VERSION 3.11)

FetchContent_Declare(
  thread-pool
  GIT_REPOSITORY https://github.com/bshoshany/thread-pool.git
  GIT_TAG        b6cd773f37b1be7718f771ae7403726a28be5f40
)
FetchContent_Declare(
  storm
  GIT_REPOSITORY https://github.com/ladislav-zezula/StormLib.git
  GIT_TAG        6ce383a2fa1ff18c32b32347a01f36173699da6c
)
FetchContent_MakeAvailable(thread-pool storm)

file(GLOB SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/scripts/*.h)
add_executable(m2-batch README.md m2.h m2.cpp skin.h skin.cpp script.h file.h config.h main.cpp ${SCRIPTS})
target_include_directories(m2-batch PRIVATE ${thread-pool_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_features(m2-batch PRIVATE cxx_std_20)
target_link_libraries(m2-batch storm)