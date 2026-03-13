import sys
import select
import time
import uuid
import os
import cv2
import serial
from roboflow import Roboflow


# -----------------------------
# Arduino Serial Configuration
# -----------------------------
arduino = serial.Serial('/dev/cu.usbmodemFX2348N1', 9600, timeout=1)
time.sleep(2)  # Wait for Arduino to reset


# -----------------------------
# Select Traffic Signal
# -----------------------------
print("🚦 Select the signal to monitor (1, 2, or 3):")
signal = input("Enter signal number: ").strip()

if signal not in ['1', '2', '3']:
    print("❌ Invalid signal.")
    sys.exit()

print(f"✅ Monitoring Signal {signal}")


# -----------------------------
# Load Roboflow Model
# -----------------------------
rf = Roboflow(api_key="YOUR_API_KEY")
project = rf.workspace("nta-ix1gi").project("ambulance-object-detection-buy6l")
model = project.version(3).model


# -----------------------------
# Start Webcam
# -----------------------------
cap = cv2.VideoCapture(0)

if not cap.isOpened():
    print("❌ Cannot open webcam")
    sys.exit()


# -----------------------------
# Main Detection Loop
# -----------------------------
while True:

    ret, frame = cap.read()
    if not ret:
        print("❌ Failed to grab frame")
        break

    # Save frame temporarily
    filename = f"temp_{uuid.uuid4().hex}.jpg"
    cv2.imwrite(filename, frame)

    try:
        result = model.predict(filename, confidence=0.5, overlap=0.3)
        predictions = result.json()['predictions']

        os.remove(filename)

        for p in predictions:

            if p['class'].lower() == 'ambulance' and p['confidence'] > 0.4:

                print(f"🚑 Ambulance detected at Signal {signal}!")

                # Send signal number to Arduino
                arduino.write(signal.encode())

                # Bounding box coordinates
                x, y = int(p['x']), int(p['y'])
                w, h = int(p['width']), int(p['height'])

                x1 = int(x - w / 2)
                y1 = int(y - h / 2)
                x2 = int(x + w / 2)
                y2 = int(y + h / 2)

                # Draw bounding box
                cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)

                label = f"{p['class']} ({p['confidence']:.2f})"
                cv2.putText(
                    frame,
                    label,
                    (x1, y1 - 10),
                    cv2.FONT_HERSHEY_SIMPLEX,
                    0.6,
                    (0, 255, 0),
                    2
                )

        cv2.imshow("Ambulance Detector", frame)

    except Exception as e:
        print(f"🚨 Error during prediction: {e}")

        if os.path.exists(filename):
            os.remove(filename)

    # Terminal exit (press 0)
    if select.select([sys.stdin], [], [], 0)[0]:
        line = sys.stdin.readline().strip()
        if line == '0':
            print("👋 Quitting...")
            break

    # Keyboard exit (press q)
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break


# -----------------------------
# Cleanup
# -----------------------------
cap.release()
cv2.destroyAllWindows()
arduino.close()
