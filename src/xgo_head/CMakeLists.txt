cmake_minimum_required(VERSION 3.5)
project(xgo_head)


set(CMAKE_PREFIX_PATH /home/sunrise/qtmultimedia-everywhere-src-5.15.3/include/QtMultimedia/;/usr/lib/aarch64-linux-gnu/)  #libQt5Multimedia.so.5.15.3


#include_directories(/home/sunrise/qtmultimedia-everywhere-src-5.15.3/include/)
# include_directories(/home/sunrise/qtmultimedia-everywhere-src-5.15.3/src/)
# include_directories(/home/sunrise/qtmultimedia-everywhere-src-5.15.3/src/QtMultimedia)
#include_directories(/home/sunrise/qtmultimedia-everywhere-src-5.15.3/include/QtMultimedia/)
#include_directories(/home/sunrise/qtmultimedia-everywhere-src-5.15.3/build/qt5_15_3-Release/include/)

set(QT5MULTIMEDIA /usr/lib/aarch64-linux-gnu/libQt5Multimedia.so.5.15.3)

add_library( Qt5Multimedia
             SHARED
             IMPORTED)
set_target_properties( Qt5Multimedia
                       PROPERTIES IMPORTED_LOCATION
                       /usr/lib/aarch64-linux-gnu/libQt5Multimedia.so.5.15.3)



# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

# Note: Compilation flag -fPIC is necessary. Otherwise, you'll receive error "You must build your code with position independent code if Qt was built with -reduce-relocations."
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -fPIC)
endif()

find_package(ament_cmake REQUIRED)

#dltang
find_package(std_msgs REQUIRED)

# 添加需要用到的 ROS2 包
find_package(rclcpp REQUIRED)
# find_package(sensor_msgs REQUIRED)
# find_package(cv_bridge REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(ai_msgs REQUIRED)
find_package(hbm_img_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(audio_msg REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Widgets Multimedia REQUIRED)
find_package(OpenCV REQUIRED)


#find_package(Qt5Multimedia REQUIRED)
# Note that ament_target_dependencies() in algo cannot export Boost::thread to downstream. As a result, qt_example_node, as a downstream of libalgo.so, needs to explicitly find_package() for Boost again so that qt_example_node is able to link to Boost::thread.
find_package(Boost 1.71.0 REQUIRED COMPONENTS thread)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
file(GLOB PRO_FORM_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} ui/*.ui)
file(GLOB PRO_RESOURCES_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} resource/*.qrc)
file(GLOB_RECURSE PRO_INCLUDE_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS include/*.hpp *.h )
file(GLOB_RECURSE PRO_SOURCES_DIR RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} FOLLOW_SYMLINKS src/*.cpp)
#搜索头文件的位置
include_directories(
        /home/sunrise/xgo_demo/src/xgo_head/include/
       ${CMAKE_CURRENT_BINARY_DIR}
)

include_directories(
  ${OpenCV_INCLUDE_DIRS}
)
#qt头文件moc文件resource文件处理
qt5_wrap_ui(QT_UI_HPP ${PRO_FORM_DIR})
qt5_wrap_cpp(QT_MOC_HPP ${PRO_INCLUDE_DIR})
qt5_add_resources(QT_RESOURCES_CPP ${PRO_RESOURCES_DIR})
add_executable(${PROJECT_NAME}
        ${PRO_SOURCES_DIR}
        ${QT_RESOURCES_CPP}
        ${PRO_INCLUDE_DIR}
        ${QT_MOC_HPP}
        ${PRO_FORM_DIR}
)
# Note that ament_target_dependencies() in algo cannot export Boost::thread to downstream. As a result, qt_example_node, as a downstream of libalgo.so, needs to explicitly ament_target_dependencies() for Boost again so that qt_example_node is able to link to Boost::thread.
#ament_target_dependencies(${PROJECT_NAME} rclcpp  Boost) # 添加依赖的 ROS2 库
ament_target_dependencies(${PROJECT_NAME} rclcpp  Boost 
	std_msgs 
	audio_msg 
	geometry_msgs 
	sensor_msgs
        ai_msgs
   	hbm_img_msgs
	) # 添加依赖的 ROS2 库
#ament_target_dependencies(${PROJECT_NAME} rclcpp Boost std_msgs)

target_link_libraries(${PROJECT_NAME}
        Qt5::Widgets
)

target_link_libraries(${PROJECT_NAME}
        Qt5::Widgets
)

target_link_libraries(${PROJECT_NAME}
        Qt5::Multimedia
)
target_link_libraries(${PROJECT_NAME}
        ${OpenCV_LIBS}
)

install(TARGETS ${PROJECT_NAME}
        DESTINATION lib/${PROJECT_NAME}
)

ament_package()
