#include "../../include/quadcopter.h"

const uint8_t QC_IO_UsbEndpoints[] =
{
        0,
        EP_TYPE_INTERRUPT_IN,
        EP_TYPE_BULK_OUT,
        EP_TYPE_BULK_IN
};

const uint16_t STRING_LANGUAGE[2] = {
        (3<<8) | (2+2),
        0x0409  // English
};

const uint8_t STRING_PRODUCT[] PROGMEM = "USB IO BOARD"; //TODO
const uint8_t STRING_MANUFACTURER[] PROGMEM = "Arduino Yun";

const qc_io_usbdevicedesc_t QC_IO_UsbDeviceDescA =
        D_DEVICE(0x00,0x00,0x00,64,USB_VID,USB_PID,0x100,IMANUFACTURER,IPRODUCT,0,1);

const qc_io_usbdevicedesc_t QC_IO_UsbDeviceDescB =
        D_DEVICE(0x02,0x00,0x00,64,USB_VID,USB_PID,0x100,IMANUFACTURER,IPRODUCT,0,1);

FILE QC_IO_UsbSTDOUT = FDEV_SETUP_STREAM(QC_IO_UsbSendByte, NULL, _FDEV_SETUP_WRITE);

void QC_IO_UsbInitEndpoint( uint8_t index, uint8_t type, uint8_t size ) {
	QC_IO_USBSETENDPOINT ( index );
	UECONX = 1;
	UECFG0X = type;
	UECFG1X = size;
}

void QC_IO_UsbInitEndpoints() {
	uint8_t i;
	for ( i = 1; i < sizeof(QC_IO_UsbEndpoints); i++ )
		QC_IO_UsbInitEndpoint ( i, pgm_read_byte(QC_IO_UsbEndpoints+i), EP_DOUBLE_64 );

	UERST = 0x7E;
	UERST = 0;
}

// Send a USB descriptor string.
void QC_IO_UsbSendStringDescriptor ( const uint8_t *string, uint8_t length ) {
	uint8_t i;

	QC_IO_UsbControlSendByte ( 2 + length * 2 );
	QC_IO_UsbControlSendByte ( 3 );
	
	for ( i = 0; i < length; i++ ) {
		QC_IO_UsbControlSendByte ( pgm_read_byte ( &string[i] ) );
		QC_IO_UsbControlSendByte ( 0 ); // high byte
		
		if ( QC_GetLastError () != QC_ERROR_SUCCESS )
			return;
	}
}

void QC_IO_UsbSendConfiguration ( uint16_t maxlen ) {
	uint8_t interfaces;
	qc_io_usbconfigdesc_t config;

	config = (qc_io_usbconfigdesc_t) D_CONFIG ( sizeof(qc_io_usbconfigdesc_t) + sizeof(qc_io_cdcdescriptor_t), 2 );

	// Send them
	QC_IO_UsbControlReset ( maxlen );

	QC_IO_UsbControlSend ( 0, &config, sizeof ( qc_io_usbconfigdesc_t ) );
	QC_IO_UsbControlSend ( 0, &QC_IO_CDCInterfaces, sizeof ( QC_IO_CDCInterfaces ));
}

uint8_t tmp = 0;

void QC_IO_UsbSendDescriptor ( qc_io_usbsetup_t *setup ) {
	const uint8_t* desc_addr = 0;
	uint8_t desc_length;

	if ( setup->wValueH == USB_CONFIGURATION_DESCRIPTOR_TYPE ) {
		QC_IO_UsbSendConfiguration ( setup->wLength );
		return;
	}
	
	QC_IO_UsbControlReset ( setup->wLength );

	desc_addr = 0;
	if ( setup->wValueH == USB_DEVICE_DESCRIPTOR_TYPE ) {
		if ( setup->wLength == 8 )
			tmp = 1;
		desc_addr = (tmp ? (const uint8_t*)&QC_IO_UsbDeviceDescB : (const uint8_t*)&QC_IO_UsbDeviceDescA );
	}
	else if ( setup->wValueH == USB_STRING_DESCRIPTOR_TYPE )
		if ( setup->wValueL == 0 )
			desc_addr = (const uint8_t*) &STRING_LANGUAGE;
		else if ( setup->wValueL == IPRODUCT )
			return QC_IO_UsbSendStringDescriptor ( STRING_PRODUCT, strlen ( STRING_PRODUCT ) );
		else if ( setup->wValueL == IMANUFACTURER )
			return QC_IO_UsbSendStringDescriptor ( STRING_MANUFACTURER, strlen ( STRING_MANUFACTURER ) );
		else
			QC_SetLastError ( QC_ERROR_IO_UNKNOWNCOMMAND );
	
	if ( desc_addr == 0 )
		QC_SetLastError ( QC_ERROR_IO_UNKNOWNCOMMAND );

	desc_length = pgm_read_byte ( desc_addr );
	
	QC_IO_UsbControlSend ( TRANSFER_PGM, desc_addr, desc_length );
}	
