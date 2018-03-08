#include "msgdrv.h"
#include "ioctl.h"

NTSTATUS MsgDrvIoControl(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
	NTSTATUS Status = STATUS_NOT_SUPPORTED;
	PIO_STACK_LOCATION pIoStackIrp = NULL;
	unsigned int dwDataWritten = 0;
	
	UNREFERENCED_PARAMETER(DeviceObject);
	
	DbgPrint("msgdrv: DeviceIoControl Called\n");
	
	pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);
	
	if(pIoStackIrp)
	{
		switch(pIoStackIrp->Parameters.DeviceIoControl.IoControlCode)
		{
			case IOCTL_MSGDRV_NEITHER_IO:
			     Status = Msgdrv_HandleSampleIoctl_NeitherIo(Irp, pIoStackIrp, &dwDataWritten);
			break;
			default:
                    DbgPrint("MsgDrv: Method not supported by the DeviceIoControl\n");
                    break;
		}
	}
	
	Irp->IoStatus.Status = Status;
	Irp->IoStatus.Information = dwDataWritten;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	
	return Status;
}

NTSTATUS Msgdrv_HandleSampleIoctl_NeitherIo(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *pdwDataWritten)
{
    NTSTATUS NtStatus = STATUS_UNSUCCESSFUL;
    PCHAR pInputBuffer;
    PCHAR pOutputBuffer;
    UINT dwDataRead = 0;
    PCHAR pReturnData = "msgdrv: IOCTL - Neither I/O From Kernel!";
    UINT dwDataSize = sizeof("msgdrv: IOCTL - Neither I/O From Kernel!");

    DbgPrint("Msgdrv_Handle_SampleIoctl_NeitherIo Called\n");

    pInputBuffer = pIoStackIrp->Parameters.DeviceIoControl.Type3InputBuffer;
    pOutputBuffer = Irp->UserBuffer;

    if(pInputBuffer && pOutputBuffer)
    {

        /*
         * We need this in an exception handler or else we could trap.
         */
        __try {
        
                ProbeForRead(pInputBuffer, pIoStackIrp->Parameters.DeviceIoControl.InputBufferLength, TYPE_ALIGNMENT(char));
                                                             
                /*
                 * We need to verify that the string is NULL terminated. Bad things can happen
                 * if we access memory not valid while in the Kernel.
                 */
               if(IsStringTerminated(pInputBuffer, pIoStackIrp->Parameters.DeviceIoControl.InputBufferLength, &dwDataRead))
               {
                    DbgPrint("msgdrv: UserModeMessage = '%s'", pInputBuffer);
                    
                    ProbeForWrite(pOutputBuffer, pIoStackIrp->Parameters.DeviceIoControl.OutputBufferLength, TYPE_ALIGNMENT(char));
                    DbgPrint("DataSize %i >= %i", pIoStackIrp->Parameters.DeviceIoControl.OutputBufferLength, dwDataSize);
                    if(pIoStackIrp->Parameters.DeviceIoControl.OutputBufferLength >= dwDataSize)
                    {
                        /*
                         * We use "RtlCopyMemory" in the kernel instead of memcpy.
                         * RtlCopyMemory *IS* memcpy, however it's best to use the
                         * wrapper in case this changes in the future.
                         */
                        RtlCopyMemory(pOutputBuffer, pReturnData, dwDataSize);
                        *pdwDataWritten = dwDataSize;
                        NtStatus = STATUS_SUCCESS;
                    }
                    else
                    {
                        *pdwDataWritten = dwDataSize;
                        NtStatus = STATUS_BUFFER_TOO_SMALL;
                    }

               }
                

        } __except( EXCEPTION_EXECUTE_HANDLER ) {

              NtStatus = GetExceptionCode();     
        }

    }

    return NtStatus;
}
