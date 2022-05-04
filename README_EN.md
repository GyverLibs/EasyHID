# EasyHID
Library for software implementation of USB keyboard and mouse on some MK AVR and boards based on them
- Letter keys
- Multimedia keys
- System keys and shortcuts
- Buffering clicks
- Mouse movement and pressing its buttons

### Compatibility
- ATmega328 16 MHz (Nano, Uno, Mini board)
- ATtiny88 (MH-ET board)
- ATtiny167 (Digispark PRO board)
-ATtiny48
- ATmega168/88/48/8

## Content
- [Install](#install)
- [Connection](#wiring)
- [Usage](#usage)
- [Example](#example)
- [Versions](#versions)
- [Bugs and feedback](#feedback)

<a id="install"></a>
## Installation
- **LIBRARY NOT AVAILABLE IN LIBRARY MANAGER** due to project structure. If someone can run the code from the src folder - throw a pull request!
- [Download library](https://github.com/GyverLibs/EasyHID/archive/refs/heads/main.zip) .zip archive for manual installation:
    - Unzip and put in *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Unzip and put in *C:\Program Files\Arduino\libraries* (Windows x32)
    - Unpack and put in *Documents/Arduino/libraries/*
    - (Arduino IDE) automatic installation from .zip: *Sketch/Include library/Add .ZIP library…* and specify the downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE% D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="wiring"></a>
## Connection
Scheme on the example of Arduino Nano
![scheme](/docs/schemes.png)
- Zener diode - any low-power at 3.6V
- Resistor 100 Ohm - can be replaced by another in the range of 47.. 200 Ohm
- Resistor 1.5 kOhm - can be replaced with another one in the range of 1.2.. 2.2 kOhm
- Diode - any ordinary silicon (with a drop of 0.7V)

> If active pin pulling is used, add `#define EASYHID_SOFT_DETACH` BEFORE including EasyHID.h

### Pins
The port and pin can be set in the *usbconfig.h* file. Standard:
- ATtiny88 (MH-ET board) - USB soldered on the board
    - **D-** - pin 0 (PD1)
    - **D+** - pin 2 (PD2) (INT0)

- ATtiny167 (Digispark PRO board) - USB soldered on the board
    - **D-** - pin 4 (PB3)
    - **D+** - pin 3 (PB6) (INT0)
    
- ATmega328/168/88/48
    - **D-** - (PD4)
    - **D+** - (PD2) (INT0)
    - **PULL** - (PD5)

<a id="usage"></a>
## Usage
### HID
```cpp
HID.begin(); // USB bus initialization
HID end(); // Disable USB bus
HID.tick(); // Bus polling (call at least once every 10ms)
HID.isConnected(); // (bool) Bus status
HID.isNumLock(); // (bool) Check numLock
HID.isCapsLock(); // (bool) Check capsLock
HID.isScrollLock(); // (bool) Check scrollLock
```

### Mouse
```cpp
Mouse.move(int8_t x, int8_t y); // Move cursor
Mouse.click(uint8_t btn); // Click on the key
Mouse.press(uint8_t btn); // Hold down the key
Mouse.releaseAll(); // Release everything

// BUTTON CONSTANTS
MOUSE_LEFT
MOUSE_RIGHT
MOUSE_MIDDLE
```

### Keyboard
```cpp
Keyboard.press(uint8_t key1, ... key5); // Press up to five buttons
Keyboard.click(uint8_t key1, ... key5); // Click up to five buttons
Keyboard.release(uint8_t key1, ... key5); // Release up to five buttons
Keyboard.releaseAll(); // Release all previously pressed
Keyboard.clickMultimediaKey(uint8_t key); // Click media keycranberry avishu
Keyboard.clickSystemKey(uint8_t key); // Click the system key

Keyboard.write(uint8_t data); // print character
Keyboard.print(); // Print any type of data (from Print.h)
Keyboard.println(); // Print any type of data (from Print.h)
```

#### Key constants
<details>
<summary>System</summary>

```cpp
KEY_LEFT_CONTROL
KEY_LEFT_SHIFT
KEY_LEFT_ALT
KEY_LEFT_WIN
KEY_RIGHT_CONTROL
KEY_RIGHT_SHIFT
KEY_RIGHT_ALT
KEY_RIGHT_WIN

KEY_POWER
KEY_SLEEP
KEY_WAKE
```
</details>

<details>
<summary>Normal</summary>

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
```
</details>

<details>
<summary>Multimedia</summary>

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


<a id="example"></a>
## Examples
> See more advanced examples in the examples folder!

### Keyboard
```cpp
#include <EasyHID.h>

void setup() {
  HID.begin(); // USB initialization
}

void loop() {
  static uint32_t timer = millis(); // Millis - Timer (NOT DELAY!!!)
  if (millis() - timer >= 1000) { // Every 1000ms
    timer = millis();

    Keyboard.press(KEY_1); // Hold down the '1' key
    Keyboard.relecranberry aseAll(); // Release all keys
    Keyboard.click(KEY_SPACE); // Click the space bar (press + releaseAll)
    keyboard.write('-'); // Output dash
    Keyboard.println("Hello world!"); // Print in println
  }

  HID.tick(); // Call the ticker at least every 10ms!
}
```

### Mouse
```cpp
#include <EasyHID.h>

void setup() {
  HID.begin(); // USB initialization
}

void loop() {
  static uint8_t count = 0; // Direction
  static uint32_t timer = millis(); // Millis - Timer (NOT DELAY!!!)
  if (millis() - timer >= 1000) { // Every 1000ms
    timer = millis();
    switch (count) {
      case 0: Mouse.move(100, 0); break; // Move the mouse to X-Y pixels.
      case 1: Mouse.move(0, 100); break;
      case 2: Mouse.move(-100, 0); break;
      case 3: Mouse.move(0, -100); break;
    }
    // Mouse.click(); // Can be left clicked
    // Mouse.click(MOUSE_MIDDLE); // Or RIGHT, LEFT, MIDDLE
    if (++count > 3) count = 0; // Cycle shift from 0 to 3
  }

  HID.tick(); // Call the ticker at least every 10ms!
}
```

<a id="versions"></a>
## Versions
-v1.0
- v2.0
    - Added buffering of keys (up to 5 pieces simultaneously pressed)
    - Added release(), from one to 5 keys
    - System keys and shortcuts work
    - Added support for Digispark PRO board based on ATtiny167
    - Added support for MH-ET board based on ATtiny88
    - Now, if an active pull-up is involved in the circuit (see the diagram), you must either add #define EASYHID_SOFT_DETACH before connecting
    - Added end() method: disables USB, is correctly perceived by the computer without error only when using a circuit with active pullup
    - Added more convenient constants
- v2.1 - small optimization
- v2.2 - fixed bug with ATmega328
- v2.2.1 - fixed bug

<a id="feedback"></a>
## Bugs and feedback
When you find bugs, create an **Issue**, or better, immediately write to the mail [alex@alexgyver.ru](mailto:alex@alexgyver.ru)
The library is open for revision and your **Pull Request**'s!