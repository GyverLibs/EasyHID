#include <Arduino.h>
#include "EasyHID.h"

EasyHID HID;
MouseClass Mouse;
KeyboardClass Keyboard;

void asciiToKey (uint8_t data, uint8_t isCaps, uint8_t* mod, uint8_t* key);

/*---------------------------------------Общий класс HID-----------------------------------*/
void EasyHID::begin(void){	
#if (defined (__AVR_ATtiny48__) || defined (__AVR_ATtiny88__))
	pinMode(1, INPUT); 		
    pinMode(2, INPUT);
#elif defined (__AVR_ATtiny167__)
	pinMode(3, INPUT); 		
    pinMode(4, INPUT);
#elif (defined (__AVR_ATmega48P__) || defined (__AVR_ATmega88P__) ||  \
	   defined (__AVR_ATmega168P__) || defined (__AVR_ATmega328P__))
	pinMode(4, INPUT); 		
    pinMode(2, INPUT); 
#if(EASYHID_SOFT_DETACH)		
	pinMode(5, OUTPUT);
#endif
#endif
	
    cli();					// Запрет работы прерываний	
    usbDeviceDisconnect();	// Ложный дисконнект usb
    _delay_ms(250);			// Небольшая задержка
    usbDeviceConnect(); 	// Коннект usb 
    usbInit();				// Инициализация usb 
    sei();					// Разрешение работы прерываний	
}

void EasyHID::end(void){	// Отключение usb
     cli();					// Запрет работы прерываний	
	 usbDeviceDisconnect();	// Дисконнект usb
	 sei();					// Разрешение работы прерываний	
}

void EasyHID::tick(void){	
    usbPoll();
}

bool EasyHID::isConnected(void){
    return !!usb_hasCommed;
}

bool EasyHID::isNumLock(void){
    return !!(led_state & LED_NUM_MSK);
}

bool EasyHID::isCapsLock(void){
    return !!(led_state & LED_CAPS_MSK);	
}

bool EasyHID::isScrollLock(void){
    return !!(led_state & LED_SCROLL_MSK);	
}

/*-------------------------------------------------------------------------------------*/

/*---------------------------------------Класс мышки-----------------------------------*/
/*
   Общая логика работы мышки не сложная -
   отправляется либо относительное смещение
   курсора, либо отправка кода одной из 
   клавиш на мышке.
*/

void MouseClass::move(int8_t x, int8_t y){	
    report_buffer[0] = REPID_MOUSE;
    report_buffer[1] = 0;
    report_buffer[2] = (int8_t)x;
    report_buffer[3] = (int8_t)y;
    usbReportSend(REPSIZE_MOUSE);
}

void MouseClass::press(uint8_t btn){
    report_buffer[0] = REPID_MOUSE;
    report_buffer[1] = btn;
    report_buffer[2] = 0;
    report_buffer[3] = 0;
    usbReportSend(REPSIZE_MOUSE);
}

void MouseClass::releaseAll(void){
    report_buffer[0] = REPID_MOUSE;
    report_buffer[1] = 0;
    report_buffer[2] = 0;
    report_buffer[3] = 0;
    usbReportSend(REPSIZE_MOUSE);
}

void MouseClass::click(uint8_t btn){
    press(btn);
    releaseAll();
}
/*--------------------------------------------------------------------------------------*/


/*---------------------------------------Класс клавиатуры-------------------------------*/																							// Нажатие клавиши
void KeyboardClass::press(uint8_t k0, uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4) {		// От 1 до 5 клавиш 
  bool flag = (pushKey(k0) | pushKey(k1) | pushKey(k2) | pushKey(k3) | pushKey(k4));		// Проверяем и записываем клавиши в буфер
  if (!flag) return;																		// Если ничего в буфере не поменялось - выходим
  uint8_t mod = 0x00;																		// Байт-модификатор
  for (uint8_t i = 0; i < 5; i++) { 														// Проходим по буферу клавиш
    if (keyBuffer[i] >= KEY_LEFT_CONTROL && keyBuffer[i] <= KEY_RIGHT_GUI) {				// Если в буфере есть клавиши - модификаторы (альт, шифт...)
      mod |= (1 << (keyBuffer[i] - KEY_LEFT_CONTROL));										// Добавляем бит в модификатор согласно клавише	
    }
  }
  report_buffer[0] = REPID_KEYBOARD;														// Репорт ID для клавы
  report_buffer[1] = mod;																	// Модификатор
  for (uint8_t i = 0; i < 5; i++) {															// Проходим по буферу
    report_buffer[i + 3] = keyBuffer[i];													// Переписываем его в буфер на отправку	
  }	
  usbReportSend(REPSIZE_KEYBOARD);															// Отправляем
}

																						    // Отпускание клавиши	
