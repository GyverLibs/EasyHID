#include <EasyHID.h>
#include <GyverButton.h>
#include <GyverEncoder.h>

/*
   Простой мультимедиа пульт
   Используем энкодер и кнопку
*/

#define BTN_PIN 6   // Пин кнопки
#define ENC_CK  7   // Пины энкодера
#define ENC_DT  8
#define ENC_SW  9

GButton btn (BTN_PIN);                  // Объект кнопки
Encoder enc (ENC_CK, ENC_DT, ENC_SW);   // Объект энкодера

void setup() {
  pinMode(BTN_PIN, INPUT_PULLUP);       // Подтяжка кнопки
  HID.begin();                          // Инициализируем USB
}

void loop() {
  btn.tick(); // Опрос кнопки
  enc.tick(); // Опрос энкодера
  HID.tick(); // Опрос USB

  if (btn.isClick()) Keyboard.clickMultimediaKey(KEY_PLAYPAUSE);  // Клик по кнопке
  if (enc.isRight()) Keyboard.clickMultimediaKey(KEY_VOL_UP);     // Вращение вправо
  if (enc.isLeft())  Keyboard.clickMultimediaKey(KEY_VOL_DOWN);   // Вращение влево
  if (enc.isClick()) Keyboard.clickMultimediaKey(KEY_MUTE);       // Клик по энкодеру
}
