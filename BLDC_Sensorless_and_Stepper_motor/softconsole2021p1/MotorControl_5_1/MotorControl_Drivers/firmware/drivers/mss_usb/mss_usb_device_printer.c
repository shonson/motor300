/*******************************************************************************
 * (c) Copyright 2012-2014 Microsemi SoC Products Group. All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Logical Layer (USB-LL)
 *          USBD-Printer class driver.
 *
 * USBD-Printer class driver implementation:
 * This source file implements Printer class functionality.
 *
 * SVN $Revision: 6995 $
 * SVN $Date: 2014-11-03 17:39:16 +0530 (Mon, 03 Nov 2014) $
 */
#include "mss_usb_device.h"
#include "../../CMSIS/mss_assert.h"
#include "mss_usb_device_printer.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MSS_USB_DEVICE_ENABLED

/*Values should be same as described in ep descriptors*/
#define PRINTER_TX_EP_MAX_PKT_SIZE_HS                       512
#define PRINTER_RX_EP_MAX_PKT_SIZE_HS                       512

/***************************************************************************//**
  Types Internal to the USBD-Printer class driver
 */
typedef enum mss_usbd_printer_events {
    PRINTER_EVENT_IDLE,
    PRINTER_EVENT_DATAIN,
    PRINTER_EVENT_DATAOUT,
    PRINTER_EVENT_DATAIN_ERROR,
    PRINTER_EVENT_DATAOUT_ERROR
} mss_usbd_printer_events_t;

/***************************************************************************//**
 Local functions used by USBD-Printer class driver.
 */
/***************************************************************************//**
 Implementations of Call-back functions used by USBD.
 */
uint8_t USBD_printer_init(uint8_t cfgidx, mss_usb_device_speed_t musb_speed);
uint8_t USBD_printer_deinit(uint8_t cfgidx);
uint8_t USBD_printer_data_in(mss_usb_ep_num_t num, uint8_t status);
uint8_t USBD_printer_cep_datain(uint8_t status);
uint8_t USBD_printer_cep_dataout(uint8_t status);

uint8_t* USBD_printer_get_descriptor
(
    uint8_t recepient,
    uint8_t type,
    uint32_t* length,
    mss_usb_device_speed_t musb_speed
);

uint8_t USBD_printer_data_out
(
    mss_usb_ep_num_t num,
    uint8_t status,
    uint32_t rx_count
);

uint8_t USBD_printer_process_request
(
    mss_usbd_setup_pkt_t* setup_pkt,
    uint8_t** buf_pp,
    uint32_t* length
);

/*******************************************************************************
 Global variables used by USBD-Printer class driver.
 */
/* This variable tracks the current state of the USBD-Printer driver. */
mss_usbd_printer_state_t g_usbd_printer_state = USBD_PRINTER_NOT_CONFIGURED;

/* The g_printer_events is used to know the current event in the BOT transfer */
volatile mss_usbd_printer_events_t g_printer_events = PRINTER_EVENT_IDLE;

/* Definition for printer application call-backs. */
mss_usbd_printer_cb_t *g_mss_usbd_printer_ops;

/* Printer class call-back function. */
mss_usbd_class_cb_t usb_printer_class_cb =
{
    USBD_printer_init,
    USBD_printer_deinit,
    USBD_printer_get_descriptor,
    USBD_printer_process_request,
    USBD_printer_data_in,
    USBD_printer_data_out,
    USBD_printer_cep_datain,
    USBD_printer_cep_dataout
};

uint8_t conf_descr[FULL_CONFIG_DESCR_LENGTH] =
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
    PRINTER_CLASS_INTERFACE_NUM,                    /* bInterfaceNumber */
    0x00u,                                          /* bAlternateSetting */
    0x02u,                                          /* bNumEndpoints */
    0x07u,                                          /* bInterfaceClass */
    0x01u,                                          /* bInterfaceSubClass */
    0x02u,                                          /* bInterfaceProtocol */
    0x05u,                                          /* bInterface */
    /*------------------------- IN Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x81u,                                          /* bEndpointAddress */
    0x02u,                                          /* bmAttributes */
    0x00u,                                          /* wMaxPacketSize LSB */ //22
    0x02u,                                          /* wMaxPacketSize MSB */ //23
    0xFFu,                                          /* bInterval *///ignored by host for Bulk IN EP
    /*------------------------- OUT Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    0x01u,                                          /* bEndpointAddress */
    0x02u,                                          /* bmAttributes */
    0x00u,                                          /* wMaxPacketSize LSB *///29
    0x02u,                                          /* wMaxPacketSize MSB *///30
    0xFFu                                           /* bInterval */                /*Max NAK rate*/
};

