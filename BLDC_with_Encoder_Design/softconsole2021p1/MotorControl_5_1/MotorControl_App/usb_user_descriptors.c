/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : usb_user_descriptors.c                                        **
 **                                                                            **
 **  PLATFORM  : SmartFusion2                                                  **
 **                                                                            **
 **  COMPILER  : Softconsole                                                   **
 **                                                                            **
 **  AUTHOR    : India Solutions Team			                               **
 **                                                                            **
 **  VENDOR    : Microsemi                                                     **
 **                                                                            **
 **  DESCRIPTION  : USB HID Class Mouse Device Example application to 		   **
 **  				demonstrate the SmartFusion2 MSS USB operations in USB	   **
 **  				Device mode.    										   **
 ** 				This file provides the Device Descriptor used by Mouse 	   **
 ** 				Device. This file implements Application call-back 		   **
 ** 				Interface structure type provided						   **
 **                                                                            **
 *******************************************************************************/


/*******************************************************************************
 **                      Includes                                              **
 *******************************************************************************/
#include "mss_usb_device.h"
#include "mss_usb_std_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************
 * Constant definitions
 */
#define  USB_MAX_STRING_DESCRIPTOR_SIZE                     64u

/* String Descriptor Indexes */
#define  USB_STRING_DESCRIPTOR_IDX_LANGID                   0x00u
#define  USB_STRING_DESCRIPTOR_IDX_MANUFACTURER             0x01u
#define  USB_STRING_DESCRIPTOR_IDX_PRODUCT                  0x02u
#define  USB_STRING_DESCRIPTOR_IDX_SERIAL                   0x03u
#define  USB_STRING_DESCRIPTOR_IDX_CONFIG                   0x04u
#define  USB_STRING_DESCRIPTOR_IDX_INTERFACE                0x05u

#define  USB_STRING_MANUFACTURER                            "Microsemi Inc"
#define  USB_STRING_PRODUCT                                 "MSCC_UsbHID"
#define  USB_STRING_SERIAL                                  "HID1234"
#define  USB_STRING_CONFIG                                  "CFG0"
#define  USB_STRING_INTERFACE                               "Interface0"


/*******************************************************************************
 * Local functions.
 */
static uint8_t* hid_device_descriptor(uint32_t* length);
static uint8_t* hid_device_qual_descriptor(mss_usb_device_speed_t speed, uint32_t* length);
static uint8_t* hid_string_descriptor( uint8_t index, uint32_t* length);

static uint8_t hid_get_string(const uint8_t *string, uint8_t* dest);
/***************************************************************************//**
  Device descriptor.
 */
/*Variables defined at Block scope MISRA - required rule 8.7*/


/***************************************************************************//**
  Device qualifiers.
 */
/*Moved to block hid_device_qual_descriptor() -  As required by
 * MISRA - required rule 8.7
 * */


/*lint -e546*/
/*546: Suspicious use of '&' operator
 * 		Ignored because '&' operator is used to indicate address of function
 * 		as required by MISRA 2004: Rule 16.9*/

mss_usbd_user_descr_cb_t hid_mouse_descriptors_cb = {
    &hid_device_descriptor,
    &hid_device_qual_descriptor,
    &hid_string_descriptor,
};
/*lint +e546*/


static uint8_t*
hid_device_descriptor
(
    uint32_t* length
)
{
	static uint8_t device_descriptor[USB_STD_DEVICE_DESCR_LEN] =
	{
	    USB_STD_DEVICE_DESCR_LEN,                           /* bLength */
	    USB_DEVICE_DESCRIPTOR_TYPE,                         /* bDescriptorType */
	    0x00u,                                              /* bcdUSB LSB */
	    0x02u,                                              /* bcdUSB MSB */
	    0x00u,                                              /* bDeviceClass */
	    0x00u,                                              /* bDeviceSubClass */
	    0x00u,                                              /* bDeviceProtocol */
	    0x40u,                                              /* bMaxPacketSize0 */
	    0x14u,                                              /* idVendor LSB */
	    0x15u,                                              /* idVendor MSB */
	    0x15u,                                              /* idProduct LSB */
	    0x20u,                                              /* idProduct MSB */
	    0x00u,                                              /* bcdDevice LSB */
	    0x30u,                                              /* bcdDevice MSB */
	    USB_STRING_DESCRIPTOR_IDX_MANUFACTURER,             /* iManufacturer */
	    USB_STRING_DESCRIPTOR_IDX_PRODUCT,                  /* iProduct */
	    USB_STRING_DESCRIPTOR_IDX_SERIAL,                   /* iSerialNumber */
	    0x01u                                               /* bNumConfigurations */
	};
   *length = sizeof(device_descriptor);
   return(device_descriptor);
}

