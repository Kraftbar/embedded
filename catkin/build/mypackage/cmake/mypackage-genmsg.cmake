# generated from genmsg/cmake/pkg-genmsg.cmake.em

message(STATUS "mypackage: 2 messages, 2 services")

set(MSG_I_FLAGS "-Imypackage:/home/nybo/catkin/src/mypackage/msg;-Istd_msgs:/opt/ros/melodic/share/std_msgs/cmake/../msg")

# Find all generators
find_package(gencpp REQUIRED)
find_package(geneus REQUIRED)
find_package(genlisp REQUIRED)
find_package(gennodejs REQUIRED)
find_package(genpy REQUIRED)

add_custom_target(mypackage_generate_messages ALL)

# verify that message/service dependencies have not changed since configure



get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv" NAME_WE)
add_custom_target(_mypackage_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mypackage" "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv" ""
)

get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv" NAME_WE)
add_custom_target(_mypackage_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mypackage" "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv" ""
)

get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg" NAME_WE)
add_custom_target(_mypackage_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mypackage" "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg" "std_msgs/Header"
)

get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/Floats.msg" NAME_WE)
add_custom_target(_mypackage_generate_messages_check_deps_${_filename}
  COMMAND ${CATKIN_ENV} ${PYTHON_EXECUTABLE} ${GENMSG_CHECK_DEPS_SCRIPT} "mypackage" "/home/nybo/catkin/src/mypackage/msg/Floats.msg" ""
)

#
#  langs = gencpp;geneus;genlisp;gennodejs;genpy
#

### Section generating for lang: gencpp
### Generating Messages
_generate_msg_cpp(mypackage
  "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mypackage
)
_generate_msg_cpp(mypackage
  "/home/nybo/catkin/src/mypackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mypackage
)

### Generating Services
_generate_srv_cpp(mypackage
  "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mypackage
)
_generate_srv_cpp(mypackage
  "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mypackage
)

### Generating Module File
_generate_module_cpp(mypackage
  ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mypackage
  "${ALL_GEN_OUTPUT_FILES_cpp}"
)

add_custom_target(mypackage_generate_messages_cpp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_cpp}
)
add_dependencies(mypackage_generate_messages mypackage_generate_messages_cpp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_cpp _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_cpp _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_cpp _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/Floats.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_cpp _mypackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mypackage_gencpp)
add_dependencies(mypackage_gencpp mypackage_generate_messages_cpp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mypackage_generate_messages_cpp)

### Section generating for lang: geneus
### Generating Messages
_generate_msg_eus(mypackage
  "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mypackage
)
_generate_msg_eus(mypackage
  "/home/nybo/catkin/src/mypackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mypackage
)

### Generating Services
_generate_srv_eus(mypackage
  "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mypackage
)
_generate_srv_eus(mypackage
  "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mypackage
)

### Generating Module File
_generate_module_eus(mypackage
  ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mypackage
  "${ALL_GEN_OUTPUT_FILES_eus}"
)

add_custom_target(mypackage_generate_messages_eus
  DEPENDS ${ALL_GEN_OUTPUT_FILES_eus}
)
add_dependencies(mypackage_generate_messages mypackage_generate_messages_eus)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_eus _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_eus _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_eus _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/Floats.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_eus _mypackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mypackage_geneus)
add_dependencies(mypackage_geneus mypackage_generate_messages_eus)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mypackage_generate_messages_eus)

### Section generating for lang: genlisp
### Generating Messages
_generate_msg_lisp(mypackage
  "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mypackage
)
_generate_msg_lisp(mypackage
  "/home/nybo/catkin/src/mypackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mypackage
)

### Generating Services
_generate_srv_lisp(mypackage
  "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mypackage
)
_generate_srv_lisp(mypackage
  "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mypackage
)

### Generating Module File
_generate_module_lisp(mypackage
  ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mypackage
  "${ALL_GEN_OUTPUT_FILES_lisp}"
)

add_custom_target(mypackage_generate_messages_lisp
  DEPENDS ${ALL_GEN_OUTPUT_FILES_lisp}
)
add_dependencies(mypackage_generate_messages mypackage_generate_messages_lisp)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_lisp _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_lisp _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_lisp _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/Floats.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_lisp _mypackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mypackage_genlisp)
add_dependencies(mypackage_genlisp mypackage_generate_messages_lisp)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mypackage_generate_messages_lisp)

