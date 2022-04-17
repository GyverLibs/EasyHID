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

    Версии:
    v1.0 - релиз
    v2.0 - крупное обновление, добавлены системные сосчетания и буферизация нажатий
    v2.1 - небольшая оптимизация
    v2.2 - пофикшен баг с ATmega328
    v2.2.1 - пофикшен баг
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
public:							// Класс HID
    void begin(void);			// Инициализация шины USB
	void end(void);				// Отключение шины USB
    void tick(void);			// Поллинг шины (вызывать не реже чем раз в 10мс!)
    bool isConnected(void);		// Статус шины
    bool isNumLock(void);		// Проверка numLock
    bool isCapsLock(void);      // Проверка capsLock
    bool isScrollLock(void);    // Проверка scrollLock
};

class MouseClass {							
public:										// Класс мышки
    void move(int8_t x, int8_t y);			// Двигаем курсор
    void click(uint8_t btn = MOUSE_LEFT);	// Кликаем на клавишу
    void press(uint8_t btn = MOUSE_LEFT);	// Зажимаем клавишу
    void releaseAll(void);					// Отпускаем все
};

class KeyboardClass : public Print {															
public:																							// Класс клавиатуры
    void press(uint8_t k0, uint8_t k1 = 0, uint8_t k2 = 0, uint8_t k3 = 0, uint8_t k4 = 0);		// Нажать от 1 до 5 клавиш	
    void click(uint8_t k0, uint8_t k1 = 0, uint8_t k2 = 0, uint8_t k3 = 0, uint8_t k4 = 0);	 	// Кликнуть (нажать + отпустить)
    void release(uint8_t k0, uint8_t k1 = 0, uint8_t k2 = 0, uint8_t k3 = 0, uint8_t k4 = 0);	// Отпустить   
    void releaseAll(void);																		// Отпустить все ранее нажатые
    void clickMultimediaKey(uint8_t key);														// Кликнуть мультимедиа клавишу
    void clickSystemKey(uint8_t key);															// Кликнуть системную клавишу
    virtual size_t write(uint8_t data);															// Напечатать символ
    using Print::write;																		
    using Print::print;																		
    using Print::println;	
private:
    bool pushKey(uint8_t key);
	bool popKey(uint8_t key);
	uint8_t keyBuffer[5] = {0x00, 0x00, 0x00, 0x00, 0x00};   	  	
};

extern EasyHID HID;
extern MouseClass Mouse;
extern KeyboardClass Keyboard;
#endif