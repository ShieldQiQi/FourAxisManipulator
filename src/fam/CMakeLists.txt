cmake_minimum_required(VERSION 2.8.3)
project(fam)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  sensor_msgs
  serial
  message_generation
  robot_state_publisher
  tf
  visualization_msgs
  #~PCL REQUIRED
)

 add_message_files(
   FILES
   Num.msg
 )


 add_service_files(
   FILES
   AddTwoInts.srv
 )

 generate_messages(
   DEPENDENCIES
   std_msgs
   sensor_msgs
   visualization_msgs
 )

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES FourAxis_manipulator
  CATKIN_DEPENDS roscpp rospy std_msgs serial sensor_msgs robot_state_publisher
#  DEPENDS system_lib
)

include_directories(include ${catkin_INCLUDE_DIRS})

# Freetype 文字引擎需要额外添加头文件和动态库目录
include_directories(include /usr/local/include/freetype2)
link_directories(/usr/local/lib/)
#~add_executable(tinyxml src/tinyxml2.cpp)
add_executable(talker src/talker.cpp)
target_link_libraries(talker ${catkin_LIBRARIES})

add_executable(listener src/listener.cpp)
target_link_libraries(listener ${catkin_LIBRARIES})

add_executable(joint_pub src/joint_pub.cpp)
target_link_libraries(joint_pub ${catkin_LIBRARIES})

add_dependencies(talker beginner_tutorials_generate_messages_cpp)

#指定（FreeType）第三方动态链接库需要将链接放在  add_executable 之后
target_link_libraries(talker /usr/local/lib/libfreetype.so)
#~target_link_libraries(talker /usr/local/lib/libtinyxml2.a)
