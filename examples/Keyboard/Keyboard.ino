#include <EasyHID.h>

/*
   Пример эмуляции клавиатуры EasyHID
   Список всех доступных клавиш см. в файле Codekeys.h
*/

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
