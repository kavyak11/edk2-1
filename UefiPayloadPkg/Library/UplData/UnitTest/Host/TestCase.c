/** @file

  Copyright (c) 2022, Intel Corporation. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent

**/
#include "CborDeclarations.h"

VOID* Buffer;
UINTN UsedSize;

// Encode UINT64 value and decode Boolean value
UNIT_TEST_STATUS
TestCase1(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  BOOLEAN    Value;

  SetUplUint64("key64", 0x2000);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplBoolean("key64", &Value);
  UT_ASSERT_EQUAL (Status, RETURN_INVALID_PARAMETER);
  return UNIT_TEST_PASSED;
}

// Encode UINT64 value and decode UINT64 value but with different key
UNIT_TEST_STATUS
TestCase2(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINT64     Value;

  SetUplUint64("newkey", 0x5000);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplUint64("key", &Value);
  UT_ASSERT_EQUAL (Status, RETURN_NOT_FOUND);
  return UNIT_TEST_PASSED;
}

// Encode UINT64 value and decode UINT64 value. Works fine
UNIT_TEST_STATUS
TestCase3(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINT64     SetValue;
  UINT64     Value;

  SetValue = 0x5000;
  SetUplUint64("Work", SetValue);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplUint64("Work", &Value);
  UT_ASSERT_EQUAL (Status, RETURN_SUCCESS);
  UT_ASSERT_EQUAL (SetValue, Value);
  return UNIT_TEST_PASSED;
}

// Encode BOOLEAN value and decode UINT64 value
UNIT_TEST_STATUS
TestCase4(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINT64     Value;

  SetUplBoolean("boolkey", FALSE);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplUint64("boolkey", &Value);
  UT_ASSERT_EQUAL (Status, RETURN_INVALID_PARAMETER);
  return UNIT_TEST_PASSED;
}

// Encode BOOLEAN value and decode BOOLEAN value but with different key
UNIT_TEST_STATUS
TestCase5(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  BOOLEAN    Value;

  SetUplBoolean("BooleanNewKey", FALSE);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplBoolean("bool", &Value);
  UT_ASSERT_EQUAL (Status, RETURN_NOT_FOUND);
  return UNIT_TEST_PASSED;
}

// Encode BOOLEAN value and decode BOOLEAN value. Works fine
UNIT_TEST_STATUS
TestCase6(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  BOOLEAN    SetValue;
  BOOLEAN    Value;

  SetValue = TRUE;
  SetUplBoolean("Random", SetValue);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplBoolean("Random", &Value);
  UT_ASSERT_EQUAL (Status, RETURN_SUCCESS);
  UT_ASSERT_EQUAL (SetValue, Value);
  return UNIT_TEST_PASSED;
}

// Decode UINT64 value with a boolean encoded key
UNIT_TEST_STATUS
TestCase7(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINT64     Value;

  InitUplFromBuffer(Buffer);
  Status = GetUplUint64("Random", &Value);
  UT_ASSERT_EQUAL (Status, RETURN_INVALID_PARAMETER);
  return UNIT_TEST_PASSED;
}

// Encode Binary value and decode UINT64 value
UNIT_TEST_STATUS
TestCase8(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINT64     Value;

  UINT32 Array[] = {0x1234, 0x4537};
  SetUplBinary ("BinaryKey", &Array, 8);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplUint64("BinaryKey", &Value);
  UT_ASSERT_EQUAL (Status, RETURN_INVALID_PARAMETER);
  return UNIT_TEST_PASSED;
}

// Encode Binary value and decode Binary value. Work fine
UNIT_TEST_STATUS
TestCase9(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINT32     Value[20];
  UINTN      Size;
  UINTN      Index;

  UINT32 Array[] = { 0x37, 0x4537, 0x14, 0x7356, 0x12345678};
  SetUplBinary("Binary", &Array, 20);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplBinary("Binary", &Value, &Size);
  UT_ASSERT_EQUAL (Status, RETURN_SUCCESS);
  for (Index = 0; Index < 5; Index++) {
    UT_ASSERT_EQUAL (Array[Index], Value[Index]);
  }
  return UNIT_TEST_PASSED;
}

