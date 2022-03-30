/** @file
  Copyright 2022 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "Declarations.h"

VOID    *mHobList;

VOID *
EFIAPI
AllocateRangeForHobLists (
  VOID  **HobList1,
  VOID  **HobList2,
  VOID  **HobListCopy
  )
{
  VOID    *MemBottom;
  VOID    *MemTop;
  VOID    *FreeMemBottom;
  VOID    *FreeMemTop;
  VOID    *Range;
  VOID    *AlignedRange;
  UINTN   AlignmentMask;
  UINTN   Alignment;
  UINT64  Length;

  Length        = SIZE_32MB;
  Alignment     = SIZE_4KB;
  AlignmentMask = Alignment - 1;

  Range = AllocatePool (SIZE_128MB + SIZE_128MB);
  if (Range == NULL) {
    DEBUG ((DEBUG_ERROR, "Memory is not allocated\n"));
    ASSERT (FALSE);
  }

  AlignedRange = (VOID *)(((UINTN)Range + AlignmentMask) & ~AlignmentMask);

  MemBottom     = AlignedRange;
  MemTop        = (VOID *)((UINTN)MemBottom + Length);
  FreeMemBottom = (VOID *)((UINTN)MemBottom);
  FreeMemTop    = (VOID *)((UINTN)MemTop);

  *HobList1 = HobConstructor (MemBottom, MemTop, FreeMemBottom, FreeMemTop);

  MemBottom     = (VOID *)((UINTN)MemBottom + SIZE_64MB);
  MemTop        = (VOID *)((UINTN)MemBottom + Length);
  FreeMemBottom = (VOID *)((UINTN)MemBottom);
  FreeMemTop    = (VOID *)((UINTN)MemTop);

  *HobList2    = HobConstructor (MemBottom, MemTop, FreeMemBottom, FreeMemTop);
  *HobListCopy = (VOID *)((UINTN)AlignedRange + SIZE_128MB);
  return Range;
}

VOID
EFIAPI
FixUpHobList1 (
  VOID  *HobList1,
  VOID  *HobList1Backup
  )
{
  EFI_PEI_HOB_POINTERS  Hob1;
  EFI_PEI_HOB_POINTERS  HobBackup;

  Hob1.Raw      = (UINT8 *)HobList1;
  HobBackup.Raw = (UINT8 *)HobList1Backup;

  while (TRUE) {
    if (HobBackup.Header->HobType == EFI_HOB_TYPE_RESOURCE_DESCRIPTOR) {
      Hob1.Header->HobType = EFI_HOB_TYPE_RESOURCE_DESCRIPTOR;
    } else if (HobBackup.Header->HobType == EFI_HOB_TYPE_MEMORY_ALLOCATION) {
      Hob1.Header->HobType = EFI_HOB_TYPE_MEMORY_ALLOCATION;
    } else if (HobBackup.Header->HobType == EFI_HOB_TYPE_END_OF_HOB_LIST) {
      break;
    }

    Hob1.Raw      = GET_NEXT_HOB (Hob1);
    HobBackup.Raw = GET_NEXT_HOB (HobBackup);
  }
}

UNIT_TEST_STATUS
TestCase(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  VOID       *Range;
  VOID       *HobList1;
  VOID       *HobList2;
  VOID       *HobList1Backup;
  VOID       *MemoryMapHob;

  Range    = AllocateRangeForHobLists (&HobList1, &HobList2, &HobList1Backup);
  mHobList = HobList1;
  CreateRemainingHobs (HobList1, HobList2, 0, Get64BitRandomNumber ()); // Create end which is above hoblist2.limit

  CopyMem (HobList1Backup, HobList1, SIZE_64MB);

  Status = BuildMemoryMapHob ();

  FixUpHobList1 (HobList1, HobList1Backup);
  MemoryMapHob = GetNextGuidHob (&gUniversalPayloadMemoryMapGuid, mHobList);
  VerifyHobList1WithMemoryMap (HobList1, (UNIVERSAL_PAYLOAD_MEMORY_MAP *)GET_GUID_HOB_DATA (MemoryMapHob));

  mHobList = HobList2;
  CreateHobsBasedOnMemoryMap ((UNIVERSAL_PAYLOAD_MEMORY_MAP *)GET_GUID_HOB_DATA (MemoryMapHob));
  VerifyHob (HobList1, HobList2);
  VerifyHobList2WithMemoryMap (HobList2, (UNIVERSAL_PAYLOAD_MEMORY_MAP *)GET_GUID_HOB_DATA (MemoryMapHob));
  //PrintHob (HobList1);
  //PrintHob (HobList2);
  FreePool (Range);

  return UNIT_TEST_PASSED;
}

TEST_CASE  gMemMapRandomTestCase[] = {
  { "Memory map Random TestCase", "MemoryMapTestCase", TestCase},
};
