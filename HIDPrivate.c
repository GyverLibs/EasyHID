#include "HIDPrivate.h"

uint8_t report_buffer[8];
char usb_hasCommed = 0;
uint8_t idle_rate = 500 / 4;  	// see HID1_11.pdf sect 7.2.4
uint8_t protocol_version = 0; 	// see HID1_11.pdf sect 7.2.6
uint8_t led_state = 0; 			// caps/num/scroll lock LEDs

void usbReportSend(uint8_t sz)
{
    // perform usb background tasks until the report can be sent, then send it
    while (1)
    {
        usbPoll(); // this needs to be called at least once every 10 ms
        if (usbInterruptIsReady())
        {
            usbSetInterrupt((uint8_t*)report_buffer, sz); // send
            break;

            // see http://vusb.wikidot.com/driver-api
        }
    }
}

// USB HID report descriptor for boot protocol keyboard
// see HID1_11.pdf appendix B section 1
// USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH is defined in usbconfig (should be 173)
const PROGMEM char usbHidReportDescriptor[USB_CFG_HID_REPORT_DESCRIPTOR_LENGTH] = {
    0x05, 0x01,           // USAGE_PAGE (Generic Desktop)
    0x09, 0x02,           // USAGE (Mouse)
    0xa1, 0x01,           // COLLECTION (Application)
    0x09, 0x01,           //   USAGE (Pointer)
    0xA1, 0x00,           //   COLLECTION (Physical)
    0x85, REPID_MOUSE,    //     REPORT_ID
    0x05, 0x09,           //     USAGE_PAGE (Button)
    0x19, 0x01,           //     USAGE_MINIMUM
    0x29, 0x03,           //     USAGE_MAXIMUM
    0x15, 0x00,           //     LOGICAL_MINIMUM (0)
    0x25, 0x01,           //     LOGICAL_MAXIMUM (1)
    0x95, 0x03,           //     REPORT_COUNT (3)
    0x75, 0x01,           //     REPORT_SIZE (1)
    0x81, 0x02,           //     INPUT (Data,Var,Abs)
    0x95, 0x01,           //     REPORT_COUNT (1)
    0x75, 0x05,           //     REPORT_SIZE (5)
    0x81, 0x03,           //     INPUT (Const,Var,Abs)
    0x05, 0x01,           //     USAGE_PAGE (Generic Desktop)
    0x09, 0x30,           //     USAGE (X)
    0x09, 0x31,           //     USAGE (Y)
    0x15, 0x81,           //     LOGICAL_MINIMUM (-127)
    0x25, 0x7F,           //     LOGICAL_MAXIMUM (127)
    0x75, 0x08,           //     REPORT_SIZE (8)
    0x95, 0x02,           //     REPORT_COUNT (2)
    0x81, 0x06,           //     INPUT (Data,Var,Rel)
    0xC0,                 //   END_COLLECTION
    0xC0,                 // END COLLECTION

    0x05, 0x01,           // USAGE_PAGE (Generic Desktop)
    0x09, 0x06,           // USAGE (Keyboard)
    0xA1, 0x01,           // COLLECTION (Application)
    0x85, REPID_KEYBOARD, // REPORT_ID
    0x75, 0x01,           //   REPORT_SIZE (1)
    0x95, 0x08,           //   REPORT_COUNT (8)
    0x05, 0x07,           //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0xE0,           //   USAGE_MINIMUM (Keyboard LeftControl)(224)
    0x29, 0xE7,           //   USAGE_MAXIMUM (Keyboard Right GUI)(231)
    0x15, 0x00,           //   LOGICAL_MINIMUM (0)
    0x25, 0x01,           //   LOGICAL_MAXIMUM (1)
    0x81, 0x02,           //   INPUT (Data,Var,Abs) ; Modifier byte
    0x95, 0x01,           //   REPORT_COUNT (1)
    0x75, 0x08,           //   REPORT_SIZE (8)
    0x81, 0x03,           //   INPUT (Cnst,Var,Abs) ; Reserved byte
    0x95, 0x05,           //   REPORT_COUNT (5)
    0x75, 0x01,           //   REPORT_SIZE (1)
    0x05, 0x08,           //   USAGE_PAGE (LEDs)
    0x19, 0x01,           //   USAGE_MINIMUM (Num Lock)
    0x29, 0x05,           //   USAGE_MAXIMUM (Kana)
    0x91, 0x02,           //   OUTPUT (Data,Var,Abs) ; LED report
    0x95, 0x01,           //   REPORT_COUNT (1)
    0x75, 0x03,           //   REPORT_SIZE (3)
    0x91, 0x03,           //   OUTPUT (Cnst,Var,Abs) ; LED report padding
    0x95, 0x05,           //   REPORT_COUNT (5)
    0x75, 0x08,           //   REPORT_SIZE (8)
    0x15, 0x00,           //   LOGICAL_MINIMUM (0)
    0x26, 0xA4, 0x00,     //   LOGICAL_MAXIMUM (164)
    0x05, 0x07,           //   USAGE_PAGE (Keyboard)(Key Codes)
    0x19, 0x00,           //   USAGE_MINIMUM (Reserved (no event indicated))(0)
    0x2A, 0xA4, 0x00,     //   USAGE_MAXIMUM (Keyboard Application)(164)
    0x81, 0x00,           //   INPUT (Data,Ary,Abs)
    0xC0,                 // END_COLLECTION

    // this second multimedia key report is what handles the multimedia keys
    0x05, 0x0C,           // USAGE_PAGE (Consumer Devices)
    0x09, 0x01,           // USAGE (Consumer Control)
    0xA1, 0x01,           // COLLECTION (Application)
    0x85, REPID_MM_KEY,    //   REPORT_ID
    0x19, 0x00,           //   USAGE_MINIMUM (Unassigned)
    0x2A, 0x3C, 0x02,     //   USAGE_MAXIMUM
    0x15, 0x00,           //   LOGICAL_MINIMUM (0)
    0x26, 0x3C, 0x02,     //   LOGICAL_MAXIMUM
    0x95, 0x01,           //   REPORT_COUNT (1)
    0x75, 0x10,           //   REPORT_SIZE (16)
    0x81, 0x00,           //   INPUT (Data,Ary,Abs)
    0xC0,                 // END_COLLECTION

    // system controls, like power, needs a 3rd different report and report descriptor
    0x05, 0x01,             // USAGE_PAGE (Generic Desktop)
    0x09, 0x80,             // USAGE (System Control)
    0xA1, 0x01,             // COLLECTION (Application)
    0x85, REPID_SYS_KEY, 	//   REPORT_ID
    0x95, 0x01,             //   REPORT_COUNT (1)
    0x75, 0x02,             //   REPORT_SIZE (2)
    0x15, 0x01,             //   LOGICAL_MINIMUM (1)
    0x25, 0x03,             //   LOGICAL_MAXIMUM (3)
    0x09, 0x82,             //   USAGE (System Sleep)
    0x09, 0x81,             //   USAGE (System Power)
    0x09, 0x83,             //   USAGE (System Wakeup)
    0x81, 0x60,             //   INPUT
    0x75, 0x06,             //   REPORT_SIZE (6)
    0x81, 0x03,             //   INPUT (Cnst,Var,Abs)
    0xC0,                   // END_COLLECTION
};

