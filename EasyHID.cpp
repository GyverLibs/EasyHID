#include "EasyHID.h"

#include <Arduino.h>

#include "Codekeys.h"

EasyHID HID;
MouseClass Mouse;
KeyboardClass Keyboard;

void asciiToKey(uint8_t data, uint8_t isCaps, uint8_t* mod, uint8_t* key);

/*---------------------------------------Общий класс HID-----------------------------------*/
void EasyHID::begin(void) {
#if (defined(__AVR_ATtiny48__) || defined(__AVR_ATtiny88__))
  pinMode(1, INPUT);
  pinMode(2, INPUT);

#elif defined(__AVR_ATtiny167__)
  pinMode(3, INPUT);
  pinMode(4, INPUT);

#elif (defined(__AVR_ATmega48P__) || defined(__AVR_ATmega88P__) ||  \
       defined(__AVR_ATmega168P__) || defined(__AVR_ATmega168__) || \
       defined(__AVR_ATmega328P__)) ||                              \
    defined(__AVR_ATmega328__)
  pinMode(4, INPUT);
  pinMode(2, INPUT);

#elif defined(__AVR_ATmega8__)
  DDRD &= ~(1 << PD2);
  PORTD &= ~(1 << PD2);
  DDRD &= ~(1 << PD4);
  PORTD &= ~(1 << PD4);
#endif

  cli();                  // Запрет работы прерываний
  usbDeviceDisconnect();  // Ложный дисконнект usb
  _delay_ms(250);         // Небольшая задержка
  usbDeviceConnect();     // Коннект usb
  usbInit();              // Инициализация usb
  sei();                  // Разрешение работы прерываний
}

void EasyHID::end(void) {  // Отключение usb
  cli();                   // Запрет работы прерываний
  usbDeviceDisconnect();   // Дисконнект usb
  sei();                   // Разрешение работы прерываний
}

void EasyHID::tick(void) {
  usbPoll();
}

bool EasyHID::isConnected(void) {
  return !!usb_hasCommed;
}

bool EasyHID::isNumLock(void) {
  return !!(led_state & LED_NUM_MSK);
}

bool EasyHID::isCapsLock(void) {
  return !!(led_state & LED_CAPS_MSK);
}