// Encode Ascii string and decode UINT64 Value
UNIT_TEST_STATUS
TestCase10(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINT64     Value;

  SetUplAsciiString("StringKey", "NewString", 10);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplUint64("StringKey", &Value);
  UT_ASSERT_EQUAL (Status, RETURN_INVALID_PARAMETER);
  return UNIT_TEST_PASSED;
}

// Encode Ascii string and decode Ascii string. Work fine
UNIT_TEST_STATUS
TestCase11(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINT8      *SetValue;
  UINT8      *Value;
  UINTN      Size;

  Value = AllocatePool (20);
  SetValue = "GetString";
  SetUplAsciiString("AsciiKey", SetValue, 10);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplAsciiString("AsciiKey", Value, &Size);
  UT_ASSERT_EQUAL (Status, RETURN_SUCCESS);
  if (AsciiStrCmp (SetValue, Value)) {
    UT_ASSERT_TRUE (FALSE);
  }
  FreePool (Value);
  return UNIT_TEST_PASSED;
}

// Encode Universal Payload Extra data and decode Universal Payload extra data. Work fine
UNIT_TEST_STATUS
TestCase12(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINTN      Count;
  UINTN      Index;
  UINTN      Ind;
  CHAR8      Entry0[] = "OldExtraData";
  CHAR8      Entry1[] = "PayloadData";
  CHAR8      Entry2[] = "NewEntry";
  UNIVERSAL_PAYLOAD_EXTRA_DATA_ENTRY Entry[3];
  UNIVERSAL_PAYLOAD_EXTRA_DATA_ENTRY Output[3];

  Count = 3;
  Index = 0;
  Entry[0].Base = 0x40;
  Entry[1].Base = 0x85;
  Entry[0].Size = 0x20;
  Entry[1].Size = 0x10;
  Entry[0].Base = 0x100;
  Entry[1].Size = 0x30;

  AsciiStrCpyS(Entry[0].Identifier, sizeof(Entry0), Entry0);
  AsciiStrCpyS(Entry[1].Identifier, sizeof(Entry1), Entry1);
  AsciiStrCpyS(Entry[2].Identifier, sizeof(Entry2), Entry2);

  SetUplExtraData(Entry, 3);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplExtraData(Output, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_SUCCESS);
  for (Ind = 0; Ind < Count; Ind++) {
    UT_ASSERT_EQUAL (Entry[Ind].Base, Output[Ind].Base);
    UT_ASSERT_EQUAL (Entry[Ind].Size, Output[Ind].Size);
    if (AsciiStrCmp (Entry[Ind].Identifier, Output[Ind].Identifier)) {
      UT_ASSERT_TRUE (FALSE);
    }
  }

  return UNIT_TEST_PASSED;
}

// Decode Universal payload Extra Data where Index is greater than or equal to total array count
// returns unsupported
UNIT_TEST_STATUS
TestCase13(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINTN      Count;
  UINTN      Index;
  UNIVERSAL_PAYLOAD_EXTRA_DATA_ENTRY Entry[2];
  UNIVERSAL_PAYLOAD_EXTRA_DATA_ENTRY Output[2];

  Count = 2;
  Index = 4;
  Entry[0].Base = 100;
  Entry[1].Base = 150;

  SetUplExtraData(Entry, 2);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplExtraData(Output, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_UNSUPPORTED);
  return UNIT_TEST_PASSED;
}

