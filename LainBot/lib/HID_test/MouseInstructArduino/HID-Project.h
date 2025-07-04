// Include guard
#pragma once

// Software version
#define HID_PROJECT_VERSION 284

#if ARDUINO < 10607
#error HID Project requires Arduino IDE 1.6.7 or greater. Please update your IDE.
#endif

#if !defined(USBCON)
#error HID Project can only be used with an USB MCU.
#endif

// Include all HID libraries (.a linkage required to work) properly
#include "SingleReport/SingleAbsoluteMouse.h"
#include "MultiReport/AbsoluteMouse.h"
#include "SingleReport/BootMouse.h"
#include "MultiReport/ImprovedMouse.h"
#include "SingleReport/SingleConsumer.h"
#include "MultiReport/Consumer.h"
#include "SingleReport/SingleGamepad.h"
#include "MultiReport/Gamepad.h"
#include "SingleReport/SingleSystem.h"
#include "MultiReport/System.h"
#include "SingleReport/RawHID.h"
#include "SingleReport/BootKeyboard.h"
#include "MultiReport/ImprovedKeyboard.h"
#include "SingleReport/SingleNKROKeyboard.h"
#include "MultiReport/NKROKeyboard.h"
#include "MultiReport/SurfaceDial.h"

// Include Teensy HID afterwards to overwrite key definitions if used
