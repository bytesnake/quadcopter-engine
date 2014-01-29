#ifndef QC_DATATYPES
#define QC_DATATYPES

#define uint8_t char
#define uint16_t short
#define uint32_t int

typedef enum
{
	QC_ERROR_SUCCESS,

	QC_ERROR_IO_READ,
	QC_ERROR_IO_WRITE,
	QC_ERROR_IO_UNKNOWNCOMMAND,

	QC_ERROR_CDCINIT
} qc_error_t;

#endif
