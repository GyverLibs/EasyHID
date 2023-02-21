[![Foo](https://img.shields.io/badge/Version-2.4-brightgreen.svg?style=flat-square)](#versions)
[![Foo](https://img.shields.io/badge/Website-AlexGyver.ru-blue.svg?style=flat-square)](https://alexgyver.ru/)
[![Foo](https://img.shields.io/badge/%E2%82%BD$%E2%82%AC%20%D0%9D%D0%B0%20%D0%BF%D0%B8%D0%B2%D0%BE-%D1%81%20%D1%80%D1%8B%D0%B1%D0%BA%D0%BE%D0%B9-orange.svg?style=flat-square)](https://alexgyver.ru/support_alex/)
[![Foo](https://img.shields.io/badge/README-ENGLISH-blueviolet.svg?style=flat-square)](https://github-com.translate.goog/GyverLibs/EasyHID?_x_tr_sl=ru&_x_tr_tl=en)  

[![Foo](https://img.shields.io/badge/ПОДПИСАТЬСЯ-НА%20ОБНОВЛЕНИЯ-brightgreen.svg?style=social&logo=telegram&color=blue)](https://t.me/GyverLibs)

# EasyHID
Библиотека для программной реализации USB клавиатуры и мышки на некоторых МК AVR и платах на их основе
- Буквенные клавиши
- Мультимедийные клавиши
- Системные клавиши и сочетания
- Буферизация нажатий
- Движение мышки и нажатие её кнопок

### Совместимость
#### МК AVR с тактированием 16 МГц !!!
- ATmega328 (плата Nano, Uno, Mini)
- ATmega168/88/48/8
- ATtiny88 (плата MH-ET) **работает через USB на плате!**
- ATtiny167 (плата Digispark PRO) **работает через USB на плате!**
- ATtiny48

#### Важные моменты
- Библиотека конфликтует со встроенными функциями `attachInterrupt()` и `detachInterrupt()`, см. пример *MultimediaRemote*
- Не используйте длинный (более 1 метра) USB кабель!
- Для корректной работы необходимо запитать микроконтроллер от того же USB кабеля, который используется для эмуляции USB. То есть запуск должен происходить при подключении USB кабеля к хосту

## Содержание
- [Установка](#install)
- [Подключение](#wiring)
- [Использование](#usage)
- [Пример](#example)
- [Версии](#versions)
- [Баги и обратная связь](#feedback)

<a id="install"></a>
## Установка
- **БИБЛИОТЕКА НЕДОСТУПНА В МЕНЕДЖЕРЕ БИБЛИОТЕК** из-за структуры проекта. Если кто то сможет запустить код из папки src - кидайте пулл реквест!
- [Скачать библиотеку](https://github.com/GyverLibs/EasyHID/archive/refs/heads/main.zip) .zip архивом для ручной установки:
    - Распаковать и положить в *C:\Program Files (x86)\Arduino\libraries* (Windows x64)
    - Распаковать и положить в *C:\Program Files\Arduino\libraries* (Windows x32)
    - Распаковать и положить в *Документы/Arduino/libraries/*
    - (Arduino IDE) автоматическая установка из .zip: *Скетч/Подключить библиотеку/Добавить .ZIP библиотеку…* и указать скачанный архив
- Читай более подробную инструкцию по установке библиотек [здесь](https://alexgyver.ru/arduino-first/#%D0%A3%D1%81%D1%82%D0%B0%D0%BD%D0%BE%D0%B2%D0%BA%D0%B0_%D0%B1%D0%B8%D0%B1%D0%BB%D0%B8%D0%BE%D1%82%D0%B5%D0%BA)
### Обновление
- Рекомендую всегда обновлять библиотеку: в новых версиях исправляются ошибки и баги, а также проводится оптимизация и добавляются новые фичи
- Через менеджер библиотек IDE: найти библиотеку как при установке и нажать "Обновить"
- Вручную: **удалить папку со старой версией**, а затем положить на её место новую. "Замену" делать нельзя: иногда в новых версиях удаляются файлы, которые останутся при замене и могут привести к ошибкам!


<a id="wiring"></a>
## Подключение
### Arduino UNO/NANO/PRO MINI (ATmega328p/168p) 16 MHz
![scheme](/docs/sch1.jpg)

### Голые ATmega328p/168p/88p и ATmega8 (Внешний кварц 16 МГц)
![scheme](/docs/sch2.jpg)

### Компоненты
- Стабилитрон - любой маломощный на 3.6V
- Резистор 100 Ом - можно заменить на другой в диапазоне 47.. 200 Ом
- Резистор 1.5 кОм - можно заменить на другой в диапазоне 1.2.. 2.2 кОм
- Диод - любой обычный кремниевый (с падением 0.7V)

> Если используется активная подтяжка при помощи пина, добавь `#define EASYHID_SOFT_DETACH` ПЕРЕД подключением EasyHID.h

### Пины
Порт и пин можно задать в файле *usbconfig.h*. Стандартные:
- ATtiny88 (плата MH-ET) - USB распаян на плате
    - **D-** - пин 0 (PD1)
    - **D+** - пин 2 (PD2) (INT0)

- ATtiny167 (плата Digispark PRO) - USB распаян на плате
    - **D-** - пин 4 (PB3)
    - **D+** - пин 3 (PB6) (INT0)
    
- ATmega328/168/88/48
    - **D-** - (PD4)
    - **D+** - (PD2) (INT0)
    - **PULL** - (PD5)

<a id="usage"></a>
## Использование
### HID
```cpp
HID.begin();            // Инициализация шины USB
HID.end();              // Отключение шины USB
HID.tick();             // Поллинг шины (вызывать не реже, чем раз в 10мс)
HID.isConnected();      // (bool) Статус шины
HID.isNumLock();        // (bool) Проверка numLock
HID.isCapsLock();       // (bool) Проверка capsLock
HID.isScrollLock();     // (bool) Проверка scrollLock
```

### Mouse
```cpp
Mouse.move(int8_t x, int8_t y);     // Двигаем курсор
Mouse.click(uint8_t btn);           // Кликаем на клавишу
Mouse.press(uint8_t btn);           // Зажимаем клавишу
Mouse.releaseAll();                 // Отпускаем все

// КОНСТАНТЫ КНОПОК
MOUSE_LEFT
MOUSE_RIGHT
MOUSE_MIDDLE
```

### Keyboard
```cpp
Keyboard.press(uint8_t key1, ... key5);     // Нажатие до пяти кнопок
Keyboard.click(uint8_t key1, ... key5);     // Клик до пяти кнопок
Keyboard.release(uint8_t key1, ... key5);   // Отпускание до пяти кнопок
Keyboard.releaseAll();                      // Отпустить все ранее нажатые
Keyboard.clickMultimediaKey(uint8_t key);   // Кликнуть мультимедиа клавишу
Keyboard.clickSystemKey(uint8_t key);       // Кликнуть системную клавишу

Keyboard.write(uint8_t data);               // Напечатать символ
Keyboard.print();                           // Напечатать любой тип данных (из Print.h)
Keyboard.println();                         // Напечатать любой тип данных (из Print.h)
```

#### Константы клавиш
<details>
<summary>Обычные (press/click/release)</summary>

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

<details>
<summary>Мультимедийные (clickMultimediaKey)</summary>

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

<details>
<summary>Системные (clickSystemKey)</summary>

```cpp
KEY_POWER
KEY_SLEEP
KEY_WAKE
```
</details>

<a id="example"></a>
## Примеры
> Смотри более расширенные примеры в папке examples!

### Клавиатура
```cpp
#include <EasyHID.h>

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
#include <EasyHID.h>

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
- v2.0
    - Добавлена буферизация клавиш (до 5 штук одновременно нажатых)
    - Добавлен release(), от одной до 5 клавиш
    - Работают системные клавиши и сочетания
    - Добавлена поддержка платы Digispark PRO на базе ATtiny167
    - Добавлена поддержка платы MH-ET на базе ATtiny88
    - Теперь если в схеме задействована активная подтяжка (см. схему) надо перед подключением либы дописать #define EASYHID_SOFT_DETACH
    - Добавлен метод end(): отключает USB, корректно воспринимается компом без ошибки только при использовании схемы с активным pullup
    - Добавлены более удобные константы
- v2.1 - небольшая оптимизация
- v2.2 - пофикшен баг с ATmega328
- v2.2.1 - пофикшен баг
- v2.3 - добавлена поддержка ATmega8
- v2.4 - добавлена поддержка Arduino IDE 2.0
- v2.5 - вызов метод move для мышки теперь сбрасывает клик
- v2.6
    - Убран функционал SOFT_DETACH, вводящий пользователей в заблуждение. (см. схемы)
    - Добавлены новые схемы подключения
    - Исправлены ошибки некорректной работы индикаторов ScrolLock, CapsLock, NumLock
    - Добавлены сканкоды KEY_CAPS_LOCK, KEY_SCROLL_LOCK и KEY_NUM_LOCK

<a id="feedback"></a>
## Баги и обратная связь
При нахождении багов создавайте **Issue**, а лучше сразу пишите на почту [alex@alexgyver.ru](mailto:alex@alexgyver.ru)  
Библиотека открыта для доработки и ваших **Pull Request**'ов!


При сообщении о багах или некорректной работе библиотеки нужно обязательно указывать:
- Версия библиотеки
- Какой используется МК
- Версия SDK (для ESP)
- Версия Arduino IDE
- Корректно ли работают ли встроенные примеры, в которых используются функции и конструкции, приводящие к багу в вашем коде
- Какой код загружался, какая работа от него ожидалась и как он работает в реальности
- В идеале приложить минимальный код, в котором наблюдается баг. Не полотно из тысячи строк, а минимальный код
