
cmake_minimum_required(VERSION 3.23)
project(format-like)

enable_testing()
add_subdirectory(lib)

add_executable(
    format-like
    src/main.cpp
    )

target_compile_features(
    format-like
    PRIVATE
    cxx_std_23
    )

find_package(Threads)
target_link_libraries(
    format-like
    PRIVATE
    ${CMAKE_THREAD_LIBS_INIT}
    )

target_include_directories(
    format-like
    PRIVATE
    include
)

add_subdirectory(test)

