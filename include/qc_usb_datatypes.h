#ifndef QC_USB_DATATYPES
#define QC_USB_DATATYPES

#include "qc_datatypes.h"

#define D_DEVICE(_class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs) \
        { 18, 1, 0x200, _class,_subClass,_proto,_packetSize0,_vid,_pid,_version,_im,_ip,_is,_configs }

#define D_CONFIG(_totalLength,_interfaces) { 9, 2, _totalLength,_interfaces, 1, 0, USB_CONFIG_BUS_POWERED, USB_CONFIG_POWER_MA(500) }

#define D_INTERFACE(_n,_numEndpoints,_class,_subClass,_protocol) \
        { 9, 4, _n, 0, _numEndpoints, _class,_subClass, _protocol, 0 }

#define D_ENDPOINT(_addr,_attr,_packetSize, _interval) \
        { 7, 5, _addr,_attr,_packetSize, _interval }

#define D_IAD(_firstInterface, _count, _class, _subClass, _protocol) \
        { 8, 11, _firstInterface, _count, _class, _subClass, _protocol, 0 }

#define D_HIDREPORT(_descriptorLength) \
        { 9, 0x21, 0x1, 0x1, 0, 1, 0x22, _descriptorLength, 0 }

#define D_CDCCS(_subtype,_d0,_d1)       { 5, 0x24, _subtype, _d0, _d1 }
#define D_CDCCS4(_subtype,_d0)          { 4, 0x24, _subtype, _d0 }

//	Device
typedef struct {
	uint8_t len;				// 18
	uint8_t dtype;			// 1 USB_DEVICE_DESCRIPTOR_TYPE
	uint16_t usbVersion;		// 0x200
	uint8_t	deviceClass;
	uint8_t	deviceSubClass;
	uint8_t	deviceProtocol;
	uint8_t	packetSize0;	// Packet 0
	uint16_t	idVendor;
	uint16_t	idProduct;
	uint16_t	deviceVersion;	// 0x100
	uint8_t	iManufacturer;
	uint8_t	iProduct;
	uint8_t	iSerialNumber;
	uint8_t	bNumConfigurations;
} qc_io_usbdevicedesc_t;

//	Config
typedef struct {
	uint8_t	len;			// 9
	uint8_t	dtype;			// 2
	uint16_t clen;			// total length
	uint8_t	numInterfaces;
	uint8_t	config;
	uint8_t	iconfig;
	uint8_t	attributes;
	uint8_t	maxPower;
} qc_io_usbconfigdesc_t;

//	String

//	Interface
typedef struct
{
	uint8_t len;		// 9
	uint8_t dtype;	// 4
	uint8_t number;
	uint8_t alternate;
	uint8_t numEndpoints;
	uint8_t interfaceClass;
	uint8_t interfaceSubClass;
	uint8_t protocol;
	uint8_t iInterface;
} InterfaceDescriptor;

//	Endpoint
typedef struct
{
	uint8_t len;		// 7
	uint8_t dtype;	// 5
	uint8_t addr;
	uint8_t attr;
	uint16_t packetSize;
	uint8_t interval;
} EndpointDescriptor;

typedef struct
{
        uint8_t bmRequestType;
        uint8_t bRequest;
        uint8_t wValueL;
        uint8_t wValueH;
        uint16_t wIndex;
        uint16_t wLength;
} qc_io_usbsetup_t;

// Interface Association Descriptor
// Used to bind 2 interfaces together in CDC compostite device
typedef struct
{
	uint8_t len;				// 8
	uint8_t dtype;			// 11
	uint8_t firstInterface;
	uint8_t interfaceCount;
	uint8_t functionClass;
	uint8_t funtionSubClass;
	uint8_t functionProtocol;
	uint8_t iInterface;
} IADDescriptor;

//	CDC CS interface descriptor
typedef struct
{
	uint8_t len;		// 5
	uint8_t dtype;	// 0x24
	uint8_t subtype;
	uint8_t d0;
	uint8_t d1;
} CDCCSInterfaceDescriptor;

typedef struct
{
	uint8_t len;		// 4
	uint8_t dtype;	// 0x24
	uint8_t subtype;
	uint8_t d0;
} CDCCSInterfaceDescriptor4;

typedef struct 
{
    uint8_t	len;
    uint8_t 	dtype;		// 0x24
    uint8_t 	subtype;	// 1
    uint8_t 	bmCapabilities;
    uint8_t 	bDataInterface;
} CMFunctionalDescriptor;
	
typedef struct 
{
    uint8_t	len;
    uint8_t 	dtype;		// 0x24
    uint8_t 	subtype;	// 1
    uint8_t 	bmCapabilities;
} ACMFunctionalDescriptor;

typedef struct 
{
	//	IAD
	IADDescriptor				iad;	// Only needed on compound device

	//	Control
	InterfaceDescriptor			cif;	// 
	CDCCSInterfaceDescriptor	header;
	CMFunctionalDescriptor		callManagement;			// Call Management
	ACMFunctionalDescriptor		controlManagement;		// ACM
	CDCCSInterfaceDescriptor	functionalDescriptor;	// CDC_UNION
	EndpointDescriptor			cifin;

	//	Data
	InterfaceDescriptor			dif;
	EndpointDescriptor			in;
	EndpointDescriptor			out;
} qc_io_cdcdescriptor_t;

typedef struct 
{
	InterfaceDescriptor			msc;
	EndpointDescriptor			in;
	EndpointDescriptor			out;
} MSCDescriptor;

#endif
