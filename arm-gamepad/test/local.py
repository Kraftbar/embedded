from xarm.wrapper import XArmAPI
import sys
import time

def init_arm():
    # Initialize the arm by connecting to its IP address
    arm = XArmAPI('10.131.29.47')
    arm.motion_enable(enable=True)
    arm.set_mode(0)  # Set to servo motion mode for Cartesian control
    arm.set_state(state=0)
    return arm

def move_yz(arm, y, z, speed=80):
    # Set the arm to move to the specified y and z angles
    pos = arm.get_position()[1]  # Get current servo angles
    print(pos)
    pos[1] = y  # Update y coordinate
    pos[2] = z  # Update z coordinate
    # Use set_servo_cartesian to move to the new y and z coordinates
    arm.set_position(*pos, speed=speed)

if __name__ == "__main__":
    arm = init_arm()
    try:
        print("Enter y and z angles separated by a comma (e.g., 30,45):")
        for line in sys.stdin:
            line=line.strip()
            line=line.strip("[]")
            if line.strip() == "":
                break  # Exit if an empty line is entered
            values = list(map(float, line.split(',')))
            y_angle = values[1]  # Adjust this index to match the y-axis input in your data
            z_angle = values[2]  # Adjust this index to match the z-axis input in your data
            
            move_yz(arm, y_angle, z_angle)
    except KeyboardInterrupt:
        print("Program exited by user.")
    finally:
        arm.disconnect()

