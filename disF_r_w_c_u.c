#include "msgdrv.h"

NTSTATUS MsgDrvCreate(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	
	DbgPrint("msgdrv: CreateFile() requested from user mode!\n");
	
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Status = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	
	return STATUS_SUCCESS;
}

NTSTATUS MsgDrvClose(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	
	DbgPrint("msgdrv: CloseHandle() requested from user mode!\n");
	
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Status = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	
	return STATUS_SUCCESS;
}

NTSTATUS MsgDrvCleanUp(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
	UNREFERENCED_PARAMETER(DeviceObject);
	
	DbgPrint("msgdrv: CleanUp() requested!\n");
	
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Status = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	
	return STATUS_SUCCESS;
}

NTSTATUS MsgDrvRead(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
    NTSTATUS NtStatus = STATUS_BUFFER_TOO_SMALL;
    PIO_STACK_LOCATION pIoStackIrp = NULL;
    PCHAR pReturnData = "\n\nDeviceRead() - Hello from the Kernel!\n\nDesigned by Marvoworks(Devmarvo)\n\n";
    UINT dwDataSize = sizeof("\n\nDeviceRead() - Hello from the Kernel!\n\nDesigned by Marvoworks(Devmarvo)\n\n");
    UINT dwDataRead = 0;
    PCHAR pReadDataBuffer;
	
	UNREFERENCED_PARAMETER(DeviceObject);

    DbgPrint("msgdrv: DeviceRead Called \n");
    
    pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);
    
    if(pIoStackIrp)
    {
        /*
         * We need this in an exception handler or else we could trap.
         */
        __try {
        
            if(pIoStackIrp->Parameters.Read.Length >= dwDataSize && Irp->UserBuffer)
            {

                ProbeForWrite(Irp->UserBuffer, pIoStackIrp->Parameters.Read.Length, TYPE_ALIGNMENT(char));
                pReadDataBuffer = Irp->UserBuffer;

                RtlCopyMemory(pReadDataBuffer, pReturnData, dwDataSize); 
                dwDataRead = dwDataSize;
                NtStatus = STATUS_SUCCESS;
            }

        } __except( EXCEPTION_EXECUTE_HANDLER ) {

              NtStatus = GetExceptionCode();     
        }
        
    }

    Irp->IoStatus.Status = NtStatus;
    Irp->IoStatus.Information = dwDataRead;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return NtStatus;
}

NTSTATUS MsgDrvWrite(__in PDEVICE_OBJECT DeviceObject, __in PIRP Irp)
{
    NTSTATUS NtStatus = STATUS_UNSUCCESSFUL;
    PIO_STACK_LOCATION pIoStackIrp = NULL;
    PCHAR pWriteDataBuffer;
    UINT dwDataWritten = 0;
	
	UNREFERENCED_PARAMETER(DeviceObject);
	
    DbgPrint("msgdrv: DeviceWrite() Called \n");
    
    /*
     * Each time the IRP is passed down the driver stack a new stack location is added
     * specifying certain parameters for the IRP to the driver.
     */
    pIoStackIrp = IoGetCurrentIrpStackLocation(Irp);
    
    if(pIoStackIrp)
    {
        /*
         * We need this in an exception handler or else we could trap.
         */
        __try {
        
                if(Irp->UserBuffer)
                {

                    ProbeForRead(Irp->UserBuffer, pIoStackIrp->Parameters.Write.Length, TYPE_ALIGNMENT(char));
                    pWriteDataBuffer = Irp->UserBuffer;
                                             
                    /*
                     * We need to verify that the string is NULL terminated. Bad things can happen
                     * if we access memory not valid while in the Kernel.
                     */
                   if(IsStringTerminated(pWriteDataBuffer, pIoStackIrp->Parameters.Write.Length, &dwDataWritten))
                   {
                        DbgPrint(pWriteDataBuffer);
                        NtStatus = STATUS_SUCCESS;
                   }
                }

        } __except( EXCEPTION_EXECUTE_HANDLER ) {

              NtStatus = GetExceptionCode();     
        }

    }

    Irp->IoStatus.Status = NtStatus;
    Irp->IoStatus.Information = dwDataWritten;

    IoCompleteRequest(Irp, IO_NO_INCREMENT);

    return NtStatus;	
}

void MsgDrvUnload(__in PDRIVER_OBJECT DriverObject)
{
    UNICODE_STRING  DosDevicesLinkName;
    
    //
    // Delete the link from our device name to a name in the Win32 namespace.
    //
    RtlInitUnicodeString(&DosDevicesLinkName, DOS_DEVICE_LINK_NAME);
    IoDeleteSymbolicLink(&DosDevicesLinkName);

    //
    // Finally delete our device object
    //

    IoDeleteDevice(DriverObject->DeviceObject);
	
	DbgPrint("msgdrv: DeviceUnload successful!\n");
}

BOOLEAN IsStringTerminated(PCHAR pString, UINT uiLength, UINT *pdwStringLength)
{
    BOOLEAN bStringIsTerminated = FALSE;
    UINT uiIndex = 0;
    
    DbgPrint("msgdrv: IsStringTerminated(0x%0x, %d)\r\n", pString, uiLength);

    *pdwStringLength = 0;

    while(uiIndex < uiLength && bStringIsTerminated == FALSE)
    {
        if(pString[uiIndex] == '\0')
        {
            *pdwStringLength = uiIndex + 1; /* Include the total count we read, includes the NULL */
            bStringIsTerminated = TRUE;
            DbgPrint("msgdrv:  String Is Terminated!\r\n");
        }
        else
        {
           uiIndex++;
        }
    }

    return bStringIsTerminated;
}
