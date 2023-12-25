import cv2
from picamera2 import Picamera2
from libcamera import Transform
import time
from i2c_numbers_tranfer import *
import smbus

# to PI
# scp -r /Users/eugene/Documents/Repo/IOT/I2C/face-detection/face_detect_pi_haar.py eugene@192.168.68.63:/home/eugene/Documents/IOT/I2C/face-detection/face_detect_pi_haar.py

# picam2 = Picamera2()
# picam2.preview_configuration.main.size = (320,240)
# picam2.preview_configuration.main.format = "RGB888"
# picam2.preview_configuration.align()
# picam2.configure("preview")
# picam2.start()

frame_width = 320
frame_height = 240
picam2 = Picamera2()
preview_config = picam2.create_preview_configuration({"format": "RGB888", "size": (frame_width, frame_height)},transform=Transform(vflip=True))
picam2.configure(preview_config)
picam2.preview_configuration.align()
picam2.start()

last_execution_time = time.time()  # Initialize the last execution time
execute_every_seconds = 1

# Draw a big red static cross
cross_size = 100
cross_thickness = 1
cross_color = (255, 255, 0)  # Red color in BGR format


def move_to_goal(delta_x, delta_y):
    bus = smbus.SMBus(1)
    address = 0x8 # bus address
    horizontal_step_ratio = 1.0
    vertical_step_ratio = 1.0

    num1 = -int(delta_x * horizontal_step_ratio)
    num2 = -int(delta_y * vertical_step_ratio)

    print("original num1:  " + str(num1))
    print("original num2:  " + str(num2))
    num1_part1, num1_part2, num1_part3, num1_part4 = bitSplitIntegerAs32Bit(num1)
    num2_part1, num2_part2, num2_part3, num2_part4 = bitSplitIntegerAs32Bit(num2)
    bus.write_i2c_block_data(address, num1_part1, [num1_part2, num1_part3, num1_part4, num2_part1, num2_part2, num2_part3, num2_part4])


classifier  = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")

while True:
    frame = picam2.capture_array()
    abuAbu = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    output = classifier.detectMultiScale(abuAbu, scaleFactor = 1.3, minNeighbors = 2)

    current_time = time.time()


    # Vertical line
    cv2.line(frame, (frame_width // 2, frame_height // 2 - cross_size // 2),
            (frame_width // 2, frame_height // 2 + cross_size // 2), cross_color, cross_thickness)

    # Horizontal line
    cv2.line(frame, (frame_width // 2 - cross_size // 2, frame_height // 2),
            (frame_width // 2 + cross_size // 2, frame_height // 2), cross_color, cross_thickness)

    for (x, y, w, h) in output:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)

        startX, endX, startY, endY = x, x+w, y, y+h
        # DRAW CROSS 
        y_offset = int((endX-startX)/3)
        centerX = int(startX+(endX-startX)/2)
        centerY = int(startY+(endY-startY)/2) - y_offset

        circle_radius = int((endX-startX)/8)
        circle_thickness = int(circle_radius/5)

        # BGR - 
        # circle (image, ceter, radius, color, thickness)
        cv2.circle(frame, (centerX, centerY), circle_radius, (0, 0, 255), circle_thickness)

        cv2.putText(frame, "TARGET", (centerX+15, centerY-15), cv2.FONT_HERSHEY_SIMPLEX, 0.45, (255, 0, 0), 2)
        # cv2.line(frame, (0, centerY), (w, centerY), (0, 255, 255), 2)  # x line
        # cv2.line(frame, (centerX, h), (centerX, 0), (0, 255, 255), 2)  # y line
        cv2.circle(frame, (centerX, centerY), 5, (0, 0, 255), cv2.FILLED)	

        ##########
        ### Calculate horizontal and vertical pixels to a goal
        # Calculate center of the detected face
        face_center_x = x + w // 2
        face_center_y = y + h // 2

        # Calculate center of the screen
        screen_center_x = frame_width // 2
        screen_center_y = frame_height // 2

        # # Calculate the number of pixels to move in the horizontal and vertical directions
        # delta_x = face_center_x - screen_center_x
        # delta_y = face_center_y - screen_center_y
        # print(f"Pixels to move: (delta_x={delta_x}, delta_y={delta_y})")

        # Calculate the percentage of screen to move in the horizontal and vertical directions
        delta_x_percentage = (face_center_x - screen_center_x) / screen_center_x * 100
        delta_y_percentage = (face_center_y - screen_center_y) / screen_center_y * 100
        print(f"Percentage to move: (delta_x={delta_x_percentage:.2f}%, delta_y={delta_y_percentage:.2f}%)")

        # Check if 3 seconds have passed since the last execution
        if current_time - last_execution_time >= execute_every_seconds:
            move_to_goal(delta_x_percentage, delta_y_percentage)
            last_execution_time = current_time  # Update the last execution time


    countString = "Found: " + str(len(output))
    cv2.putText(frame, countString, (0, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 1)

    cv2.imshow("Frame", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    
cv2.destroyAllWindows()
