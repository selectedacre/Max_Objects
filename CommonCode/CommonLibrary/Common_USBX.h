/*--------------------------------------------------------------------------------------*/
/*                                                                                      */
/*  File:       Common_USBX.h                                                           */
/*                                                                                      */
/*  Contains:   The data type declarations and forward references to routines.          */
/*                                                                                      */
/*  Written by: Norman Jaffe                                                            */
/*                                                                                      */
/*  Copyright:  (c) 2003 by T. H. Schiphorst, N. Jaffe, K. Gregory and G. I. Gregson.   */
/*                                                                                      */
/*              All rights reserved. Redistribution and use in source and binary forms, */
/*              with or without modification, are permitted provided that the following */
/*              conditions are met:                                                     */
/*                * Redistributions of source code must retain the above copyright      */
/*                  notice, this list of conditions and the following disclaimer.       */
/*                * Redistributions in binary form must reproduce the above copyright   */
/*                  notice, this list of conditions and the following disclaimer in the */
/*                  documentation and/or other materials provided with the              */
/*                  distribution.                                                       */
/*                * Neither the name of the copyright holders nor the names of its      */
/*                  contributors may be used to endorse or promote products derived     */
/*                  from this software without specific prior written permission.       */
/*                                                                                      */
/*              THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS     */
/*              "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT       */
/*              LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A */
/*              PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT      */
/*              OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,   */
/*              SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT        */
/*              LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,   */
/*              DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY   */
/*              THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT     */
/*              (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE   */
/*              OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.    */
/*                                                                                      */
/*  Created:    2003/11/11                                                              */
/*                                                                                      */
/*--------------------------------------------------------------------------------------*/

#if (! defined(COMMON_USBX_H_))
 #define COMMON_USBX_H_ /* */

 #include "Common_IOKitX.h"

 #if defined(COMPILE_FOR_CATS)
// The following datatypes and defines are copied from the relevant Framework headers,
// which aren't directly useable in CFM source:
  #define kIOUSBDeviceClassName					"IOUSBDevice"
  #define kIOUSBInterfaceClassName			"IOUSBInterface"
  #define kUSBVendorID									"idVendor"
  #define kUSBProductID									"idProduct"
  #define kUSBDevicePropertyAddress			"USB Address"
  #define kUSBDevicePropertyLocationID	"locationID"
  #define kUSBDeviceNumConfigs					"bNumConfigurations"
  #define kUSBNumEndpoints							"bNumEndpoints"

// 5c8187d0-9ef3-11d4-8b45-000a27052861
  #define kIOUSBDeviceInterfaceID CFUUIDGetConstantUUIDWithBytes(NULL,	\
    0x5c, 0x81, 0x87, 0xd0, 0x9e, 0xf3, 0x11, 0xD4,			\
    0x8b, 0x45, 0x00, 0x0a, 0x27, 0x05, 0x28, 0x61)
// 9dc7b780-9ec0-11d4-a54f-000a27052861
  #define kIOUSBDeviceUserClientTypeID CFUUIDGetConstantUUIDWithBytes(NULL,	\
    0x9d, 0xc7, 0xb7, 0x80, 0x9e, 0xc0, 0x11, 0xD4,			\
    0xa5, 0x4f, 0x00, 0x0a, 0x27, 0x05, 0x28, 0x61)
// 73c97ae8-9ef3-11d4-b1d0-000a27052861
  #define kIOUSBInterfaceInterfaceID CFUUIDGetConstantUUIDWithBytes(NULL,	\
    0x73, 0xc9, 0x7a, 0xe8, 0x9e, 0xf3, 0x11, 0xD4,			\
    0xb1, 0xd0, 0x00, 0x0a, 0x27, 0x05, 0x28, 0x61)
// 2d9786c6-9ef3-11d4-ad51-000a27052861
  #define kIOUSBInterfaceUserClientTypeID CFUUIDGetConstantUUIDWithBytes(NULL,	\
    0x2d, 0x97, 0x86, 0xc6, 0x9e, 0xf3, 0x11, 0xD4,			\
    0xad, 0x51, 0x00, 0x0a, 0x27, 0x05, 0x28, 0x61)

typedef void (* IOAsyncCallback1)
	(Pvoid		refcon,
	 IOReturn	result,
	 Pvoid		arg0);
	 
struct IOUSBFindInterfaceRequest
{
	UInt16	bInterfaceClass;		// requested class
	UInt16 	bInterfaceSubClass;		// requested subclass
	UInt16 	bInterfaceProtocol;		// requested protocol
	UInt16	bAlternateSetting;		// requested alt setting
}; /* IOUSBFindInterfaceRequest */

enum
{
	kIOUSBFindInterfaceDontCare	= 0xFFFF
};

enum
{
	kUSBHIDClass	= 3
};

struct IOUSBDeviceInterface;

struct IOUSBInterfaceInterface;

 // End of copies ...

  #define kUSBSerialNumber	"USB Serial Number"
 #else /* not COMPILE_FOR_CATS */
  #include "usb.h"
  
struct USBContext
{
  bool		fActive;
 	UInt32	fNotificationDeviceToken;
 	bool		fNotificationDeviceTokenValid;
 	Pvoid		fOwner;
}; /* USBContext */ 

 #endif /* not COMPILE_FOR_CATS */
 
 #if defined(COMPILE_FOR_CATS)
IOReturn closeUSBDevice
	(IOUSBDeviceInterface * *	theInterface);

IOReturn closeUSBInterface
	(IOUSBInterfaceInterface * *	theInterface);

IOReturn configureUSBDevice
	(IOUSBDeviceInterface * *	theInterface);

IOReturn createUSBInterfaceIterator
	(IOUSBDeviceInterface * *			theInterface,
	 IOUSBFindInterfaceRequest &	request,
	 io_iterator_t &							iter);

IOReturn getUSBDeviceProduct
	(IOUSBDeviceInterface * *	theInterface,
   UInt16 &									devProduct);

IOReturn getUSBDeviceReleaseNumber
	(IOUSBDeviceInterface * *	theInterface,
   UInt16 &									devRelNum);

IOReturn getUSBDeviceVendor
	(IOUSBDeviceInterface * *	theInterface,
   UInt16 &									devVendor);
   
IOReturn getUSBInterfaceClass
	(IOUSBInterfaceInterface * *	theInterface,
   UInt8 &											intfClass);
	
IOReturn getUSBInterfaceSubClass
	(IOUSBInterfaceInterface * *	theInterface,
   UInt8 &											intfSubClass);

IOReturn openUSBDevice
	(IOUSBDeviceInterface * *	theInterface);

IOReturn openUSBInterface
	(IOUSBInterfaceInterface * *	theInterface);

ULONG releaseUSBDevice
	(IOUSBDeviceInterface * *	theInterface);

ULONG releaseUSBInterface
	(IOUSBInterfaceInterface * *	theInterface);
 
 #else /* not COMPILE_FOR_CATS */
bool setUpUSB
	(Pvoid																obj,
	 Pchar																name,
	 USBContext &													rec,
	 USBDeviceNotificationCallbackProcPtr	notificationFunction);

void shutDownUSB
	(USBContext &	rec);
  
 #endif /* not COMPILE_FOR_CATS */

#endif /* not COMMON_USBX_H_ */