#include "../../include/quadcopter.h"

void QC_IO_UsbInit() {
	qc_io_usbselectedep = 0;

	// Aktiviere USB	
	UHWCON = 0x01;
	USBCON = (1<<USBE)|(1<<FRZCLK);
	
	// wir haben ein atmega32
	PLLCSR = 0x12; // 16 Mhz
	
	while ( !(PLLCSR & (1<<PLOCK)) );
	
	msleep ( 1 );
	
	USBCON = ((1<<USBE)|(1<<OTGPADE)); // USB clock
	UDIEN = (1<<EORSTE)|(1<<SOFE); // isr for EOR ( End of rest) and SOF (start of frame)
	UDCON = 0;
}

// Interrupt endpoint 0
ISR ( USB_COM_vect )
{
	QC_IO_USBSETENDPOINT ( 0 );

	if ( !QC_IO_USBRECEIVEDSETUP() )
		return;

	qc_io_usbsetup_t setup;

	uint8_t i = 8;
	uint8_t *d = (uint8_t*) &setup;	
	
	while ( i -- )	
		*d++ = QC_IO_USBRECVBYTE ();

	QC_IO_USBCLEARSETUP ();

	if ( setup.bmRequestType & REQUEST_DEVICETOHOST )
		QC_IO_USBWAITIN ();
	else
		QC_IO_USBCLEARIN ();

	QC_ResetError ();	

	if ( (setup.bmRequestType & REQUEST_TYPE) == REQUEST_STANDARD )
		switch(setup.bRequest) {
			case GET_STATUS:
				QC_IO_USBSENDBYTE ( 0 );
				QC_IO_USBSENDBYTE ( 0 );
				break;
			case SET_ADDRESS:
				QC_IO_USBWAITIN ();
				UDADDR = setup.wValueL | (1<<ADDEN);
				break;
			case GET_DESCRIPTOR:	
				QC_IO_UsbSendDescriptor ( &setup ); //TODO
				break;
			case SET_DESCRIPTOR:
				QC_SetLastError ( QC_ERROR_IO_UNKNOWNCOMMAND );
				break;	
			case GET_CONFIGURATION:
				QC_IO_USBSENDBYTE ( 1 );
				break;
			case SET_CONFIGURATION:
				if ( (setup.bmRequestType & REQUEST_RECIPIENT) == REQUEST_DEVICE ) {
					QC_IO_UsbInitEndpoints ();
					qc_io_usbselectedep = setup.wValueL; // TODO!!
				} else
					QC_SetLastError ( QC_ERROR_IO_UNKNOWNCOMMAND );
				break;
			default:
				break;
		}
	else {
		QC_IO_UsbControlReset( setup.wLength );
		if ( setup.wIndex == CDC_ACM_INTERFACE )
			QC_IO_CDCInit ( &setup );
		else 
			QC_SetLastError ( QC_ERROR_IO_UNKNOWNCOMMAND );
	}

	if( QC_GetLastError () != QC_ERROR_SUCCESS ) 
		QC_IO_USBSTALL ();
	else
		QC_IO_USBCLEARIN ();
}
	
ISR ( USB_GEN_vect )
{
	uint8_t udint_tmp;

	udint_tmp = UDINT;
	UDINT = 0;

	// End of reset
	if ( udint_tmp & (1 << EORSTI) ) {
		QC_IO_UsbInitEndpoint ( 0, EP_TYPE_CONTROL, EP_SINGLE_64 );
		qc_io_usbselectedep = 0;
		UEIENX = 1 << RXSTPE;
	}

	// Start of frame
	if ( udint_tmp & (1 << SOFI) ) {
		QC_IO_UsbFlush ( CDC_TX );
	
		QC_IO_USBSETENDPOINT ( CDC_RX );
	
		uint8_t reg = QC_DisableISR ( );
		uint8_t used = QC_IO_USBFIFOBYTEUSED ();
		QC_RestoreISR ( reg );

		while ( used -- ) {
			uint8_t b = QC_IO_UsbRecvByte ( CDC_RX );
			QC_SerialPutChar ( &QC_UsbSerialIn, b );
		}
	}
}