### Section generating for lang: gennodejs
### Generating Messages
_generate_msg_nodejs(mypackage
  "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mypackage
)
_generate_msg_nodejs(mypackage
  "/home/nybo/catkin/src/mypackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mypackage
)

### Generating Services
_generate_srv_nodejs(mypackage
  "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mypackage
)
_generate_srv_nodejs(mypackage
  "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mypackage
)

### Generating Module File
_generate_module_nodejs(mypackage
  ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mypackage
  "${ALL_GEN_OUTPUT_FILES_nodejs}"
)

add_custom_target(mypackage_generate_messages_nodejs
  DEPENDS ${ALL_GEN_OUTPUT_FILES_nodejs}
)
add_dependencies(mypackage_generate_messages mypackage_generate_messages_nodejs)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_nodejs _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_nodejs _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_nodejs _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/Floats.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_nodejs _mypackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mypackage_gennodejs)
add_dependencies(mypackage_gennodejs mypackage_generate_messages_nodejs)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mypackage_generate_messages_nodejs)

### Section generating for lang: genpy
### Generating Messages
_generate_msg_py(mypackage
  "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg"
  "${MSG_I_FLAGS}"
  "/opt/ros/melodic/share/std_msgs/cmake/../msg/Header.msg"
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mypackage
)
_generate_msg_py(mypackage
  "/home/nybo/catkin/src/mypackage/msg/Floats.msg"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mypackage
)

### Generating Services
_generate_srv_py(mypackage
  "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mypackage
)
_generate_srv_py(mypackage
  "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv"
  "${MSG_I_FLAGS}"
  ""
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mypackage
)

### Generating Module File
_generate_module_py(mypackage
  ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mypackage
  "${ALL_GEN_OUTPUT_FILES_py}"
)

add_custom_target(mypackage_generate_messages_py
  DEPENDS ${ALL_GEN_OUTPUT_FILES_py}
)
add_dependencies(mypackage_generate_messages mypackage_generate_messages_py)

# add dependencies to all check dependencies targets
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/AddTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_py _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/srv/BadTwoInts.srv" NAME_WE)
add_dependencies(mypackage_generate_messages_py _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/HeaderString.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_py _mypackage_generate_messages_check_deps_${_filename})
get_filename_component(_filename "/home/nybo/catkin/src/mypackage/msg/Floats.msg" NAME_WE)
add_dependencies(mypackage_generate_messages_py _mypackage_generate_messages_check_deps_${_filename})

# target for backward compatibility
add_custom_target(mypackage_genpy)
add_dependencies(mypackage_genpy mypackage_generate_messages_py)

# register target for catkin_package(EXPORTED_TARGETS)
list(APPEND ${PROJECT_NAME}_EXPORTED_TARGETS mypackage_generate_messages_py)



if(gencpp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mypackage)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gencpp_INSTALL_DIR}/mypackage
    DESTINATION ${gencpp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_cpp)
  add_dependencies(mypackage_generate_messages_cpp std_msgs_generate_messages_cpp)
endif()

if(geneus_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mypackage)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${geneus_INSTALL_DIR}/mypackage
    DESTINATION ${geneus_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_eus)
  add_dependencies(mypackage_generate_messages_eus std_msgs_generate_messages_eus)
endif()

if(genlisp_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mypackage)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genlisp_INSTALL_DIR}/mypackage
    DESTINATION ${genlisp_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_lisp)
  add_dependencies(mypackage_generate_messages_lisp std_msgs_generate_messages_lisp)
endif()

if(gennodejs_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mypackage)
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${gennodejs_INSTALL_DIR}/mypackage
    DESTINATION ${gennodejs_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_nodejs)
  add_dependencies(mypackage_generate_messages_nodejs std_msgs_generate_messages_nodejs)
endif()

if(genpy_INSTALL_DIR AND EXISTS ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mypackage)
  install(CODE "execute_process(COMMAND \"/usr/bin/python2\" -m compileall \"${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mypackage\")")
  # install generated code
  install(
    DIRECTORY ${CATKIN_DEVEL_PREFIX}/${genpy_INSTALL_DIR}/mypackage
    DESTINATION ${genpy_INSTALL_DIR}
  )
endif()
if(TARGET std_msgs_generate_messages_py)
  add_dependencies(mypackage_generate_messages_py std_msgs_generate_messages_py)
endif()
