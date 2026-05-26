/*******************************************************************************
 * (c) Copyright 2012-2014 Microsemi SoC Products Group. All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Logical Layer (USB-LL)
 *          USBD-VENDOR class driver Template.
 *
 * USBD-VENDOR class driver Template:
 * This source file implements a template for Vendor class implementation.
 * This template must be modified to implement the actual vendor specific
 * class functionality.
 *
 * SVN $Revision: 7024 $
 * SVN $Date: 2014-11-13 19:22:25 +0530 (Thu, 13 Nov 2014) $
 */
#include "mss_usb_device_vendor.h"
#include "mss_usb_device.h"
#include "mss_usb_std_def.h"
#include "../../CMSIS/mss_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MSS_USB_DEVICE_ENABLED

#define VENDOR_INTR_IN_EP                              MSS_USB_TX_EP_1
#define VENDOR_INTR_OUT_EP                             MSS_USB_RX_EP_1
#define VENDOR_BULK_IN_EP                              MSS_USB_TX_EP_2
#define VENDOR_BULK_OUT_EP                             MSS_USB_RX_EP_2

#define VENDOR_CLASS_INTERFACE_NUM                        0x00
/*HS Operation values*/
#define VENDOR_TX_EP_FIFO_SIZE_HS                         512
#define VENDOR_RX_EP_FIFO_SIZE_HS                         512

/*Values should be same as described in ep descriptors*/
#define VENDOR_TX_EP_MAX_PKT_SIZE_HS                      512
#define VENDOR_RX_EP_MAX_PKT_SIZE_HS                      512

/*LS Operation values*/
#define VENDOR_TX_EP_FIFO_SIZE_FS                         64
#define VENDOR_RX_EP_FIFO_SIZE_FS                         64

/*Values should be same as described in ep descriptors*/
#define VENDOR_TX_EP_MAX_PKT_SIZE_FS                      64
#define VENDOR_RX_EP_MAX_PKT_SIZE_FS                      64

/* Full configuration descriptor length*/
#define FULL_CONFIG_DESCR_LENGTH                    (USB_STD_CONFIG_DESCR_LEN + \
                                                     USB_STD_INTERFACE_DESCR_LEN + \
                                                     USB_STD_ENDPOINT_DESCR_LEN + \
                                                     USB_STD_ENDPOINT_DESCR_LEN + \
                                                     USB_STD_ENDPOINT_DESCR_LEN + \
                                                     USB_STD_ENDPOINT_DESCR_LEN )


/***************************************************************************//**
 Implementations of Call-back functions used by USBD.
 */
uint8_t* USBD_VENDOR_get_descriptor_cb(uint8_t recepient,
                                    uint8_t type,
                                    uint32_t* length,
                                    mss_usb_device_speed_t musb_speed);

uint8_t USBD_VENDOR_init_cb(uint8_t cfgidx, mss_usb_device_speed_t musb_speed);
uint8_t USBD_VENDOR_deinit_cb(uint8_t cfgidx);
uint8_t USBD_VENDOR_data_in_cb(mss_usb_ep_num_t num, uint8_t status);

uint8_t USBD_VENDOR_data_out_cb(mss_usb_ep_num_t num,
                             uint8_t status,
                             uint32_t rx_count);

uint8_t USBD_VENDOR_process_request_cb(mss_usbd_setup_pkt_t* setup_pkt,
                                    uint8_t** buf_pp,
                                    uint32_t* length);

uint8_t USBD_VENDOR_cep_data_in_cb(uint8_t status);

uint8_t USBD_VENDOR_cep_data_out_cb(uint8_t status);

/*******************************************************************************
 Global variables used by USBD-VENDOR class driver.
 */

/* USB current Speed of operation selected by user*/
mss_usb_device_speed_t g_usbd_vendor_user_speed;

