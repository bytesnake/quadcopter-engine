#ifndef QC_USB_H
#define QC_USB_H

void QC_IO_UsbInit();

extern qc_serial_t QC_UsbSerialIn;
extern qc_serial_t QC_UsbSerialOut;
extern FILE QC_UsbSTDOUT;
extern FILE QC_UsbSTDIN;

// Basic
#define QC_IO_USBWAITIN() while (!(UEINTX & (1<<TXINI)))
#define QC_IO_USBWAITOUT() while (!(UEINTX & (1<<RXOUTI)))
#define QC_IO_USBCLEARIN() UEINTX = ~(1<<TXINI)
#define QC_IO_USBCLEAROUT() UEINTX = ~(1<<RXOUTI)
#define QC_IO_USBSENDBYTE(byte) UEDATX = byte
#define QC_IO_USBRECVBYTE() UEDATX
#define QC_IO_USBRELEASERX() UEINTX = 0x6B
#define QC_IO_USBRELEASETX() UEINTX = 0x3A
#define QC_IO_USBIOALLOWED() UEINTX & (1<<RWAL)
#define QC_IO_USBFIFOBYTEUSED() UEBCLX
#define QC_IO_USBSETENDPOINT(ep) UENUM = ep
#define QC_IO_USBRECEIVEDSETUP() UEINTX & (1<<RXSTPI)
#define QC_IO_USBCLEARSETUP() UEINTX = ~((1<<RXSTPI) | (1<<RXOUTI) | (1<<TXINI))
#define QC_IO_USBSTALL() UECONX = (1<<STALLRQ) | (1<<EPEN)

inline uint8_t QC_IO_UsbFifoByteFree ( uint8_t ep );
inline void QC_IO_UsbFlush ( uint8_t ep );

// ungesteurtes IO
uint8_t QC_IO_UsbRecvByte ( uint8_t ep ); 
uint16_t QC_IO_UsbRecv ( uint8_t ep, void *data, uint16_t count );
void QC_IO_UsbSendByte ( uint8_t byte, FILE *stream );
uint16_t QC_IO_UsbSend ( uint8_t ep, const void *data, uint16_t count );

// kontrolliertes IO
uint16_t qc_io_usbcontrolmark;
uint16_t qc_io_usbcontrolend;

void QC_IO_UsbControlReset ( uint16_t end );
void QC_IO_UsbControlSendByte ( uint8_t b );
uint16_t QC_IO_UsbControlSend ( uint8_t flags, const void *data, uint16_t count );
uint16_t QC_IO_UsbControlRecv ( void *data, uint16_t count );

// Funktionen

volatile uint8_t qc_io_usbselectedep;

void QC_IO_UsbInitEndpoint ( uint8_t index, uint8_t type, uint8_t size );
void QC_IO_UsbInitEndpoints ();

void QC_IO_UsbSendStringDescriptor ( const uint8_t *string, uint8_t length );
void QC_IO_UsbSendConfiguration ( uint16_t maxlen );
void QC_IO_UsbSendDescriptor ( qc_io_usbsetup_t *setup );

#endif
