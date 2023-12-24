import tkinter as tk
# from i2c_numbers_tranfer import *

def on_button_click(direction):
    text = entry_dict[direction].get()
    print(f"{direction} button clicked. Text: {text}")

# Function to create buttons and text inputs
def create_button_and_entry(direction):
    frame = tk.Frame(root)
    frame.pack(side=direction)

    label = tk.Label(frame, text=f"{direction.capitalize()}:")
    label.pack()

    entry = tk.Entry(frame)
    entry.pack()

    button = tk.Button(frame, text=f"Click {direction}", command=lambda: on_button_click(direction))
    button.pack()

    # Store the entry widget in the dictionary
    entry_dict[direction] = entry


if __name__ == '__main__':

    # Create the main window
    root = tk.Tk()
    root.title("4-Sided Controller")

    # Dictionary to store entry widgets for each direction
    entry_dict = {}

    # Create buttons and text inputs for each direction
    create_button_and_entry("top")
    create_button_and_entry("bottom")
    create_button_and_entry("left")
    create_button_and_entry("right")

    # Start the Tkinter event loop
    root.mainloop()