#if defined(__GNUC__)
static uint8_t g_req_tx_data[25] __attribute__ ((aligned (4))) = "Example instruction data";
static uint8_t g_bulk_tx_data[64] __attribute__ ((aligned (4))) = "Bulk Endpoint data";
static uint8_t g_intr_tx_data[64] __attribute__ ((aligned (4))) = "Interrupt Endpoint data";

static uint8_t g_req_rx_data[25] __attribute__ ((aligned (4))) = {0};
static uint8_t g_bulk_rx_data[64] __attribute__ ((aligned (4))) = {0};
static uint8_t g_intr_rx_data[64] __attribute__ ((aligned (4))) = {0};
#elif defined(__ICCARM__)
#pragma data_alignment = 4
static uint8_t g_req_tx_data[25] = "Example instruction data";
static uint8_t g_bulk_tx_data[64]="Bulk Endpoint data";
static uint8_t g_intr_tx_data[64]="Interrupt Endpoint data";

static uint8_t g_req_rx_data[25]={0};
static uint8_t g_bulk_rx_data[64]={0};
static uint8_t g_intr_rx_data[64]={0};
#elif defined(__CC_ARM)
__align(4) static uint8_t g_req_tx_data[25] = "Example instruction data";
__align(4) static uint8_t g_bulk_tx_data[64]="Bulk Endpoint data";
__align(4) static uint8_t g_intr_tx_data[64]="Interrupt Endpoint data";

__align(4) static uint8_t g_req_rx_data[25]={0};
__align(4) static uint8_t g_bulk_rx_data[64]={0};
__align(4) static uint8_t g_intr_rx_data[64]={0};
#endif

mss_usbd_class_cb_t usb_vendor_class_cb = {USBD_VENDOR_init_cb,
                                           0,
                                           USBD_VENDOR_get_descriptor_cb,
                                           USBD_VENDOR_process_request_cb,
                                           USBD_VENDOR_data_in_cb,
                                           USBD_VENDOR_data_out_cb,
                                           USBD_VENDOR_cep_data_in_cb,
                                           USBD_VENDOR_cep_data_out_cb };

uint8_t vendor_fs_conf_descr[FULL_CONFIG_DESCR_LENGTH] =
{
    /*----------------------- Configuration Descriptor -----------------------*/
    USB_STD_CONFIG_DESCR_LEN,                       /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,              /* bDescriptorType */
    FULL_CONFIG_DESCR_LENGTH,                       /* wTotalLength LSB */
    0x00u,                                          /* wTotalLength MSB */
    0x01u,                                          /* bNumInterfaces */
    0x01u,                                          /* bConfigurationValue */
    0x04u,                                          /* iConfiguration */
    0xC0u,                                          /* bmAttributes */
    0x32u,                                          /* bMaxPower */
        /*------------------------- Interface Descriptor -------------------------*/
    USB_STD_INTERFACE_DESCR_LEN,                    /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                  /* bDescriptorType */
    VENDOR_CLASS_INTERFACE_NUM,                     /* bInterfaceNumber */
    0x00u,                                          /* bAlternateSetting */
    0x04u,                                          /* bNumEndpoints */
    0xFFu,                                          /* bInterfaceClass */
    0xFFu,                                          /* bInterfaceSubClass */
    0xFFu,                                          /* bInterfaceProtocol */
    0x05u,                                          /* bInterface */
    /*------------------------- Interrupt IN Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    (0x80u |VENDOR_INTR_IN_EP),                     /* bEndpointAddress */
    0x03u,                                          /* bmAttributes --Interrupt */
    0x40u,                                          /* wMaxPacketSize LSB */ //22
    0x00u,                                          /* wMaxPacketSize MSB */ //23
    0xFFu,                                          /* bInterval */
    /*------------------------- Interrupt OUT Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    VENDOR_INTR_OUT_EP,                             /* bEndpointAddress */
    0x03u,                                          /* bmAttributes -- Interrupt */
    0x40u,                                          /* wMaxPacketSize LSB *///29
    0x00u,                                          /* wMaxPacketSize MSB *///30
    0xFFu,                                          /* bInterval *//*Max NAK rate*/
    /*------------------------- Bulk IN Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    (0x80u | VENDOR_BULK_IN_EP),                    /* bEndpointAddress */
    0x02u,                                          /* bmAttributes --Bulk */
    0x40u,                                          /* wMaxPacketSize LSB */ //22
    0x00u,                                          /* wMaxPacketSize MSB */ //23
    0xFFu,                                          /* bInterval *///ignored by host for Bulk IN EP
    /*--------------------- Bulk  OUT endpoint Descriptor ----------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    VENDOR_BULK_OUT_EP,                             /* bEndpointAddress */
    0x02u,                                          /* bmAttributes -- Bulk */
    0x40u,                                          /* wMaxPacketSize LSB *///29
    0x00u,                                          /* wMaxPacketSize MSB *///30
    0xFFu                                           /* bInterval */ /*Max NAK rate*/
};

