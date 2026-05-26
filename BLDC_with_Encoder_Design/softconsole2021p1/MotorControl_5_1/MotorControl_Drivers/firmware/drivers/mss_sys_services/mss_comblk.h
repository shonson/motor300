/*******************************************************************************
 * (c) Copyright 2012 Microsemi SoC Products Group.  All rights reserved.
 *
 * SmartFusion2 COMBLK access functions.
 *
 * SVN $Revision: 6348 $
 * SVN $Date: 2014-04-25 18:44:50 +0530 (Fri, 25 Apr 2014) $
 */
#ifndef __MSS_COMBLK_H_
#define __MSS_COMBLK_H_ 1

#include "../../CMSIS/m2sxxx.h"
#include "mss_comblk_page_handler.h"

#ifdef __cplusplus
extern "C" {
#endif

#define TAMPER_ATTEMPT_DETECT_OPCODE_RANGE_MIN           0x80u
#define TAMPER_ATTEMPT_DETECT_OPCODE_RANGE_MAX           0x8Fu
#define TAMPER_FAILURE_DETECT_OPCODE_RANGE_MIN           0x90u
#define TAMPER_FAILURE_DETECT_OPCODE_RANGE_MAX           0x9Fu
#define TAMPER_CLOCK_MONITOR_ERROR_OPCODE                0xA0u
#define TAMPER_HARDWARE_MONITOR_ERROR_OPCODE_RANGE_MIN   0xB1u
#define TAMPER_HARDWARE_MONITOR_ERROR_OPCODE_RANGE_MAX   0xB7u
  
/*------------------------------------------------------------------------------
 *
 */
typedef void(*comblk_completion_handler_t)(uint8_t * p_response, uint16_t response_size);

typedef void (*comblk_async_event_handler_t)(uint8_t event_opcode);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_init
(
    comblk_async_event_handler_t async_event_handler, 
    uint8_t* p_response
);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_send_cmd_with_ptr
(
    uint8_t cmd_opcode,
    uint32_t cmd_params_ptr,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_send_cmd
(
    const uint8_t * p_cmd,
    uint16_t cmd_size,
    const uint8_t * p_data,
    uint32_t data_size,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_read
(
    const uint8_t * p_data,
    uint16_t cmd_size,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_completion_handler_t completion_handler
);

/*------------------------------------------------------------------------------
 *
 */
void MSS_COMBLK_send_paged_cmd
(
    const uint8_t * p_cmd,
    uint16_t cmd_size,
    uint8_t * p_response,
    uint16_t response_size,
    comblk_page_handler_t page_read_handler,
    comblk_completion_handler_t completion_handler
);

#ifdef __cplusplus
}
#endif

#endif /* __MSS_COMBLK_H_ */
