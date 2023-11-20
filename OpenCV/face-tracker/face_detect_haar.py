import cv2

classifier  = cv2.CascadeClassifier("haarcascade_frontalface_default.xml")
videoCam = cv2.VideoCapture(0)

if not videoCam.isOpened():
    print("No success opening the camera")
    exit()

while True:
    ret, frame = videoCam.read()

    if ret == True:
        abuAbu = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
        output = classifier.detectMultiScale(abuAbu, scaleFactor = 1.3, minNeighbors = 2)
        for (x, y, w, h) in output:
            cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
        countString = "Found: " + str(len(output))

        font = cv2.FONT_HERSHEY_SIMPLEX
        cv2.putText(frame, countString, (0, 30), font, 1, (255, 0, 0), 1)

        cv2.imshow("Frame", frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

videoCam.release()
cv2.destroyAllWindows()
