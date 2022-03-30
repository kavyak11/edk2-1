/** @file

  Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _DECLARATIONS_
#define _DECLARATIONS_

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/UnitTestLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/MemoryMapLib.h>
#include <Library/BaseMemoryLib.h>
#include <UniversalPayload/MemoryMap.h>
#include <Library/HobLib.h>

typedef struct {
  CHAR8                   *Description;
  CHAR8                   *Name;
  UNIT_TEST_FUNCTION      Function;
} TEST_CASE;

VOID *
EFIAPI
AllocateRangeForHobLists (
  VOID  **HobList1,
  VOID  **HobList2,
  VOID  **HobListCopy
  );

VOID
CreateRemainingHobs (
  VOID    *HobList1,
  VOID    *HobList2,
  UINT64  Base,
  UINT64  Limit
  );

VOID
PrintHob (
  IN CONST VOID  *HobStart
  );

UINT64
Get64BitRandomNumber (
  VOID
  );

VOID
VerifyHobList1WithMemoryMap (
  IN CONST VOID                    *HobList1Start,
  IN UNIVERSAL_PAYLOAD_MEMORY_MAP  *MemoryMapHob
  );

VOID
VerifyHobList2WithMemoryMap (
  IN CONST VOID                    *HobList2Start,
  IN UNIVERSAL_PAYLOAD_MEMORY_MAP  *MemoryMapHob
  );

VOID
VerifyHob (
  IN CONST VOID  *HobList1Start,
  IN CONST VOID  *HobList2Start
  );

EFI_RESOURCE_ATTRIBUTE_TYPE
ConvertCapabilitiesToResourceDescriptorHobAttributes(
  UINT64  Capabilities
  );

EFI_RESOURCE_TYPE
ConvertEfiMemoryTypeToResourceDescriptorHobResourceType(
  EFI_MEMORY_TYPE  Type
  );

EFI_STATUS
EFIAPI
CreateHobsBasedOnMemoryMap (
  IN UNIVERSAL_PAYLOAD_MEMORY_MAP  *MemoryMapHob
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

#endif // _DECLARATIONS_
