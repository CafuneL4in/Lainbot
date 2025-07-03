#include <Mouse.h>

void setup() {
  Serial.begin(115200);
  Mouse.begin();
}

void loop() {
  if (Serial.available() >= 3) {
    uint8_t button_mask = Serial.read();
    int8_t dx = Serial.read();
    int8_t dy = Serial.read();

    Mouse.move(dx, dy);

    if (button_mask & 0x01) {
      Mouse.press(MOUSE_LEFT);
    } else {
      Mouse.release(MOUSE_LEFT);
    }
  }
}