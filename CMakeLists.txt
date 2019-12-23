cmake_minimum_required(VERSION 2.8.2)
project(simple_message_quque)


SET(CMAKE_CXX_STANDARD 17)

add_compile_options("-Wall")

include_directories("src")


file(GLOB_RECURSE SRC_FILES "src/*.cpp")

SET(TARGET_NAME "simple_mq_utest")


# gtest
add_executable(${TARGET_NAME} ${SRC_FILES} "test/mq_test.cpp")
target_link_libraries(${TARGET_NAME} gtest)