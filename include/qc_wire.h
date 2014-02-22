/*
  twi.h - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef TWI_H
#define TWI_H

#include <inttypes.h>

//#define ATMEGA8

#ifndef TWI_FREQ
#define TWI_FREQ 100000L
#endif

#ifndef TWI_BUFFER_LENGTH
#define TWI_BUFFER_LENGTH 32
#endif

#define TWI_READY 0
#define TWI_MRX   1
#define TWI_MTX   2
#define TWI_SRX   3
#define TWI_STX   4
  
void QC_IO_TwiInit(void);
void QC_IO_TwiSetAddress(uint8_t);
uint8_t QC_IO_TwiReadFrom(uint8_t, uint8_t*, uint8_t, uint8_t);
uint8_t QC_IO_TwiWriteFrom(uint8_t, uint8_t*, uint8_t, uint8_t, uint8_t);
uint8_t QC_IO_TwiTransmit(const uint8_t*, uint8_t);
void QC_IO_TwiAttachSlaveRxEvent( void (*)(uint8_t*, int) );
void QC_IO_TwiAttachSlaveTxEvent( void (*)(void) );
void QC_IO_TwiReply(uint8_t);
void QC_IO_TwiStop(void);
void QC_IO_TwiReleaseBus(void);

#endif

