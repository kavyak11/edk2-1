/** @file
  This file defines the Univeral Payload Platfrom BootManager Protocol.

  Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef __PLD_PLATFORM_BOOTMANAGER_H__
#define __PLD_PLATFORM_BOOTMANAGER_H__

typedef
VOID
(EFIAPI *PLD_PLATFORM_BOOTMANAGER_BEFORE_CONSOLE) (
  VOID
  );

typedef
VOID
(EFIAPI *PLD_PLATFORM_BOOTMANAGER_AFTER_CONSOLE) (
  VOID
  );

typedef
VOID
(EFIAPI *PLD_PLATFORM_BOOTMANAGER_WAIT_CALLBACK) (
  UINT16          TimeoutRemain
  );

typedef
VOID
(EFIAPI *PLD_PLATFORM_BOOTMANAGER_UNABLE_TO_BOOT) (
  VOID
  );

typedef struct {
  PLD_PLATFORM_BOOTMANAGER_BEFORE_CONSOLE        BeforeConsole;
  PLD_PLATFORM_BOOTMANAGER_AFTER_CONSOLE         AfterConsole;
  PLD_PLATFORM_BOOTMANAGER_WAIT_CALLBACK         WaitCallback;
  PLD_PLATFORM_BOOTMANAGER_UNABLE_TO_BOOT        UnableToBoot;
} PLD_PLATFORM_BOOTMANAGER_PROTOCOL;

extern GUID gPldPlatformBootManagerProtocolGuid;

#endif
