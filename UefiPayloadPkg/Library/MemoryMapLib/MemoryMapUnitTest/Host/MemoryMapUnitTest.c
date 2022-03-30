/** @file
  Copyright 2022 Intel Corporation. <BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "Declarations.h"

#define UNIT_TEST_NAME     "Memory Map Unit Test"
#define UNIT_TEST_VERSION  "0.1"

VOID  *mHobList;

extern TEST_CASE gMemMapRandomTestCase[];

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
  UNIT_TEST_SUITE_HANDLE      MemoryMapTests;
  UINTN                       Index;

  DEBUG ((DEBUG_INFO, "%a v%a\n", UNIT_TEST_NAME, UNIT_TEST_VERSION));

  Framework = NULL;
  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, UNIT_TEST_NAME, gEfiCallerBaseName, UNIT_TEST_VERSION);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "InitUnitTestFramework failed. Status = %r\n", Status));
    goto EXIT;
  }

  //
  // Populate the MemoryMapTests Unit Test Suite.
  //
  Status = CreateUnitTestSuite (&MemoryMapTests, Framework, "Memory Map Test Cases", "MemoryMapTest", NULL, NULL);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "CreateUnitTestSuite failed for MemoryMapTests. Status = %r\n", Status));
    Status = EFI_OUT_OF_RESOURCES;
    goto EXIT;
  }

  for (Index = 0; Index < 100; Index++) {
    DEBUG ((DEBUG_INFO, "Run Random TestCase for %d times\n", Index + 1));
    AddTestCase (MemoryMapTests, gMemMapRandomTestCase[0].Description, 
                 gMemMapRandomTestCase[0].Name, gMemMapRandomTestCase[0].Function, 
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

/**
  The main() function for setting up and running the tests.

  @retval EFI_SUCCESS on successful running.
  @retval Other error code on failure.
**/
int main()
{
  EFI_STATUS Status;

  Status = TestMain ();
  return Status;
}