void KeyboardClass::release(uint8_t k0, uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4) {	// От 1 до 5 клавиш 
  bool flag = (popKey(k0) | popKey(k1) | popKey(k2) | popKey(k3) | popKey(k4));				// Проверяем и чистим буфер от указаных клавиш
  if (!flag) return;																		// Если ничего в буфере не поменялось - выходим
  report_buffer[0] = REPID_KEYBOARD;														// Репорт ID для клавы														
  report_buffer[1] = 0;																	    // Модификатор пуст
  for (uint8_t i = 0; i < 5; i++) {															// Проходим по буферу
    report_buffer[i + 3] = keyBuffer[i];													// Переписываем его в буфер на отправку	
  }
  usbReportSend(REPSIZE_KEYBOARD);															// Отправляем
}

void KeyboardClass::releaseAll(void) {														// Отпускание всех нажатых 	
  for (uint8_t i = 0; i < 5; i++) keyBuffer[i] = 0;											// Очищаем буфер нажатых клавиш		
  report_buffer[0] = REPID_KEYBOARD;														// Репорт ID для клавы	
  report_buffer[1] = 0;																	    // Модификатор пуст
  for (uint8_t i = 0; i < 5; i++) {															// Проходим по буферу
    report_buffer[i + 3] = keyBuffer[i];													// Чистим буфер на отправку	
  }
  usbReportSend(REPSIZE_KEYBOARD);															// Отправляем
}

void KeyboardClass::click(uint8_t k0, uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4) {		// Клик - нажатие + отпускание	
  press(k0, k1, k2, k3, k4);																// Жмем
  release(k0, k1, k2, k3, k4);																// Отпускаем
}

/*
   Буфер реализован похожим на стек, push-pop
   Пустые места это нули, занятые - коды клавиш.
   press и release изменяет содержимое буфера,
   после чего обновленный список отправляется хосту.   
*/

bool KeyboardClass::pushKey(uint8_t key) {		// Функция проверки и упаковки клавиш в буфер
  if (!key) return false;						// Нули (не нажатые) не нужны - сразу выходим
  for (uint8_t i = 0; i < 5; i++) {				// Проходим по всему буферу
    if (key == keyBuffer[i]) {					// Если в буфере уже есть такая клавиша	
      return false;								// Просто выходим		
    }
  }
  for (uint8_t i = 0; i < 5; i++) {				// Проходим по всему буферу
    if (!keyBuffer[i]) {						// Если нашли пустое место в буфере (ноль)
      keyBuffer[i] = key;						// Пишем туда новую клавишу	
      return true;								// Выходим с почетом
    }
  }
}

bool KeyboardClass::popKey(uint8_t key) {		// Попки! Чистим список нажатых клавиш
  if (!key) return false;						// Нули сразу пропускаем - выходим
  for (uint8_t i = 0; i < 5; i++) {				// Проходим по всему буферу
    if (key == keyBuffer[i]) {					// Если нашли такую клавишу в буфере
      keyBuffer[i] = 0;							// Освобождаем место	
      return true;								// Выходим	
    }
  }
}

/*
   Тут все аналогично, просто отправляется
   код нажатой клавиши но чуть в другом формате
   Запихать мультимедиа и системные клавиши в
   методы для обычных клавиш в теории можно,
   но тогда придется отсеивать это дерьмо
   перед отправкой и плодить костыли.
*/

void KeyboardClass::clickMultimediaKey(uint8_t key) {		
    report_buffer[0] = REPID_MM_KEY;
    report_buffer[1] = key;
    report_buffer[2] = 0;
    usbReportSend(REPSIZE_MM_KEY);
    
    report_buffer[0] = REPID_MM_KEY;
    report_buffer[1] = 0;
    report_buffer[2] = 0;
    usbReportSend(REPSIZE_MM_KEY);
}