bool EasyHID::isScrollLock(void) {
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

void MouseClass::moveLong(int16_t x, int16_t y) {
  move(x, y);
}

void MouseClass::move(int16_t x, int16_t y) {
  int8_t xDir = 1;
  int8_t yDir = 1;
  if (x < 0) {
    xDir = -1;
    x *= -1;
  };
  if (y < 0) {
    yDir = -1;
    y *= -1;
  };

  while (x > 0 || y > 0) {
    int8_t nx = (int8_t)x, ny = (int8_t)y;
    if (x >= 127) {
      nx = 127;
      x -= 127;
    } else x -= nx;
    if (y >= 127) {
      ny = 127;
      y -= 127;
    } else y -= ny;
    _move(nx * xDir, ny * yDir);
  }
}

void MouseClass::_move(int8_t x, int8_t y) {
  report_buffer[0] = REPID_MOUSE;
  report_buffer[1] = btnBuffer;
  report_buffer[2] = (int8_t)x;
  report_buffer[3] = (int8_t)y;
  usbReportSend(REPSIZE_MOUSE);
}

void MouseClass::press(uint8_t btn) {
  btnBuffer = btn;
  report_buffer[0] = REPID_MOUSE;
  report_buffer[1] = btnBuffer;
  report_buffer[2] = 0;
  report_buffer[3] = 0;
  usbReportSend(REPSIZE_MOUSE);
}

void MouseClass::releaseAll(void) {
  btnBuffer = 0;
  report_buffer[0] = REPID_MOUSE;
  report_buffer[1] = btnBuffer;
  report_buffer[2] = 0;
  report_buffer[3] = 0;
  usbReportSend(REPSIZE_MOUSE);
}

void MouseClass::click(uint8_t btn) {
  press(btn);
  releaseAll();
  btnBuffer = 0;
}
/*--------------------------------------------------------------------------------------*/

/*---------------------------------------Класс клавиатуры-------------------------------*/  // Нажатие клавиши
void KeyboardClass::press(uint8_t k0, uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4) {     // От 1 до 5 клавиш
  bool flag = (pushKey(k0) | pushKey(k1) | pushKey(k2) | pushKey(k3) | pushKey(k4));        // Проверяем и записываем клавиши в буфер
  if (!flag) return;                                                                        // Если ничего в буфере не поменялось - выходим
  uint8_t mod = 0x00;                                                                       // Байт-модификатор
  for (uint8_t i = 0; i < 5; i++) {                                                         // Проходим по буферу клавиш
    if (keyBuffer[i] >= KEY_LEFT_CONTROL && keyBuffer[i] <= KEY_RIGHT_GUI) {                // Если в буфере есть клавиши - модификаторы (альт, шифт...)
      mod |= (1 << (keyBuffer[i] - KEY_LEFT_CONTROL));                                      // Добавляем бит в модификатор согласно клавише
    }
  }
  report_buffer[0] = REPID_KEYBOARD;      // Репорт ID для клавы
  report_buffer[1] = mod;                 // Модификатор
  for (uint8_t i = 0; i < 5; i++) {       // Проходим по буферу
    report_buffer[i + 3] = keyBuffer[i];  // Переписываем его в буфер на отправку
  }
  usbReportSend(REPSIZE_KEYBOARD);  // Отправляем
}

// Отпускание клавиши
void KeyboardClass::release(uint8_t k0, uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4) {  // От 1 до 5 клавиш
  bool flag = (popKey(k0) | popKey(k1) | popKey(k2) | popKey(k3) | popKey(k4));            // Проверяем и чистим буфер от указаных клавиш
  if (!flag) return;                                                                       // Если ничего в буфере не поменялось - выходим
  uint8_t mod = 0x00;                                                                      // Байт-модификатор
  for (uint8_t i = 0; i < 5; i++) {                                                        // Проходим по буферу клавиш
    if (keyBuffer[i] >= KEY_LEFT_CONTROL && keyBuffer[i] <= KEY_RIGHT_GUI) {               // Если в буфере есть клавиши - модификаторы (альт, шифт...)
      mod |= (1 << (keyBuffer[i] - KEY_LEFT_CONTROL));                                     // Добавляем бит в модификатор согласно клавише
    }
  }
  report_buffer[0] = REPID_KEYBOARD;      // Репорт ID для клавы
  report_buffer[1] = mod;                 // Модификатор
  for (uint8_t i = 0; i < 5; i++) {       // Проходим по буферу
    report_buffer[i + 3] = keyBuffer[i];  // Переписываем его в буфер на отправку
  }
  usbReportSend(REPSIZE_KEYBOARD);  // Отправляем
}

void KeyboardClass::releaseAll(void) {               // Отпускание всех нажатых
  for (uint8_t i = 0; i < 5; i++) keyBuffer[i] = 0;  // Очищаем буфер нажатых клавиш
  report_buffer[0] = REPID_KEYBOARD;                 // Репорт ID для клавы
  report_buffer[1] = 0;                              // Модификатор пуст
  for (uint8_t i = 0; i < 5; i++) {                  // Проходим по буферу
    report_buffer[i + 3] = keyBuffer[i];             // Чистим буфер на отправку
  }
  usbReportSend(REPSIZE_KEYBOARD);  // Отправляем
}

void KeyboardClass::click(uint8_t k0, uint8_t k1, uint8_t k2, uint8_t k3, uint8_t k4) {  // Клик - нажатие + отпускание
  press(k0, k1, k2, k3, k4);                                                             // Жмем
  release(k0, k1, k2, k3, k4);                                                           // Отпускаем
}

/*
   Буфер реализован похожим на стек, push-pop
   Пустые места это нули, занятые - коды клавиш.
   press и release изменяет содержимое буфера,
   после чего обновленный список отправляется хосту.
*/

bool KeyboardClass::pushKey(uint8_t key) {  // Функция проверки и упаковки клавиш в буфер
  if (!key) return false;                   // Нули (не нажатые) не нужны - сразу выходим
  for (uint8_t i = 0; i < 5; i++) {         // Проходим по всему буферу
    if (key == keyBuffer[i]) {              // Если в буфере уже есть такая клавиша
      return false;                         // Просто выходим
    }
  }
  for (uint8_t i = 0; i < 5; i++) {  // Проходим по всему буферу
    if (!keyBuffer[i]) {             // Если нашли пустое место в буфере (ноль)
      keyBuffer[i] = key;            // Пишем туда новую клавишу
      return true;                   // Выходим с почетом
    }
  }
}

bool KeyboardClass::popKey(uint8_t key) {  // Попки! Чистим список нажатых клавиш
  if (!key) return false;                  // Нули сразу пропускаем - выходим
  for (uint8_t i = 0; i < 5; i++) {        // Проходим по всему буферу
    if (key == keyBuffer[i]) {             // Если нашли такую клавишу в буфере
      keyBuffer[i] = 0;                    // Освобождаем место
      return true;                         // Выходим
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

size_t KeyboardClass::write(uint8_t data) {                                 // Отправка ascii символа
  uint8_t modifier, keycode;                                                // Модификатор (шифт) и код клавиши
  asciiToKey(data, (bool)(led_state & LED_CAPS_MSK), &modifier, &keycode);  // Просим перевести ascii в код
  report_buffer[0] = REPID_KEYBOARD;                                        // Репорт ID для клавы
  report_buffer[1] = modifier;                                              // Модификатор
  report_buffer[2] = 0;                                                     // Ничего
  report_buffer[3] = keycode;                                               // Нужная клавиша
  usbReportSend(REPSIZE_KEYBOARD);                                          // Отправляем
  releaseAll();                                                             // Отпускаем
  return 1;
}

#define MOD_SHIFT (KEY_MOD_LEFT_SHIFT)

void asciiToKey(uint8_t data, uint8_t isCaps, uint8_t* mod, uint8_t* key) {
  /*
    Данная функция преобразует ascii символ в scan-код клавиши,
    с учетом caps lock, а так же регистра символов. Включает в
    себя все основные символы, НЕ поддерживает кириллицу
  */
  *key = 0x00;                                            // Код клавиши
  *mod = 0x00;                                            // Модификатор
  if (data >= 'A' && data <= 'Z') {                       // Прописные буквы
    *key = KEY_A + (data - 'A');                          // Получаем код клавиши
    *mod = (isCaps ? 0 : MOD_SHIFT);                      // Если капс нажат - ничего не делаем, не нажат - давим шифт
  } else if (data >= 'a' && data <= 'z') {                // Строчные буквы
    *key = KEY_A + (data - 'a');                          // Получаем код клавиши
    *mod = (isCaps ? MOD_SHIFT : 0);                      // Если капс нажат - давим шифт, не нажат - ничего не делаем
  } else if (data >= '0' && data <= '9') {                // Цифры
    *key = (data != '0' ? KEY_1 + (data - '1') : KEY_0);  // Получаем код клавиши
  } else {                                                // Все остальные символы
    switch (data) {                                       // Выбираем прямо через switch по символу
      case '!':
        *mod = MOD_SHIFT;
        *key = KEY_1;
        break;
      case '@':
        *mod = MOD_SHIFT;
        *key = KEY_2;
        break;
      case '#':
        *mod = MOD_SHIFT;
        *key = KEY_3;
        break;
      case '$':
        *mod = MOD_SHIFT;
        *key = KEY_4;
        break;
      case '%':
        *mod = MOD_SHIFT;
        *key = KEY_5;
        break;
      case '^':
        *mod = MOD_SHIFT;
        *key = KEY_6;
        break;
      case '&':
        *mod = MOD_SHIFT;
        *key = KEY_7;
        break;
      case '*':
        *mod = MOD_SHIFT;
        *key = KEY_8;
        break;
      case '(':
        *mod = MOD_SHIFT;
        *key = KEY_9;
        break;
      case ')':
        *mod = MOD_SHIFT;
        *key = KEY_0;
        break;
      case ' ':
        *key = KEY_SPACE;
        break;
      case '\t':
        *key = KEY_TAB;
        break;
      case '\n':
        *key = KEY_ENTER;
        break;
      case '_':
        *mod = MOD_SHIFT;
      case '-':
        *key = KEY_MINUS;
        break;
      case '+':
        *mod = MOD_SHIFT;
      case '=':
        *key = KEY_EQUAL;
        break;
      case '{':
        *mod = MOD_SHIFT;
      case '[':
        *key = KEY_SQBRAK_LEFT;
        break;
      case '}':
        *mod = MOD_SHIFT;
      case ']':
        *key = KEY_SQBRAK_RIGHT;
        break;
      case '<':
        *mod = MOD_SHIFT;
      case ',':
        *key = KEY_COMMA;
        break;
      case '>':
        *mod = MOD_SHIFT;
      case '.':
        *key = KEY_PERIOD;
        break;
      case '?':
        *mod = MOD_SHIFT;
      case '/':
        *key = KEY_SLASH;
        break;
      case '|':
        *mod = MOD_SHIFT;
      case '\\':
        *key = 0x31;
        break;
      case '"':
        *mod = MOD_SHIFT;
      case '\'':
        *key = 0x34;
        break;
      case ':':
        *mod = MOD_SHIFT;
      case ';':
        *key = 0x33;
        break;
      case '~':
        *mod = MOD_SHIFT;
      case '`':
        *key = 0x35;
        break;
    }
  }
}

/*--------------------------------------------------------------------------------------*/