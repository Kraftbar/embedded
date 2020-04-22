# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "myPackage: 2 messages, 2 services")

set(MSG_I_FLAGS "-ImyPackage:/home/nybo/catkin/src/myPackage/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(myPackage_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv" NAME_WE)
add_custom_target(_myPackage_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "myPackage" "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv" ""
)

get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg" NAME_WE)
add_custom_target(_myPackage_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "myPackage" "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv" NAME_WE)
add_custom_target(_myPackage_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "myPackage" "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv" ""
)

get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/Floats.msg" NAME_WE)
add_custom_target(_myPackage_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "myPackage" "/home/nybo/catkin/src/myPackage/msg/Floats.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(myPackage
  "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/myPackage
)
_generate_msg_cpp(myPackage
  "/home/nybo/catkin/src/myPackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/myPackage
)

### Generating Services
_generate_srv_cpp(myPackage
  "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/myPackage
)
_generate_srv_cpp(myPackage
  "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/myPackage
)

### Generating Module File
_generate_module_cpp(myPackage
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/myPackage
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(myPackage_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(myPackage_generate_messages myPackage_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_cpp _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_cpp _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_cpp _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/Floats.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_cpp _myPackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(myPackage_gencpp)
add_dependencies(myPackage_gencpp myPackage_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS myPackage_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(myPackage
  "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/myPackage
)
_generate_msg_eus(myPackage
  "/home/nybo/catkin/src/myPackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/myPackage
)

### Generating Services
_generate_srv_eus(myPackage
  "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/myPackage
)
_generate_srv_eus(myPackage
  "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/myPackage
)

### Generating Module File
_generate_module_eus(myPackage
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/myPackage
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(myPackage_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(myPackage_generate_messages myPackage_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_eus _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_eus _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_eus _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/Floats.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_eus _myPackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(myPackage_geneus)
add_dependencies(myPackage_geneus myPackage_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS myPackage_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(myPackage
  "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/myPackage
)
_generate_msg_lisp(myPackage
  "/home/nybo/catkin/src/myPackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/myPackage
)

### Generating Services
_generate_srv_lisp(myPackage
  "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/myPackage
)
_generate_srv_lisp(myPackage
  "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/myPackage
)

### Generating Module File
_generate_module_lisp(myPackage
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/myPackage
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(myPackage_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(myPackage_generate_messages myPackage_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_lisp _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_lisp _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_lisp _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/Floats.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_lisp _myPackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(myPackage_genlisp)
add_dependencies(myPackage_genlisp myPackage_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS myPackage_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(myPackage
  "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/myPackage
)
_generate_msg_nodejs(myPackage
  "/home/nybo/catkin/src/myPackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/myPackage
)

### Generating Services
_generate_srv_nodejs(myPackage
  "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/myPackage
)
_generate_srv_nodejs(myPackage
  "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/myPackage
)

### Generating Module File
_generate_module_nodejs(myPackage
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/myPackage
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(myPackage_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(myPackage_generate_messages myPackage_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_nodejs _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_nodejs _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_nodejs _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/Floats.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_nodejs _myPackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(myPackage_gennodejs)
add_dependencies(myPackage_gennodejs myPackage_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS myPackage_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(myPackage
  "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/myPackage
)
_generate_msg_py(myPackage
  "/home/nybo/catkin/src/myPackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/myPackage
)

### Generating Services
_generate_srv_py(myPackage
  "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/myPackage
)
_generate_srv_py(myPackage
  "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/myPackage
)

### Generating Module File
_generate_module_py(myPackage
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/myPackage
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(myPackage_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(myPackage_generate_messages myPackage_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_py _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_py _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(myPackage_generate_messages_py _myPackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/myPackage/msg/Floats.msg" NAME_WE)
add_dependencies(myPackage_generate_messages_py _myPackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(myPackage_genpy)
add_dependencies(myPackage_genpy myPackage_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS myPackage_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/myPackage)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/myPackage
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(myPackage_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/myPackage)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/myPackage
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(myPackage_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/myPackage)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/myPackage
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(myPackage_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/myPackage)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/myPackage
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(myPackage_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/myPackage)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/myPackage\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/myPackage
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(myPackage_generate_messages_py std_msgs_generate_messages_py)
endif()
