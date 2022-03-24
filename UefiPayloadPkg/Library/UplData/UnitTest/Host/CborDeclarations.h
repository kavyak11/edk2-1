/** @file

  Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _CBOR_DECLARATIONS_
#define _CBOR_DECLARATIONS_

#include <PiPei.h>
#include <Library/SetUplDataLib.h>
#include <Library/GetUplDataLib.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>

typedef struct {
  CHAR8                   *Description;
  CHAR8                   *Name;
  UNIT_TEST_FUNCTION      Function;
} TEST_CASE;

/**
  Auto-generated function that calls library constructors for all of the module's
  dependent libraries.
**/
VOID
EFIAPI
ProcessLibraryConstructorList (
  VOID
  );

/**
  Build a Handoff Information Table HOB

  This function initialize a HOB region from EfiMemoryBottom to
  EfiMemoryTop. And EfiFreeMemoryBottom and EfiFreeMemoryTop should
  be inside the HOB region.

  @param[in] EfiMemoryBottom       Total memory start address
  @param[in] EfiMemoryTop          Total memory end address.
  @param[in] EfiFreeMemoryBottom   Free memory start address
  @param[in] EfiFreeMemoryTop      Free memory end address.

  @return   The pointer to the handoff HOB table.

**/
EFI_HOB_HANDOFF_INFO_TABLE*
EFIAPI
HobConstructor(
  IN VOID* EfiMemoryBottom,
  IN VOID* EfiMemoryTop,
  IN VOID* EfiFreeMemoryBottom,
  IN VOID* EfiFreeMemoryTop
  );

UINTN
GetCborTestCaseCount(
  VOID
  );

#endif // _CBOR_DECLARATIONS_
