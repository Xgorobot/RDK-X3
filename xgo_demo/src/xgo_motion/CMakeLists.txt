cmake_minimum_required(VERSION 3.5)
project(xgo_motion)

if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 17)
endif()

find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(xgolib REQUIRED)
find_package(xgoscreen REQUIRED)
find_package(Qt5 REQUIRED COMPONENTS Widgets Core Gui)
find_package(ament_index_cpp REQUIRED)


add_executable(dog_show_combined src/dog_show_combined.cpp)
ament_target_dependencies(dog_show_combined rclcpp xgolib xgoscreen ament_index_cpp)
target_link_libraries(dog_show_combined xgolib::XGO xgoscreen::xgoscreen Qt5::Widgets Qt5::Core Qt5::Gui)

target_include_directories(dog_show_combined PRIVATE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

install(TARGETS dog_show_combined
  DESTINATION lib/${PROJECT_NAME})

# 安装资源到 share/xgo_motion 下，供 ament_index_cpp 访问
install(DIRECTORY xgo_motion/expression
  DESTINATION share/${PROJECT_NAME})
install(DIRECTORY xgo_motion/music
  DESTINATION share/${PROJECT_NAME})

ament_package()