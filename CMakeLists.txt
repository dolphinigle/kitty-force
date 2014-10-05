cmake_minimum_required (VERSION 2.8)

project (Codeforces)

SET(CMAKE_CXX_FLAGS "-std=c++0x -Wall -pedantic")

add_executable(main main.cc)
add_executable(a a.cc)
add_executable(b b.cc)
add_executable(c c.cc)
add_executable(d d.cc)
add_executable(e e.cc)

target_compile_options(main PUBLIC "-D DOLPHINIGLE_ENV")

install(TARGETS main DESTINATION bin)
install(TARGETS a DESTINATION bin)
install(TARGETS b DESTINATION bin)
install(TARGETS c DESTINATION bin)
install(TARGETS d DESTINATION bin)
install(TARGETS e DESTINATION bin)

