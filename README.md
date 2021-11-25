![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)
![author](https://img.shields.io/badge/author-AlexGyver-informational.svg)
# EasyHID
Библиотека программного USB клавиатуры и мыши для Arduino Nano/UNO/Mega и прочих
- Программный USB
- Работает на любой AVR Arduino
- Поддержка мультимедийных клавиш

### Совместимость
AVR Arduino

## Содержание
- [Установка](#install)
- [Подключение](#wiring)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- **БИБЛИОТЕКА НЕДОСТУПНА В МЕНЕДЖЕРЕ БИБЛИОТЕК**
- [Скачать библиотеку](https://github.com/GyverLibs/EasyHID/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)

<a id="wiring"></a>
## Подключение
![scheme](/docs/schemes.png)

<a id="usage"></a>
## Использование
### HID
```cpp
HID.begin();        // Инициализация шины USB
HID.tick();         // Поллинг шины (вызывать не реже чем раз в 10мс!)
HID.isConnected();  // (bool) Статус шины
HID.isNumLock();    // (bool) проверка numLock
HID.isCapsLock();   // (bool) проверка capsLock
HID.isScrollLock(); // (bool) проверка scrollLock
```

### Mouse
```cpp
Mouse.move(int8_t x, int8_t y);         // Двигаем курсор
Mouse.click(uint8_t btn = MOUSE_LEFT);  // Кликаем на клавишу
Mouse.press(uint8_t btn = MOUSE_LEFT);  // Зажимаем клавишу
Mouse.releaseAll();                     // Отпускаем все

// КОНСТАНТЫ КНОПОК
MOUSE_LEFT
MOUSE_RIGHT
MOUSE_MIDDLE
```

### Keyboard
```cpp
Keyboard.releaseAll(void);                                      // Отпускаем все

Keyboard.press(uint8_t key);                                    // Жмем 1 клавишу
Keyboard.press(uint8_t key_0, uint8_t key_1);                   // Жмем 2 клавиши
Keyboard.press(uint8_t key_0, uint8_t key_1, uint8_t key_2);    // Жмем 3 клавиши

Keyboard.click(uint8_t key);                                    // Кликаем 1 клавишу
Keyboard.click(uint8_t key_0, uint8_t key_1);                   // Кликаем 2 клавиши
Keyboard.click(uint8_t key_0, uint8_t key_1, uint8_t key_2);    // Кликаем 3 клавиши

Keyboard.clickMultimediaKey(uint8_t key);                       // Кликаем мультимедиа клавишу
Keyboard.clickSystemKey(uint8_t key);                           // Кликаем системную клавишу
```

<details>
<summary>КОНСТАНТЫ КЛАВИШ</summary>

```cpp
// для сочетаний
KEY_MOD_LEFT_CONTROL
KEY_MOD_LEFT_SHIFT
KEY_MOD_LEFT_ALT
KEY_MOD_LEFT_GUI
KEY_MOD_RIGHT_CONTROL
KEY_MOD_RIGHT_SHIFT
KEY_MOD_RIGHT_ALT
KEY_MOD_RIGHT_GUI

// обычные
KEY_LEFT_CONTROL
KEY_LEFT_SHIFT
KEY_LEFT_ALT
KEY_LEFT_GUI
KEY_RIGHT_CONTROL
KEY_RIGHT_SHIFT
KEY_RIGHT_ALT
KEY_RIGHT_GUI

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

// multimedia
KEY_VOL_UP
KEY_VOL_DOWN
KEY_SCAN_NEXT_TRACK
KEY_SCAN_PREV_TRACK
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

// system
KEY_POWER
KEY_SLEEP
KEY_WAKE
```
</details>

<a id="example"></a>
## Пример
### Клавиатура
```cpp
void setup() {
  HID.begin();                          // Инициализация USB
}

void loop() {
  static uint32_t timer = millis();     // Миллис - таймер (НЕ DELAY!!!)
  if (millis() - timer >= 1000) {       // Каждые 1000 мс
    timer = millis();

    Keyboard.press(KEY_1);              // Зажимаем клавишу '1'
    Keyboard.releaseAll();              // Отпускаем все клавиши
    Keyboard.click(KEY_SPACE);          // Кликаем пробел (press + releaseAll)
    Keyboard.write('-');                // Выводим тире
    Keyboard.println(" Hello world!");  // Печать в println
  }

  HID.tick();   // Вызываем тикер не реже чем каждые 10 мс!
}
```

### Мышь
```cpp
void setup() {
  HID.begin();                          // Инициализация USB
}

void loop() {
  static uint8_t count = 0;             // Направление
  static uint32_t timer = millis();     // Миллис - таймер (НЕ DELAY!!!)
  if (millis() - timer >= 1000) {       // Каждые 1000 мс
    timer = millis();
    switch (count) {
      case 0: Mouse.move(100, 0);  break; // Двигаем мышкой на X-Y пикс.
      case 1: Mouse.move(0, 100);  break;
      case 2: Mouse.move(-100, 0); break;
      case 3: Mouse.move(0, -100); break;
    }
    // Mouse.click();                   // Можно кликнуть ЛКМ
    // Mouse.click(MOUSE_MIDDLE);       // Или RIGHT, LEFT, MIDDLE
    if (++count > 3) count = 0;         // Циклический сдвиг от 0 до 3
  }

  HID.tick();   // Вызываем тикер не реже чем каждые 10 мс!
}
```

<a id="versions"></a>
## Версии
- v1.0

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!