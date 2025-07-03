import mss
import numpy as np
import cv2
import hid
import time
import keyboard  # pip install keyboard

# 🔧 Ayarlar
FOV_RADIUS = 120
HSV_LOWER = np.array([130, 60, 120])   # mor/pembe
HSV_UPPER = np.array([165, 255, 255])
SCREEN_CENTER = (960, 540)            # 1080p için

# 🎯 Logitech spoof HID (senin verdiğin değerler)
VID = 0x046D
PID = 0xC534

device = hid.device()
device.open(VID, PID)
device.set_nonblocking(1)

def send_mouse(dx, dy, click=False):
    btn = 0x01 if click else 0x00
    packet = bytearray([btn, dx & 0xFF, dy & 0xFF])
    device.write(packet)

def target_is_centered(dx, dy, threshold=6):
    return abs(dx) < threshold and abs(dy) < threshold

def get_target_offset(frame):
    hsv = cv2.cvtColor(frame, cv2.COLOR_BGR2HSV)
    mask = cv2.inRange(hsv, HSV_LOWER, HSV_UPPER)

    cx, cy = SCREEN_CENTER
    roi = mask[cy-FOV_RADIUS:cy+FOV_RADIUS, cx-FOV_RADIUS:cx+FOV_RADIUS]
    coords = cv2.findNonZero(roi)

    if coords is not None:
        avg = np.mean(coords, axis=0)[0]
        dx = int(avg[0] - FOV_RADIUS)
        dy = int(avg[1] - FOV_RADIUS)
        return dx, dy
    return None

# 🧠 Ana loop
sct = mss.mss()
monitor = sct.monitors[1]

print("[+] LainColorbot çalışıyor. Sağ tık ile hedefe kilitlenir, otomatik ateş eder.")
time.sleep(1)

try:
    while True:
        if keyboard.is_pressed("right"):
            frame = np.array(sct.grab(monitor))[:, :, :3]
            target = get_target_offset(frame)

            if target:
                dx, dy = target
                if target_is_centered(dx, dy):
                    send_mouse(dx, dy, click=True)
                else:
                    send_mouse(dx, dy)
        time.sleep(0.005)

except KeyboardInterrupt:
    print("[!] Çıkış yapılıyor...")
    device.close()