static uint8_t*
hid_device_qual_descriptor
(
    mss_usb_device_speed_t speed,
    uint32_t* length
)
{
	static uint8_t fs_dev_qualifier_descriptor[USB_STD_DEV_QUAL_DESCR_LENGTH] =
	{
	    USB_STD_DEV_QUAL_DESCR_LENGTH,                         /* bLength */
	    USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE,                /* bDescriptorType */
	    0x00u,                                              /* bcdUSB LSB */
	    0x02u,                                              /* bcdUSB MSB */
	    0x00u,                                              /* bDeviceClass */
	    0x00u,                                              /* bDeviceSubClass */
	    0x00u,                                              /* bDeviceProtocol */
	    0x40u,                                              /* bMaxPacketSize0 */
	    0x01u,                                              /* bNumConfigurations */
	    0x00u                                               /* Reserved */
	};
	static uint8_t hs_dev_qualifier_descriptor[USB_STD_DEV_QUAL_DESCR_LENGTH] =
	{
	    USB_STD_DEV_QUAL_DESCR_LENGTH,                         /* bLength */
	    USB_DEVICE_QUALIFIER_DESCRIPTOR_TYPE,                /* bDescriptorType */
	    0x00u,                                              /* bcdUSB LSB */
	    0x02u,                                              /* bcdUSB MSB */
	    0x00u,                                              /* bDeviceClass */
	    0x00u,                                              /* bDeviceSubClass */
	    0x00u,                                              /* bDeviceProtocol */
	    0x40u,                                              /* bMaxPacketSize0 */
	    0x01u,                                              /* bNumConfigurations */
	    0x00u                                               /* Reserved */
	};
	uint8_t* RetVal;
    if(speed == MSS_USB_DEVICE_HS)
    {
        *length = sizeof(fs_dev_qualifier_descriptor);
         RetVal = fs_dev_qualifier_descriptor;
    }
    else
    {
        *length = sizeof(hs_dev_qualifier_descriptor);
         RetVal = hs_dev_qualifier_descriptor;
    }
    return(RetVal);
}

static uint8_t*
hid_string_descriptor
(
    uint8_t index,
    uint32_t* length
)
{
	uint8_t* RetVal;
	static uint8_t lang_string_descriptor[] =
	{
	    0x04u,                                              /* bLength */
	    USB_STRING_DESCRIPTOR_TYPE,                          /* bDescriptorType */
	    0x09u,                                              /* LangID-LSB */
	    0x04u                                                /* LangID-MSB */
	};

	static uint8_t g_string_descriptor[USB_MAX_STRING_DESCRIPTOR_SIZE];
	/*lint -e926*/
	/*926: cast from pointer to pointer [MISRA 2004 Rule 11.4,
	 *     advisory]
	 *     This is an info message provided by PC-lint.
	 *     This is a known behaviour because the functions used
	 *     return a pointer.
     */
    switch(index)
    {
        case USB_STRING_DESCRIPTOR_IDX_LANGID:
                *length = sizeof(lang_string_descriptor);
            break;

        case USB_STRING_DESCRIPTOR_IDX_MANUFACTURER:
                *length = hid_get_string((uint8_t *) USB_STRING_MANUFACTURER,
                                         g_string_descriptor);
            break;

        case USB_STRING_DESCRIPTOR_IDX_PRODUCT:
            *length = hid_get_string((uint8_t*)USB_STRING_PRODUCT,
                                     g_string_descriptor);
            break;

        case USB_STRING_DESCRIPTOR_IDX_SERIAL:
            *length = hid_get_string((uint8_t*)USB_STRING_SERIAL,
                                     g_string_descriptor);
            break;

        case USB_STRING_DESCRIPTOR_IDX_CONFIG:
            *length = hid_get_string((uint8_t*)USB_STRING_CONFIG,
                                     g_string_descriptor);
            break;

        case USB_STRING_DESCRIPTOR_IDX_INTERFACE:
            *length = hid_get_string((uint8_t*)USB_STRING_INTERFACE,
                                     g_string_descriptor);
            break;
        default:
           /*Raise error*/
          *length = 0u;
        break;
        /*lint +e926*/
    }

    if(USB_STRING_DESCRIPTOR_IDX_LANGID == index)
    {
        RetVal = lang_string_descriptor;
    }
    else
    {
        RetVal = g_string_descriptor;
    }
    return(RetVal);
}

static uint8_t
hid_get_string
(
    const uint8_t *string,
    uint8_t* dest
)
{
    const uint8_t *idx = string;
    uint8_t *cp_dest;
    uint8_t len = 0u;
    cp_dest = dest;

    if((dest != 0u) && (string != 0u))
    {
    /*lint -e960*/
        for (; *(idx); ++idx)
        {
        	*(dest + 2u) = *(idx);
            dest++;
            *(dest + 2u) = 0x00u;
            dest++;
            len++;
        }
        len = (uint8_t)(len * 2u) + 2u;
        *cp_dest = len;                					/*bLength*/
        *(cp_dest+1u) = USB_STRING_DESCRIPTOR_TYPE;      /*bDesriptorType*/
    /*lint +e960*/
    }

    return(len);
}

#ifdef __cplusplus
}
#endif
