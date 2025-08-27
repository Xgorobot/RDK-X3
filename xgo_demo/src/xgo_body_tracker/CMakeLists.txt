cmake_minimum_required(VERSION 3.5)
project(xgo_body_tracker)

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -fPIC)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(ai_msgs REQUIRED)
find_package(xgolib REQUIRED)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(body_tracker_node src/body_tracker_node.cpp)
ament_target_dependencies(body_tracker_node rclcpp ai_msgs xgolib)

target_link_libraries(body_tracker_node xgolib::XGO)

install(TARGETS body_tracker_node
  DESTINATION lib/${PROJECT_NAME}
)

install(DIRECTORY launch/
  DESTINATION share/${PROJECT_NAME}/launch/
  FILES_MATCHING PATTERN "*.launch.py"
)

ament_export_dependencies(rclcpp ai_msgs xgolib)
ament_package()