// Decode Universal payload Extra Data where count is zero on output
// returns RETURN_BUFFER_TOO_SMALL
UNIT_TEST_STATUS
TestCase14(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINTN      Count;
  UINTN      Index;
  UNIVERSAL_PAYLOAD_EXTRA_DATA_ENTRY Entry[1];
  UNIVERSAL_PAYLOAD_EXTRA_DATA_ENTRY Output[1];

  Count = 0;
  Index = 0;
  Entry[0].Base = 20;

  SetUplExtraData(Entry, 1);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplExtraData(Output, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_BUFFER_TOO_SMALL);
  return UNIT_TEST_PASSED;
}

// Encode Universal Payload memory map information and decode Universal Payload memory map information. 
// Work fine
UNIT_TEST_STATUS
TestCase15(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_MEMORY_DESCRIPTOR In[2];
  EFI_MEMORY_DESCRIPTOR Out[2];
  EFI_STATUS            Status;
  UINTN                 Count;
  UINTN                 Index;
  UINTN                 Ind;

  Count = 2;
  Index = 0;

  In[0].PhysicalStart = 0x1000;
  In[0].NumberOfPages = 2;
  In[0].Type          = EfiBootServicesData;
  In[0].Attribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                  EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                  EFI_RESOURCE_ATTRIBUTE_WRITE_BACK_CACHEABLE |
                  EFI_RESOURCE_ATTRIBUTE_TESTED;

  In[1].PhysicalStart = 0x5000;
  In[1].NumberOfPages = 1;
  In[1].Type          = EfiReservedMemoryType;
  In[1].Attribute = EFI_RESOURCE_ATTRIBUTE_PRESENT |
                  EFI_RESOURCE_ATTRIBUTE_INITIALIZED |
                  EFI_RESOURCE_ATTRIBUTE_TESTED;

  SetUplMemoryMap(In, 2);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplMemoryMap(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_SUCCESS);
  for (Ind = 0; Ind < Count; Ind++) {
    UT_ASSERT_EQUAL (In[Ind].PhysicalStart, Out[Ind].PhysicalStart);
    UT_ASSERT_EQUAL (In[Ind].NumberOfPages, Out[Ind].NumberOfPages);
    UT_ASSERT_EQUAL (In[Ind].Type, Out[Ind].Type);
    UT_ASSERT_EQUAL (In[Ind].Attribute, Out[Ind].Attribute);
  }

  return UNIT_TEST_PASSED;
}

// Decode Universal Payload memory map information where count is zero on output.
UNIT_TEST_STATUS
TestCase16(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_MEMORY_DESCRIPTOR Input[3];
  EFI_MEMORY_DESCRIPTOR Output[3];
  EFI_STATUS            Status;
  UINTN                 Count;
  UINTN                 Index;

  Count = 0;
  Index = 0;

  Input[0].PhysicalStart = 0x2000;
  Input[0].NumberOfPages = 1;
  Input[1].PhysicalStart = 0x4000;
  Input[1].NumberOfPages = 2;
  Input[2].PhysicalStart = 0x8000;
  Input[2].NumberOfPages = 3;

  SetUplMemoryMap(Input, 3);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplMemoryMap(Output, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_BUFFER_TOO_SMALL);
  return UNIT_TEST_PASSED;
}

// Decode Universal Payload memory map information where Index is greater than or equal to total array count.
UNIT_TEST_STATUS
TestCase17(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_MEMORY_DESCRIPTOR Input[2];
  EFI_MEMORY_DESCRIPTOR Output[2];
  EFI_STATUS            Status;
  UINTN                 Count;
  UINTN                 Index;

  Count = 2;
  Index = 2;

  Input[0].PhysicalStart = 0x3000;
  Input[0].NumberOfPages = 2;
  Input[1].PhysicalStart = 0x9000;
  Input[1].NumberOfPages = 1;

  SetUplMemoryMap(Input, 2);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplMemoryMap(Output, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_UNSUPPORTED);
  return UNIT_TEST_PASSED;
}

