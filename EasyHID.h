/*
    Библиотека программного USB клавиатуры и мыши для Arduino Nano/UNO/Mega и прочих
    GitHub: https://github.com/GyverLibs/EasyHID
    Возможности:
    - Программный USB
    - Работает на любой AVR Arduino
    - Поддержка мультимедийных клавиш

    Nich1con, nich1con36@mail.ru, alex@alexgyver.ru
    https://alexgyver.ru/
    MIT License

    Версии:
    v1.0 - релиз
*/

#ifndef _EasyHID_h
#define _EasyHID_h

#include <Arduino.h>
#include "HIDPrivate.h"
#include "Codekeys.h"

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

class EasyHID {
public:
    void begin(void);			// Инициализация шины USB
    void tick(void);			// Поллинг шины (вызывать не реже чем раз в 10мс!)
    bool isConnected(void);		// Статус шины
    bool isNumLock(void);		// проверка numLock
    bool isCapsLock(void);      // проверка capsLock
    bool isScrollLock(void);    // проверка scrollLock
};

class MouseClass {							// Класс мышки
public:
    void move(int8_t x, int8_t y);			// Двигаем курсор
    void click(uint8_t btn = MOUSE_LEFT);	// Кликаем на клавишу
    void press(uint8_t btn = MOUSE_LEFT);	// Зажимаем клавишу
    void releaseAll(void);					// Отпускаем все
};

class KeyboardClass : public Print {							// Класс клавиатуры
public:
    void releaseAll(void);										// Отпускаем все

    void press(uint8_t key);									// Жмем 1 клавишу
    void press(uint8_t key_0, uint8_t key_1);					// Жмем 2 клавиши
    void press(uint8_t key_0, uint8_t key_1, uint8_t key_2);	// Жмем 3 клавиши

    void click(uint8_t key);									// Кликаем 1 клавишу
    void click(uint8_t key_0, uint8_t key_1);					// Кликаем 2 клавиши
    void click(uint8_t key_0, uint8_t key_1, uint8_t key_2);	// Кликаем 3 клавиши
    
    void clickMultimediaKey(uint8_t key);						// Кликаем мультимедиа клавишу
    void clickSystemKey(uint8_t key);							// Кликаем системную клавишу
    
    virtual size_t write(uint8_t data);							// Отправка ascii + print
    using Print::write;
    using Print::print;
    using Print::println;
};

extern EasyHID HID;
extern MouseClass Mouse;
extern KeyboardClass Keyboard;
#endif