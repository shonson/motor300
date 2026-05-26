/***************************************************************************//**
 * (c) Copyright 2012-2014 Microsemi SoC Products Group. All rights reserved.
 *
 * Smartfusion2 MSS USB Driver Stack
 *      USB Logical Layer (USB-LL)
 *          USBH driver
 *
 * USBH driver implementation:
 * This source file implements the common functionality of USB host mode,
 *
 * V2.3 Added Add SetAddrDelay 2ms fix
 * V2.3 Host performance improvement changes
 *
 * SVN $Revision: 6994 $
 * SVN $Date: 2014-11-03 17:21:16 +0530 (Mon, 03 Nov 2014) $
 */

#include "mss_usb_host.h"
#include "mss_usb_std_def.h"
#include "mss_usb_host_cif.h"
#include "mss_usb_common_cif.h"
#include "mss_usb_common_reg_io.h"
#include <string.h>
#include <stdio.h>
#include "../../CMSIS/mss_assert.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef MSS_USB_HOST_ENABLED

/***************************************************************************//**
 Constant values internally used by USBH driver.
 */
#define MSS_USB_1ms_DIVISION_VAL                        1000u /*In miliSeconds*/
#define MSS_USB_TARGET_RESET_DELAY                      20u   /*In miliSeconds*/
#define MSS_USB_TARGET_VRISE_DELAY                      20u   /*In miliSeconds*/
#define MSS_USB_TARGET_SUSPEND_DELAY                    20u   /*In miliSeconds*/

/***************************************************************************//**
 Types internally used by USBH driver.
 */
typedef enum enum_state {
    ENUM_IDLE,
    ENUM_GET_DEF_DEV_DESC,
    ENUM_WAIT_GET_DEF_DEV_DESC,
    ENUM_RESET_TDEV,
    ENUM_CLR_RESET_TDEV,
    ENUM_SET_TDEV_ADDR,
    ENUM_WAIT_SET_TDEV_ADDR,
    ENUM_GET_FUL_DEV_DESC,
    ENUM_WAIT_GET_FUL_DEV_DESC,
    ENUM_GET_DEF_CONFG_DESC,
    ENUM_WAIT_GET_DEF_CONFG_DESC,
    ENUM_GET_FUL_CONFG_DESC,
    ENUM_WAIT_GET_FUL_CONFG_DESC,
    ENUM_CHECK_TDEV_CLASS_SUPPORT,
    ENUM_SUCCESS,
    ENUM_ERROR,
    ENUM_PET_SET_CONFIG,
    ENUM_PET_WAIT_SET_CONFIG
} mss_usb_enum_state_t;

typedef enum host_state {
    HOST_IDLE,
    HOST_ROOT_TARGET_DETECTED,
    HOST_ROOT_TARGET_RESETING,
    HOST_ROOT_TARGET_ENUMERATING,
    HOST_ROOT_TARGET_ALLOCATE_CLASSD,
    HOST_ROOT_TARGET_SERVICING,
    HOST_ROOT_TARGET_ERROR
} mss_usbh_state_t;

typedef enum {
    TDEV_R,
    TDEV_RHP1,
    TDEV_RHP2
} tdev_id_t;

/***************************************************************************//**
 Data structures internally used by USBH driver.
 */
typedef struct {
    uint8_t alloc_state;
    mss_usbh_class_cb_t* class_handle;
} class_driver_info_t;

typedef struct {
    union
    {
        uint8_t da[20];
        dev_desc_t ds;
    }dev_desc;
    union
    {
         uint8_t ca[20];        /*Def_confg + First Interface Desc+AddressAlign*/
         def_conf_desc_t cs;
    }conf_desc;
    int8_t desc_err_code;
} tdev_desc_t;

/******************************************************************************
 Global veriable declarations for this file (USBH Driver).
 */

/*Flags to indicate the Callback Events*/
uint8_t volatile gh_tdev_connect_event = 0u;
uint8_t volatile gh_tdev_discon_event = 0u;
uint8_t volatile gh_cep_cb_event = 0u;

/*Track the physically connected number of target devices*/
tdev_id_t tdev_idx = TDEV_R;

/*
 Information about the connected target devices
 Index of this array is tdev_idx i.e. physical location on SF2 receptacle.
*/
static mss_usbh_tdev_info_t g_tdev[1];
int8_t g_tdev_error_code = 0;

/*
 Holds the Target device descriptor information.
 MPH:Number of connected target Devices.
*/
static tdev_desc_t g_tdev_desc[1];

/*General purpose milliseconds count*/
volatile uint32_t ms = 0;

/*Track current state of the Host*/
static mss_usbh_state_t g_host_state = HOST_IDLE;

/*Track current state of ENUMERATION FSM*/
static mss_usb_enum_state_t g_enum_state = ENUM_IDLE;

mss_usb_ep_t gh_tx_ep[5];                               /*[0] ==> Control EP*/
mss_usb_ep_t gh_rx_ep[5];                               /*[0] ==> Empty*/

/*Use this at time of class allocation after device is enumerated*/
/*MPH:Number of registered classes*/
static class_driver_info_t g_rcd[1];

static volatile uint8_t g_cep_xfr_result = MSS_USB_EP_XFR_SUCCESS;

/*user application call-back handler*/
static mss_usbh_user_cb_t* g_user_cb;

static volatile uint8_t g_internal_cep_xfr = 0u;

/******************************************************************************
 Private function declarations for this file (USBH driver).
 */
static void MSS_USBH_enum_fsm(void);
static void MSS_USBH_control_xfr_fsm(void);


/*Call-back functions used by HCIF layer to communicate with this layer*/
static void MSS_USB_host_ep_tx_complete_cb(uint8_t ep_num, uint8_t status);
static void MSS_USB_host_ep_rx_cb(uint8_t ep_num, uint8_t status);
static void MSS_USB_host_cep_cb(uint8_t status);
static void MSS_USB_host_sof_cb(uint32_t frame_number);
static void MSS_USB_host_connect_cb(mss_usb_device_speed_t target_speed,
                             mss_usb_vbus_level_t vbus_level);
static void MSS_USB_host_disconnect_cb(void);
static void MSS_USB_host_vbus_err_cb(mss_usb_vbus_level_t vbus_level);
static void MSS_USB_host_babble_err_cb(void);
static void MSS_USB_host_session_request_cb(void);

static int8_t host_enum_check_class_support(tdev_id_t tid);
static int8_t validate_dev_desc(dev_desc_t* p_desc);
static int8_t validate_def_conf_desc(def_conf_desc_t* p_desc);
static void MSS_USBH_fsm(void);
static void MSS_USB_host_reset_enum_fsm(void);
static void MSS_USB_host_start_enum_fsm(void);
static mss_usb_enum_state_t MSS_USB_host_get_enum_fsm_state(void);
static void MSS_USB_handle_discon_event(void);

/*Host call-back functions*/
mss_usb_host_cb_t g_mss_usb_host_cb = {
    MSS_USB_host_ep_tx_complete_cb,
    MSS_USB_host_ep_rx_cb,
    MSS_USB_host_cep_cb,
    MSS_USB_host_sof_cb,
    MSS_USB_host_connect_cb,
    MSS_USB_host_disconnect_cb,
    MSS_USB_host_vbus_err_cb,
    MSS_USB_host_babble_err_cb,
    MSS_USB_host_session_request_cb
};