/*
 Configuration descriptor and sub-ordinate descriptors to enumerate the USB device
 as Mass Storage class Device by host.
 */
uint8_t vendor_hs_conf_descr[FULL_CONFIG_DESCR_LENGTH] =
{
    /*----------------------- Configuration Descriptor -----------------------*/
    USB_STD_CONFIG_DESCR_LEN,                       /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,              /* bDescriptorType */
    FULL_CONFIG_DESCR_LENGTH,                       /* wTotalLength LSB */
    0x00u,                                          /* wTotalLength MSB */
    0x01u,                                          /* bNumInterfaces */
    0x01u,                                          /* bConfigurationValue */
    0x04u,                                          /* iConfiguration */
    0xC0u,                                          /* bmAttributes */
    0x32u,                                          /* bMaxPower */
    /*------------------------- Interface Descriptor -------------------------*/
    USB_STD_INTERFACE_DESCR_LEN,                    /* bLength */
    USB_INTERFACE_DESCRIPTOR_TYPE,                  /* bDescriptorType */
    VENDOR_CLASS_INTERFACE_NUM,                     /* bInterfaceNumber */
    0x00u,                                          /* bAlternateSetting */
    0x04u,                                          /* bNumEndpoints */
    0xFFu,                                          /* bInterfaceClass */
    0xFFu,                                          /* bInterfaceSubClass */
    0xFFu,                                          /* bInterfaceProtocol */
    0x05u,                                          /* bInterface */
    /*------------------------- Interrupt IN Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    (0x80u |VENDOR_INTR_IN_EP),                     /* bEndpointAddress */
    0x03u,                                          /* bmAttributes --Interrupt */
    0x40u,                                          /* wMaxPacketSize LSB */ //22
    0x00u,                                          /* wMaxPacketSize MSB */ //23
    0xFFu,                                          /* bInterval */  //ignored by host for Bulk IN EP
    /*------------------------- Interrupt OUT Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    VENDOR_INTR_OUT_EP,                             /* bEndpointAddress */
    0x03u,                                          /* bmAttributes -- Interrupt */
    0x40u,                                          /* wMaxPacketSize LSB *///29
    0x00u,                                          /* wMaxPacketSize MSB *///30
    0xFFu,                                          /* bInterval *//*Max NAK rate*/
    /*------------------------- Bulk IN Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    (0x80u | VENDOR_BULK_IN_EP),                    /* bEndpointAddress */
    0x02u,                                          /* bmAttributes --Bulk */
    0x00u,                                          /* wMaxPacketSize LSB */ //22
    0x02u,                                          /* wMaxPacketSize MSB */ //23
    0xFFu,                                          /* bInterval *///ignored by host for Bulk IN EP
    /*------------------------- Bulk  Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    VENDOR_BULK_OUT_EP,                             /* bEndpointAddress */
    0x02u,                                          /* bmAttributes -- Bulk */
    0x00u,                                          /* wMaxPacketSize LSB *///29
    0x02u,                                          /* wMaxPacketSize MSB *///30
    0xFFu                                           /* bInterval */ /*Max NAK rate*/
};

