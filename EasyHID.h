/*
    Библиотека программного USB клавиатуры и мыши для Arduino Nano/UNO/Mega и прочих
    GitHub: https://github.com/GyverLibs/EasyHID
    Возможности:
    - Программный USB
    - Работает на любой AVR Arduino
    - Мультимедийные клавиши
    - Системные сочетания
    - Буферизация нажатий

    Nich1con, nich1con36@mail.ru, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

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
        - Исправлены ошибки некорректной работы индикаторов ScrolLock,CapsLock, NumLock
        - Добавлены сканкоды KEY_CAPS_LOCK, KEY_SCROLL_LOCK и KEY_NUM_LOCK
*/

#ifndef _EasyHID_h
#define _EasyHID_h

#include <Arduino.h>

#include "Codekeys.h"
#include "HIDPrivate.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

class EasyHID {
 public:                    // Класс HID
  void begin(void);         // Инициализация шины USB
  void end(void);           // Отключение шины USB
  void tick(void);          // Поллинг шины (вызывать не реже чем раз в 10мс!)
  bool isConnected(void);   // Статус шины
  bool isNumLock(void);     // Проверка numLock
  bool isCapsLock(void);    // Проверка capsLock
  bool isScrollLock(void);  // Проверка scrollLock
};

class MouseClass {
 public:                                 // Класс мышки
  void move(int16_t x, int16_t y);       // Двигаем курсор
  void moveLong(int16_t x, int16_t y);   // Двигаем курсор
  void click(uint8_t btn = MOUSE_LEFT);  // Кликаем на клавишу
  void press(uint8_t btn = MOUSE_LEFT);  // Зажимаем клавишу
  void releaseAll(void);                 // Отпускаем все
 private:
  void _move(int8_t x, int8_t y);  // Двигаем курсор
  uint8_t btnBuffer = 0;           // Состояние кнопки
};

class KeyboardClass : public Print {
 public:                                                                                     // Класс клавиатуры
  void press(uint8_t k0, uint8_t k1 = 0, uint8_t k2 = 0, uint8_t k3 = 0, uint8_t k4 = 0);    // Нажать от 1 до 5 клавиш
  void click(uint8_t k0, uint8_t k1 = 0, uint8_t k2 = 0, uint8_t k3 = 0, uint8_t k4 = 0);    // Кликнуть (нажать + отпустить)
  void release(uint8_t k0, uint8_t k1 = 0, uint8_t k2 = 0, uint8_t k3 = 0, uint8_t k4 = 0);  // Отпустить
  void releaseAll(void);                                                                     // Отпустить все ранее нажатые
  void clickMultimediaKey(uint8_t key);                                                      // Кликнуть мультимедиа клавишу
  void clickSystemKey(uint8_t key);                                                          // Кликнуть системную клавишу
  virtual size_t write(uint8_t data);                                                        // Напечатать символ
  using Print::print;
  using Print::println;
  using Print::write;

 private:
  bool pushKey(uint8_t key);
  bool popKey(uint8_t key);
  uint8_t keyBuffer[5] = {0x00, 0x00, 0x00, 0x00, 0x00};
};

extern EasyHID HID;
extern MouseClass Mouse;
extern KeyboardClass Keyboard;
#endif