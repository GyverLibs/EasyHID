#include <EasyHID.h>

/*
   Пример эмуляции системных клавиш EasyHID
   Список всех доступных клавиш см. в файле Codekeys.h
*/

void setup() {
  HID.begin();                          // Инициализация USB
}

void loop() {
  static uint32_t timer = millis();     // Миллис - таймер (НЕ DELAY!!!)
  if (millis() - timer >= 5000) {       // Спустя 5 секунд после включения
    Keyboard.clickSystemKey(KEY_POWER); // Выключаем компьютер
  }

  HID.tick();   // Вызываем тикер не реже чем каждые 10 мс!
}