// Encode and decode Universal Payload pci root bridge information. Work fine.
UNIT_TEST_STATUS
TestCase18(
  IN UNIT_TEST_CONTEXT  Context
)
{
  UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGE In[2];
  UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGE Out[2];
  EFI_STATUS                        Status;
  UINTN                             Count;
  UINTN                             Index;
  UINTN                             Ind;

  Count = 2;
  Index = 0;

  In[0].Bus.Base = 0x35;
  In[0].Bus.Limit = 0x4A;
  In[0].Io.Base = 0x6000;
  In[0].Io.Limit = 0x7000;
  In[0].DmaAbove4G = TRUE;
  In[0].NoExtendedConfigSpace = TRUE;

  In[1].Bus.Base = 0x86;
  In[1].Bus.Limit = 0xA0;
  In[1].Io.Base = 0xB000;
  In[1].Io.Limit = 0xC000;
  In[1].DmaAbove4G = FALSE;
  In[1].NoExtendedConfigSpace = TRUE;

  SetUplPciRootBridges(In, 2);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplPciRootBridges(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_SUCCESS);
  for (Ind = 0; Ind < Count; Ind++) {
    UT_ASSERT_EQUAL (In[Ind].Bus.Base, Out[Ind].Bus.Base);
    UT_ASSERT_EQUAL (In[Ind].Bus.Limit, Out[Ind].Bus.Limit);
    UT_ASSERT_EQUAL (In[Ind].Io.Base, Out[Ind].Io.Base);
    UT_ASSERT_EQUAL (In[Ind].Io.Limit, Out[Ind].Io.Limit);
  }

  return UNIT_TEST_PASSED;
}

// Encode and decode Universal Payload pci root bridge information where Count is zero on output.
UNIT_TEST_STATUS
TestCase19(
  IN UNIT_TEST_CONTEXT  Context
)
{
  UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGE In[1];
  UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGE Out[1];
  EFI_STATUS                        Status;
  UINTN                             Count;
  UINTN                             Index;

  Count = 0;
  Index = 0;

  In[0].Bus.Base = 0x02;
  In[0].Bus.Limit = 0x07;
  In[0].Io.Base = 0x2000;
  In[0].Io.Limit = 0x3000;

  SetUplPciRootBridges(In, 1);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplPciRootBridges(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_BUFFER_TOO_SMALL);
  return UNIT_TEST_PASSED;
}

// Encode and decode Universal Payload pci root bridge information where Index is greater than or equal to total array count.
UNIT_TEST_STATUS
TestCase20(
  IN UNIT_TEST_CONTEXT  Context
)
{
  UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGE In[1];
  UNIVERSAL_PAYLOAD_PCI_ROOT_BRIDGE Out[1];
  EFI_STATUS                        Status;
  UINTN                             Count;
  UINTN                             Index;

  Count = 1;
  Index = 3;

  In[0].Bus.Base = 0x0B;
  In[0].Bus.Limit = 0x14;
  In[0].Io.Base = 0x5000;
  In[0].Io.Limit = 0x6000;

  SetUplPciRootBridges(In, 1);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplPciRootBridges(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_UNSUPPORTED);
  return UNIT_TEST_PASSED;
}

// Encode and decode Universal Payload resource descriptor information. Work fine.
UNIT_TEST_STATUS
TestCase21(
  IN UNIT_TEST_CONTEXT  Context
)
{
  UNIVERSAL_PAYLOAD_RESOURCE_DESCRIPTOR In[4];
  UNIVERSAL_PAYLOAD_RESOURCE_DESCRIPTOR Out[4];
  EFI_STATUS                            Status;
  UINTN                                 Count;
  UINTN                                 Index;
  UINTN                                 Ind;

  Count = 4;
  Index = 0;

  In[0].PhysicalStart = 0x2000;
  In[0].ResourceLength = 0x3000;
  In[1].PhysicalStart = 0x7000;
  In[1].ResourceLength = 0x2000;
  In[2].PhysicalStart = 0xA000;
  In[2].ResourceLength = 0x1000;
  In[3].PhysicalStart = 0xC000;
  In[3].ResourceLength = 0x1000;

  SetUplResourceData(In, 4);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplResourceData(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_SUCCESS);
  for (Ind = 0; Ind < Count; Ind++) {
    UT_ASSERT_EQUAL (In[Ind].PhysicalStart, Out[Ind].PhysicalStart);
    UT_ASSERT_EQUAL (In[Ind].ResourceLength, Out[Ind].ResourceLength);
  }

  return UNIT_TEST_PASSED;
}

