#include "../include/quadcopter.h"

qc_error_t lasterror = QC_ERROR_SUCCESS;

void QC_SetLastError ( qc_error_t error )
{
	if ( lasterror == QC_ERROR_SUCCESS )
		lasterror = error;
}

qc_error_t QC_GetLastError ()
{
	return lasterror;
}

void QC_ResetError () 
{
	lasterror = QC_ERROR_SUCCESS;
}
