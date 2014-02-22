#ifndef QC_DATATYPES
#define QC_DATATYPES

#define f32 float
#define f64 double

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
