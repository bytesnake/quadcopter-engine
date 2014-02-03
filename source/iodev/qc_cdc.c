#include "../../include/quadcopter.h"

#define WEAK __attribute__ ((weak))

const qc_io_cdcdescriptor_t QC_IO_CDCInterfaces =
{
        D_IAD(0,2,CDC_COMMUNICATION_INTERFACE_CLASS,CDC_ABSTRACT_CONTROL_MODEL,1),

        //      CDC communication interface
        D_INTERFACE(CDC_ACM_INTERFACE,1,CDC_COMMUNICATION_INTERFACE_CLASS,CDC_ABSTRACT_CONTROL_MODEL,0),
        D_CDCCS(CDC_HEADER,0x10,0x01),                                                          // Header (1.10 bcd)
        D_CDCCS(CDC_CALL_MANAGEMENT,1,1),                                                       // Device handles call management (not)
        D_CDCCS4(CDC_ABSTRACT_CONTROL_MANAGEMENT,6),                            // SET_LINE_CODING, GET_LINE_CODING, SET_CONTROL_LINE_STATE supported
        D_CDCCS(CDC_UNION,CDC_ACM_INTERFACE,CDC_DATA_INTERFACE),        // Communication interface is master, data interface is slave 0
        D_ENDPOINT(USB_ENDPOINT_IN (CDC_ENDPOINT_ACM),USB_ENDPOINT_TYPE_INTERRUPT,0x10,0x40),

        //      CDC data interface
        D_INTERFACE(CDC_DATA_INTERFACE,2,CDC_DATA_INTERFACE_CLASS,0,0),
        D_ENDPOINT(USB_ENDPOINT_OUT(CDC_ENDPOINT_OUT),USB_ENDPOINT_TYPE_BULK,0x40,0),
        D_ENDPOINT(USB_ENDPOINT_IN (CDC_ENDPOINT_IN ),USB_ENDPOINT_TYPE_BULK,0x40,0)
};

volatile qc_io_usblineinfo_t QC_IO_UsbLineInfo = { 57600, 0x00, 0x00, 0x00, 0x00 };

void WEAK QC_IO_CDCInit( qc_io_usbsetup_t *setup ) { 

	QC_ResetError ();
	
	if ( setup->bmRequestType == REQUEST_DEVICETOHOST_CLASS_INTERFACE ) {
		if ( setup->bRequest == CDC_GET_LINE_CODING ) {
			QC_IO_UsbControlSend ( 0, (const void*) &QC_IO_UsbLineInfo, 7 );
			return;
		}	
	}

	if ( setup->bmRequestType == REQUEST_HOSTTODEVICE_CLASS_INTERFACE ) {
		if ( setup->bRequest == CDC_SET_LINE_CODING ) {
			QC_IO_UsbControlRecv ( (void*) &QC_IO_UsbLineInfo, 7 );
			return;
		}
		else if ( setup->bRequest == CDC_SET_CONTROL_LINE_STATE )
		{
			QC_IO_UsbLineInfo.lineState = setup->wValueL;

			QC_IO_SendOutputBuffer ();
		
			if ( QC_IO_UsbLineInfo.dwDTERate == 1200 ) {
				if ((QC_IO_UsbLineInfo.lineState & 0x01) == 0) {
					*(uint16_t *)0x0800 = 0x7777;
					wdt_enable(WDTO_120MS);
				} else {
					wdt_disable ();
					wdt_reset ();
				
					*(uint16_t *)0x0800 = 0x0;
				}
			}
			return ;
		}	
	}

	QC_SetLastError ( QC_ERROR_CDCINIT );
}
