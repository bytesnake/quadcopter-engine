#ifndef QC_CDC_H
#define QC_CDC_H

#include <stdbool.h>

#include "quadcopter.h"

typedef struct
{
        uint32_t     dwDTERate;
        uint8_t      bCharFormat;
        uint8_t      bParityType;
        uint8_t      bDataBits;
        uint8_t      lineState;
} qc_io_usblineinfo_t;

extern const qc_io_cdcdescriptor_t QC_IO_CDCInterfaces;
extern volatile qc_io_usblineinfo_t QC_IO_UsbLineInfo;

void QC_IO_CDCInit( qc_io_usbsetup_t *setup);

#endif