uint8_t fs_conf_descr[FULL_CONFIG_DESCR_LENGTH] =
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
    PRINTER_CLASS_INTERFACE_NUM,                    /* bInterfaceNumber */
    0x00u,                                          /* bAlternateSetting */
    0x02u,                                          /* bNumEndpoints */
    0x07u,                                          /* bInterfaceClass */
    0x01u,                                          /* bInterfaceSubClass */
    0x02u,                                          /* bInterfaceProtocol */
    0x05u,                                          /* bInterface */
    /*------------------------- IN Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    (0x80u | PRINTER_CLASS_BULK_IN_EP),             /* bEndpointAddress */
    0x02u,                                          /* bmAttributes */
    0x00u,                                          /* wMaxPacketSize LSB */ //22
    0x02u,                                          /* wMaxPacketSize MSB */ //23
    0xFFu,                                          /* bInterval *///ignored by host for Bulk IN EP
    /*------------------------- OUT Endpoint Descriptor --------------------------*/
    USB_STD_ENDPOINT_DESCR_LEN,                     /* bLength */
    USB_ENDPOINT_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    PRINTER_CLASS_BULK_OUT_EP,                      /* bEndpointAddress */
    0x02u,                                          /* bmAttributes */
    0x00u,                                          /* wMaxPacketSize LSB *///29
    0x02u,                                          /* wMaxPacketSize MSB *///30
    0xFFu                                           /* bInterval */                /*Max NAK rate*/
};

/***************************************************************************//**
  See mss_usb_device_printer.h for details.
 */
void
MSS_USBD_printer_init
(
    void
)
{
    MSS_USBD_set_class_cb_handler(&usb_printer_class_cb);
}

/***************************************************************************//**
  See mss_usb_device_printer.h for details.
 */
void
MSS_USBD_PRINTER_set_cb_handler
(
    mss_usbd_printer_cb_t* user_desc_cb
)
{
    g_mss_usbd_printer_ops = user_desc_cb;
}

/***************************************************************************//**
  See mss_usb_device_printer.h for details.
 */
void
MSS_USBD_PRINTER_write
(
    uint8_t* buf,
    uint32_t len
)
{
    MSS_USBD_tx_ep_write(PRINTER_CLASS_BULK_IN_EP, buf, len);
}

/***************************************************************************//**
  See mss_usb_device_printer.h for details.
 */
void
MSS_USBD_PRINTER_read_prepare
(
    uint8_t* buf,
    uint32_t len
)
{
    MSS_USBD_rx_ep_read_prepare(PRINTER_CLASS_BULK_OUT_EP,
                                (uint8_t*)buf,
                                len);
}

/***************************************************************************//**
  Function to read the printer driver state.
 */
mss_usbd_printer_state_t
MSS_USBD_printer_get_state
(
    void
)
{
    return g_usbd_printer_state;
}

/***************************************************************************//**
  Call back function definition.
 */
/***************************************************************************//**
 USBD_printer_init() call-back is called by USB Device mode driver on
 receiving SET_CONFIGURATION command. This function configure the transmit and
 receive endpoint as per parameter provided with this function and inform to the
 application about Printer initialization event.
 */
uint8_t
USBD_printer_init
(
    uint8_t cfgidx,
    mss_usb_device_speed_t musb_speed
)
{
    uint16_t bulk_rxep_fifo_sz = 0u;
    uint16_t bulk_rxep_maxpktsz = 0u;
    uint16_t bulk_txep_fifo_sz = 0u;
    uint16_t bulk_txep_maxpktsz = 0u;

    g_printer_events = PRINTER_EVENT_IDLE;
    g_usbd_printer_state = USBD_PRINTER_NOT_CONFIGURED;

    bulk_txep_fifo_sz = (uint16_t)((conf_descr[23u] << 8u) | (conf_descr[22u]));
    bulk_txep_maxpktsz = (uint16_t)((conf_descr[23u] << 8u) | (conf_descr[22u]));
    bulk_rxep_fifo_sz = (uint16_t)((conf_descr[30u] << 8u) | (conf_descr[29u]));
    bulk_rxep_maxpktsz = (uint16_t)((conf_descr[30u] << 8u) | (conf_descr[29u]));

    MSS_USBD_rx_ep_configure(PRINTER_CLASS_BULK_OUT_EP,
                             0x100u,
                             bulk_rxep_fifo_sz,
                             bulk_rxep_maxpktsz,
                             1u,
                             DMA_ENABLE,
                             MSS_USB_DMA_CHANNEL1,
                             MSS_USB_XFR_BULK,
                             NO_ZLP_TO_XFR);

    MSS_USBD_tx_ep_configure(PRINTER_CLASS_BULK_IN_EP,
                             0x200u,
                             bulk_txep_fifo_sz,
                             bulk_txep_maxpktsz,
                             1u,
                             DMA_ENABLE,
                             MSS_USB_DMA_CHANNEL2,
                             MSS_USB_XFR_BULK,
                             NO_ZLP_TO_XFR);

    /* Inform to the application. */
    if(g_mss_usbd_printer_ops->usb_printer_init != 0)
    {
        g_mss_usbd_printer_ops->usb_printer_init(cfgidx, musb_speed);
    }

    g_usbd_printer_state = USBD_PRINTER_CONFIGURED;

    return 1;
}

