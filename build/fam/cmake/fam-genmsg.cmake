# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "fam: 1 messages, 1 services")

set(MSG_I_FLAGS "-Ifam:/home/pi/catkin_qi/src/fam/msg;-Istd_msgs:/opt/ros/kinetic/share/std_msgs/cmake/../msg;-Isensor_msgs:/opt/ros/kinetic/share/sensor_msgs/cmake/../msg;-Igeometry_msgs:/opt/ros/kinetic/share/geometry_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(fam_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv" NAME_WE)
add_custom_target(_fam_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "fam" "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv" ""
)

get_filename_component(_filename "/home/pi/catkin_qi/src/fam/msg/Num.msg" NAME_WE)
add_custom_target(_fam_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "fam" "/home/pi/catkin_qi/src/fam/msg/Num.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(fam
  "/home/pi/catkin_qi/src/fam/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/fam
)

### Generating Services
_generate_srv_cpp(fam
  "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/fam
)

### Generating Module File
_generate_module_cpp(fam
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/fam
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(fam_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(fam_generate_messages fam_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(fam_generate_messages_cpp _fam_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/msg/Num.msg" NAME_WE)
add_dependencies(fam_generate_messages_cpp _fam_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(fam_gencpp)
add_dependencies(fam_gencpp fam_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS fam_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(fam
  "/home/pi/catkin_qi/src/fam/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/fam
)

### Generating Services
_generate_srv_eus(fam
  "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/fam
)

### Generating Module File
_generate_module_eus(fam
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/fam
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(fam_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(fam_generate_messages fam_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(fam_generate_messages_eus _fam_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/msg/Num.msg" NAME_WE)
add_dependencies(fam_generate_messages_eus _fam_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(fam_geneus)
add_dependencies(fam_geneus fam_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS fam_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(fam
  "/home/pi/catkin_qi/src/fam/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/fam
)

### Generating Services
_generate_srv_lisp(fam
  "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/fam
)

### Generating Module File
_generate_module_lisp(fam
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/fam
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(fam_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(fam_generate_messages fam_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(fam_generate_messages_lisp _fam_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/msg/Num.msg" NAME_WE)
add_dependencies(fam_generate_messages_lisp _fam_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(fam_genlisp)
add_dependencies(fam_genlisp fam_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS fam_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(fam
  "/home/pi/catkin_qi/src/fam/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/fam
)

### Generating Services
_generate_srv_nodejs(fam
  "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/fam
)

### Generating Module File
_generate_module_nodejs(fam
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/fam
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(fam_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(fam_generate_messages fam_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(fam_generate_messages_nodejs _fam_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/msg/Num.msg" NAME_WE)
add_dependencies(fam_generate_messages_nodejs _fam_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(fam_gennodejs)
add_dependencies(fam_gennodejs fam_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS fam_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(fam
  "/home/pi/catkin_qi/src/fam/msg/Num.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/fam
)

### Generating Services
_generate_srv_py(fam
  "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/fam
)

### Generating Module File
_generate_module_py(fam
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/fam
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(fam_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(fam_generate_messages fam_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(fam_generate_messages_py _fam_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/pi/catkin_qi/src/fam/msg/Num.msg" NAME_WE)
add_dependencies(fam_generate_messages_py _fam_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(fam_genpy)
add_dependencies(fam_genpy fam_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS fam_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/fam)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/fam
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(fam_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()
if(TARGET sensor_msgs_generate_messages_cpp)
  add_dependencies(fam_generate_messages_cpp sensor_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/fam)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/fam
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(fam_generate_messages_eus std_msgs_generate_messages_eus)
endif()
if(TARGET sensor_msgs_generate_messages_eus)
  add_dependencies(fam_generate_messages_eus sensor_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/fam)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/fam
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(fam_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()
if(TARGET sensor_msgs_generate_messages_lisp)
  add_dependencies(fam_generate_messages_lisp sensor_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/fam)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/fam
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(fam_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()
if(TARGET sensor_msgs_generate_messages_nodejs)
  add_dependencies(fam_generate_messages_nodejs sensor_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/fam)
  install(CODE "execute_process(COMMAND \"/usr/bin/python\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/fam\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/fam
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(fam_generate_messages_py std_msgs_generate_messages_py)
endif()
if(TARGET sensor_msgs_generate_messages_py)
  add_dependencies(fam_generate_messages_py sensor_msgs_generate_messages_py)
endif()
