#include <PiPei.h>

#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/PeCoffLib.h>
#include <Library/HobLib.h>
#include <nanopb/pb_encode.h>
#include <nanopb/pb_decode.h>
#include "UniveralPayloadProtoBuffer.pb.h"
#include <Base.h>
#include <Library/DebugLib.h>
#include "Proto.h"
#include <Library/MemoryAllocationLib.h>



#include <UniversalPayload/SerialPortInfo.h>
#define ROW_LIMITER 16



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

static bool read_string(pb_istream_t *stream, const pb_field_t *field, void **arg)
{
    
    size_t len = stream->bytes_left;
    DEBUG ((DEBUG_ERROR, "%a: status =0x%x  len = %d\n",  __FUNCTION__,  1, len));
    
    VOID* buf = AllocatePool(len);
    if (!pb_read(stream, buf, len))
        return false;
    
    ((UsefulBufC*)*arg) -> ptr = buf;
    ((UsefulBufC*)*arg) -> len = len;
    return true;
}

RETURN_STATUS
EFIAPI
GetProtoBuffer (
  OUT VOID                *Buffer,
  OUT UINTN               Size
  )
{

  VOID *pointer;
  UsefulBufC buf;
  pointer = &buf;
  /* Allocate space for the decoded message. */
  UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO_PROTO SerialPortInfo = UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO_PROTO_init_zero;
  
  /* Create a stream that reads from the buffer. */
  pb_istream_t stream = pb_istream_from_buffer(Buffer, Size);
  UINTN status;
  /* Now we are ready to decode the message. */

  SerialPortInfo.RawBytes.funcs.decode = &read_string; 
  SerialPortInfo.RawBytes.arg = pointer; 
  status = pb_decode(&stream, UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO_PROTO_fields, &SerialPortInfo);
  
  /* Check for errors... */

  DEBUG ((DEBUG_ERROR, "%a: status =0x%x  %a\n",  __FUNCTION__,  status, stream.errmsg));
  /* Print the data contained in the message. */
  
  PrintHex1((UINT8 *)(buf.ptr), (UINT16)buf.len );

  DEBUG ((DEBUG_ERROR, " SerialPortInfo.BaudRate       =%d\n",    SerialPortInfo.BaudRate      ));
  DEBUG ((DEBUG_ERROR, " SerialPortInfo.RegisterBase   =0x%x\n",  SerialPortInfo.RegisterBase  ));
  DEBUG ((DEBUG_ERROR, " SerialPortInfo.RegisterStride =0x%x\n",  SerialPortInfo.RegisterStride));
  DEBUG ((DEBUG_ERROR, " SerialPortInfo.UseMmio        =0x%x\n",  SerialPortInfo.UseMmio       ));

  UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO  *Serial;
  Serial = BuildGuidHob (&gUniversalPayloadSerialPortInfoGuid, sizeof (UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO));
  Serial->BaudRate = (UINT32)SerialPortInfo.BaudRate;
  Serial->RegisterBase = (UINT64)SerialPortInfo.RegisterBase;
  Serial->RegisterStride = (UINT32)SerialPortInfo.RegisterStride;
  Serial->Header.Revision = UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO_REVISION;
  Serial->Header.Length = sizeof (UNIVERSAL_PAYLOAD_SERIAL_PORT_INFO);
  Serial->UseMmio = (BOOLEAN)SerialPortInfo.UseMmio;
  return 0;
}
