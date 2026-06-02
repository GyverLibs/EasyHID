This is an automatic translation and may be incorrect in some places. See the source README and examples for authoritative information.

[![Foo](https://img.shields.io/badge/Version-2.4-brightgreen.svg?style=flat-square)](#versions)
[![PIO](https://badges.registry.platformio.org/packages/gyverlibs/library/EasyHID.svg)](https://registry.platformio.org/libraries/gyverlibs/EasyHID)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD%24%E2%82%AC%20%D0%9F%D0%BE%D0%B4%D0%B4%D0%B5%D1%80%D0%B6%D0%B0%D1%82%D1%8C-%D0%B0%D0%B2%D1%82%D0%BE%D1%80%D0%B0-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/EasyHID?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# EasyHID
Library for software implementation of USB keyboard and mouse on some MK AVR and boards based on them
- Letter keys
- Multimedia keys
- System keys and combinations
- Press buffering
- Mouse movement and pressing its buttons

### Library video
[Watch on YouTube](https://www.youtube.com/watch?v=WSHHDPDFKD0)

### Compatibility
#### MK AVR with 16 MHz tacting!!!
- ATmega328 (Nano, Uno, Mini)
- ATmega168/88/48/8
- ATtiny88 (MH-ET board) **works via USB on the board! **
- ATtiny167 (Digispark PRO board) ** works via USB on the board! **
- ATtiny48
- ATtiny85/45 (Digispark board, etc.) ** works via USB on the board! **

#### Important points
- Library conflicts with built-in functions`attachInterrupt()`and`detachInterrupt()`, see example *MultimediaRemote*
- Do not use a long (over 1 meter) USB cable.
- To work properly, you need to power the microcontroller from the same USB cable used to emulate USB. That is, the launch must occur when the USB cable is connected to the host.

## I'm not working!
The library is very stable on the boards listed above, so:
- Check the connection.
- Read the "Important Moments" above
- Try another Arduina, preferably from another seller/manufacturer. USB is very sensitive to tacting frequency, if the board is cheap "quartz" - may not work

## Contents
- [Installation](#install)
- [Connection](#wiring)
- [Use of use](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- **Library is inaccessible in library management because of the structure of the project. If someone can run the code from the src folder, throw the request pool!
- [Download the library](https://github.com/GyverLibs/EasyHID/archive/refs/heads/main.zip).zip archive for manual installation:
    - Unpack and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unpack and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/ *
    - (Arduino IDE) Automatic installation from .zip: *Sketch/Connect library/Add .ZIP library...* and specify downloaded archive
- Read more detailed instructions for installing libraries[here](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: new versions fix errors and bugs, as well as optimize and add new features.
- Through the library manager IDE: find the library as when installing and click "Update"
- Manually: **Delete the folder with the old version** and then put the new one in its place. “Replacement” can not be done: sometimes new versions delete files that will remain when replaced and can lead to errors!

<a id="wiring"></a>
## Connection
### Arduino UNO/NANO/PRO MINI (ATmega328p/168p) 16 MHz
![scheme](/docs/sch1.png)

### Components
- Stabilitron is any low power at 3. 6V
- Resistor 100 ohms - can be replaced by another in the range of 47. 200 ohm
- Resistor 1.5 kΩ - can be replaced by another in the range of 1.2..2.2 kΩ
- Diode - any conventional silicon (with a drop of 0.7V)

### Pins
Port and pin can be set in the file *usbconfig.h*. Standard:
- ATtiny88 (MH-ET board) - USB soldered on board
    - **D-** - pin 0 (PD1)
    - **D+** - pin 2 (PD2) (INT0)

- ATtiny167 (Digispark PRO board) - USB soldered on the board
    - **D-** - pin 4 (PB3)
    - **D+** - pin 3 (PB6) (INT0)

- ATtiny45/85 (Digispark boards, etc.) - USB soldered on the board
    - **D-** - pin 3 (PB3)
    - **D+** - pin 4 (PB4)
    
- ATmega328/168/88/48
    - **D-** - (PD4)
    - **D+** - (PD2) (INT0)
    - **PULL** - (PD5)

<a id="usage"></a>
## Use of use
### HID
```cpp
HID.begin();            // Initialization of the USB bus
HID.end();              // Shutting down the USB bus
HID.tick();             // Polling tires (call at least once every 10ms)
HID.isConnected();      // (bool) Tyre status
HID.isNumLock();        // (bool) Check numLock
HID.isCapsLock();       // (bool) Checking capsLock
HID.isScrollLock();     // (bool) Check scrolllock
```

### Mouse
```cpp
Mouse.move(int16_t x, int16_t y);   // Move the cursor.
Mouse.click(uint8_t btn);           // Click on the key.
Mouse.press(uint8_t btn);           // Hold the key.
Mouse.releaseAll();                 // Let it all go.

// KNOP CONSTANTS
MOUSE_LEFT
MOUSE_RIGHT
MOUSE_MIDDLE
```

### Keyboard
```cpp
Keyboard.press(uint8_t key1, ... key5);     // Pressing up to five buttons
Keyboard.click(uint8_t key1, ... key5);     // Click to five buttons
Keyboard.release(uint8_t key1, ... key5);   // Release up to five buttons
Keyboard.releaseAll();                      // Release all previously pressed
Keyboard.clickMultimediaKey(uint8_t key);   // Click the multimedia key
Keyboard.clickSystemKey(uint8_t key);       // Click the system key

Keyboard.write(uint8_t data);               // Print a symbol
Keyboard.print();                           // Print any type of data (from Print.h)
Keyboard.println();                         // Print any type of data (from Print.h)
```

#### Keyboard constants
<details>
<summary>Regular (press/click/release)</summary >>

```cpp
KEY_1
KEY_2
KEY_3
KEY_4
KEY_5
KEY_6
KEY_7
KEY_8
KEY_9
KEY_0

KEY_A
KEY_B
KEY_C
KEY_D
KEY_E
KEY_F
KEY_G
KEY_H
KEY_I
KEY_J
KEY_K
KEY_L
KEY_M
KEY_N
KEY_O
KEY_P
KEY_Q
KEY_R
KEY_S
KEY_T
KEY_U
KEY_V
KEY_W
KEY_X
KEY_Y
KEY_Z

KEY_COMMA
KEY_PERIOD
KEY_MINUS
KEY_EQUAL
KEY_BACKSLASH
KEY_SQBRAK_LEFT
KEY_SQBRAK_RIGHT
KEY_SLASH

KEY_F1
KEY_F2
KEY_F3
KEY_F4
KEY_F5
KEY_F6
KEY_F7
KEY_F8
KEY_F9
KEY_F10
KEY_F11
KEY_F12

KEY_APP
KEY_ENTER
KEY_BACKSPACE
KEY_ESC
KEY_TAB
KEY_SPACE
KEY_INSERT
KEY_HOME
KEY_PAGE_UP
KEY_DELETE
KEY_END
KEY_PAGE_DOWN
KEY_PRINTSCREEN

KEY_ARROW_RIGHT
KEY_ARROW_LEFT
KEY_ARROW_DOWN
KEY_ARROW_UP
    
KEY_LEFT_CONTROL
KEY_LEFT_SHIFT
KEY_LEFT_ALT
KEY_LEFT_WIN
KEY_RIGHT_CONTROL
KEY_RIGHT_SHIFT
KEY_RIGHT_ALT
KEY_RIGHT_WIN
```
</details>

<details >>
<summary>ClickMultimediaKey</summary >>

```cpp
KEY_VOL_UP
KEY_VOL_DOWN
KEY_NEXT_TRACK
KEY_PREV_TRACK
KEY_STOP
KEY_PLAYPAUSE
KEY_MUTE
KEY_BASSBOOST
KEY_LOUDNESS
KEY_KB_EXECUTE
KEY_KB_HELP
KEY_KB_MENU
KEY_KB_SELECT
KEY_KB_STOP
KEY_KB_AGAIN
KEY_KB_UNDO
KEY_KB_CUT
KEY_KB_COPY
KEY_KB_PASTE
KEY_KB_FIND
```
</details>

<details >>
<summary>ClickSystemKey</summary >>

```cpp
KEY_POWER
KEY_SLEEP
KEY_WAKE
```
</details>

<a id="example"></a>
## Examples
> See more examples in the examples folder!

### keyboard
```cpp
#include <EasyHID.h>

void setup() {
  HID.begin();                          // Initialization of USB
}

void loop() {
  static uint32_t timer = millis();     // Millis is a timer (NOT DELAY!!!!)
  if (millis() - timer >= 1000) {       // Every 1,000 ms.
    timer = millis();

    Keyboard.press(KEY_1);              // Press the "1" key.
    Keyboard.releaseAll();              // Let go of all keys.
    Keyboard.click(KEY_SPACE);          // Click the space (press + releaseAll)
    Keyboard.write('-');                // Bringing out the dash
    Keyboard.println(" Hello world!");  // Printing in println
  }

  HID.tick();   // Call the ticker at least every 10 ms!
}
```

### Mouse.
```cpp
#include <EasyHID.h>

void setup() {
  HID.begin();                          // Initialization of USB
}

void loop() {
  static uint8_t count = 0;             // Direction
  static uint32_t timer = millis();     // Millis is a timer (NOT DELAY!!!!)
  if (millis() - timer >= 1000) {       // Every 1,000 ms.
    timer = millis();
    switch (count) {
      case 0: Mouse.move(100, 0);  break; // Mouse on the X-Y peak.
      case 1: Mouse.move(0, 100);  break;
      case 2: Mouse.move(-100, 0); break;
      case 3: Mouse.move(0, -100); break;
    }
    // Mouse.click(); You can click the LCM.
    // Mouse.click(MOUSE MIDDLE); // Or RIGHT, LEFT, MIDDLE
    if (++count > 3) count = 0;         // Cyclical shift from 0 to 3
  }

  HID.tick();   // Call the ticker at least every 10 ms!
}
```

<a id="versions"></a>
## Versions
- v1.0
- v2.0
    - Added buffering keys (up to 5 pieces simultaneously pressed)
    - Added release(), from one to 5 keys
    - System keys and combinations work
    - Added support for Digispark PRO board based on ATtiny167
    - Added MH-ET support based on ATtiny88
    - Now, if an active lift is involved in the scheme (see diagram), you must add #define EASYHID SOFT DETACH before connecting the liba.
    - End(): Disables USB, correctly perceived by the computer without error only when using an active pullup circuit
    - More convenient constants added
- v2.1 - a small optimization
- v2.2 - Fixed bug with ATmega328
- v2.2.1 - fixation bug
- v2.3 - ATmega8 support added
- v2.4 - Added support for Arduino IDE 2. 0
- v2.5 – Mouse Mouse Mouse Call No Longer Resets Click
- v2.6
    - Removed SOFT DETACH functionality, misleading users. (see diagrams)
    - New connectivity schemes added
    - Fixed errors in the incorrect operation of indicators ScrolLock, CapsLock, NumLock
    - KEY CAPS LOCK, KEY SCROLL LOCK and KEY NUM LOCK
- v2.7:
  - The mouse cursor can now be moved to a distance int16 t (was int8 t)
  - Fixed a bug with modifier keys when calling the Keyboard.release( method, now all combinations work correctly

<a id="feedback"></a>
## Bugs and feedback
If you find bugs, create **Issue**, or better write to the mail immediately.[alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
The library is open for revision and your **Pull Requests*!

When reporting bugs or incorrect work of the library, it is necessary to specify:
- Library version
- What is used by the IC
- SDK version (for ESP)
- Arduino IDE version
- Are embedded examples that use features and designs that cause bugs in your code working correctly?
- What code was downloaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed. Not a canvas of a thousand lines, but a minimum code.