/*******************************************************************************
 * EXPORTED API Functions
 *******************************************************************************/

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
void
MSS_USBH_init
(
     mss_usbh_user_cb_t* app_cb
)
{

    /*
    By Default Prepare CEP for HS target.
    Actual speed will be found in Enumeration FSM
    */
    g_tdev[TDEV_R].addr = USB_DEFAULT_TARGET_ADDR;
    g_tdev[TDEV_R].tdev_maxpktsz0 = 64u;
    g_tdev[TDEV_R].speed = MSS_USB_DEVICE_HS;
    g_tdev[TDEV_R].state = MSS_USB_NOT_ATTACHED_STATE;
    g_tdev[TDEV_R].hub_addr = 0u;
    g_tdev[TDEV_R].hub_port = 0u;
    g_tdev[TDEV_R].hub_mtt = 0u;
    g_tdev[TDEV_R].tdev_maxpktsz0 = CEP_MAX_PKT_SIZE;
    g_tdev[TDEV_R].class_handle = 0u;
    g_user_cb = app_cb;
    /*Initialize host core interface layer*/
    MSS_USB_HCIF_init();
    MSS_USBH_configure_control_pipe(TDEV_R);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
int8_t
MSS_USBH_register_class_driver
(
    void* class_handle
)
{
    g_rcd[0].alloc_state = 0;
    g_rcd[0].class_handle = (mss_usbh_class_cb_t*)class_handle;
    return(0);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
int8_t
MSS_USBH_configure_control_pipe
(
    uint8_t target_addr
)
{
    tdev_id_t tid = (tdev_id_t)0u;
    /*
     Find the physical location of Target Device from Assigned address.
     This will be used to call the correct call-back function assigned to this
     address.
    */
    tid = (tdev_id_t)(target_addr & 0x03u);
    gh_tx_ep[MSS_USB_CEP].tdev_idx = tid;

    gh_tx_ep[MSS_USB_CEP].num = MSS_USB_CEP;
    gh_tx_ep[MSS_USB_CEP].fifo_size = 64u;
    gh_tx_ep[MSS_USB_CEP].max_pkt_size = g_tdev[tid].tdev_maxpktsz0;
    gh_tx_ep[MSS_USB_CEP].num_usb_pkt = 1u;
    gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;

    /*Control Endpoint Config is fixed. NakLimit = MaxVal.*/
    gh_tx_ep[MSS_USB_CEP].interval = 32768u;

    MSS_USB_HCIF_cep_configure(&gh_tx_ep[MSS_USB_CEP]);

    /*Type0:Default target speed*/
    MSS_USB_HCIF_cep_set_type0_reg(g_tdev[tid].speed);
    MSS_USB_HCIF_tx_ep_set_target_func_addr(MSS_USB_CEP,target_addr);

    return(0);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
int8_t
MSS_USBH_configure_out_pipe
(
    uint8_t target_addr,
    uint8_t outpipe_num,
    uint8_t target_ep,
    uint16_t fifo_addr,
    uint16_t fifo_size,
    uint16_t max_pkt_size,
    uint8_t num_usb_pkt,
    uint8_t dma_enable,
    mss_usb_dma_channel_t dma_channel,
    mss_usb_xfr_type_t xfr_type,
    uint32_t add_zlp,
    uint32_t interval
)
{
    /*TODO: Error check for all the parameters.*/
    tdev_id_t tid = (tdev_id_t)0u;

    tid = (tdev_id_t)(target_addr & 0x03u);
    gh_tx_ep[outpipe_num].tdev_idx = tid;

    gh_tx_ep[outpipe_num].num = (mss_usb_ep_num_t)outpipe_num;
    gh_tx_ep[outpipe_num].dpb_enable = DPB_DISABLE;
    gh_tx_ep[outpipe_num].fifo_size = fifo_size;
    gh_tx_ep[outpipe_num].fifo_addr =fifo_addr;
    gh_tx_ep[outpipe_num].dma_enable =dma_enable;
    gh_tx_ep[outpipe_num].dma_channel =dma_channel;
    gh_tx_ep[outpipe_num].max_pkt_size = max_pkt_size;
    gh_tx_ep[outpipe_num].stall = 0u;
    gh_tx_ep[outpipe_num].state = MSS_USB_EP_VALID;
    gh_tx_ep[outpipe_num].xfr_type = xfr_type;
    gh_tx_ep[outpipe_num].add_zlp = add_zlp;
    gh_tx_ep[outpipe_num].num_usb_pkt = 1u;             /*Update with parameter*/
    gh_tx_ep[outpipe_num].buf_addr = 0u;                /*update in Pipe_write*/

    gh_tx_ep[outpipe_num].xfr_length = 0u;
    gh_tx_ep[outpipe_num].xfr_count = 0u;
    gh_tx_ep[outpipe_num].txn_length = 0u;
    gh_tx_ep[outpipe_num].txn_count = 0u;

    gh_tx_ep[outpipe_num].disable_ping = 1u;
    gh_tx_ep[outpipe_num].req_pkt_n = 0u;

    gh_tx_ep[outpipe_num].interval = interval;

    /*Configure MP Registers with Target Device informations*/
    MSS_USB_HCIF_tx_ep_mp_configure(outpipe_num,
                                    target_ep,
                                    target_addr,
                                    g_tdev[tid].hub_addr,
                                    g_tdev[tid].hub_port,
                                    g_tdev[tid].hub_mtt,
                                    g_tdev[tid].speed,
                                    interval,
                                    xfr_type
                                    );

    MSS_USB_HCIF_tx_ep_configure(&gh_tx_ep[outpipe_num]);

    return(0);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
int8_t
MSS_USBH_configure_in_pipe
(
    uint8_t target_addr,
    uint8_t inpipe_num,
    uint8_t target_ep,
    uint16_t fifo_addr,
    uint16_t fifo_size,
    uint16_t max_pkt_size,
    uint8_t num_usb_pkt,
    uint8_t dma_enable,
    mss_usb_dma_channel_t dma_channel,
    mss_usb_xfr_type_t xfr_type,
    uint32_t add_zlp,
    uint32_t interval
)
{
    /*TODO: Error check for all the parameters.*/
    tdev_id_t tid = (tdev_id_t)0u;

    tid = (tdev_id_t)(target_addr & 0x03u);
    gh_rx_ep[inpipe_num].tdev_idx = tid;

    gh_rx_ep[inpipe_num].num = (mss_usb_ep_num_t)inpipe_num;
    gh_rx_ep[inpipe_num].dpb_enable = DPB_DISABLE;
    gh_rx_ep[inpipe_num].fifo_size = fifo_size;
    gh_rx_ep[inpipe_num].fifo_addr =fifo_addr;
    gh_rx_ep[inpipe_num].dma_enable =dma_enable;
    gh_rx_ep[inpipe_num].dma_channel =dma_channel;
    gh_rx_ep[inpipe_num].max_pkt_size = max_pkt_size;
    gh_rx_ep[inpipe_num].stall = 0u;
    gh_rx_ep[inpipe_num].state = MSS_USB_EP_VALID;
    gh_rx_ep[inpipe_num].xfr_type = xfr_type;
    gh_rx_ep[inpipe_num].add_zlp = add_zlp;
    gh_rx_ep[inpipe_num].num_usb_pkt = 1u;             /*Update with parameter*/
    gh_rx_ep[inpipe_num].buf_addr = 0u;                /*update in Pipe_write*/

    gh_rx_ep[inpipe_num].xfr_length = 0u;
    gh_rx_ep[inpipe_num].xfr_count = 0u;
    gh_rx_ep[inpipe_num].txn_length = 0u;
    gh_rx_ep[inpipe_num].txn_count = 0u;

    gh_rx_ep[inpipe_num].disable_ping = 1u;
    gh_rx_ep[inpipe_num].req_pkt_n = 0u;

    gh_rx_ep[inpipe_num].interval = interval;

    /*Configure MP Registers with Target Device informations*/
    MSS_USB_HCIF_rx_ep_mp_configure(inpipe_num,
                                    target_ep,
                                    target_addr,
                                    g_tdev[tid].hub_addr,
                                    g_tdev[tid].hub_port,
                                    g_tdev[tid].hub_mtt,
                                    g_tdev[tid].speed,
                                    interval,
                                    xfr_type);

    MSS_USB_HCIF_rx_ep_configure(&gh_rx_ep[inpipe_num]);
    return(0);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
int8_t
MSS_USBH_write_out_pipe
(
    uint8_t target_addr,
    uint8_t outpipe_num,
    uint8_t tdev_ep_num,
    uint16_t maxpktsz,
    uint8_t* buf,
    uint32_t length
)
{
    /*TODO: Error check for all the parameters.*/
    tdev_id_t tid = (tdev_id_t)0u;

    ASSERT(target_addr);
    ASSERT(outpipe_num);
    ASSERT(tdev_ep_num);
    ASSERT(maxpktsz);
    ASSERT(buf);

    if((target_addr != 0) && (outpipe_num != 0) && (tdev_ep_num != 0) &&
       (maxpktsz != 0) && (buf != 0) )
    {
        tid = (tdev_id_t)(target_addr & 0x03u);
        gh_tx_ep[outpipe_num].tdev_idx = tid;

        gh_tx_ep[outpipe_num].xfr_length = length;

        if(length > maxpktsz)
            gh_tx_ep[outpipe_num].txn_length = maxpktsz;
        else
            gh_tx_ep[outpipe_num].txn_length = length;

        gh_tx_ep[outpipe_num].xfr_count = 0;
        gh_tx_ep[outpipe_num].txn_count = 0;

        gh_tx_ep[outpipe_num].buf_addr = buf;
        MSS_USB_HCIF_tx_ep_write_pkt(&gh_tx_ep[outpipe_num]);
        return(0);
    }
    else
        return(1);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
int8_t
MSS_USBH_read_in_pipe
(
    uint8_t target_addr,
    uint8_t inpipe_num,
    uint8_t tdev_ep_num,
    uint16_t tdev_ep_maxpktsz,
    uint8_t* buf,
    uint32_t length
)
{
    /*TODO: Error check for all the parameters.*/
    tdev_id_t tid = (tdev_id_t)0u;

    ASSERT(target_addr);
    ASSERT(inpipe_num);
    ASSERT(tdev_ep_num);
    ASSERT(tdev_ep_maxpktsz);
    ASSERT(buf);

    if((target_addr != 0) && (inpipe_num != 0) && (tdev_ep_num != 0) &&
       (tdev_ep_maxpktsz != 0) && (buf != 0) )
    {
        tid = (tdev_id_t)(target_addr & 0x03u);
        gh_rx_ep[inpipe_num].tdev_idx = tid;

        gh_rx_ep[inpipe_num].xfr_length = length;

        if(length > tdev_ep_maxpktsz)
            gh_rx_ep[inpipe_num].txn_length = tdev_ep_maxpktsz;
        else
            gh_rx_ep[inpipe_num].txn_length = length;

        gh_rx_ep[inpipe_num].xfr_count = 0;
        gh_rx_ep[inpipe_num].txn_count = 0;

        gh_rx_ep[inpipe_num].buf_addr = buf;

        MSS_USB_HCIF_rx_ep_read_prepare(&gh_rx_ep[inpipe_num]);
        MSS_USB_HCIF_rx_ep_set_reqpkt((mss_usb_ep_num_t)inpipe_num);
        return(0);
    }
    else
        return(1);

}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
mss_usb_state_t
MSS_USBH_get_tdev_state
(
    uint8_t target_addr
)
{
    tdev_id_t tid = (tdev_id_t)0;

    if(target_addr)
    {
        tid = (tdev_id_t)(target_addr & 0x03u);
    }

    ASSERT(g_tdev[tid].addr == target_addr);
    return(g_tdev[tid].state);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
void
MSS_USBH_task
(
    void
)
{
    MSS_USBH_control_xfr_fsm();
    MSS_USBH_enum_fsm();
    MSS_USBH_fsm();
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
uint8_t
MSS_USBH_start_control_xfr
(
    uint8_t* cbuf_addr,
    uint8_t* dbuf_addr,
    uint8_t  data_dir,
    uint32_t data_len
    /*Add tdev_addr, speed and tdevmaxpktsz as the parameter*/
)
{

    ASSERT(cbuf_addr != (uint8_t*)0);
    ASSERT(dbuf_addr != (uint8_t*)0);
    ASSERT(!(((uint32_t)cbuf_addr) & 0x00000002));
    ASSERT(!(((uint32_t)dbuf_addr) & 0x00000002));
    ASSERT((data_dir == USB_STD_REQ_DATA_DIR_IN) ||
           (data_dir == USB_STD_REQ_DATA_DIR_OUT));

    //    ASSERT(gh_tx_ep[MSS_USB_CEP].state == MSS_USB_CEP_IDLE);

    gh_tx_ep[MSS_USB_CEP].buf_addr = dbuf_addr;
    gh_tx_ep[MSS_USB_CEP].cep_cmd_addr = cbuf_addr;

    gh_tx_ep[MSS_USB_CEP].xfr_length = data_len;

    /*Set the logical MaxPktSize of Host CEP to the MazPktsz of the associated Target CEP*/
    /*TODO: Copy from Parameter*/
//    gh_tx_ep[MSS_USB_CEP].max_pkt_size = gh_tx_ep[MSS_USB_CEP].tdev_addr

    gh_tx_ep[MSS_USB_CEP].cep_data_dir = data_dir;
    gh_tx_ep[MSS_USB_CEP].xfr_count = 0u;
    gh_tx_ep[MSS_USB_CEP].txn_count = 0u;
    gh_tx_ep[MSS_USB_CEP].xfr_type = MSS_USB_XFR_CONTROL;

    g_cep_xfr_result = 0u;

    MSS_USB_HCIF_load_tx_fifo(MSS_USB_CEP,
                              cbuf_addr,
                              USB_SETUP_PKT_LEN);

    gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_SETUP;
    MSS_USB_HCIF_cep_set_setuppktrdy();

    return(0);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
void
MSS_USBH_construct_get_descr_command
(
    uint8_t* buf,
    uint8_t xfr_dir,
    uint8_t req_type,
    uint8_t recip_type,
    uint8_t request,
    uint8_t desc_type,
    uint8_t strng_idx,
    uint16_t length
)
{
    buf[0] = (xfr_dir| req_type| recip_type);                   /*bmRequestType*/
    buf[1] = request;                                           /*bRequest*/

    buf[3] = desc_type;                                         /*wValue-MSB = desc_type*/

    if((request == USB_STD_REQ_GET_DESCRIPTOR) &&
       (desc_type == USB_STRING_DESCRIPTOR_TYPE))
    {
        buf[2] = strng_idx;                                     /*wValue-LSB = string_idx*/
        buf[4] = (uint8_t)0x09;                                 /*wIndex-LSB = LangID for string Desc*/
        buf[5] = (uint8_t)0x04;                                 /*wIndex-MSB*/
    }
    else
    {
        /*
         wValue-LSB = Conf_idx, Field should be used only for Conf_desc or
         String Desc.
         Since we support only one configuration, we set it to zero for Conf_desc.
         For all other descriptors this field must be zero
         */
        buf[2] = 0x00;

        /*wIndex-LSB and MSB => other than Strng Desc, these values must be zero*/
        buf[4] = 0x00;
        buf[5] = 0x00;
    }

    /*wLength-LSB and MSB, Length of data to be received*/
    buf[6] = (uint8_t)length;
    buf[7] = (uint8_t)(length >> 8u);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
mss_usb_ep_state_t
MSS_USBH_get_cep_state
(
    void
)
{
    return(gh_tx_ep[MSS_USB_CEP].state);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
void
MSS_USBH_1ms_tick
(
    void
)
{
    ms++;
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
uint32_t
MSS_USBH_get_milis
(
    void
)
{
    return(ms);
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
int8_t
MSS_USBH_get_std_dev_descr
(
    uint8_t* buffer
)
{
    uint8_t command_buf[USB_SETUP_PKT_LEN]={0};

    g_internal_cep_xfr = 1u;
    MSS_USBH_configure_control_pipe(0x10);
    memset(command_buf, 0u, USB_SETUP_PKT_LEN *(sizeof(uint8_t)));
    MSS_USBH_construct_get_descr_command(command_buf,
                                         USB_STD_REQ_DATA_DIR_IN,
                                         USB_STANDARD_REQUEST,
                                         USB_STD_REQ_RECIPIENT_DEVICE,
                                         USB_STD_REQ_GET_DESCRIPTOR,
                                         USB_DEVICE_DESCRIPTOR_TYPE,
                                         0u,
                                         8u);

    MSS_USBH_start_control_xfr(command_buf,
                               buffer,
                               USB_STD_REQ_DATA_DIR_IN,
                               8u);

    while((0 == g_cep_xfr_result) && (g_internal_cep_xfr == 1));

    if(MSS_USB_EP_XFR_SUCCESS == g_cep_xfr_result)
    {
        return 0;
    }
    else
    {
        return -1;
    }
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
void
MSS_USBH_test_mode
(
    uint8_t test_case
)
{
    switch(test_case)
    {
        case USB_TEST_MODE_SELECTOR_TEST_PACKET:
            MSS_USB_CIF_start_testpacket();
        break;

        case USB_TEST_MODE_SELECTOR_TEST_J:
            MSS_USB_CIF_start_testj();
        break;

        case USB_TEST_MODE_SELECTOR_TEST_K:
            MSS_USB_CIF_start_testk();
        break;

        case USB_TEST_MODE_SELECTOR_TEST_SE0NAK:
            MSS_USB_CIF_start_testse0nak();
        break;

        case USB_TEST_MODE_SELECTOR_TEST_FORCE_ENA:
            MSS_USB_CIF_start_forcehost_ena();
        break;
        default:
        break;
    }
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
void
MSS_USBH_suspend
(
    void
)
{
    MSS_USB_HCIF_bus_suspend(0u);   //suspendM enabled. RemoteWakeup not supported.
}

/******************************************************************************
 See mss_usb_host.h for details of how to use this function.
 */
void
MSS_USBH_resume
(
    void
)
{
    volatile static uint32_t resume_milis = 0u;

    resume_milis = MSS_USBH_get_milis();

    MSS_USB_HCIF_clr_suspend_bus();

    MSS_USB_HCIF_assert_bus_resume();

    while((MSS_USBH_get_milis() - resume_milis) <= 40u);/*This delay should be 20ms*/

    MSS_USB_HCIF_clr_bus_resume();
}

/*******************************************************************************
 * Internal Functions
 *******************************************************************************/

/*
 This function is the main FSM of the USB Host. It handles connect/disconnect
 events, initiate enumeration FSM when device is detected and handles allocation
 of class driver to the device.
*/
static void
MSS_USBH_fsm
(
    void
)
{
    static uint32_t reset_milis = 0u;
    mss_usb_enum_state_t enum_st;

    if(1u == gh_tdev_discon_event)
    {
        MSS_USB_handle_discon_event();
        gh_tdev_discon_event = 0u;
        g_host_state = HOST_IDLE;
    }
    switch(g_host_state)
    {
        case HOST_IDLE:
            /*
             Repeatedly enable session bit so that MUSB sample Idig pin to
             detect device connection
             */
//            MSS_USB_CIF_start_session();
            if(gh_tdev_connect_event)
            {
                gh_tdev_connect_event = 0u;
                g_tdev[tdev_idx].state = MSS_USB_ATTACHED_STATE;
                reset_milis = MSS_USBH_get_milis();
                g_host_state = HOST_ROOT_TARGET_DETECTED;
                /*USB2.0:7.1.7.3, TATTDB is applicable to Hubbed device*/
            }
        break;

        case HOST_ROOT_TARGET_DETECTED:
            if((MSS_USBH_get_milis() - reset_milis) >= 100)
            {
                MSS_USB_CIF_enable_usbirq(VBUS_ERROR_IRQ_MASK);
                g_tdev[TDEV_R].state = MSS_USB_POWERED_STATE;
                reset_milis = MSS_USBH_get_milis();
                MSS_USB_HCIF_assert_bus_reset();           /*Reset Root device*/
                g_host_state = HOST_ROOT_TARGET_RESETING;
            }
        break;

        case HOST_ROOT_TARGET_RESETING:
            if((MSS_USBH_get_milis() - reset_milis) == 20)
            {
                MSS_USB_HCIF_clr_bus_reset();
            }
            else if((MSS_USBH_get_milis() - reset_milis) >= 40)
            {
                /*USB 2.0 spec, 7.1.7.3, Reset Recovery*/
                /*If target was operating at FS, check if can work with HS as well*/
                if((g_tdev[TDEV_R].speed == MSS_USB_DEVICE_FS)&&
                   (MSS_USB_HCIF_is_hs_mode()))
                {
                    g_tdev[TDEV_R].speed = MSS_USB_DEVICE_HS;
                }
                if(0 != g_user_cb->usbh_tdev_attached)
                    g_user_cb->usbh_tdev_attached(g_tdev[tdev_idx].speed);

                /*Configure CEP for the detected speed*/
                MSS_USBH_configure_control_pipe(TDEV_R);

                g_tdev[TDEV_R].state = MSS_USB_DEFAULT_STATE;
                g_host_state = HOST_ROOT_TARGET_ENUMERATING;
            }
        break;

        case HOST_ROOT_TARGET_ENUMERATING:
            enum_st = MSS_USB_host_get_enum_fsm_state();
            if(ENUM_IDLE == enum_st)
            {
                MSS_USB_host_start_enum_fsm();
            }
            else if(ENUM_SUCCESS == enum_st)
            {
                g_tdev[TDEV_R].state = MSS_USB_ADDRESS_STATE;

                MSS_USB_host_reset_enum_fsm();
                g_host_state = HOST_ROOT_TARGET_ALLOCATE_CLASSD;
            }
            else if(ENUM_ERROR == enum_st)
            {
                MSS_USB_host_reset_enum_fsm();
                g_tdev[TDEV_R].state = MSS_USB_ATTACHED_STATE;
                g_host_state = HOST_ROOT_TARGET_ERROR;
            }
            else
            {
                /*EnumFSM in progress, Do nothing*/
            }
        break;

        case HOST_ROOT_TARGET_ALLOCATE_CLASSD:
            g_rcd[TDEV_R].class_handle->usbh_class_allocate(g_tdev[TDEV_R].addr);
            g_host_state = HOST_ROOT_TARGET_SERVICING;

            if(0 != g_user_cb->usbh_tdev_class_driver_assigned)
                g_user_cb->usbh_tdev_class_driver_assigned();

        break;

        case HOST_ROOT_TARGET_SERVICING:
        /*Class driver FSM is active now*/
        break;

        case HOST_ROOT_TARGET_ERROR:
            if(0 != g_user_cb->usbh_tdev_not_supported)
                g_user_cb->usbh_tdev_not_supported(g_tdev_error_code);
            g_host_state = HOST_IDLE;
            /*
            Clear Enum-FSM
            Clear Target_info for all targets.
            Clear EP0 config
            Give Visual Err Indication and remain in this state,
            till Disconnect Interrupt occurs or USER wants to give a retry command.
            */
        break;
        default:
        break;
    }
}

/*
 This function handles the enumeration process which includes, device reset,
 speed detection, and standard USB request for retrieving Device and Config
 Descriptor from the device.
*/
static void
MSS_USBH_enum_fsm
(
    void
)
{
    uint8_t command_buf[USB_SETUP_PKT_LEN] = {0};
    static uint32_t reset_milis = 0u;
    int8_t cd_idx = 0x04;
    uint8_t dummy[8];
    /*
     We do not do any assert checks here.
     Before Calling this FSM, Calling function should make sure that This FSM
     can be executed
    */
    switch(g_enum_state)
    {
        case ENUM_IDLE:
            /*Free running Enum FSM hits here when doing nothing*/
        break;

        case ENUM_GET_DEF_DEV_DESC:
            MSS_USBH_construct_get_descr_command(command_buf,
                                                 USB_STD_REQ_DATA_DIR_IN,
                                                 USB_STANDARD_REQUEST,
                                                 USB_STD_REQ_RECIPIENT_DEVICE,
                                                 USB_STD_REQ_GET_DESCRIPTOR,
                                                 USB_DEVICE_DESCRIPTOR_TYPE,
                                                 0,
                                                 USB_SETUP_PKT_LEN);

            g_enum_state = ENUM_WAIT_GET_DEF_DEV_DESC;

            MSS_USBH_start_control_xfr(command_buf,
                                      (uint8_t*)&g_tdev_desc[tdev_idx].dev_desc,
                                      USB_STD_REQ_DATA_DIR_IN,
                                      USB_SETUP_PKT_LEN);
        break;

        case ENUM_WAIT_GET_DEF_DEV_DESC:
            if(g_cep_xfr_result)
            {
                if(MSS_USB_EP_XFR_SUCCESS == g_cep_xfr_result)
                {
                    g_tdev[TDEV_R].tdev_maxpktsz0 =
                                g_tdev_desc[TDEV_R].dev_desc.ds.bMaxPacketSize0;

                    g_enum_state = ENUM_RESET_TDEV;
                }
                else
                {
                    g_enum_state = ENUM_ERROR;
                }
            }
        break;

        case ENUM_RESET_TDEV:
            reset_milis = MSS_USBH_get_milis();
            MSS_USB_HCIF_assert_bus_reset();
            g_enum_state = ENUM_CLR_RESET_TDEV;
        break;

        case ENUM_CLR_RESET_TDEV:
            if((MSS_USBH_get_milis() - reset_milis) == 20)
            {
                MSS_USB_HCIF_clr_bus_reset();
            }
            else if((MSS_USBH_get_milis() - reset_milis) >= 40)
            {
                /*Wait for 20ms to let the Target settle down*/
                g_enum_state = ENUM_SET_TDEV_ADDR;
            }
        break;

        case ENUM_SET_TDEV_ADDR:
            /*Configure CEP Since Maxpkt0 might have changed.*/
            MSS_USBH_configure_control_pipe(TDEV_R);

            memset(command_buf, 0, USB_SETUP_PKT_LEN*(sizeof(uint8_t)));
            command_buf[1] = USB_STD_REQ_SET_ADDRESS;
            command_buf[2] = 0x10;                         /*New non-zero ADDR*/
            reset_milis = MSS_USBH_get_milis();
            g_enum_state = ENUM_WAIT_SET_TDEV_ADDR;
            MSS_USBH_start_control_xfr(command_buf,
                                      command_buf,     /*Dummy Buf for zld req*/
                                      USB_STD_REQ_DATA_DIR_OUT,
                                      0u);
        break;
        case ENUM_WAIT_SET_TDEV_ADDR:
            if(g_cep_xfr_result)
            {
                if(MSS_USB_EP_XFR_SUCCESS == g_cep_xfr_result)
                {
                    int8_t res;

                    /*USB2.0-9.2.6.3: Add SetAddrDelay 2ms*/
                    if((MSS_USBH_get_milis() - reset_milis) >= 5)
                    {
                        g_tdev[TDEV_R].addr = 0x10;         /*New non-zero ADDR*/
                        MSS_USB_HCIF_tx_ep_set_target_func_addr(gh_tx_ep[MSS_USB_CEP].num,
                                                                g_tdev[TDEV_R].addr);
                        /*Validate DevDescriptor
                        Take exception for FS LS devices*/
                        res = validate_dev_desc((dev_desc_t*)&g_tdev_desc[tdev_idx].dev_desc);
                        g_tdev_desc[tdev_idx].desc_err_code = res;
                        if(0 == res)
                        {
                            g_enum_state = ENUM_GET_FUL_DEV_DESC;
                        }
                        else
                        {
                            g_enum_state = ENUM_ERROR;
                            g_tdev_error_code = res;
                        }
                    }
                }
                else
                {
                    g_tdev[TDEV_R].addr = 0x00u;
                    g_enum_state = ENUM_ERROR;
                }
            }
        break;
        case ENUM_GET_FUL_DEV_DESC:
            MSS_USBH_construct_get_descr_command(command_buf,
                                                 USB_STD_REQ_DATA_DIR_IN,
                                                 USB_STANDARD_REQUEST,
                                                 USB_STD_REQ_RECIPIENT_DEVICE,
                                                 USB_STD_REQ_GET_DESCRIPTOR,
                                                 USB_DEVICE_DESCRIPTOR_TYPE,
                                                 0u,
                                                 18u);

            g_enum_state = ENUM_WAIT_GET_FUL_DEV_DESC;

            MSS_USBH_start_control_xfr(command_buf,
                                       (uint8_t*)&g_tdev_desc[tdev_idx].dev_desc,
                                       USB_STD_REQ_DATA_DIR_IN,
                                       18u);
        break;

        case ENUM_WAIT_GET_FUL_DEV_DESC:
            if(g_cep_xfr_result)
            {
                if(MSS_USB_EP_XFR_SUCCESS == g_cep_xfr_result)
                {
                    /*Support for PET device with VID = 0x1A0A PID = PID=0x0200*/
                    if((g_tdev_desc[tdev_idx].dev_desc.ds.idVendor == 0x1A0A) &&
                       (g_tdev_desc[tdev_idx].dev_desc.ds.idProduct == 0x0200))
                    {
                        g_enum_state = ENUM_PET_SET_CONFIG;
                    }
                    else
                    {
                        int8_t res;

                        /*Validate DevDescriptor*/
                        res = validate_dev_desc((dev_desc_t*)&g_tdev_desc[tdev_idx].dev_desc);
                        g_tdev_desc[tdev_idx].desc_err_code = res;
                        if(0 == res)
                        {
                            g_enum_state = ENUM_GET_DEF_CONFG_DESC;
                        }
                        else
                        {
                            g_enum_state = ENUM_ERROR;
                            g_tdev_error_code = res;
                        }
                    }
                }
                else
                {
                    g_enum_state = ENUM_ERROR;
                }
            }
        break;

        case ENUM_GET_DEF_CONFG_DESC:
            memset(command_buf, 0u, USB_SETUP_PKT_LEN*(sizeof(uint8_t)));
            MSS_USBH_construct_get_descr_command(command_buf,
                                                 USB_STD_REQ_DATA_DIR_IN,
                                                 USB_STANDARD_REQUEST,
                                                 USB_STD_REQ_RECIPIENT_DEVICE,
                                                 USB_STD_REQ_GET_DESCRIPTOR,
                                                 USB_CONFIGURATION_DESCRIPTOR_TYPE,
                                                 0, /*stringID*/
                                                 9u);

            g_enum_state = ENUM_WAIT_GET_DEF_CONFG_DESC;

            MSS_USBH_start_control_xfr(command_buf,
                                      (uint8_t*)&g_tdev_desc[tdev_idx].conf_desc,
                                      USB_STD_REQ_DATA_DIR_IN,
                                      9u);
        break;

        case ENUM_WAIT_GET_DEF_CONFG_DESC:
            if(g_cep_xfr_result)
            {
                if(MSS_USB_EP_XFR_SUCCESS == g_cep_xfr_result)
                {
                    int8_t res;
                    res = validate_def_conf_desc((def_conf_desc_t*)&g_tdev_desc[tdev_idx].conf_desc);
                    g_tdev_desc[tdev_idx].desc_err_code = res;
                    if(0 == res)
                        g_enum_state = ENUM_GET_FUL_CONFG_DESC;
                    else
                    {
                        g_enum_state = ENUM_ERROR;
                        g_tdev_error_code = res;
                    }
                }
                else
                {
                    g_enum_state = ENUM_ERROR;
                }
            }
        break;

        case ENUM_GET_FUL_CONFG_DESC:
            memset(command_buf, 0u, USB_SETUP_PKT_LEN*(sizeof(uint8_t)));
            MSS_USBH_construct_get_descr_command(command_buf,
                                                 USB_STD_REQ_DATA_DIR_IN,
                                                 USB_STANDARD_REQUEST,
                                                 USB_STD_REQ_RECIPIENT_DEVICE,
                                                 USB_STD_REQ_GET_DESCRIPTOR,
                                                 USB_CONFIGURATION_DESCRIPTOR_TYPE,
                                                 0, /*stringID*/
                                                 18u);

            g_enum_state = ENUM_WAIT_GET_FUL_CONFG_DESC;

            MSS_USBH_start_control_xfr(command_buf,
                                      (uint8_t*)&g_tdev_desc[tdev_idx].conf_desc,
                                      USB_STD_REQ_DATA_DIR_IN,
                                      18u);

        break;

        case ENUM_WAIT_GET_FUL_CONFG_DESC:
            if(g_cep_xfr_result)
            {
                if(MSS_USB_EP_XFR_SUCCESS == g_cep_xfr_result)
                {
                    g_enum_state = ENUM_CHECK_TDEV_CLASS_SUPPORT;
                }
                else
                {
                    g_enum_state = ENUM_ERROR;
                }
            }
        break;

        case ENUM_CHECK_TDEV_CLASS_SUPPORT:
            cd_idx =  host_enum_check_class_support(TDEV_R);
            if(cd_idx < (int8_t)0)
            {
               g_enum_state = ENUM_ERROR;
               g_tdev_error_code = cd_idx;
            }
            else if(cd_idx < 3)                   /*Max No of classDriver is 3*/
            {
                g_rcd[cd_idx].alloc_state = 1u;
                g_tdev[TDEV_R].class_handle = g_rcd[cd_idx].class_handle;
                g_enum_state = ENUM_SUCCESS;
            }
        break;

        case ENUM_PET_SET_CONFIG:
            memset(command_buf, 0u, USB_SETUP_PKT_LEN*(sizeof(uint8_t)));
            command_buf[1] = USB_STD_REQ_SET_CONFIG;
            command_buf[2] = 0x01;                   /*ConfigNum for PET Device*/
            g_enum_state = ENUM_PET_WAIT_SET_CONFIG;

            MSS_USBH_start_control_xfr(command_buf,
                                       (uint8_t*)dummy,
                                       USB_STD_REQ_DATA_DIR_IN,
                                       0u);
        break;

        case ENUM_PET_WAIT_SET_CONFIG:
            if(g_cep_xfr_result)
            {
                if(MSS_USB_EP_XFR_SUCCESS == g_cep_xfr_result)
                {
                    g_enum_state = ENUM_CHECK_TDEV_CLASS_SUPPORT;
                }
                else
                {
                    g_enum_state = ENUM_ERROR;
                }
            }
        break;

        case ENUM_SUCCESS:
        break;

        case ENUM_ERROR:
        default:
            ASSERT(0);          /*Should never happen*/
        break;
    }
}

/*
 This function is the FSM for the control transfer. Once the control transfer
 is initiated, this function will monitor it's progress in all the stages and
 record the status at the end.
*/
static void
MSS_USBH_control_xfr_fsm
(
    void
)
{
    volatile mss_usb_ep_state_t cep_event_st = MSS_USB_EP_VALID;
    uint32_t next_addr = 0;
    uint32_t rem_length = 0;

    MSS_USB_CIF_cep_disable_irq();
    cep_event_st = (mss_usb_ep_state_t)gh_cep_cb_event;/*gh_cep_cb_event is critical*/
    gh_cep_cb_event = 0u;
    MSS_USB_CIF_cep_enable_irq();
    MSS_USB_CIF_set_index_reg(MSS_USB_CEP);

    switch(gh_tx_ep[MSS_USB_CEP].state)
    {
        case MSS_USB_CEP_IDLE:
            /*do nothing. Free running CEP-FSM hits here when doing no transfers*/
        break;
        case MSS_USB_CEP_SETUP:
            if(cep_event_st)
            {
                if(cep_event_st == MSS_USB_EP_TXN_SUCCESS)
                {
                    if(0u == gh_tx_ep[MSS_USB_CEP].xfr_length)
                    {
                        /*zdl-request is sent. Get the status now*/
                        gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_STATUS_AFTER_OUT;
                        MSS_USB_HCIF_cep_set_statuspktrdy_after_out();
                    }
                    else
                    {
                        rem_length = gh_tx_ep[MSS_USB_CEP].xfr_length -
                                        gh_tx_ep[MSS_USB_CEP].xfr_count;

                        if(rem_length > gh_tx_ep[MSS_USB_CEP].max_pkt_size)
                            gh_tx_ep[MSS_USB_CEP].txn_length =
                                        gh_tx_ep[MSS_USB_CEP].max_pkt_size;
                        else
                            gh_tx_ep[MSS_USB_CEP].txn_length = rem_length;

                        if(gh_tx_ep[MSS_USB_CEP].cep_data_dir == USB_STD_REQ_DATA_DIR_IN)
                        {
                            gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_DATAIN;
                            MSS_USB_HCIF_cep_set_request_in_pkt();  /*One Packet at a time*/
                        }
                        else if(gh_tx_ep[MSS_USB_CEP].cep_data_dir == USB_STD_REQ_DATA_DIR_OUT)
                        {
                            MSS_USB_HCIF_cep_write_pkt(&gh_tx_ep[MSS_USB_CEP]);
                            gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_DATAOUT;
                            MSS_USB_HCIF_cep_set_txpktrdy();
                        }
                         else
                        {
                            ASSERT(0);                  /*DataDir not valid*/
                        }
                    }
                }
                else
                {
                    gh_tx_ep[MSS_USB_CEP].state = cep_event_st;
                }
            }
        break;

        case MSS_USB_CEP_DATAIN:
            if(cep_event_st)
            {
                if(cep_event_st == MSS_USB_EP_TXN_SUCCESS)
                {
                    MSS_USB_HCIF_cep_read_pkt(&gh_tx_ep[MSS_USB_CEP]);
                    if(gh_tx_ep[MSS_USB_CEP].xfr_count == gh_tx_ep[MSS_USB_CEP].xfr_length)
                    {
                        gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_STATUS_AFTER_IN;
                        MSS_USB_HCIF_cep_set_statuspktrdy_after_in();
                    }
                    else
                    {
                        rem_length = gh_tx_ep[MSS_USB_CEP].xfr_length -
                                     gh_tx_ep[MSS_USB_CEP].xfr_count;

                        if(rem_length > gh_tx_ep[MSS_USB_CEP].max_pkt_size)
                        {
                            gh_tx_ep[MSS_USB_CEP].txn_length = gh_tx_ep[MSS_USB_CEP].max_pkt_size;
                        }
                        else
                        {
                            gh_tx_ep[MSS_USB_CEP].txn_length = rem_length;
                        }

                        next_addr = gh_tx_ep[MSS_USB_CEP].xfr_length;
                        next_addr += gh_tx_ep[MSS_USB_CEP].txn_length;
                        gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_DATAIN;
                        MSS_USB_HCIF_cep_set_request_in_pkt();  /*One Packet at a time*/
                    }
                }
                else
                {
                    gh_tx_ep[MSS_USB_CEP].state = cep_event_st;
                    /*Flush fifo, if RxpktRdy is set.MUSB:21.2.2*/
                }
            }
        break;

        case MSS_USB_CEP_DATAOUT:
            if(cep_event_st)
            {
                if(cep_event_st == MSS_USB_EP_TXN_SUCCESS)
                {
                    if(gh_tx_ep[MSS_USB_CEP].xfr_count ==
                       gh_tx_ep[MSS_USB_CEP].xfr_length)
                    {
                        gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_STATUS_AFTER_OUT;
                        MSS_USB_HCIF_cep_set_statuspktrdy_after_out();
                    }
                    else
                    {
                        rem_length = gh_tx_ep[MSS_USB_CEP].xfr_length -
                                     gh_tx_ep[MSS_USB_CEP].xfr_count;

                        if(rem_length > gh_tx_ep[MSS_USB_CEP].max_pkt_size)
                            gh_tx_ep[MSS_USB_CEP].txn_length =
                                        gh_tx_ep[MSS_USB_CEP].max_pkt_size;
                        else
                            gh_tx_ep[MSS_USB_CEP].txn_length = rem_length;

                        MSS_USB_HCIF_cep_write_pkt(&gh_tx_ep[MSS_USB_CEP]);
                    }
                }
                else
                {
                    gh_tx_ep[MSS_USB_CEP].state = cep_event_st;
                    ASSERT(0); /*Error response received in INTR*/
                }
            }
            /*TODO: Check if more data needs to Transmitted*/
            /* Initiate IN status phase. */
        break;

        case MSS_USB_CEP_STATUS_AFTER_IN:
            if(cep_event_st)
            {
                uint8_t this_tdev = gh_tx_ep[MSS_USB_CEP].tdev_idx;
                if(cep_event_st == MSS_USB_EP_TXN_SUCCESS)
                {
                    gh_tx_ep[MSS_USB_CEP].state = MSS_USB_EP_XFR_SUCCESS;

                    if((g_tdev[(gh_tx_ep[MSS_USB_CEP].tdev_idx)].class_handle == 0) ||
                       (g_internal_cep_xfr == 1))
                    {
                        g_cep_xfr_result = MSS_USB_EP_XFR_SUCCESS;
                        g_internal_cep_xfr = 0u;
                    }
                    else
                    {
                        if(g_tdev[this_tdev].class_handle->usbh_class_cep_xfr_done != 0)
                        {
                            g_tdev[this_tdev].class_handle->
                                usbh_class_cep_xfr_done(g_tdev[this_tdev].addr,
                                                        gh_tx_ep[MSS_USB_CEP].state,
                                                        gh_tx_ep[MSS_USB_CEP].xfr_count);
                        }
                        else
                            ASSERT(0);
                    }

                    gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
                }
                else
                {
                    gh_tx_ep[MSS_USB_CEP].state = cep_event_st;
                }
            }
        break;

        case MSS_USB_CEP_STATUS_AFTER_OUT:
            if(cep_event_st)
            {
                uint8_t this_tdev = gh_tx_ep[MSS_USB_CEP].tdev_idx;
                MSS_USB_HCIF_cep_clr_statusRxpktrdy();
                if(cep_event_st == MSS_USB_EP_TXN_SUCCESS)
                {
                    gh_tx_ep[MSS_USB_CEP].state = MSS_USB_EP_XFR_SUCCESS;

                    if((g_tdev[(gh_tx_ep[MSS_USB_CEP].tdev_idx)].class_handle == 0) ||
                       (g_internal_cep_xfr == 1))
                    {
                        g_cep_xfr_result = MSS_USB_EP_XFR_SUCCESS;
                    }
                    else
                    {
                        if(g_tdev[this_tdev].class_handle->usbh_class_cep_xfr_done != 0)
                        {
                            g_tdev[this_tdev].class_handle->usbh_class_cep_xfr_done(g_tdev[this_tdev].addr,
                                                                                    gh_tx_ep[MSS_USB_CEP].state,
                                                                                    gh_tx_ep[MSS_USB_CEP].xfr_count);
                        }
                        else
                            ASSERT(0);
                    }
                    gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
                }
                else
                {
                    gh_tx_ep[MSS_USB_CEP].state = cep_event_st;
                }
            }
        break;

        case MSS_USB_EP_NAK_TOUT:
        case MSS_USB_EP_NO_RESPONSE:
        case MSS_USB_EP_STALL_RCVD:
        {
            uint8_t this_tdev = gh_tx_ep[MSS_USB_CEP].tdev_idx;
            if((g_tdev[(gh_tx_ep[MSS_USB_CEP].tdev_idx)].class_handle == 0) || (g_internal_cep_xfr == 1))
            {
                g_cep_xfr_result = gh_tx_ep[MSS_USB_CEP].state;
                g_internal_cep_xfr = 0u;
            }
            else
            {
                if(g_tdev[this_tdev].class_handle->usbh_class_cep_xfr_done != 0)
                {
                    g_tdev[this_tdev].class_handle->
                        usbh_class_cep_xfr_done(g_tdev[this_tdev].addr,
                                                gh_tx_ep[MSS_USB_CEP].state,
                                                gh_tx_ep[MSS_USB_CEP].xfr_count);
                }
                else
                    ASSERT(0);

                gh_tx_ep[MSS_USB_CEP].state = MSS_USB_CEP_IDLE;
            }
        }
        break;

        default:
            ASSERT(0);
            break;
    }
}

/*
 This function is used to start the enumeration FSM
*/
static void
MSS_USB_host_start_enum_fsm
(
    void
)
{
    g_enum_state = ENUM_GET_DEF_DEV_DESC;
}

/*
 This function is used to bring the enum FSM to it's initial IDLE state.
*/
static void
MSS_USB_host_reset_enum_fsm
(
    void
)
{
    g_enum_state = ENUM_IDLE;
}

/*
 This function is used to find the current status of the enumeration FSM. The
 state information is used to know if the enumeration was successful or not.
*/
static mss_usb_enum_state_t
MSS_USB_host_get_enum_fsm_state
(
    void
)
{
    return(g_enum_state);
}

/*
 This Call-back function is executed on completion of current OUT transfer.
*/
static void
MSS_USB_host_ep_tx_complete_cb
(
    uint8_t ep_num,
    uint8_t status
)
{
    uint8_t this_tdev = 0u;
    uint32_t rem_length = 0u;
    this_tdev = gh_tx_ep[ep_num].tdev_idx;//retrieve tdev_idx which transmitted data.

    if(MSS_USB_EP_TXN_SUCCESS == status)
    {
        if(gh_tx_ep[ep_num].xfr_count == gh_tx_ep[ep_num].xfr_length)
        {
            gh_tx_ep[ep_num].state = MSS_USB_EP_XFR_SUCCESS;

            if(g_tdev[this_tdev].class_handle->usbh_class_dataout_done != 0)
            {
                /* call-back class driver */
                g_tdev[this_tdev].class_handle->usbh_class_dataout_done(g_tdev[this_tdev].addr,
                                                                        status,
                                                                        gh_tx_ep[ep_num].xfr_count);
            }
            else
                ASSERT(0);

        }
        else
        {
            rem_length = gh_tx_ep[ep_num].xfr_length - gh_tx_ep[ep_num].xfr_count;
            gh_tx_ep[ep_num].buf_addr += gh_tx_ep[ep_num].txn_count;

            gh_tx_ep[ep_num].txn_count = 0u;//reset txn_count for next txn
            if(rem_length > gh_tx_ep[ep_num].max_pkt_size)
            {
                gh_tx_ep[ep_num].txn_length = gh_tx_ep[ep_num].max_pkt_size;
            }
            else
            {
                gh_tx_ep[ep_num].txn_length = rem_length;
            }

            MSS_USB_HCIF_tx_ep_write_pkt(&gh_tx_ep[ep_num]);
        }
    }
    else
    {
        gh_tx_ep[ep_num].state = (mss_usb_ep_state_t)status;

        if(g_tdev[this_tdev].class_handle->usbh_class_dataout_done != 0)
        {
            /* call-back class driver */
            g_tdev[this_tdev].class_handle->usbh_class_dataout_done(g_tdev[this_tdev].addr,
                                                                    status,
                                                                    gh_tx_ep[ep_num].xfr_count);
        }
        else
            ASSERT(0);

    }
}

/*
 This Call-back function is executed on completion of current IN transfer.
*/
static void
MSS_USB_host_ep_rx_cb
(
    uint8_t ep_num,
    uint8_t status
)
{
    uint8_t this_tdev = 0u;
    uint32_t rem_length = 0u;
    this_tdev = gh_rx_ep[ep_num].tdev_idx;//retrieve tdev_idx which received data.

    if(MSS_USB_EP_TXN_SUCCESS == status)
    {
        if(gh_rx_ep[ep_num].xfr_count == gh_rx_ep[ep_num].xfr_length)
        {
            gh_rx_ep[ep_num].state = MSS_USB_EP_XFR_SUCCESS;

            if(g_tdev[this_tdev].class_handle->usbh_class_datain_done != 0)
            {
                /* call-back class driver */
                g_tdev[this_tdev].class_handle->
                    usbh_class_datain_done(g_tdev[this_tdev].addr,
                                           status,
                                           gh_rx_ep[ep_num].xfr_count);
            }
            else
                ASSERT(0);

        }
        else
        {
            rem_length = gh_rx_ep[ep_num].xfr_length - gh_rx_ep[ep_num].xfr_count;
            gh_rx_ep[ep_num].buf_addr += gh_rx_ep[ep_num].txn_count;

            gh_rx_ep[ep_num].txn_count = 0u;//reset txn_count for next txn
            if(rem_length > gh_rx_ep[ep_num].max_pkt_size)
                gh_rx_ep[ep_num].txn_length = gh_rx_ep[ep_num].max_pkt_size;
            else
                gh_rx_ep[ep_num].txn_length = rem_length;

            MSS_USB_HCIF_rx_ep_set_reqpkt((mss_usb_ep_num_t)ep_num);
        }
    }
    else
    {
        gh_rx_ep[ep_num].state = (mss_usb_ep_state_t)status;

        if(g_tdev[this_tdev].class_handle->usbh_class_datain_done != 0)
        {
            /* call-back class driver */
            g_tdev[this_tdev].class_handle->usbh_class_datain_done(g_tdev[this_tdev].addr,
                                                                   status,
                                                                   0u);   // count = 0 since there was error
        }
    }
}

/*
 This Call-back function is executed on completion of current CONTROL transfer.
*/
static void
MSS_USB_host_cep_cb
(
    uint8_t status
)
{
    gh_cep_cb_event = status;
}

/*
 This Call-back function is executed on receiving SOF interrupt in HOST mode
*/
static void
MSS_USB_host_sof_cb
(
    uint32_t frame_number
)
{
}

/*
 This Call-back function is executed on detecting the device attach event
*/
static void
MSS_USB_host_connect_cb
(
    mss_usb_device_speed_t target_speed,
    mss_usb_vbus_level_t vbus_level
)
{
    g_tdev[TDEV_R].speed = target_speed;
    gh_tdev_connect_event = 1u;
}

/*
 This Call-back function is executed on detecting the device detach event
*/
static void
MSS_USB_host_disconnect_cb
(
    void
)
{
    gh_tdev_discon_event = 1u;
}

/*
 This Call-back function is executed on detecting a Vbus level error event
*/
static void
MSS_USB_host_vbus_err_cb
(
    mss_usb_vbus_level_t vbus_level
)
{

}

/*
 This Call-back function is executed on detecting the babble error event
*/
static void
MSS_USB_host_babble_err_cb
(
    void
)
{

}

/*
 This Call-back function is executed on detecting the session request event
 when the attached device is also a OTG device.
*/
static void
MSS_USB_host_session_request_cb
(
    void
)
{

}

/*
 This function checks errors in the received device descriptor.
*/
static int8_t
validate_dev_desc
(
    dev_desc_t* p_desc
)
{
    int8_t result = 0;

    if(p_desc->bLength != USB_STD_DEVICE_DESCR_LEN)
        result = -1;
    if(p_desc->bDescriptorType != USB_DEVICE_DESCRIPTOR_TYPE)
        result = -2;
    if(!((p_desc->bcdUSB != USB_BCD_VERSION_2_0) ||
            (p_desc->bcdUSB != USB_BCD_VERSION_1_1) ||
            (p_desc->bcdUSB != USB_BCD_VERSION_1_0)))
        result = -3;

    if(!((p_desc->bMaxPacketSize0 != 8u) ||
       (p_desc->bMaxPacketSize0 != 16u) ||
       (p_desc->bMaxPacketSize0 != 32u) ||
       (p_desc->bMaxPacketSize0 != 64u)))

    {
        result = -4;
    }
    if((MSS_USB_DEVICE_HS == g_tdev[TDEV_R].speed))
    {
        if(p_desc->bcdUSB != USB_BCD_VERSION_2_0)
            result = -5;
        if(p_desc->bMaxPacketSize0 != 64u)
            result = -6;
    }
    if((MSS_USB_DEVICE_LS == g_tdev[TDEV_R].speed))
    {
        if(p_desc->bMaxPacketSize0 != 8u)
            result = -7;
    }

    if((MSS_USB_DEVICE_FS == g_tdev[TDEV_R].speed))
    {
            result = FS_DEV_NOT_SUPPORTED;
    }
    if((MSS_USB_DEVICE_LS == g_tdev[TDEV_R].speed))
    {
            result = LS_DEV_NOT_SUPPORTED;
    }

    /*0 return value indicates success*/
    return(result);
}

/*
 This function checks errors in the received configuration descriptor (9bytes)
*/
static int8_t
validate_def_conf_desc
(
    def_conf_desc_t* p_desc
)
{
    int8_t result = 0;
    if(p_desc->bLength != USB_STD_CONFIG_DESCR_LEN)
        result = -8;
    if(p_desc->bDescriptorType != USB_CONFIGURATION_DESCRIPTOR_TYPE)
        result = -9;
    if(p_desc->bMaxPower > USB_MAX_BUS_POWER)
        result = -10;

    /*0 return value indicates success*/
    return(result);
}

/*
 This function executes the sequence of actions when device is disconnected.
*/
static void
MSS_USB_handle_discon_event
(
    void
)
{
    gh_tdev_discon_event = 0;
    tdev_idx = TDEV_R;


    if(0 != g_user_cb->usbh_tdev_dettached)
        g_user_cb->usbh_tdev_dettached();

    if(g_rcd[TDEV_R].alloc_state == 1u)
        g_rcd[TDEV_R].class_handle->usbh_class_release(g_tdev[TDEV_R].addr);

    g_tdev[TDEV_R].addr = USB_DEFAULT_TARGET_ADDR;
    g_tdev[TDEV_R].tdev_maxpktsz0 = 64u;
    g_tdev[TDEV_R].speed = MSS_USB_DEVICE_HS;
    g_tdev[TDEV_R].state = MSS_USB_NOT_ATTACHED_STATE;
    g_tdev[TDEV_R].hub_addr = 0u;
    g_tdev[TDEV_R].hub_port = 0u;
    g_tdev[TDEV_R].hub_mtt = 0u;
    g_tdev[TDEV_R].tdev_maxpktsz0 = CEP_MAX_PKT_SIZE;
    g_tdev[TDEV_R].class_handle = 0u;
    g_tdev_error_code = 0;

    memset(&g_tdev_desc[TDEV_R], 0u, sizeof(tdev_desc_t));
    /*Un-register Class driver*/
    g_rcd[TDEV_R].alloc_state = 0u;

    MSS_USB_host_reset_enum_fsm();

    MSS_USB_CIF_enable_usbirq(CONNECT_IRQ_MASK |
                              DISCONNECT_IRQ_MASK);

    MSS_USBH_configure_control_pipe(USB_DEFAULT_TARGET_ADDR);

    MSS_USB_HCIF_tx_ep_set_target_func_addr(MSS_USB_CEP,0u);
    MSS_USB_HCIF_cep_set_type0_reg((mss_usb_device_speed_t)0u);

}

static int8_t
host_enum_check_class_support
(
    tdev_id_t tid
)
{
    uint8_t i=0;

    for(i=0;i<1;i++)    /*Max number of registered Class drivers allowed is 1*/
    {
        if(0x09 == g_tdev_desc[tid].conf_desc.cs.upended_desc[5])
            return(HUB_CLASS_NOT_SUPPORTED);

        if(((uint8_t)(g_rcd[i].class_handle->class_id>>16u) ==
             g_tdev_desc[tid].conf_desc.cs.upended_desc[5]) &&
            ((uint8_t)(g_rcd[i].class_handle->class_id>>8u) ==
             g_tdev_desc[tid].conf_desc.cs.upended_desc[6]) &&
            ((uint8_t)(g_rcd[i].class_handle->class_id) ==
             g_tdev_desc[tid].conf_desc.cs.upended_desc[7]))
        {
            if(g_rcd[i].alloc_state == 0u) /*class driver is matched and available*/
            {
                /*Allocation successful*/
                return(i);
            }
            else
            {
                /*Match found but Driver is already assigned*/
                return(CLASS_DRIVER_NOT_FREE);
            }
        }
    }
    return(CLASS_DRIVER_NOT_MATCHED);    /*No matching class driver found*/
}

#endif //MSS_USB_HOST_ENABLED

#ifdef __cplusplus
}
#endif