void KeyboardClass::clickSystemKey(uint8_t key) {	
    report_buffer[0] = REPID_SYS_KEY;
    report_buffer[1] = key;
    usbReportSend(REPSIZE_SYS_KEY);
    
    report_buffer[0] = REPID_SYS_KEY;
    report_buffer[1] = 0;
    usbReportSend(REPSIZE_SYS_KEY);
}

/*
   А дальше печать текста, работает не очень эффективно,
   отправляя всего по одной клавише за вызов, вместо 
   возможных пяти. Однако так работает print-овский
   write. В теории можно завести буфер с очередью,
   но пока это мало кому нужно, вы часто печатаете тексты?
*/

size_t KeyboardClass::write(uint8_t data){													// Отправка ascii символа
    uint8_t modifier, keycode;																// Модификатор (шифт) и код клавиши
    asciiToKey(data, (bool)(led_state & LED_CAPS_MSK), &modifier, &keycode);				// Просим перевести ascii в код
    report_buffer[0] = REPID_KEYBOARD;														// Репорт ID для клавы															
    report_buffer[1] = modifier;															// Модификатор
    report_buffer[2] = 0; 																	// Ничего
    report_buffer[3] = keycode;																// Нужная клавиша
    usbReportSend(REPSIZE_KEYBOARD);														// Отправляем
    releaseAll();																			// Отпускаем 		
    return 1;							
}


/*
 Я отказываюсь это комментировать, 
 просто вот такой алгоритм ascii -> code
 Явно экономнее чем таблица
 Кстати компилятор варнинги кидает, гнида
*/
void asciiToKey (uint8_t data, uint8_t isCaps, uint8_t* mod, uint8_t* key) {
    *key = 0b00;
    *mod = 0b00;

    if (data >= 'A' && data <= 'Z'){
        *key = 4 + data - 'A';
        if (isCaps) *mod = 0b00;
        else *mod = 0b10;
    } else if (data >= 'a' && data <= 'z') {
        *key = 4 + data - 'a';
        if (isCaps) *mod = 0b10;
        else *mod = 0b00;
    } else if (data >= '0' && data <= '9') {
        *mod = 0b00;
        if (data == '0')*key = 0x27;
        else *key = 30 + data - '1';
    } else {
        switch (data) {
        case '!': *mod = 0b10; *key = 29 + 1;  break;
        case '@': *mod = 0b10; *key = 29 + 2;  break;
        case '#': *mod = 0b10; *key = 29 + 3;  break;
        case '$': *mod = 0b10; *key = 29 + 4;  break;
        case '%': *mod = 0b10; *key = 29 + 5;  break;
        case '^': *mod = 0b10; *key = 29 + 6;  break;
        case '&': *mod = 0b10; *key = 29 + 7;  break;
        case '*': *mod = 0b10; *key = 29 + 8;  break;
        case '(': *mod = 0b10; *key = 29 + 9;  break;
        case ')': *mod = 0b10; *key = 0x27;    break;
        case '~': *mod = 0b10;
        case '`': *key = 0x35; break;
        case '_': *mod = 0b10;
        case '-': *key = 0x2D; break;
        case '+': *mod = 0b10;
        case '=': *key = 0x2E; break;
        case '{': *mod = 0b10;
        case '[': *key = 0x2F; break;
        case '}': *mod = 0b10;
        case ']': *key = 0x30; break;
        case '|': *mod = 0b10;
        case '\\':*key = 0x31; break;
        case ':': *mod = 0b10;
        case ';': *key = 0x33; break;
        case '"': *mod = 0b10;
        case '\'':*key = 0x34; break;
        case '<': *mod = 0b10;
        case ',': *key = 0x36; break;
        case '>': *mod = 0b10;
        case '.': *key = 0x37; break;
        case '?': *mod = 0b10;
        case '/': *key = 0x38; break;
        case ' ': *key = 0x2C; break;
        case '\t':*key = 0x2B; break;
        case '\n':*key = 0x28; break;
        }
    }
}

/*--------------------------------------------------------------------------------------*/