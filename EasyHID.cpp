#include <Arduino.h>
#include "EasyHID.h"

EasyHID HID;
MouseClass Mouse;
KeyboardClass Keyboard;

void asciiToKey (uint8_t data, uint8_t isCaps, uint8_t* mod, uint8_t* key);

/*---------------------------------------Общий класс HID-----------------------------------*/
void EasyHID::begin(void){	
    pinMode(4, INPUT); 		// Настраиваем пины	
    pinMode(2, INPUT); 
    pinMode(5, OUTPUT);
    
    cli();					// Инициализация V-USB
    PORTB &= ~(_BV(USB_CFG_DMINUS_BIT) | _BV(USB_CFG_DPLUS_BIT)); 
    usbDeviceDisconnect();
    _delay_ms(250);
    usbDeviceConnect(); 	
    usbInit();
    sei();
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


/*---------------------------------------Класс клавиатуры-------------------------------*/
void KeyboardClass::releaseAll(void) {
    for (uint8_t i = 1; i < 8; i++) {
        report_buffer[i] = 0;
    }
    report_buffer[0] = REPID_KEYBOARD;
    usbReportSend(REPSIZE_KEYBOARD);
}

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

void KeyboardClass::press(uint8_t key_0, uint8_t key_1, uint8_t key_2){
    report_buffer[0] = REPID_KEYBOARD;
    report_buffer[3] = key_0;
    report_buffer[4] = key_1;
    report_buffer[5] = key_2;
    usbReportSend(REPSIZE_KEYBOARD);
}

void KeyboardClass::press(uint8_t key_0, uint8_t key_1){
    press(key_0, key_1, 0); 
}

void KeyboardClass::press(uint8_t key){
    press(key, 0, 0); 
}

void KeyboardClass::click(uint8_t key_0, uint8_t key_1, uint8_t key_2){
    press(key_0, key_1, key_2);
    releaseAll();
}

void KeyboardClass::click(uint8_t key_0, uint8_t key_1){
    click(key_0, key_1, 0);
}

void KeyboardClass::click(uint8_t key){
    click(key, 0, 0);
}

size_t KeyboardClass::write(uint8_t data){	
    uint8_t modifier, keycode;
    asciiToKey(data, led_state, &modifier, &keycode);
    report_buffer[0] = REPID_KEYBOARD;
    report_buffer[1] = modifier; 
    report_buffer[2] = 0; 
    report_buffer[3] = keycode;
    usbReportSend(REPSIZE_KEYBOARD);
    releaseAll();
    return 1;
}

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