// Encode and decode Universal Payload resource descriptor information where count is zero on output.
UNIT_TEST_STATUS
TestCase22(
  IN UNIT_TEST_CONTEXT  Context
)
{
  UNIVERSAL_PAYLOAD_RESOURCE_DESCRIPTOR In[1];
  UNIVERSAL_PAYLOAD_RESOURCE_DESCRIPTOR Out[1];
  EFI_STATUS                            Status;
  UINTN                                 Count;
  UINTN                                 Index;

  Count = 0;
  Index = 0;

  In[0].PhysicalStart = 0x1000;
  In[0].ResourceLength = 0x5000;

  SetUplResourceData(In, 1);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplResourceData(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_BUFFER_TOO_SMALL);
  return UNIT_TEST_PASSED;
}

// Encode and decode Universal Payload resource descriptor information where Index is greater than or equal to total array count.
UNIT_TEST_STATUS
TestCase23(
  IN UNIT_TEST_CONTEXT  Context
)
{
  UNIVERSAL_PAYLOAD_RESOURCE_DESCRIPTOR In[1];
  UNIVERSAL_PAYLOAD_RESOURCE_DESCRIPTOR Out[1];
  EFI_STATUS                            Status;
  UINTN                                 Count;
  UINTN                                 Index;

  Count = 1;
  Index = 6;

  In[0].PhysicalStart = 0x4000;
  In[0].ResourceLength = 0x4000;

  SetUplResourceData(In, 1);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplResourceData(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_UNSUPPORTED);
  return UNIT_TEST_PASSED;
}

// Encode and decode Universal Payload memory allocation information. Work fine.
UNIT_TEST_STATUS
TestCase24(
  IN UNIT_TEST_CONTEXT  Context
)
{
  UNIVERSAL_PAYLOAD_MEMORY_ALLOCATION   In[3];
  UNIVERSAL_PAYLOAD_MEMORY_ALLOCATION   Out[3];
  EFI_STATUS                            Status;
  UINTN                                 Count;
  UINTN                                 Index;
  UINTN                                 Ind;

  Count = 3;
  Index = 0;

  In[0].MemoryBaseAddress = 0x1000;
  In[0].MemoryLength = 0x2000;
  In[1].MemoryBaseAddress = 0x5000;
  In[1].MemoryLength = 0x1000;
  In[2].MemoryBaseAddress = 0x8000;
  In[2].MemoryLength = 0x1000;

  SetUplMemoryAllocationData(In, 3);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplMemoryAllocationData(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_SUCCESS);
  for (Ind = 0; Ind < Count; Ind++) {
    UT_ASSERT_EQUAL (In[Ind].MemoryBaseAddress, Out[Ind].MemoryBaseAddress);
    UT_ASSERT_EQUAL (In[Ind].MemoryLength, Out[Ind].MemoryLength);
  }

  return UNIT_TEST_PASSED;
}

// Encode and decode Universal Payload memory allocation information where count is zero on output.
UNIT_TEST_STATUS
TestCase25(
  IN UNIT_TEST_CONTEXT  Context
)
{
  UNIVERSAL_PAYLOAD_MEMORY_ALLOCATION   In[1];
  UNIVERSAL_PAYLOAD_MEMORY_ALLOCATION   Out[1];
  EFI_STATUS                            Status;
  UINTN                                 Count;
  UINTN                                 Index;

  Count = 0;
  Index = 0;

  In[0].MemoryBaseAddress = 0x7000;
  In[0].MemoryLength = 0x3000;

  SetUplMemoryAllocationData(In, 1);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplMemoryAllocationData(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_BUFFER_TOO_SMALL);
  return UNIT_TEST_PASSED;
}

