## @file
#
# Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##
[Defines]
  PLATFORM_NAME                       = CborHostTest
  PLATFORM_GUID                       = 94C83DC1-63D9-4D9C-A0E1-03C0F278A611
  PLATFORM_VERSION                    = 0.01
  DSC_SPECIFICATION                   = 0x00010019
  OUTPUT_DIRECTORY                    = Build/CborHostTest
  SUPPORTED_ARCHITECTURES             = IA32|X64
  BUILD_TARGETS                       = NOOPT

!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc

[Components.common.HOST_APPLICATION]
  UefiPayloadPkg/Library/UplData/UnitTest/Host/CborUnitTest.inf {
    <LibraryClasses>
      GetUplDataLib|UefiPayloadPkg/Library/UplData/GetUplDataLib/GetUplDataLib.inf
      SetUplDataLib|UefiPayloadPkg/Library/UplData/SetUplDataLib/SetUplDataLib.inf
      HobLib|UefiPayloadPkg/Library/PayloadEntryHobLib/HobLib.inf
  }