void
MSS_USBD_VENDOR_init
(
    mss_usb_device_speed_t speed
)
{
    g_usbd_vendor_user_speed = speed;

    MSS_USBD_set_class_cb_handler(&usb_vendor_class_cb);
}

uint8_t
USBD_VENDOR_init_cb
(
    uint8_t cfgidx,
    mss_usb_device_speed_t musb_speed
)
{
    uint16_t bulk_rxep_fifo_sz = 0u;
    uint16_t bulk_rxep_maxpktsz = 0u;

    uint16_t bulk_txep_fifo_sz = 0u;
    uint16_t bulk_txep_maxpktsz = 0u;

    /*User Selected FS: Operate only in FS
      User Selected HS:
        Device connected to 2.0 Host(musb_speed = HS):Operate in HS
        Device connected to 1.x Host(musb_speed = FS):Operate in FS
    */
    if(musb_speed == MSS_USB_DEVICE_FS)
    {
        bulk_txep_fifo_sz = (uint16_t)((vendor_fs_conf_descr[23u] << 8u) | (vendor_fs_conf_descr[22u]));
        bulk_txep_maxpktsz = (uint16_t)((vendor_fs_conf_descr[23u] << 8u) | (vendor_fs_conf_descr[22u]));
        bulk_rxep_fifo_sz = (uint16_t)((vendor_fs_conf_descr[30u] << 8u) | (vendor_fs_conf_descr[29u]));
        bulk_rxep_maxpktsz = (uint16_t)((vendor_fs_conf_descr[30u] << 8u) | (vendor_fs_conf_descr[29u]));
    }
    else if(musb_speed == MSS_USB_DEVICE_HS)
    {
        bulk_txep_fifo_sz = (uint16_t)((vendor_hs_conf_descr[23u] << 8u) | (vendor_hs_conf_descr[22u]));
        bulk_txep_maxpktsz = (uint16_t)((vendor_hs_conf_descr[23u] << 8u) | (vendor_hs_conf_descr[22u]));
        bulk_rxep_fifo_sz = (uint16_t)((vendor_hs_conf_descr[30u] << 8u) | (vendor_hs_conf_descr[29u]));
        bulk_rxep_maxpktsz = (uint16_t)((vendor_hs_conf_descr[30u] << 8u) | (vendor_hs_conf_descr[29u]));
    }
    else
        ASSERT(0);

    MSS_USBD_rx_ep_configure(VENDOR_INTR_OUT_EP,
                             0x00,
                             64u,
                             64u,
                             1u,
                             DMA_DISABLE,
                             MSS_USB_DMA_CHANNEL_NA,
                             MSS_USB_XFR_INTERRUPT,
                             NO_ZLP_TO_XFR);

    MSS_USBD_rx_ep_read_prepare(VENDOR_INTR_OUT_EP,
                                (uint8_t*)&g_intr_rx_data,
                                sizeof(g_intr_rx_data));

    MSS_USBD_tx_ep_configure(VENDOR_INTR_IN_EP,
                             64u,
                             64u,
                             64u,
                             1u,
                             DMA_DISABLE,
                             MSS_USB_DMA_CHANNEL_NA,
                             MSS_USB_XFR_INTERRUPT,
                             NO_ZLP_TO_XFR);

    MSS_USBD_rx_ep_configure(VENDOR_BULK_OUT_EP,
                             128u,
                             bulk_rxep_fifo_sz,
                             bulk_rxep_maxpktsz,
                             1u,
                             DMA_DISABLE,
                             MSS_USB_DMA_CHANNEL_NA,
                             MSS_USB_XFR_BULK,
                             ADD_ZLP_TO_XFR);

    MSS_USBD_rx_ep_read_prepare(VENDOR_BULK_OUT_EP,
                                (uint8_t*)&g_bulk_rx_data,
                                sizeof(g_bulk_rx_data));

    MSS_USBD_tx_ep_configure(VENDOR_BULK_IN_EP,
                             640u,
                             bulk_txep_fifo_sz,
                             bulk_txep_maxpktsz,
                             1u,
                             DMA_DISABLE,
                             MSS_USB_DMA_CHANNEL_NA,
                             MSS_USB_XFR_BULK,
                             ADD_ZLP_TO_XFR);

    return USB_SUCCESS;
}

