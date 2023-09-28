#ifndef _HIDPrivate_h
#define _HIDPrivate_h

#ifdef __cplusplus
extern "C" {
#endif

#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/power.h>
#include <stdint.h>
#include <util/delay.h>

#include "usbconfig.h"
#include "usbdrv/usbdrv.h"

extern char usb_hasCommed;
extern uint8_t led_state;
extern uint8_t report_buffer[8];

void usbReportSend(uint8_t sz);

#define REPID_MOUSE 1
#define REPID_KEYBOARD 2
#define REPID_MM_KEY 3
#define REPID_SYS_KEY 4

#define REPSIZE_MOUSE 4
#define REPSIZE_MM_KEY 3
#define REPSIZE_SYS_KEY 2
#define REPSIZE_KEYBOARD 8

#ifdef __cplusplus
}
#endif

#endif