/***************************************************************************//**
  USBD_printer_deinit() call-back is called on receiving a CLEAR_CONFIGURATION
  request from the host. This function calls application call-back function
  pointed by usb_printer_deinit function to clear the configuration.
 */
uint8_t
USBD_printer_deinit
(
    uint8_t cfgidx
)
{
    /* Inform to the application about clear configuration command. */
    if(g_mss_usbd_printer_ops->usb_printer_deinit != 0)
    {
        g_mss_usbd_printer_ops->usb_printer_deinit(cfgidx);
    }

    g_usbd_printer_state = USBD_PRINTER_NOT_CONFIGURED;

    return 1;
}

/***************************************************************************//**
  USBD_printer_get_descriptor() function is called on receiving a GET_DESCRIPTOR
  request from the USB Host. This function internally calls application call-back
  function pointed by usb_printer_get_descriptor() function to returns the
  configuration descriptor requested by Host.
*/
uint8_t*
USBD_printer_get_descriptor
(
    uint8_t recepient,
    uint8_t type,
    uint32_t* length,
    mss_usb_device_speed_t musb_speed
)
{
    uint8_t* conf_descripter = 0x00u;

    /* Inform to the application about GET_DESCRIPTOR request from USB Host. */
    if(g_mss_usbd_printer_ops->usb_printer_get_descriptor != 0)
    {
        conf_descripter = g_mss_usbd_printer_ops->usb_printer_get_descriptor(recepient,
                                                                            type,
                                                                            length,
                                                                            musb_speed);
    }
    return conf_descripter;
}

/***************************************************************************//**
  USBD_printer_process_request() function is called on receiving a class
  specific request from the USB Host. This function internally calls the
  application call-back function pointed by usb_printer_process_request function
  to process the class specific request.
 */
uint8_t
USBD_printer_process_request
(
    mss_usbd_setup_pkt_t* setup_pkt,
    uint8_t** buf_pp,
    uint32_t* length
)
{
    uint8_t status = 0x0u;

    if(g_mss_usbd_printer_ops->usb_printer_process_request != 0)
    {
        status = g_mss_usbd_printer_ops->usb_printer_process_request(setup_pkt,
                                                                    buf_pp,
                                                                    length);
    }
    return status;
}

/***************************************************************************//**
  USBD_printer_data_in() call-back function is called on completion of the
  Current Data Transmissions (IN Transaction) which was previously initiated
  using MSS_USBD_tx_ep_configure(). This function calls the application
  call-back function pointed by usb_printer_datain function to inform about
  completion of IN transaction.
 */
uint8_t USBD_printer_data_in
(
    mss_usb_ep_num_t num,
    uint8_t status
)
{
    if((status & 0x1Fu) > 0x01u)
    {
        g_printer_events = PRINTER_EVENT_DATAIN_ERROR;
    }
    else
    {
        g_printer_events = PRINTER_EVENT_DATAIN;
    }

    /* Inform to the application about IN transaction completion. */
    if(g_mss_usbd_printer_ops->usb_printer_datain != 0)
    {
        g_mss_usbd_printer_ops->usb_printer_datain(num, status);
    }

    return 1;
}

/***************************************************************************//**
  USBD_printer_data_out() call-back function is called on completion of data
  reception. This function calls the application call-back function pointed by
  usb_printer_dataout function to inform about completion of OUT transaction.
 */
uint8_t
USBD_printer_data_out
(
    mss_usb_ep_num_t num,
    uint8_t status,
    uint32_t rx_count
)
{
    if((status & 0x1Fu) > 0x01u)
    {
        g_printer_events = PRINTER_EVENT_DATAOUT_ERROR;
    }
    else
    {
        g_printer_events = PRINTER_EVENT_DATAOUT;
    }

    /* Inform to the application about OUT transaction completion. */
    if(g_mss_usbd_printer_ops->usb_printer_dataout != 0)
    {
        g_mss_usbd_printer_ops->usb_printer_dataout(num, status, rx_count);
    }

    return 1;
}

/***************************************************************************//**
  USBD_printer_cep_datain() function is called when data transmission initiated
  by the USBD-Printer driver on Control Endpoint is complete.
*/
uint8_t
USBD_printer_cep_datain
(
    uint8_t status
)
{
    uint8_t return_status = 0x00u;

    if(g_mss_usbd_printer_ops->usb_printer_cep_datain != 0)
    {
        return_status = g_mss_usbd_printer_ops->usb_printer_cep_datain(status);
    }
    return return_status;
}

/***************************************************************************//**
  USBD_printer_cep_dataout() function is called when data reception initiated by
  the USBD-Printer driver on the control endpoint is complete.
*/
uint8_t
USBD_printer_cep_dataout
(
    uint8_t status
)
{
    uint8_t return_status = 0x00u;

    if(g_mss_usbd_printer_ops->usb_printer_cep_dataout != 0)
    {
        return_status = g_mss_usbd_printer_ops->usb_printer_cep_dataout(status);
    }

    return return_status;
}

#endif //MSS_USB_DEVICE_ENABLED

#ifdef __cplusplus
}
#endif
