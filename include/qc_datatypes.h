#ifndef QC_DATATYPES
#define QC_DATATYPES

//#define uint8_t unsigned char
//#define uint16_t unsigned short
//#define uint32_t unsigned int

typedef enum
{
	QC_ERROR_SUCCESS,

	QC_ERROR_IO_READ,
	QC_ERROR_IO_WRITE,
	QC_ERROR_IO_UNKNOWNCOMMAND,

	QC_ERROR_CDCINIT,

	QC_ERROR_TWI_READ,
	QC_ERROR_TWI_WRITE
} qc_error_t;

#endif
