#include <nanopb/pb_encode.h>
#include "UniveralPayloadProtoBuffer.pb.h"
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

    //UINT8 buffer[128];
    UINT8* buffer = AllocatePool(128);
    UINTN message_length;
    BOOLEAN status;
    VOID *pointer;
    UsefulBufC buf;
    UINT8 buffer1[128];

    buffer1[5] = 0xFE;
    buffer1[4] = 0xDC;
    buffer1[3] = 0xBA;
    buffer1[11] = 0xFE;
    buffer1[12] = 0xDC;
    buffer1[13] = 0x11;
    buf.ptr = buffer1;

    buf.len = 15;
    pointer = (VOID*)&buf;


    UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO_PROTO SerialPortInfo = UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO_PROTO_init_zero;

    pb_ostream_t stream = pb_ostream_from_buffer(buffer, 128);
    

    SerialPortInfo.BaudRate = PcdGet32 (PcdSerialBaudRate);
    SerialPortInfo.RegisterBase = PcdGet64 (PcdSerialRegisterBase);
    SerialPortInfo.RegisterStride = (UINT8) PcdGet32 (PcdSerialRegisterStride);
    SerialPortInfo.UseMmio = PcdGetBool (PcdSerialUseMmio);
    SerialPortInfo.RawBytes.funcs.encode = &write_string; 
    SerialPortInfo.RawBytes.arg = pointer; 
    

    /* Now we are ready to encode the message! */
    status = pb_encode(&stream, UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO_PROTO_fields, &SerialPortInfo);
    message_length = (UINTN)stream.bytes_written;
    *Buffer = buffer;
    *Size = message_length;
    DEBUG ((DEBUG_ERROR, "%a: Size = %d\n status =0x%x\n",  __FUNCTION__, *Size, status));
    PrintHex1(*Buffer,(UINT16)*Size );
    return 0;
}
