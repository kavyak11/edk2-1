/** @file
  Copyright 2022 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include <CborDeclarations.h>

#define UNIT_TEST_NAME     "Cbor Unit Test"
#define UNIT_TEST_VERSION  "0.1"

VOID*   Buffer;
UINTN   UsedSize;

extern TEST_CASE gCborTestCase[];

/**
  Initialize the unit test framework, suite, and unit tests for the
  cbor unit tests and run the unit tests.

  @retval  EFI_SUCCESS           All test cases were dispatched.
  @retval  EFI_OUT_OF_RESOURCES  There are not enough resources available to
                                 initialize the unit tests.
**/
EFI_STATUS
EFIAPI
TestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      CborTests;
  UINTN                       Index;
  UINTN                       TestCaseCount;

  DEBUG ((DEBUG_INFO, "%a v%a\n", UNIT_TEST_NAME, UNIT_TEST_VERSION));

  Framework = NULL;
  TestCaseCount = GetCborTestCaseCount();

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, UNIT_TEST_NAME, gEfiCallerBaseName, UNIT_TEST_VERSION);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InitUnitTestFramework failed. Status = %r\n", Status));
    goto EXIT;
  }

  //
  // Populate the CborTests Unit Test Suite.
  //
  Status = CreateUnitTestSuite (&CborTests, Framework, "Cbor Test Cases", "CborTest", NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateUnitTestSuite failed for CborTests. Status = %r\n", Status));
    Status = EFI_OUT_OF_RESOURCES;
    goto EXIT;
  }

  for (Index = 0; Index < TestCaseCount; Index++) {
    DEBUG ((DEBUG_ERROR, "Run Test case for %d times\n", Index+1));
    AddTestCase (CborTests, gCborTestCase[Index].Description, 
                 gCborTestCase[Index].Name, gCborTestCase[Index].Func, 
                 NULL, NULL, NULL);
  }

  //
  // Execute the tests.
  //
  Status = RunAllTestSuites (Framework);

EXIT:
  if (Framework) {
    FreeUnitTestFramework (Framework);
  }

  return Status;
}

VOID*
CreateHandOffHob(
  VOID
)
{
  VOID*   MemBottom;
  VOID*   MemTop;
  VOID*   FreeMemBottom;
  VOID*   FreeMemTop;
  VOID*   Range;
  VOID*   AlignedRange;
  UINTN   AlignmentMask;
  UINTN   Alignment;
  UINT64  Length;

  Length = SIZE_8MB;
  Alignment = SIZE_4KB;
  AlignmentMask = Alignment - 1;

  Range = malloc(SIZE_16MB);
  if (Range == NULL) {
      printf("Memory is not allocated\n");
      ASSERT(FALSE);
  }

  AlignedRange = (VOID*)(((UINTN)Range + AlignmentMask) & ~AlignmentMask);

  MemBottom = AlignedRange;
  MemTop = (VOID*)((UINTN)MemBottom + Length);
  FreeMemBottom = (VOID*)((UINTN)MemBottom);
  FreeMemTop = (VOID*)((UINTN)MemTop);

  HobConstructor(MemBottom, MemTop, FreeMemBottom, FreeMemTop);
  return Range;
}

/**
  The main() function for setting up and running the tests.

  @retval EFI_SUCCESS on successful running.
  @retval Other error code on failure.
**/
int main()
{
  EFI_STATUS Status;
  VOID*      Range;

  Range = CreateHandOffHob ();
  SetUplDataLibConstructor();

  Status = TestMain ();
  free(Range);
  return Status;
}
