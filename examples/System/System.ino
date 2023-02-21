/*
   Пример эмуляции системных клавиш EasyHID
   Список всех доступных клавиш см. в файле Codekeys.h
   Библиотека для энкодеров/кнопок: https://github.com/GyverLibs/EncButton

*/

#define BTN_PIN 6 // Пин кнопки

#include <EasyHID.h>
#include <EncButton.h>

EncButton <EB_TICK, BTN_PIN> btn;       // Обьект кнопки

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);       // Подтяжка кнопки
  HID.begin();                          // Инициализация USB
}

void loop() {
  /* KEY_SLEEP - перевести в сон, KEY_POWER - завершение работы */
  
  if (btn.click()) {                    // По клику
    Keyboard.clickSystemKey(KEY_POWER); // Выключаем компьютер
  }
  btn.tick();                           // Опрашиваем кнопку
  HID.tick();                           // Вызываем тикер не реже чем каждые 10 мс!
}
