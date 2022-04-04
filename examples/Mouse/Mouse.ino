/*
   Пример эмуляции мышки EasyHID
   Список всех доступных клавиш см. в файле Codekeys.h
   
   Если используется активная подтяжка при помощи пина,
   обьявляем "EASYHID_SOFT_DETACH" ПЕРЕД подключением EasyHID.h
   (Раскоментируйте строку ниже)
*/
// #define EASYHID_SOFT_DETACH

#include <EasyHID.h>					            // Либа HID 

void setup() {
  HID.begin();                            // Инициализация USB
}

void loop() {
  static uint8_t count = 0;               // Направление
  static uint32_t timer = millis();       // Миллис - таймер (НЕ DELAY!!!)
  if (millis() - timer >= 1000) {         // Каждые 1000 мс
    timer = millis();
    switch (count) {
      case 0: Mouse.move(100, 0);  break; // Двигаем мышкой на X-Y пикс.
      case 1: Mouse.move(0, 100);  break;
      case 2: Mouse.move(-100, 0); break;
      case 3: Mouse.move(0, -100); break;
    }
    // Mouse.click();                     // Можно кликнуть ЛКМ
    // Mouse.click(MOUSE_MIDDLE);         // Или RIGHT, LEFT, MIDDLE
    if (++count > 3) count = 0;           // Циклический сдвиг от 0 до 3
  }

  HID.tick();   // Вызываем тикер не реже чем каждые 10 мс!
}
