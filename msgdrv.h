/*

Module Name:
      
	  common.h
	  
Abstract:

      Definitions common to both the driver and the executable
	  
Environment:
      
      User and Kernel mode
	  
*/

#pragma once

//Driver includes

#include <ntifs.h>
#include <ntstrsafe.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned int UINT;
typedef char * PCHAR;

// Driver and device names.

#define DRIVER_NAME           L"msgdrv"
#define Driver_NAME_WITH_EXT  L"msgdrv.sys"

#define NT_DEVICE_NAME        L"\\Device\\MsgDrv"
#define DOS_DEVICE_LINK_NAME  L"\\DosDevices\\MsgDrv"
#define WIN32_DEVICE_NAME     L"\\\\.\\MsgDrv"


DRIVER_INITIALIZE DriverEntry;
DRIVER_UNLOAD     MsgDrvUnload;

__drv_dispatchType(IRP_MJ_CREATE)            DRIVER_DISPATCH MsgDrvCreate;
__drv_dispatchType(IRP_MJ_CLOSE)             DRIVER_DISPATCH MsgDrvClose;
__drv_dispatchType(IRP_MJ_CLEANUP)           DRIVER_DISPATCH MsgDrvCleanUp;
__drv_dispatchType(IRP_MJ_READ)              DRIVER_DISPATCH MsgDrvRead;
__drv_dispatchType(IRP_MJ_WRITE)             DRIVER_DISPATCH MsgDrvWrite;
__drv_dispatchType(IRP_MJ_DEVICE_CONTROL)    DRIVER_DISPATCH MsgDrvIoControl;

#ifndef IO_TYPE
#define IO_TYPE 0
#define USE_READ_FUNCTION   MsgDrvRead
#define USE_WRITE_FUNCTION  MsgDrvWrite
#endif

/* Internal Function */
BOOLEAN IsStringTerminated(PCHAR pString, UINT uiLength, UINT *pdwStringLength);
NTSTATUS Msgdrv_HandleSampleIoctl_NeitherIo(PIRP Irp, PIO_STACK_LOCATION pIoStackIrp, UINT *pdwDataWritten);

__in PDEVICE_OBJECT DeviceObj;
