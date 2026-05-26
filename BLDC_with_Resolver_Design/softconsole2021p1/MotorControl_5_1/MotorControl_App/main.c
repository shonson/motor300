/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : main.c			                                               **
 **                                                                            **
 **  PLATFORM  : SmartFusion2                                                  **
 **                                                                            **
 **  COMPILER  : Softconsole                                                   **
 **                                                                            **
 **  AUTHOR    : India Solutions Team			                               **
 **                                                                            **
 **  VENDOR    : Microsemi                                                     **
 **                                                                            **
 **  DESCRIPTION  : This source file includes                                  **
 **                 - the main() procedure.						               **
 **                                                                            **
 *******************************************************************************/

#include "mss_usb_device.h"
/*lint -e537*/
/*537: Multiple includes of header files stdint.h is included from Typedef.h
 * 		as well as mss_usb_device_hid.h. The latter is a driver, and so user
 * 		cannot modify it, nor does it make sense to include the USB driver in
 * 		typedef.*/
#include "mss_usb_device_hid.h"
/*lint -e537*/
#include "USBMsgHandler.h"

/*lint -e970
 * Note 970: Use of modifier or type 'int' outside of a typedef [MISRA 2004
 *   Rule 6.3, advisory]
 *   Softconsole requires main() to have a return type int, so this advisory
 *   is suppressed*/
int main() {
	/*lint +e970*/

	g_streamdata = 2U;
	MotorControl_Init();
	/*Initialize USB Interface*/
	USB_init();

	for (;;) {
		if (g_streamdata == 0U) {
			if (g_streamaxis == 0U) {

				g_base_addr = (addr_t) BASE_ADDR_0;
#if 1

					/*Variable_1 is Speed PI Output */
					g_mc_bldc_foc.mc_graph.variable1 = (int32_t) (HW_get_32bit_reg(
							g_base_addr + (addr_t) C_SPEED_PI_OUT_ADDR)) / 4;
					/*Variable_2 is IQ_Park */
					g_mc_bldc_foc.mc_graph.variable2 = (int32_t) (HW_get_32bit_reg(
							g_base_addr + (addr_t) C_IQ_PARK_ADDR)) / 4;


#else
				/*Variable_1 is A phase current */
				g_mc_bldc_foc.mc_graph.variable1
				= (int32_t)(HW_get_32bit_reg(g_base_addr
								+ (addr_t) C_IA_ADDR))/4;
				/*Variable_2 is FSM State */
				g_mc_bldc_foc.mc_graph.variable2
				= (int32_t)(HW_get_32bit_reg(g_base_addr
								+ (addr_t) C_MC_FSM_STATE_ADDR)) * 25;
#endif
#if(ALGORITHM_SELECTION == SENSOR_LESS)
				/*Variable_3 is IQ_PI_out */
				g_mc_bldc_foc.mc_graph.variable3 = (int32_t) (HW_get_32bit_reg(
						g_base_addr + (addr_t) C_IQ_PI_OUT_ADDR)) / 4;

				/*Variable_4 is Speed Filter Output */
				g_mc_bldc_foc.mc_graph.variable4
						= (int32_t) ((HW_get_32bit_reg(g_base_addr
								+ (addr_t) C_OMEGA_FILTER_OUT_ADDR)) / 4);

#endif
#if(ALGORITHM_SELECTION == ENCODER)
				/*Variable_3 is IQ_PI_out */
				g_mc_bldc_foc.mc_graph.variable3 = (int32_t) ((HW_get_32bit_reg(
						g_base_addr + (addr_t) C_IQ_PI_OUT_ADDR)) / 4);

				/*Variable_4 is Speed Filter Output */
				g_mc_bldc_foc.mc_graph.variable4
						= (int32_t) ((HW_get_32bit_reg(g_base_addr
								+ (addr_t) C_EN_SPEED_ADDR)) / 4);

#endif
#if(ALGORITHM_SELECTION == HALL_INTERFACE)
				/*Variable_3 is IQ_PI_out */
				g_mc_bldc_foc.mc_graph.variable3 = (int32_t) ((HW_get_32bit_reg(
						g_base_addr + (addr_t) C_IQ_PI_OUT_ADDR)) / 4);

				/*Variable_4 is Speed Filter Output */
				g_mc_bldc_foc.mc_graph.variable4
						= (int32_t) ((HW_get_32bit_reg(g_base_addr
								+ (addr_t) C_HALL_OMEGA_OUT_ADDR)) / 4);

#endif
#if(ALGORITHM_SELECTION == RESOLVER)
				/*Variable_3 is IQ_PI_out */
				g_mc_bldc_foc.mc_graph.variable3 = (int32_t) ((HW_get_32bit_reg(
						g_base_addr + (addr_t) C_IQ_PI_OUT_ADDR)) / 4);

								/*Variable_4 is Speed Filter Output */
				g_mc_bldc_foc.mc_graph.variable4
						= (int32_t) ((HW_get_32bit_reg(g_base_addr
												+ (addr_t) C_OMEGA_FILTER_OUT_ADDR)) / 4);
#endif
				SendStreamDATA();
			}
			if (g_streamaxis == 1U) {

				g_base_addr = (addr_t) BASE_ADDR_1;
				/*Variable_1 is Actual ID Current */
				g_stepper.mc_graph.variable1 = (int32_t) (HW_get_32bit_reg(
						g_base_addr + (addr_t) C_ID_PARK_ADDR)) / 4;
				/*Variable_2 is Motor Reference Current*/
				g_stepper.mc_graph.variable2 = ((g_stepper.i_ref * 65535)
						/ (int32_t) MOTOR_CURRENT_ST) / 4;
				/*Variable_3 is ID PI output */
				g_stepper.mc_graph.variable3 = (int32_t) (HW_get_32bit_reg(
						g_base_addr + (addr_t) C_ID_PI_OUT_ADDR)) / 4;
				/*Variable_4 is Stepper phase current */
				g_stepper.mc_graph.variable4 = (int32_t) (HW_get_32bit_reg(
						g_base_addr + (addr_t) C_IA_ADDR)) / 4;

				SendStreamDATA();
			}
		}
	}
}