// Encode and decode Universal Payload memory allocation information where Index is greater than
// or equal to total array count
UNIT_TEST_STATUS
TestCase26(
  IN UNIT_TEST_CONTEXT  Context
)
{
  UNIVERSAL_PAYLOAD_MEMORY_ALLOCATION   In[1];
  UNIVERSAL_PAYLOAD_MEMORY_ALLOCATION   Out[1];
  EFI_STATUS                            Status;
  UINTN                                 Count;
  UINTN                                 Index;

  Count = 1;
  Index = 5;

  In[0].MemoryBaseAddress = 0x3000;
  In[0].MemoryLength = 0x4000;

  SetUplMemoryAllocationData(In, 1);
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplMemoryAllocationData(Out, &Count, Index);
  UT_ASSERT_EQUAL (Status, RETURN_UNSUPPORTED);
  return UNIT_TEST_PASSED;
}

// Case where encode function returns out of resources.
UNIT_TEST_STATUS
TestCase27(
  IN UNIT_TEST_CONTEXT  Context
)
{
  EFI_STATUS Status;
  UINT64     Value;
  UINTN      Index;

  for (Index = 0; Index < 250; Index++) {
    Status = SetUplUint64("CheckForResource", 0x2000);
    if (Status != 0) {
      UT_ASSERT_EQUAL (Status, RETURN_OUT_OF_RESOURCES);
      return UNIT_TEST_PASSED;
    }
  }
  LockUplAndGetBuffer(&Buffer, &UsedSize);
  InitUplFromBuffer(Buffer);
  Status = GetUplUint64("CheckForResource", &Value);
  return UNIT_TEST_PASSED;
}

TEST_CASE gCborTestCase[] = {
  {"CborTestCase1", "TestCase1", TestCase1},
  {"CborTestCase2", "TestCase2", TestCase2},
  {"CborTestCase3", "TestCase3", TestCase3},
  {"CborTestCase4", "TestCase4", TestCase4},
  {"CborTestCase5", "TestCase5", TestCase5},
  {"CborTestCase6", "TestCase6", TestCase6},
  {"CborTestCase7", "TestCase7", TestCase7},
  {"CborTestCase8", "TestCase8", TestCase8},
  {"CborTestCase9", "TestCase9", TestCase9},
  {"CborTestCase10", "TestCase10", TestCase10},
  {"CborTestCase11", "TestCase11", TestCase11},
  {"CborTestCase12", "TestCase12", TestCase12},
  {"CborTestCase13", "TestCase13", TestCase13},
  {"CborTestCase14", "TestCase14", TestCase14},
  {"CborTestCase15", "TestCase15", TestCase15},
  {"CborTestCase16", "TestCase16", TestCase16},
  {"CborTestCase17", "TestCase17", TestCase17},
  {"CborTestCase18", "TestCase18", TestCase18},
  {"CborTestCase19", "TestCase19", TestCase19},
  {"CborTestCase20", "TestCase20", TestCase20},
  {"CborTestCase21", "TestCase21", TestCase21},
  {"CborTestCase22", "TestCase22", TestCase22},
  {"CborTestCase23", "TestCase23", TestCase23},
  {"CborTestCase24", "TestCase24", TestCase24},
  {"CborTestCase25", "TestCase25", TestCase25},
  {"CborTestCase26", "TestCase26", TestCase26},
  {"CborTestCase27", "TestCase27", TestCase27}
};

UINTN
GetCborTestCaseCount(
  VOID
)
{
  return ARRAY_SIZE(gCborTestCase);
}
