/**********************************************************************
 * 
 *  Marvin Kikomeko
 *
 *  msgdrv Module
 *
 **********************************************************************/

#ifndef __IOCTL_H__
#define __IOCTL_H__

/* 
 *   IOCTL's are defined by the following bit layout.
 *   [ Common | Device Type | Required Access | Custom | Function Code | Transfer Type ]
 *      31     30         16 15             14    13    12           2  1            0
 * 
 *   Common          - 1 bit.  This is set for user-defined device types.
 *   Device Type     - This is the type of device the IOCTL belongs to.  This can be user defined (Common bit set).  This must match the device type of the device object.
 *   Required Access - FILE_READ_DATA, FILE_WRITE_DATA, etc.  This is the required access for the device.
 *   Custom          - 1 bit.  This is set for user-defined IOCTL's.  This is used in the same manner as "WM_USER".
 *   Function Code   - This is the function code that the system or the user defined (custom bit set)
 *   Transfer Type   - METHOD_IN_DIRECT, METHOD_OUT_DIRECT, METHOD_NEITHER, METHOD_BUFFERED, This the data transfer method to be used.
 *
 */
 
#define IOCTL_MSGDRV_NEITHER_IO      CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_NEITHER,FILE_READ_DATA | FILE_WRITE_DATA) 

#endif