uint8_t USBD_VENDOR_deinit_cb(uint8_t cfgidx)
{
    return USB_SUCCESS;
}

uint8_t* USBD_VENDOR_get_descriptor_cb(uint8_t recepient,
                                    uint8_t type,
                                    uint32_t* length,
                                    mss_usb_device_speed_t musb_speed)
{
    uint8_t* conf_desc=0;
    uint8_t* os_conf_desc=0;
    uint8_t conf_desc_len=0;
    uint8_t os_conf_desc_len=0;

    /*User Selected FS:
        Operate only in FS
      User Selected HS:
        Device connected to 2.0 Host(musb_speed = HS):Operate in HS
        Device connected to 1.x Host(musb_speed = FS):Operate in FS
    */
    if(g_usbd_vendor_user_speed == MSS_USB_DEVICE_FS)
    {
        conf_desc = vendor_fs_conf_descr;
        conf_desc[1u] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
        conf_desc_len = sizeof(vendor_fs_conf_descr);
        os_conf_desc = 0u;
        os_conf_desc_len = 0u;
    }
    else if(g_usbd_vendor_user_speed == MSS_USB_DEVICE_HS)
    {
        if(musb_speed == MSS_USB_DEVICE_HS)
        {
            conf_desc = vendor_hs_conf_descr;
            conf_desc[1u] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
            conf_desc_len = sizeof(vendor_hs_conf_descr);
            os_conf_desc = vendor_fs_conf_descr;
            os_conf_desc[1u] = USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE;
            os_conf_desc_len = sizeof(vendor_fs_conf_descr);
        }
        else if(musb_speed == MSS_USB_DEVICE_FS)
        {
            conf_desc = vendor_fs_conf_descr;
            conf_desc[1u] = USB_CONFIGURATION_DESCRIPTOR_TYPE;
            conf_desc_len = sizeof(vendor_fs_conf_descr);
            os_conf_desc = vendor_hs_conf_descr;
            os_conf_desc[1u] = USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE;
            os_conf_desc_len = sizeof(vendor_hs_conf_descr);
        }
    }
    else
        ASSERT(0);      //user must select FS or HS, nothing else.

    if(recepient == USB_STD_REQ_RECIPIENT_DEVICE)
    {
        if(type == USB_CONFIGURATION_DESCRIPTOR_TYPE)
        {
           *length = conf_desc_len;
            return(conf_desc);
        }
        else if (type == USB_OTHER_SPEED_CONFIG_DESCRIPTOR_TYPE)
        {
            ASSERT(os_conf_desc != 0u);
            *length = os_conf_desc_len;
            return(os_conf_desc);
        }
    }
    else if(recepient == USB_STD_REQ_RECIPIENT_ENDPOINT)
    {
        /*Do nothing*/
    }
    else if(recepient == USB_STD_REQ_RECIPIENT_INTERFACE)
    {
        /*Do nothing*/
    }
    else
    {
        /*Do nothing*/
    }
    return USB_FAIL;

}
/***************************************************************************//**
 USBD_VENDOR_process_request_cb() call-back function Process the VENDOR class requests.
 */
