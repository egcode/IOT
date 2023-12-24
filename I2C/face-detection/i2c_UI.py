import tkinter as tk
from i2c_numbers_tranfer import *
import smbus
import time

def on_button_click(direction):
    text = entry_dict[direction].get()
    print(f"{direction} button clicked. Custom String: {text}")

    ########
    bus = smbus.SMBus(1)
    address = 0x8 # bus address

    vertical_step_num = 100
    horizontal_step_num = 300

    # Add conditions for each button
    if direction == "top":
        # Code for the "Up" button
        print("Up button clicked.")

        num1 = 0
        num2 = int(vertical_step_num)
        print("original num1:  " + str(num1))
        print("original num2:  " + str(num2))
        num1_part1, num1_part2, num1_part3, num1_part4 = bitSplitIntegerAs32Bit(num1)
        num2_part1, num2_part2, num2_part3, num2_part4 = bitSplitIntegerAs32Bit(num2)
        bus.write_i2c_block_data(address, num1_part1, [num1_part2, num1_part3, num1_part4, num2_part1, num2_part2, num2_part3, num2_part4])
        time.sleep(1)
        
    elif direction == "bottom":
        # Code for the "Down" button
        print("Down button clicked.")
        num1 = 0
        num2 = -int(vertical_step_num)
        print("original num1:  " + str(num1))
        print("original num2:  " + str(num2))
        num1_part1, num1_part2, num1_part3, num1_part4 = bitSplitIntegerAs32Bit(num1)
        num2_part1, num2_part2, num2_part3, num2_part4 = bitSplitIntegerAs32Bit(num2)
        bus.write_i2c_block_data(address, num1_part1, [num1_part2, num1_part3, num1_part4, num2_part1, num2_part2, num2_part3, num2_part4])
        time.sleep(1)

    elif direction == "left":
        # Code for the "Left" button
        print("Left button clicked.")
        num1 = int(horizontal_step_num)
        num2 = 0
        print("original num1:  " + str(num1))
        print("original num2:  " + str(num2))
        num1_part1, num1_part2, num1_part3, num1_part4 = bitSplitIntegerAs32Bit(num1)
        num2_part1, num2_part2, num2_part3, num2_part4 = bitSplitIntegerAs32Bit(num2)
        bus.write_i2c_block_data(address, num1_part1, [num1_part2, num1_part3, num1_part4, num2_part1, num2_part2, num2_part3, num2_part4])
        time.sleep(1)

    elif direction == "right":
        # Code for the "Right" button
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
    frame = tk.Frame(root)
    frame.pack(side=direction)

    entry = tk.Entry(frame)
    entry.pack()

    button = tk.Button(frame, text=custom_string, command=lambda dir=direction: on_button_click(dir))
    button.pack()

    # Store the entry widget in the dictionary
    entry_dict[direction] = entry

# Create buttons and text inputs for each direction with custom strings
create_button_and_entry("top", "Up")
create_button_and_entry("bottom", "Down")
create_button_and_entry("left", "Left")
create_button_and_entry("right", "Right")

# Start the Tkinter event loop
root.mainloop()
