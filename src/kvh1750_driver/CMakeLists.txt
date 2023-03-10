cmake_minimum_required(VERSION 3.0.2)
project(kvh1750_driver)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES derder
#  CATKIN_DEPENDS other_catkin_pkg
#  DEPENDS system_lib
)
find_package(catkin REQUIRED COMPONENTS roscpp std_msgs nav_msgs sensor_msgs)
include_directories(${catkin_INCLUDE_DIRS})
add_executable(bufftest src/main.cpp)

add_subdirectory(lib/CppLinuxSerial)
# add_subdirectory(lib/Ring-Buffer)
# add_subdirectory(lib/SerialStateMachine)

add_library(buffer "")
target_sources(buffer PRIVATE "${CMAKE_SOURCE_DIR}/kvh1750_driver/lib/Ring-Buffer/ringbuffer.c")
include_directories("${CMAKE_SOURCE_DIR}/kvh1750_driver/lib/Ring-Buffer")

# include_directories("${CMAKE_SOURCE_DIR}/lib/CppLinuxSerial")
# include_directories("${CMAKE_SOURCE_DIR}/lib/CppLinuxSerial/include")
# include_directories("${CMAKE_SOURCE_DIR}/lib")
# include_directories(bufftest PRIVATE "${CMAKE_SOURCE_DIR}/../include")
# include_directories("/usr/include")
# add_library()
target_link_libraries(bufftest CppLinuxSerial buffer ${catkin_LIBRARIES})

# target_link_libraries(bufftest SerialStateMachine)

#target_include_directories(enumtests PUBLIC "${CMAKE_SOURCE_DIR}/src")
