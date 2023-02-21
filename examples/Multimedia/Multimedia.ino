/*
   Пример эмуляции мультимедиа клавиш EasyHID
   
*/

#include <EasyHID.h>					          // Либа HID 	

void setup() {
  HID.begin();                          // Инициализация USB
}

void loop() {
  static uint8_t count = 0;             // Направление
  static uint32_t timer = millis();     // Миллис - таймер (НЕ DELAY!!!)
  if (millis() - timer >= 1000) {       // Каждые 1000 мс
    timer = millis();
    switch (count) {
      case 0: Keyboard.clickMultimediaKey(KEY_VOL_UP);     break; // Громче
      case 1: Keyboard.clickMultimediaKey(KEY_VOL_DOWN);   break; // Тише
      case 2: Keyboard.clickMultimediaKey(KEY_MUTE);       break; // Мьют
      case 3: Keyboard.clickMultimediaKey(KEY_MUTE);       break; // Размьют
    }
    if (++count > 3) count = 0;         // Циклический сдвиг от 0 до 3
  }
  HID.tick();                           // Вызываем тикер не реже чем каждые 10 мс!
}
