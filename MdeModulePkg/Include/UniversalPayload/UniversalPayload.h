/** @file
  Universal Payload general definations.

Copyright (c) 2021, Intel Corporation. All rights reserved.<BR>
SPDX-License-Identifier: BSD-2-Clause-Patent

**/

#ifndef __UNIVERSAL_PAYLOAD_H__
#define __UNIVERSAL_PAYLOAD_H__

#include <Library/HobLib.h>

typedef  VOID   (EFIAPI *UNIVERSAL_PAYLOAD_ENTRY) (VOID *HobList);

#define PLD_IDENTIFIER                   SIGNATURE_32('U', 'P', 'L', 'D')
#define PLD_INFO_SEC_NAME                ".upld_info"
#define PLD_EXTRA_SEC_NAME_PREFIX        ".upld."
#define PLD_EXTRA_SEC_NAME_PREFIX_LENGTH (sizeof (PLD_EXTRA_SEC_NAME_PREFIX) - 1)

#pragma pack(1)

typedef struct {
  UINT32                          Identifier;
  UINT32                          HeaderLength;
  UINT16                          SpecRevision;
  UINT8                           Reserved[2];
  UINT32                          Revision;
  UINT32                          Attribute;
  UINT32                          Capability;
  CHAR8                           ProducerId[16];
  CHAR8                           ImageId[16];
} PLD_INFO_HEADER;

typedef struct {
  UINT8                Revision;
  UINT8                Reserved;
  UINT16               Length;
} PLD_GENERIC_HEADER;

#pragma pack()

/**
  Returns the size of a structure of known type, up through and including a specified field.

  @param   TYPE     The name of the data structure that contains the field specified by Field.
  @param   Field    The name of the field in the data structure.

  @return  size, in bytes.

**/
#define PLD_SIZEOF_THROUGH_FIELD(TYPE, Field) (OFFSET_OF(TYPE, Field) + sizeof (((TYPE *) 0)->Field))

#define IS_PLD_HEADER_HAS_REVISION(GuidHob, ExpectedRevision) \
  ( \
    (GuidHob != NULL) && \
    (sizeof (PLD_GENERIC_HEADER) <= GET_GUID_HOB_DATA_SIZE (GuidHob)) && \
    (((PLD_GENERIC_HEADER *) GET_GUID_HOB_DATA (GuidHob))->Length <= GET_GUID_HOB_DATA_SIZE (GuidHob)) && \
    (((PLD_GENERIC_HEADER *) GET_GUID_HOB_DATA (GuidHob))->Revision == ExpectedRevision) \
  )

#endif // __UNIVERSAL_PAYLOAD_H__