uint8_t
USBD_VENDOR_process_request_cb
(
    mss_usbd_setup_pkt_t* setup_pkt,
    uint8_t** buf_pp,
    uint32_t* length
)
{
    if(setup_pkt->length)
    {
        if(setup_pkt->request_type & USB_STD_REQ_DATA_DIR_MASK) /*DtoH*/
        {
            *buf_pp = (uint8_t*)&g_req_tx_data;
            *length = setup_pkt->length;
            return USB_SUCCESS;
        }
        else
        {
            /*CEP configuration and read_prepare is taken care by USBD.
              Here we only pass the buffer address
              Once the data is received the callback function
              USBD_VENDOR_cep_data_out_cb will be called by USBD driver.
              The actual processing of the data shall be done then.*/
            *buf_pp = (uint8_t*)&g_req_rx_data;
            *length = setup_pkt->length;
            return USB_SUCCESS;
        }

    }
    else
        return USB_SUCCESS;
}

/***************************************************************************//**
 USBD_VENDOR_data_in_cb() call-back function is called by USB Device mode driver
 on completion of the Current Data Transmissions (IN Transaction) which was
 previously initiated using MSS_USBD_tx_ep_configure().
 */
uint8_t USBD_VENDOR_data_in_cb
(
    mss_usb_ep_num_t num,
    uint8_t status
)
{
    if((status & 0x1Fu) > 0x01u)
    {
        /* Take error mitigation action based on the error indication "status" */
    }
    else
    {
        if (VENDOR_INTR_IN_EP == num)
        {
            MSS_USBD_rx_ep_read_prepare(VENDOR_INTR_OUT_EP,
                                        (uint8_t*)&g_intr_rx_data,
                                        sizeof(g_intr_rx_data));
        }
        else if (VENDOR_BULK_IN_EP == num)
        {
            MSS_USBD_rx_ep_read_prepare(VENDOR_BULK_OUT_EP,
                                        (uint8_t*)&g_bulk_rx_data,
                                        sizeof(g_bulk_rx_data));
        }
        else
        {
            ASSERT(0); /*Endpoint number not as per descriptors.*/
        }
    }

    return USB_SUCCESS;
}

/***************************************************************************//**
 USBD_VENDOR_data_out_cb() call-back function is called by USB Device mode driver
 on completion of data reception. USB Device mode driver must have been
 previously prepared for this data reception using
 MSS_USBD_rx_ep_read_prepare()
 */
uint8_t
USBD_VENDOR_data_out_cb
(
    mss_usb_ep_num_t num,
    uint8_t status,
    uint32_t rx_count
)
{
    if((status & 0x1Fu) > 0x01u)
    {
        /* Take error mitigation action based on the error indication "status" */
    }
    else
    {
        if (VENDOR_INTR_IN_EP == num)
        {
            MSS_USBD_tx_ep_write(VENDOR_INTR_IN_EP, g_intr_tx_data, sizeof(g_intr_tx_data));
        }
        else if (VENDOR_BULK_IN_EP == num)
        {
            MSS_USBD_tx_ep_write(VENDOR_BULK_IN_EP, g_bulk_tx_data, sizeof(g_bulk_tx_data));
        }
        else
        {
            ASSERT(0); /*Endpoint number not as per descriptors.*/
        }

    }

    return USB_SUCCESS;
}

uint8_t USBD_VENDOR_cep_data_in_cb(uint8_t status)
{
    /*
     If there was data phase in the IN control transfer then this callback function is called
     at the end of the IN data phase of that control transfer.
     The status parameter of this function indicates the error status of the OUT data phase.
     */
    return USB_SUCCESS;
}

uint8_t USBD_VENDOR_cep_data_out_cb(uint8_t status)
{
    /*
     If there was data phase in the OUT control transfer then this callback function is called
     at the end of OUT data phase of that control transfer.
     You can process the data now.
     The status parameter of this function indicates the error status of the OUT data phase.
     */

    return USB_SUCCESS;
}
#endif //MSS_USB_DEVICE_ENABLED

#ifdef __cplusplus
}
#endif
