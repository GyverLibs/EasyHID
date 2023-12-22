This is an automatic translation, may be incorrect in some places. See sources and examples!

# Easyhid
Library for the software implementation of the USB keyboard and mouse on some AVR MK and boards based on them
- Lamp keys
- Multimedia keys
- System keys and combinations
- Booferization of presses
- mouse movement and pressing its buttons

### video on the library
[Look at YouTube] (https://www.youtube.com/watch?v=wshdpdfkd0)

## compatibility
### MK AVR with tacting 16 MHz !!!
- Atmega328 (Nano, UNO, Mini)
- Atmega168/88/48/8
- Attiny88 (MH -T-ST) ** works through USB on the board! **
- Attiny167 (Digispark Pro) ** works through USB on the board! **
- Attiny48

#### Important points
- The library conflicts with built -in functions `Attachinterept ()` and `Detachinterrapt ()`, see example *Multimediaramote *
- Do not use a long (more than 1 meter) USB cable!
- For the correct operation, you need to power the microcontroller from the same USB cable that is used to emulate USB.That is, the launch should occur when connecting the USB cable to the host

## Content
- [installation] (# Install)
- [connection] (#wiring)
- [use] (#usage)
- [Example] (# Example)
- [versions] (#varsions)
- [bugs and feedback] (#fedback)

<a id="install"> </a>
## Installation
- ** The library is not available in the library manager ** due to the structure of the project.If someone can start the code from the SRC folder - throw the Pool of the Orvest!
- [download the library] (https://github.com/gyverlibs/easyhid/archive/refs/heads/main.zip) .Zip archive for manual installation:
    - unpack and put in * C: \ Program Files (X86) \ Arduino \ Libraries * (Windows X64)
    - unpack and put in * C: \ Program Files \ Arduino \ Libraries * (Windows X32)
    - unpack and put in *documents/arduino/libraries/ *
    - (Arduino id) Automatic installation from. Zip: * sketch/connect the library/add .Zip library ... * and specify downloaded archive
- Read more detailed instructions for installing libraries [here] (https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%BD%D0%BE%BE%BE%BED0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Update
- I recommend always updating the library: errors and bugs are corrected in the new versions, as well as optimization and new features are added
- through the IDE library manager: find the library how to install and click "update"
- Manually: ** remove the folder with the old version **, and then put a new one in its place.“Replacement” cannot be done: sometimes in new versions, files that remain when replacing are deleted and can lead to errors!


<a id="wiring"> </a>
## connection
### Arduino UNO/Nano/Pro mini (atmega328p/168p) 16 mhz
! [Scheme] (/doc/sch1.png)

### Components
- zener diode - any low -power at 3.6V
- resistor 100 Ohms - can be replaced with another in the range of 47 .. 200 Ohms
- resistor 1.5 kOhm - can be replaced with another in the range 1.2 .. 2.2 kom
- diode - any ordinary silicon (with a fall of 0.7v)

### Pina
The port and pin can be set in the *usbconfig.h *file.Standard:
- Attiny88 (MH -ST board) - USB is rolled up on the board
    - ** D - ** - PIN 0 (PD1)
    - ** d+** - PIN 2 (PD2) (int0)

- Attiny167 (Digispark Pro) - USB rolled up on the board
    - ** d - ** - PIN 4 (pb3)
    - ** d+** - PIN 3 (PB6) (int0)
    
- Atmega328/168/88/48
    - ** D - ** - (PD4)
    - ** d+** - (PD2) (int0)
    - ** pull ** - (PD5)

<a id="usage"> </a>
## use
## hid
`` `CPP
Hid.begin ();// initialization of USB tires
Hid.end ();// Tires Tires USB
Hid.tick ();// Polling Tires (call at least once every 10MS)
Hid.isconnected ();// (bool) tire status
Hid.isnumlock ();// (bool) check numlock
Hid.iscapslock ();// (bool) check Capslock
Hid.isscrolllock ();// (bool) check scroll
`` `

### Mouse
`` `CPP
Mouse.move (int16_t x, int16_t y);// Moving the cursor
Mouse.click (uint8_t btn);// click on the key
Mouse.press (uint8_t BTN);// Clound the key
Mouse.releaseall ();// let out everything

// Constant buttons
Mouse_left
Mouse_right
Mouse_middle
`` `

## K Keyboard
`` `CPP
Keyboard.press (uint8_t key1, ... key5);// pressing up to five buttons
Keyboard.click (uint8_t key1, ... key5);// Click up to five buttons
Keyboard.release (uint8_t key1, ... key5);// release up to five buttons
Keyboard.releaseall ();// let go all previously pressed
Keyboard.clickmultimediakey (uint8_t key);// Click multimedia key
Keyboard.clickSystemkey (uint8_t key);// Click the system key

Keyboard.write (uint8_t data);// Print a symbol
Keyboard.print ();// Print any type of data (from print.h)
Keyboard.println ();// Print any type of data (from print.h)
`` `

#### Constant Key
<details>
<summary> ordinary (Press/Click/Release) </ Summary>

`` `CPP
Key_1
Key_2
Key_3
Key_4
Key_5
Key_6
Key_7
Key_8
Key_9
Key_0

Key_a
Key_b
Key_c
Key_d
Key_e
Key_f
Key_g
Key_h
Key_i
Key_j
Key_k
Key_l
Key_m
Key_n
Key_o
Key_p
Key_q
Key_r
Key_s
Key_t
Key_u
Key_v
Key_w
Key_x
Key_y
Key_z

Key_commma
Key_period
Key_minus
Key_equal
Key_Backslash
Key_sqbrak_left
Key_sqbrak_right
Key_slash

Key_f1
Key_f2
Key_f3
Key_f4
Key_f5
Key_f6
Key_f7
Key_f8
Key_f9
Key_f10
Key_f11
Key_f12

Key_app
Key_enter
Key_Backspace
Key_esc
Key_tab
Key_Space
Key_insert
Key_home
Key_page_up
Key_delete
Key_end
Key_page_Down
Key_printscreen

Key_arrow_right
Key_arrow_left
Key_arrow_down
Key_arrow_up
    
Key_left_control
Key_left_shift
Key_left_alt
Key_left_win
Key_right_control
Key_right_shift
Key_Right_alt
Key_right_win
`` `
</details>

<details>
<summary> multimedia (Clickmultimediakey) </ Summary>

`` `CPP
Key_vol_up
Key_vol_Down
Key_next_track
Key_prev_track
Key_stop
Key_playpause
Key_Mute
Key_bassboost
Key_loudness
Key_kb_execute
Key_kb_help
Key_kb_Menu
Key_kb_select
Key_kb_stop
Key_kb_again
Key_kb_undo
Key_kb_cut
Key_kb_copy
Key_kb_paste
Key_kb_find
`` `
</details>

<details>
<summary> Systemic (ClickSystemkey) </ Summary>

`` `CPP
Key_power
Key_Sleep
Key_wake
`` `
</details>

<a id="EXAMPLE"> </a>
## Examples
> See more advanced examples in the Examples folder!

### Keyboard
`` `CPP
#include <Easyhid.h>

VOID setup () {
  Hid.begin ();// initialization of USB
}

VOID loop () {
  static uint32_t timer = millis ();// millis - timer (not delay !!!)
  if (millis () - timer> = 1000) {// every 1000 ms
    Timer = Millis ();

    Keyboard.press (key_1);// clamp the key '1'
    Keyboard.releaseall ();// let out all the keys
    Keyboard.click (key_space);// Click a gap (Press + Releaseall)
    Keyboard.write ('-');// Display the dash
    Keyboard.println ("Hello World!");// Println Printing
  }

  Hid.tick ();// Call the ticker at least every 10 ms!
}
`` `

### Mouse
`` `CPP
#include <Easyhid.h>

VOID setup () {
  Hid.begin ();// initialization of USB
}

VOID loop () {
  static uint8_t count = 0;// Direction
  static uint32_t timer = millis ();// millis - timer (not delay !!!)
  if (millis () - timer> = 1000) {// every 1000 ms
    Timer = Millis ();
    Switch (Count) {
      Case 0: Mouse.move (100, 0);Break;// Move the mouse on the X-Y Pix.
      Case 1: Mouse.move (0, 100);Break;
      Case 2: Mouse.move (-100, 0);Break;
      Case 3:mouse.move (0, -100);Break;
    }
    // mouse.click ();// you can click LKM
    // mouse.click (mouse_middle);// or Right, Left, Middle
    if (++ count> 3) Count = 0;// Cyclic shift from 0 to 3
  }

  Hid.tick ();// Call the ticker at least every 10 ms!
}
`` `

<a id="versions"> </a>
## versions
- V1.0
- V2.0
    - Added keyboardization of keys (up to 5 pieces of simultaneously pressed)
    - added Release (), from one to 5 keys
    - Work system keys and combinations
    - Added support for Digispark Pro board based on Attiny167
    - Added support for the MH-ST board based on Attiny88
    - Now, if the circuit is involved in the active lifellation (see diagram) before connecting the LIBE, add # DEFINE EASYHID_SOLID_DETACH before connecting
    - Added the End () method: Disaches usb, is correctly perceived by a computer without error only when using a scheme with active pullup
    - Added more convenient constants
- V2.1 - Small optimization
- V2.2 - Filled Bag with Atmega328
- V2.2.1 - Filled Bag
- V2.3 - Added support atmega8
- V2.4 - Added support for Arduino IDE 2.0
- V2.5 - Calling Move method for the mouse now does not drop a click
- V2.6
    - The functionality of Soft_Detach is removed, misleading users.(see schemes)
    - Added new connection schemes
    - Fixed errors of incorrect operation of indicators Scrollock, Capslock, Numlock
    - Added scankds Key_caps_Lock, Key_Scroll_LOCK and KEY_NUM_LOCK
- V2.7:
  - the mouse cursor can now be moved to the distance int16_t (it was int8_t)
  - Fixed a bug with modifier keys when calling the keyboard.release () method, any combinations are now working correctly

<a id="feedback"> </a>
## bugs and feedback
Create ** Issue ** when you find the bugs, and better immediately write to the mail [alex@alexgyver.ru] (mailto: alex@alexgyver.ru)
The library is open for refinement and your ** pull Request ** 'ow!


When reporting about bugs or incorrect work of the library, it is necessary to indicate:
- The version of the library
- What is MK used
- SDK version (for ESP)
- version of Arduino ide
- whether the built -in examples work correctly, in which the functions and designs are used, leading to a bug in your code
- what code has been loaded, what work was expected from it and how it works in reality
- Ideally, attach the minimum code in which the bug is observed.Not a canvas of a thousand lines, but a minimum code