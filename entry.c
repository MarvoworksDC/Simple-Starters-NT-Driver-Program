#include "msgdrv.h"

NTSTATUS DriverEntry(__in PDRIVER_OBJECT DriverObject, __in PUNICODE_STRING RegistryPath)
{
	NTSTATUS Status;
	UNICODE_STRING NtDeviceName;
	UNICODE_STRING DosDevicesLinkName;
	
	UNREFERENCED_PARAMETER(RegistryPath);
	
	DbgPrint("msgdrv: DriverEntry()\n");
	
	RtlInitUnicodeString(&NtDeviceName, NT_DEVICE_NAME);
	
	Status = IoCreateDevice(DriverObject, 0, &NtDeviceName, FILE_DEVICE_UNKNOWN, 0, FALSE, &DeviceObj);
	
	if(!NT_SUCCESS(Status))
	{
		return Status;
	} else
	{
		DbgPrint("msgdrv: Device successful created\n");
	}
	
	/* Dispatch routines */
	
	DriverObject->MajorFunction[IRP_MJ_CREATE]         = MsgDrvCreate;
	DriverObject->MajorFunction[IRP_MJ_CLOSE]          = MsgDrvClose;
	DriverObject->MajorFunction[IRP_MJ_CLEANUP]        = MsgDrvCleanUp;
	DriverObject->MajorFunction[IRP_MJ_READ]           = USE_READ_FUNCTION;
	DriverObject->MajorFunction[IRP_MJ_WRITE]          = USE_WRITE_FUNCTION;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = MsgDrvIoControl;
	DriverObject->DriverUnload                         = MsgDrvUnload;
	
	DeviceObj->Flags |= IO_TYPE;
	
	DeviceObj->Flags &= (~DO_DEVICE_INITIALIZING);
	
	/* Create a link in the win32 namespace */
	
	RtlInitUnicodeString(&DosDevicesLinkName, DOS_DEVICE_LINK_NAME);
	
	Status = IoCreateSymbolicLink(&DosDevicesLinkName, &NtDeviceName);
	
	if(!NT_SUCCESS(Status))
	{
		IoDeleteDevice(DriverObject->DeviceObject);
		return Status;
	} else
	{
		DbgPrint("msgdrv: Win32 namespace successful created\n");
	}	
    return Status;  	
}