usbMsgLen_t usbFunctionSetup(uint8_t data[8])
{
    usb_hasCommed = 1;

    // see HID1_11.pdf sect 7.2 and http://vusb.wikidot.com/driver-api
    usbRequest_t *rq = (void *)data;

    if ((rq->bmRequestType & USBRQ_TYPE_MASK) != USBRQ_TYPE_CLASS)
    return 0; // ignore request if it's not a class specific request

    // see HID1_11.pdf sect 7.2
    switch (rq->bRequest)
    {
    case USBRQ_HID_GET_IDLE:
        usbMsgPtr = &idle_rate; // send data starting from this byte
        return 1; // send 1 byte
    case USBRQ_HID_SET_IDLE:
        idle_rate = rq->wValue.bytes[1]; // read in idle rate
        return 0; // send nothing
    case USBRQ_HID_GET_PROTOCOL:
        usbMsgPtr = &protocol_version; // send data starting from this byte
        return 1; // send 1 byte
    case USBRQ_HID_SET_PROTOCOL:
        protocol_version = rq->wValue.bytes[1];
        return 0; // send nothing
    case USBRQ_HID_GET_REPORT:
        usbMsgPtr = (uint8_t*)&report_buffer; // send the report data
        report_buffer[0] = rq->wValue.bytes[0];
        report_buffer[1] = report_buffer[2] = report_buffer[3] = report_buffer[4] = report_buffer[5] = report_buffer[6] = report_buffer[7] = 0; // clear the report
        // determine the return data length based on which report ID was requested
        if (rq->wValue.bytes[0] == REPID_MOUSE)      return REPSIZE_MOUSE;
        if (rq->wValue.bytes[0] == REPID_KEYBOARD)   return REPSIZE_KEYBOARD;
        if (rq->wValue.bytes[0] == REPID_MM_KEY)     return REPSIZE_MM_KEY;
        if (rq->wValue.bytes[0] == REPID_SYS_KEY) 	 return REPSIZE_SYS_KEY;
        return 8; // default
    case USBRQ_HID_SET_REPORT:
        if (rq->wLength.word == 1) // check data is available
        {
            // 1 byte, we don't check report type (it can only be output or feature)
            // we never implemented "feature" reports so it can't be feature
            // so assume "output" reports
            // this means set LED status
            // since it's the only one in the descriptor
            return USB_NO_MSG; // send nothing but call usbFunctionWrite
        }
        else // no data or do not understand data, ignore
        {
            return 0; // send nothing
        }
    default: // do not understand data, ignore
        return 0; // send nothing
    }
}

// see http://vusb.wikidot.com/driver-api
usbMsgLen_t usbFunctionWrite(uint8_t * data, uchar len)
{
    led_state = data[0];
    return 1; // 1 byte read
}

