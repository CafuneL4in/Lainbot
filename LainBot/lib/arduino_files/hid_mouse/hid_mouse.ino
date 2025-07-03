#define RAWHID_TX_INTERVAL 0

#include <SPI.h>
#include <Usb.h>
#include <usbhub.h>
#include <hiduniversal.h>
#include <HID-Project.h>
#include <HID-Settings.h>

USB          Usb;
USBHub       Hub(&Usb);
HIDUniversal Hid(&Usb);

// RawHID packet buffer
uint8_t rawBuf[64];

// Bitmask order for up to 5 mouse buttons
const uint8_t buttonMasks[5] = { 0x01, 0x02, 0x04, 0x08, 0x10 };

// Custom parser that works with most HID/boot mice
class MouseParser : public HIDReportParser {
  uint8_t prevButtons = 0;

  void Parse(USBHID*, bool isRptID, uint8_t len, uint8_t *buf) override {
    uint8_t offset = isRptID ? 1 : 0;

    // Ensure the report has enough bytes for movement + scroll
    if (len < offset + 6) return;

    uint8_t buttons = buf[offset + 0];
    int8_t dx       = (int8_t)buf[offset + 1];
    int8_t dy       = (int8_t)buf[offset + 3];
    int8_t wheel    = (int8_t)buf[offset + 5];

    // Handle each of the 5 supported buttons
    for (uint8_t i = 0; i < 5; i++) {
      uint8_t mask = buttonMasks[i];
      bool now  = buttons & mask;
      bool prev = prevButtons & mask;

      if (now && !prev) BootMouse.press(mask);
      if (!now && prev) BootMouse.release(mask);
    }
    prevButtons = buttons;

    // Send movement and scroll
    BootMouse.move(dx, dy, wheel);
  }
} mouseParser;

void setup() {
  BootMouse.begin();                                // Enable USB HID mouse
  RawHID.begin(rawBuf, sizeof(rawBuf));             // Setup RawHID
  RawHID.enable();                                  

  if (Usb.Init() == -1) while (1);                  // Halt if USB init fails
  Hid.SetReportParser(0, &mouseParser);             // Assign mouse parser
}

void loop() {
  Usb.Task(); // Poll USB mouse

  // Check for RawHID injection input
  int len = RawHID.read();
  if (len > 0) {
    uint8_t btns = rawBuf[0];
    int8_t  dx   = (int8_t)rawBuf[1];
    int8_t  dy   = (int8_t)rawBuf[2];

    // Simulate button press and immediate release
    for (uint8_t i = 0; i < 5; i++) {
      uint8_t mask = buttonMasks[i];
      if (btns & mask) {
        BootMouse.press(mask);
        delayMicroseconds(100);  // Tap-style press
        BootMouse.release(mask);
      }
    }

    // Inject movement (scroll not handled here for safety)
    BootMouse.move(dx, dy, 0);

    RawHID.enable();  // Re-arm USB after sending
  }
}
