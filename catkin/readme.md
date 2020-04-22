roscore               
source bashrc               
source devel/setup.sh               
rosrun joy joy_node               
rostopic echo joy               
rosrun mypackage listener.py               
rostopic echo /cmd_vel               
