import tkinter as tk
from i2c_numbers_tranfer import *
import smbus
import time

def on_button_click(direction):
    print(f"{direction} button clicked.")

    ########
    bus = smbus.SMBus(1)
    address = 0x8 # bus address
    vertical_step_num = 30
    horizontal_step_num = 200
    num1, num2 = 0, 0

    if direction == "top":
        print("Up button clicked.")
        num1 = 0
        num2 = int(vertical_step_num)
    elif direction == "bottom":
        print("Down button clicked.")
        num1 = 0
        num2 = -int(vertical_step_num)
    elif direction == "left":
        print("Left button clicked.")
        num1 = int(horizontal_step_num)
        num2 = 0
    elif direction == "right":
        print("Right button clicked.")
        num1 = -int(horizontal_step_num)
        num2 = 0

    print("original num1:  " + str(num1))
    print("original num2:  " + str(num2))
    num1_part1, num1_part2, num1_part3, num1_part4 = bitSplitIntegerAs32Bit(num1)
    num2_part1, num2_part2, num2_part3, num2_part4 = bitSplitIntegerAs32Bit(num2)
    bus.write_i2c_block_data(address, num1_part1, [num1_part2, num1_part3, num1_part4, num2_part1, num2_part2, num2_part3, num2_part4])
    time.sleep(1)
        
# Create the main window
root = tk.Tk()
root.title("4-Sided Controller")

# Dictionary to store entry widgets for each direction
entry_dict = {}

# Function to create buttons and text inputs
def create_button_and_entry(direction, custom_string):
    button = tk.Button(root, text=custom_string, command=lambda dir=direction: on_button_click(dir))
    button.pack(side=direction)

# Create buttons and text inputs for each direction with custom strings
create_button_and_entry("top", "Up")
create_button_and_entry("bottom", "Down")
create_button_and_entry("left", "Left")
create_button_and_entry("right", "Right")

# Start the Tkinter event loop
root.mainloop()
