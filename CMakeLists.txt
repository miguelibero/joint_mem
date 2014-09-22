cmake_minimum_required(VERSION 2.8) 

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )

project(joint_mem)

ADD_DEFINITIONS(-std=c++0x)

set(JOINT_MEM_TESTS_DIR "test/tests")
file(GLOB_RECURSE JOINT_MEM_TESTS
    "${JOINT_MEM_TESTS_DIR}/*.cpp"
)

add_subdirectory("lib/test/gtest")
enable_testing()
include_directories("src" "test")
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ${CURL_INCLUDE_DIR})
add_executable(runUnitTests ${JOINT_MEM_TESTS})
target_link_libraries(runUnitTests gtest gtest_main)
add_test(runUnitTests runUnitTests)
