#include <nanopb/pb_encode.h>
#include "UniversalPayloadProtoBuffer.pb.h"
#include <Base.h>
#include <Library/DebugLib.h>
#include "Proto.h"
#include <Library/MemoryAllocationLib.h>
#define ROW_LIMITER 16


UINT8 buf11[16];

UINTN
PrintHex1 (
  IN  UINT8         *DataStart,
  IN  UINT16         DataSize
  )
{
  UINTN  Index1;
  UINTN  Index2;
  UINT8  *StartAddr;

  StartAddr = DataStart;
  for (Index1 = 0; Index1 * ROW_LIMITER < DataSize; Index1++) {
    DEBUG ((DEBUG_ERROR, "   0x%04p:", (DataStart - StartAddr)));
    for (Index2 = 0; (Index2 < ROW_LIMITER) && (Index1 * ROW_LIMITER + Index2 < DataSize); Index2++){
      DEBUG ((DEBUG_ERROR, " %02x", *DataStart));
      DataStart++;
    }
    DEBUG ((DEBUG_ERROR, "\n"));
  }

  return 0;
}

static bool write_string(pb_ostream_t *stream, const pb_field_t *field, void * const *arg)
{
  DEBUG ((DEBUG_ERROR, "%a: status =0x%x  len = %d\n",  __FUNCTION__,  1, ((UsefulBufC*)*arg) -> len));
  return pb_encode_tag_for_field(stream, field) && pb_encode_string(stream, ((UsefulBufC*)*arg) -> ptr, ((UsefulBufC*)*arg) -> len);
}

RETURN_STATUS
EFIAPI
SetProtoBuffer (
  OUT VOID                **Buffer,
  OUT UINTN               *Size
  )
{
    UINT8* buffer = AllocatePool(128);
    UINTN message_length;
    BOOLEAN status;
    UsefulBufC buf;
    UINT32 buffer1[] = { 0x12345678, 0x0, 0x90ABCDEF};
    buf.ptr = buffer1;
    buf.len = ARRAY_SIZE(buffer1) * 4;


    MY_PROTO_BUFFER                          MyProtoBuffer  = MY_PROTO_BUFFER_init_zero;


    pb_ostream_t stream = pb_ostream_from_buffer(buffer, 128);
    
    MyProtoBuffer.has_SerialPortInfo = TRUE;
    MyProtoBuffer.SerialPortInfo.BaudRate = PcdGet32 (PcdSerialBaudRate);
    MyProtoBuffer.SerialPortInfo.RegisterBase = PcdGet64 (PcdSerialRegisterBase);
    MyProtoBuffer.SerialPortInfo.RegisterStride = (UINT8) PcdGet32 (PcdSerialRegisterStride);
    MyProtoBuffer.SerialPortInfo.UseMmio = PcdGetBool (PcdSerialUseMmio);
    MyProtoBuffer.RawBytes.funcs.encode = &write_string; 
    MyProtoBuffer.RawBytes.arg = (VOID*)&buf; 
    

    MyProtoBuffer.has_MyGuid = TRUE;
    CopyMem(MyProtoBuffer.MyGuid, &gUefiPayloadPkgTokenSpaceGuid, 16);

    

    /* Now we are ready to encode the message! */
    status = pb_encode(&stream, MY_PROTO_BUFFER_fields, &MyProtoBuffer);
    message_length = (UINTN)stream.bytes_written;
    *Buffer = buffer;
    *Size = message_length;
    DEBUG ((DEBUG_ERROR, "%a: Size = %d\n status =0x%x\n",  __FUNCTION__, *Size, status));
    PrintHex1(*Buffer,(UINT16)*Size );
    return 0;
}
