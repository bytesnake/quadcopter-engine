#include "../../include/quadcopter.h"

qc_serial_t QC_UsbSerial;

FILE QC_UsbSTDOUT = FDEV_SETUP_STREAM(QC_IO_UsbSendByte, NULL, _FDEV_SETUP_WRITE);

inline uint8_t QC_IO_UsbWaitForIO () {
	while (!(UEINTX & ((1<<TXINI)|(1<<RXOUTI))));

	return (UEINTX & (1<<RXOUTI)) == 0;
}

void QC_IO_UsbFlush( uint8_t ep ) {
	QC_IO_USBSETENDPOINT ( ep );
	if ( QC_IO_USBFIFOBYTEUSED() > 0 )
		QC_IO_USBRELEASETX ();
}

uint8_t QC_IO_UsbFifoByteFree( uint8_t ep ) {
	uint8_t size, reg;

	QC_IO_USBSETENDPOINT ( ep );

	reg = QC_DisableISR ( );

	if ( !QC_IO_USBIOALLOWED() )
		size = 0;
	else
		size = 64 - QC_IO_USBFIFOBYTEUSED ();

	QC_RestoreISR ( reg );

	return size;
}

uint16_t QC_IO_UsbRecv( uint8_t ep, void *data, uint16_t count ) {
	uint16_t i;
	uint8_t reg;
	uint8_t *d = (uint8_t*) data;
	
	QC_ResetError ();

	if ( qc_io_usbselectedep == 0 ) {
		QC_SetLastError ( QC_ERROR_IO_WRITE );
		return 0;
	}

	QC_IO_USBSETENDPOINT ( ep );

	reg = QC_DisableISR ( );

	count = i = MIN ( QC_IO_USBFIFOBYTEUSED (), count );

	while ( i-- )
		*d++ = QC_IO_USBRECVBYTE ();

	if ( count != 0 && QC_IO_USBFIFOBYTEUSED() == 0 )
		QC_IO_USBRELEASERX ();
	
	QC_RestoreISR ( reg );

	return count;	
}

uint8_t QC_IO_UsbRecvByte( uint8_t ep ) {
	uint8_t b = 0;
	
	QC_IO_UsbRecv ( ep, &b, 1 );

	return b;
}

uint16_t QC_IO_UsbSend( uint8_t ep, const void *data, uint16_t count ) {
	const uint8_t *d = (const uint8_t*) data;
	uint8_t timeout, n, reg;
	uint16_t i;	


	QC_ResetError ();

	if ( qc_io_usbselectedep == 0 ) {
		QC_SetLastError ( QC_ERROR_IO_READ );
		return 0;
	}
	
	i = count;
	timeout = 250;

	while ( i > 0 ) {
		n = 64;
		n = QC_IO_UsbFifoByteFree( ep );
		if ( n == 0 ) {
			timeout --;

			if ( timeout == 0 ) {
				QC_SetLastError ( QC_ERROR_IO_READ );
				return 0;
			}
			else {
				msleep(1);
				continue;
			}
		}
		else if ( n > i )
			n = i;
		
		i -= n;
	
		QC_IO_USBSETENDPOINT ( ep );
	
		reg = QC_DisableISR ( );

		if ( ep & TRANSFER_ZERO )
			while ( n-- )
				QC_IO_USBSENDBYTE ( 0 );
		else if (ep & TRANSFER_PGM)
			while ( n-- )
				QC_IO_USBSENDBYTE ( pgm_read_byte ( d++ ));
		else {
			while ( n-- )
				QC_IO_USBSENDBYTE ( *d++ );

		}
	
		if ( !QC_IO_USBIOALLOWED () || ((count == 0) && (ep & TRANSFER_RELEASE)) )
			QC_IO_USBRELEASETX ();
		

		QC_RestoreISR ( reg );	

	}

	return count;
}

void QC_IO_UsbSendByte ( uint8_t byte, FILE *stream ) {
	if ( QC_IO_UsbLineInfo.lineState > 0 ) {
		QC_IO_UsbSend ( CDC_TX, &byte, 1 );

		if ( byte == '\n' ) {
			byte = '\r';
			QC_IO_UsbSend ( CDC_TX, &byte, 1 );
		}
	}
}

void QC_IO_UsbControlReset( uint16_t end ) {
	QC_IO_USBSETENDPOINT ( 0 );
	
	qc_io_usbcontrolmark = 0;
	qc_io_usbcontrolend = end;
}

uint16_t QC_IO_UsbControlSend( uint8_t flags, const void *data, uint16_t count ) {
	const uint8_t *d = (const uint8_t*) data;
	uint16_t i;

	i = count;

	QC_ResetError ();

	QC_IO_USBSETENDPOINT ( 0 );

	while ( i -- && qc_io_usbcontrolmark < qc_io_usbcontrolend ) {
		if ( !QC_IO_UsbWaitForIO () ) {
			QC_SetLastError ( QC_ERROR_IO_WRITE );
			return 0;
		}

		if ( flags & TRANSFER_PGM )
			QC_IO_USBSENDBYTE ( pgm_read_byte ( d++ ));
		else
			QC_IO_USBSENDBYTE ( *d++ );
	
		if (!((qc_io_usbcontrolmark + 1) & 0x3F))
			QC_IO_USBCLEARIN ();
	
		qc_io_usbcontrolmark ++;	
	}

	return count;
}

void QC_IO_UsbControlSendByte ( uint8_t byte ) {
	QC_IO_UsbControlSend ( 0, &byte, 1 );
}

uint16_t QC_IO_UsbControlRecv( void *data, uint16_t count ) {
	uint8_t *d = (uint8_t*) data;

	QC_IO_USBSETENDPOINT ( 0 );
	
	// TODO !
	QC_IO_USBWAITOUT ();

	while ( count -- )
		*d++ = QC_IO_USBRECVBYTE ();	
	
	QC_IO_USBCLEAROUT ();

	return count;
}

