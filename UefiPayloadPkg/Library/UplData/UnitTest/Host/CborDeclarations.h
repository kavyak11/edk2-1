/** @file

  Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef _CBORDECLARATIONS_
#define _CBORDECLARATIONS_

#include <PiPei.h>
#include <Library/SetUplDataLib.h>
#include <Library/GetUplDataLib.h>
#include <Library/UnitTestLib.h>
#include <Library/DebugLib.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern VOID* mHobList;

typedef struct {
  CHAR8                   *Description;
  CHAR8                   *Name;
  UNIT_TEST_FUNCTION      Func;
} TEST_CASE;

RETURN_STATUS
EFIAPI
SetUplDataLibConstructor(
  VOID
);

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

void*
malloc(
  size_t
  );

void
free(
  void *
  );

int
printf(
  const char *,
  ...
  );

int
strcmp(
  const char *,
  const char *
  );

#endif // _CBORDECLARATIONS_
