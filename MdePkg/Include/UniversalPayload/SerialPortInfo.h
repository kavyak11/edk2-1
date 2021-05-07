/** @file
  This file defines the structure for serial port info.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __PLD_SERIAL_PORT_INFO_H__
#define __PLD_SERIAL_PORT_INFO_H__

#include <UniversalPayload/UniversalPayload.h>

#pragma pack(1)
typedef struct {
  PLD_GENERIC_HEADER   PldHeader;
  BOOLEAN              UseMmio;
  UINT8                RegisterStride;
  UINT32               BaudRate;
  EFI_PHYSICAL_ADDRESS RegisterBase;
} PLD_SERIAL_PORT_INFO;
#pragma pack()

#define PLD_SERIAL_PORT_INFO_REVISION 1

extern GUID gPldSerialPortInfoGuid;

#endif
