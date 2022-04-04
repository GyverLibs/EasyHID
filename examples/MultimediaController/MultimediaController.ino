/*
   Простой мультимедиа пульт, всего 1 энкодер для управления:
    -Поворот влево              volume down
    -Поворот вправо             volume up
    -Поворот влево + нажатие    previous track
    -Поворот влево + нажатие    next track
    -Клик по энкодеру           play / pause
    
   Если используется активная подтяжка при помощи пина,
   обьявляем "EASYHID_SOFT_DETACH" ПЕРЕД подключением EasyHID.h
   (Раскоментируйте строку ниже)
*/
// #define EASYHID_SOFT_DETACH
#define _EB_HOLD  300   // Переопределяем таймаут удержания
#define ENC_CK    6     // CK пин энкодера   
#define ENC_DT    7     // DT пин энкодера    
#define ENC_SW    8     // SW пин энкодера (кнопка) 

#include <EasyHID.h>    // Либа HID  
#include <EncButton.h>  // Либа энкодера

EncButton <EB_TICK, ENC_CK, ENC_DT, ENC_SW> enc;  // Обьект энкодера

void setup() {
  HID.begin();          // Запускаем USB
}

void loop() {
  if (enc.tick()) {
    if (enc.isClick())  Keyboard.clickMultimediaKey(KEY_PLAYPAUSE);           // Клик по кнопке - playpause
    else if (enc.isRight())  Keyboard.clickMultimediaKey(KEY_VOL_UP);         // Вращение вправо - vol+
    else if (enc.isLeft())   Keyboard.clickMultimediaKey(KEY_VOL_DOWN);       // Вращение влево - vol-
    else if (enc.isRightH()) Keyboard.clickMultimediaKey(KEY_NEXT_TRACK);     // Вращение вправо с нажатием - next track
    else if (enc.isLeftH())  Keyboard.clickMultimediaKey(KEY_PREV_TRACK);     // Вращение влево с нажатием - previous track
  }
  HID.tick();                                                                 // Вызываем тикер не реже чем каждые 10 мс!
}
