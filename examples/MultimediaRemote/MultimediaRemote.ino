/*
   Данный пример позволяет удаленно, при помощи ИК пульта с протоколом NEC (большинство китайских Arduino ИК пультов)
   управлять поведением мультимедиа на ПК. Остановка и воспроизведение, громкость, переключение треков и т.д.

   В чем проблема и как какать?
   Библиотека V-USB на которой основана EasyHID, использует аппаратное прерывание INT0, библиотеке ИК пульта тоже
   необходимо аппаратное прерывание (например INT1) для приема пакетов. Однако ардуиновский attachInterrupt занимает
   сразу все аппаратные прерывания, и даже если использовать свободное 1е прерывание - компиляция приведет к ошибке.

   Данный пример реализует свободное прерывание INT1 "напрямую", минуя ардуиновский attachInterrupt.
   Пример подойдет для ATmega328p / 168p (Плат Arduino NANO/UNO/MINI/PRO MINI),
   а так же nano-подобных плат на базе ATtiny88.

   Необходима библиотека NecDecoder: https://github.com/GyverLibs/NecDecoder
   Команды кнопок необходимо заранее считать и заменить на свои, чтобы все работало корректно.

   В случае и с первой и второй платой, DATA пин ИК приемника подключается к D3 (INT1)

   Если используется активная подтяжка при помощи пина,
   обьявляем "EASYHID_SOFT_DETACH" ПЕРЕД подключением EasyHID.h
   (Раскоментируйте строку ниже)
*/

// #define EASYHID_SOFT_DETACH

// Команды пульта - 8ми битные команды, считываемые методом ir.readCommand()!
#define IR_CMD_STOP       0xB0  // STOP     
#define IR_CMD_MUTE       0x68  // MUTE 
#define IR_CMD_NEXT       0x5A  // NEXT TRACK  
#define IR_CMD_PREV       0x10  // PREVIOUS TRACK    
#define IR_CMD_VOL_UP     0x18  // VOLUME UP  
#define IR_CMD_VOL_DOWN   0x4A  // VOLUME DOWN 
#define IR_CMD_PLAYPAUSE  0x38  // PLAY / PAUSE  

#include <EasyHID.h>      // Либа EasyHID
#include <NecDecoder.h>   // Либа NecDecoder
NecDecoder ir;            // Обьект декодера NEC

void setup() {
  HID.begin();            // Заводим USB
  EICRA |= FALLING << 2;  // Настраиваем режим прерывания INT1 (FALLING)
  EIMSK |= 1 << INT1;     // Активируем прерывание INT1
}

void loop() {       
  if (ir.available()) {                                                             // Как только поймана команда
    switch (ir.readCommand()) {                                                     // Читаем команду и подставляем в switch
      case IR_CMD_STOP: Keyboard.clickMultimediaKey(KEY_STOP); break;               // STOP
      case IR_CMD_MUTE: Keyboard.clickMultimediaKey(KEY_MUTE); break;               // MUTE
      case IR_CMD_NEXT: Keyboard.clickMultimediaKey(KEY_NEXT_TRACK); break;    		// NEXT
      case IR_CMD_PREV: Keyboard.clickMultimediaKey(KEY_PREV_TRACK); break;    		// PREV
      case IR_CMD_VOL_UP: Keyboard.clickMultimediaKey(KEY_VOL_UP); break;           // VOL+
      case IR_CMD_VOL_DOWN: Keyboard.clickMultimediaKey(KEY_VOL_DOWN); break;       // VOL-
      case IR_CMD_PLAYPAUSE: Keyboard.clickMultimediaKey(KEY_PLAYPAUSE); break;     // PLAY/PAUSE
    }
  }
  HID.tick();                                                                       // Обслуживаем HID
}

ISR(INT1_vect) {            // Вектор прерывания INT1         
  ir.tick();                // Обслуживаем ИК приемник 
}
