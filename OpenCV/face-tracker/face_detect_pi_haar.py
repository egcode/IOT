import cv2
from picamera2 import Picamera2
from libcamera import Transform

# to PI
# scp -r /Users/eugene/Documents/Repo/IOT/OpenCV/face-tracker/face_detect_pi_haar.py eugene@192.168.68.63:/home/eugene/Documents/IOT/OpenCV/face-tracker/face_detect_pi_haar.py

# picam2 = Picamera2()
# picam2.preview_configuration.main.size = (320,240)
# picam2.preview_configuration.main.format = "RGB888"
# picam2.preview_configuration.align()
# picam2.configure("preview")
# picam2.start()

picam2 = Picamera2()
preview_config = picam2.create_preview_configuration({"format": "RGB888", "size": (320, 240)},transform=Transform(vflip=True))
picam2.configure(preview_config)
picam2.preview_configuration.align()
picam2.start()


classifier  = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")

while True:
    frame = picam2.capture_array()
    abuAbu = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    output = classifier.detectMultiScale(abuAbu, scaleFactor = 1.3, minNeighbors = 2)
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


    countString = "Found: " + str(len(output))
    cv2.putText(frame, countString, (0, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 1)

    cv2.imshow("Frame", frame)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break
    
cv2.destroyAllWindows()
