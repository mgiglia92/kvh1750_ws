cmake_minimum_required(VERSION 3.8)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED True)
project(bufftest LANGUAGES C CXX)


add_executable(bufftest src/main.cpp)

add_subdirectory(lib/CppLinuxSerial)
# add_subdirectory(lib/Ring-Buffer)
# add_subdirectory(lib/SerialStateMachine)

add_library(buffer "")
target_sources(buffer PRIVATE "${CMAKE_SOURCE_DIR}/lib/Ring-Buffer/ringbuffer.c")
include_directories("${CMAKE_SOURCE_DIR}/lib/Ring-Buffer")

# include_directories("${CMAKE_SOURCE_DIR}/lib/CppLinuxSerial")
# include_directories("${CMAKE_SOURCE_DIR}/lib/CppLinuxSerial/include")
# include_directories("${CMAKE_SOURCE_DIR}/lib")
# include_directories(bufftest PRIVATE "${CMAKE_SOURCE_DIR}/../include")
# include_directories("/usr/include")
# add_library()
target_link_libraries(bufftest CppLinuxSerial buffer)

# target_link_libraries(bufftest SerialStateMachine)

#target_include_directories(enumtests PUBLIC "${CMAKE_SOURCE_DIR}/src")

# find dependencies