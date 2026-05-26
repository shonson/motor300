/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : USBMsgHandler.c                                               **
 **                                                                            **
 **  PLATFORM  : SmartFusion2                                                  **
 **                                                                            **
 **  COMPILER  : Softconsole                                                   **
 **                                                                            **
 **  AUTHOR    : India Solutions Team			                               **
 **                                                                            **
 **  VENDOR    : Microsemi                                                     **
 **                                                                            **
 **  DESCRIPTION  :This source file includes                                   **
 **                 - function definitions of Application driver               **
 **                                                                            **
 *******************************************************************************/
/*******************************************************************************
 **                      Includes                                              **
 *******************************************************************************/
#include "USBMsgHandler.h"
#include "mss_usb_device_hid.h"


static mss_usbd_hid_report_t g_report = { 0U };
volatile uint8_t g_streamdata = 2U;
volatile uint8_t g_streamaxis = 2U;

uint32_t g_encoder_steps;
/*******************************************************************************
 ** Service Name     : USB_init	                                      	       **
 **                                                                            **
 ** Syntax           : void USB_init(void)				                       **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none			                                           **
 **                                                                            **
 ** Description 	 : This procedure is used to initialize USB.			   **
 **                                                                            **
 *******************************************************************************/
void USB_init() {
	/*lint -esym(960,87)*/
	/* USB descriptors for HID class mouse enumeration. */
	extern mss_usbd_user_descr_cb_t hid_mouse_descriptors_cb;
	/**/
	/*Keep USB PHY out of Reset*/
	MSS_GPIO_set_output(MSS_GPIO_0, 0u);

	/*Assign call-back function handler structure needed by USB Device Core driver*/
	MSS_USBD_set_descr_cb_handler(&hid_mouse_descriptors_cb);

	/*Initialize HID Class driver.*/
	MSS_USBD_HID_init(MSS_USB_DEVICE_HS);

	/*Initialize USB Device Core driver.*/
	MSS_USBD_init(MSS_USB_DEVICE_HS);

	MSS_USBD_rx_ep_configure(MSS_USB_RX_EP_1, 0x100u, 64U, 25U, 1u,
			DMA_DISABLE, MSS_USB_DMA_CHANNEL1, MSS_USB_XFR_INTERRUPT,
			NO_ZLP_TO_XFR);

	MSS_USB_CIF_rx_ep_enable_irq(MSS_USB_RX_EP_1);

	MSS_USBD_rx_ep_flush_fifo(MSS_USB_RX_EP_1);
	MSS_USBD_tx_ep_flush_fifo(MSS_USB_TX_EP_1);

	MSS_USBD_rx_ep_read_prepare(MSS_USB_RX_EP_1, g_rx_data, 25U);
}

/*******************************************************************************
 ** Service Name     : USBMsgHandler	                                       **
 **                                                                            **
 ** Syntax           : void USBMsgHandler(void)			                       **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none			                                           **
 **                                                                            **
 ** Description 	: This procedure is used to handle USB messages from GUI.  **
 **                                                                            **
 *******************************************************************************/
/*lint -e764 -e765*/
void USBMsgHandler() {
	/*lint +e764 +e765*/
	uint16_t temp_uint16;
	uint32_t temp_uint32;
	switch (g_rx_data[0]) /*command id*/
	{
	case 0x00: /*StartMotor*/
	{
		switch (g_rx_data[1]) {
		case 0x00:
			if (g_mc_bldc_foc.run_status == 0) {
				g_mc_bldc_foc.run_status = 1;
				g_base_addr = (addr_t) BASE_ADDR_0;
				MotorControl_Start();
			}
			break;
		case 0x01:
			if (g_stepper.run_status == 0) {
				g_stepper.run_status = 1;
				g_base_addr = (addr_t) BASE_ADDR_1;
				MotorControl_Start();
			}
			break;
		default:
			/*Do nothing*/
			break;
		}
	}
		break;

	case 0x01: /*StopMotor*/
	{
		switch (g_rx_data[1]) {
		case 0x00:
			if (g_mc_bldc_foc.run_status == 1) {
				g_mc_bldc_foc.run_status = 0;
				g_base_addr = (addr_t) BASE_ADDR_0;
				MotorControl_Stop();
			}
			break;
		case 0x01:
			if (g_stepper.run_status == 1) {
				g_stepper.run_status = 0;
				g_base_addr = (addr_t) BASE_ADDR_1;
				MotorControl_Stop();
			}
			break;
		default:
			/*Do nothing*/
			break;
		}
	}
		break;

	case 0x02:
		/*set direction clockwise */
	{
		switch (g_rx_data[1]) {
		case 0x00:
			g_mc_bldc_foc.direction = 1;
			MC_BLDCSetDirn(g_mc_bldc_foc.direction);

			break;
		case 0x01:
			g_stepper.direction = -1;
			MC_StepperSetDirn(g_stepper.direction);
			break;
		default:
			/*Do nothing*/
			break;
		}
		break;
	}
	case 0x03:
		/*set direction anti clockwise */
	{
		switch (g_rx_data[1]) {
		case 0x00:
			g_mc_bldc_foc.direction = 0;
			MC_BLDCSetDirn(g_mc_bldc_foc.direction);
			break;
		case 0x01:
			g_stepper.direction = 1;
			MC_StepperSetDirn(g_stepper.direction);
			break;
		default:
			/*Do nothing*/
			break;
		}
		break;
	}

	case 0x04: {
		switch (g_rx_data[1]) {
		case 0x00:
			MC_BLDCInit();
			break;
		case 0x01:
			MC_StepperInit();
			break;
		default:
			/*Do nothing*/
			break;
		}

	}

		break;

	case 0x05:
		/*Unused*/
		break;

		//**********************GET FROM MSS TO GUI****************************//

	case 0x06: /*Get command*/
	{
		switch (g_rx_data[1]) {
		case 0x00:
			g_report.A = (uint8_t) 0x06;
			g_report.B = (uint8_t) 0x00;
			g_report.C = (uint8_t) ((uint32_t) g_mc_bldc_foc.speed_ref >> 16U);
			g_report.D = (uint8_t) ((uint32_t) g_mc_bldc_foc.speed_ref >> 8U);
			g_report.E = (uint8_t) (g_mc_bldc_foc.speed_ref);
			g_report.F = (uint8_t) ((uint32_t) g_mc_bldc_foc.speed_ramp >> 8);
			g_report.G = (uint8_t) (g_mc_bldc_foc.speed_ramp);
			g_report.H = (uint8_t) 0x00u;
			g_report.I = (uint8_t) 0x00u;
			g_report.J = (uint8_t) 0x00u;
			g_report.K = (uint8_t) 0x00u;
			g_report.L = (uint8_t) 0x00u;
			g_report.M = (uint8_t) 0x00u;
			g_report.N = (uint8_t) 0x00u;
			g_report.O = (uint8_t) 0x00u;
			g_report.P = (uint8_t) 0x00u;
			g_report.Q = (uint8_t) 0x00u;
			g_report.R = (uint8_t) 0x00u;
			g_report.S = (uint8_t) 0x00u;
			g_report.T = (uint8_t) 0x00U;
			g_report.U = (uint8_t) 0x00U;
			g_report.V = (uint8_t) 0x00U;
			g_report.W = (uint8_t) 0x00U;
			g_report.X = (uint8_t) 0x00U;
			g_report.Y = (uint8_t) 0x00U;
			break;
		case 0x01:
			g_report.A = (uint8_t) 0x06;
			g_report.B = (uint8_t) (g_stepper.mode);
			g_report.C = (uint8_t) ((uint16_t) g_stepper.speed_rpm >> 8U);
			g_report.D = (uint8_t) (g_stepper.speed_rpm);
			g_report.E = (uint8_t) ((uint32_t) g_stepper.cmd_steps >> 8U);
			g_report.F = (uint8_t) (g_stepper.cmd_steps);
			g_report.G = (uint8_t) ((uint32_t) g_stepper.i_ref >> 8U);
			g_report.H = (uint8_t) (g_stepper.i_ref);
			g_report.I = (uint8_t) ((uint32_t) g_stepper.i_kp_gain >> 8U);
			g_report.J = (uint8_t) (g_stepper.i_kp_gain);
			g_report.K = (uint8_t) ((uint32_t) g_stepper.i_ki_gain >> 8U);
			g_report.L = (uint8_t) (g_stepper.i_ki_gain);
			g_report.M = (uint8_t) ((uint16_t) g_stepper.microstep_res >> 8U);
			g_report.N = (uint8_t) (g_stepper.microstep_res);
			g_report.O = (uint8_t) ((uint16_t) g_stepper.step_num >> 8U);
			g_report.P = (uint8_t) (g_stepper.step_num);
			g_report.Q = (uint8_t) 0x00U;
			g_report.R = (uint8_t) 0x00U;
			g_report.S = (uint8_t) 0x00U;
			g_report.T = (uint8_t) 0x00U;
			g_report.U = (uint8_t) 0x00U;
			g_report.V = (uint8_t) 0x00U;
			g_report.W = (uint8_t) 0x00U;
			g_report.X = (uint8_t) 0x00U;
			g_report.Y = (uint8_t) 0x00U;

			break;
		default:
			/*Do nothing*/
			break;
		}
		if (1U == MSS_USBD_HID_tx_done()) {
			/*lint -e534*/
			/* 534: Warning on ignoring Return value */
			MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));
			/*lint +e534*/
		}
		break;
	}

		//*********************SET FROM GUI TO MSS******************************//

	case 0x07: {
		switch (g_rx_data[1]) {
		case 0x00:

			g_base_addr = (addr_t) BASE_ADDR_0;
			temp_uint32 = ((uint32_t) g_rx_data[2] << 16u)
					+ ((uint32_t) g_rx_data[3] << 8u) + (uint32_t) g_rx_data[4];/*Speed ref*/
			g_mc_bldc_foc.speed_ref = (int32_t) temp_uint32;
			temp_uint32 = (((uint32_t) g_rx_data[5] << 8)
					+ (uint32_t) g_rx_data[6]);
			g_mc_bldc_foc.speed_ramp = (int32_t) temp_uint32;

			BLDC_SetSpeedSRamp();
			break;
		case 0x01:
			g_base_addr = (addr_t) BASE_ADDR_1;
			g_stepper.mode = (int8_t) g_rx_data[3];
			temp_uint16 = (((uint16_t) g_rx_data[4]) << 8u);
			temp_uint16 += (uint16_t) g_rx_data[5];
			g_stepper.speed_rpm = (int16_t) temp_uint16;
			temp_uint32 = (((uint32_t) g_rx_data[6] << 8)
					+ (uint32_t) g_rx_data[7]);
			g_stepper.cmd_steps = (int32_t) temp_uint32;
			temp_uint32 = ((uint32_t) g_rx_data[8] << 8);
			temp_uint32 += (uint32_t) g_rx_data[9];
			g_stepper.i_ref = (int32_t) temp_uint32;
			g_stepper.i_kp_gain = (((uint32_t) g_rx_data[10] << 8)
					+ (uint32_t) g_rx_data[11]);
			g_stepper.i_ki_gain = (((uint32_t) g_rx_data[12] << 8)
					+ (uint32_t) g_rx_data[13]);
			temp_uint16 = ((uint16_t) g_rx_data[14] << 8);
			temp_uint16 += (uint16_t) g_rx_data[15];
			g_stepper.microstep_res = (int16_t) temp_uint16;
			temp_uint16 = ((uint16_t) g_rx_data[16] << 8);
			temp_uint16 += (uint16_t) g_rx_data[17];
			g_stepper.step_num = (int16_t) temp_uint16;
			MC_StepperConstCal();
			MC_StepperSetFabReg();

			break;
		default:
			/*Do nothing*/
			break;
		}
		break;
	}

	case 0x08:
		/*if g_streamdata = 0, SendStreamDATA() function will called.*/
		g_streamdata = g_rx_data[3];
		g_streamaxis = g_rx_data[2];
		break;

	case 0x09:/* Test Connect Command*/
		g_report.A = 0x09U;
		g_report.B = 0x00U;
		g_report.C = 0x0FU;
		if (1U == MSS_USBD_HID_tx_done()) {
			/*lint -e534*/
			/* 534: Warning on ignoring Return value */
			MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));

			/*lint +e534*/
		}
		break;
	case 0x0A:
		g_report.A = 0x0AU;
		MC_BLDCGetSpdI();
		g_report.B = (uint8_t) ((uint32_t) g_mc_bldc_foc.mc_graph.rpm >> 8u);
		g_report.C = (uint8_t) (g_mc_bldc_foc.mc_graph.rpm);
		g_report.D = (uint8_t) (g_mc_bldc_foc.mc_graph.current >> 8u);
		g_report.E = (uint8_t) (g_mc_bldc_foc.mc_graph.current);
		if (1U == MSS_USBD_HID_tx_done()) {
			/*lint -e534*/
			/* 534: Warning on ignoring Return value */
			MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));
			/*lint +e534*/
		}
		/*lint -e950 -e960*/
		/*950: Non-ANSI reserved word or construct		**
		 * 		The asm keyword has been used to create exactly one delay
		 *960: Violates MISRA 2004 Required Rule 14.3, null statement not
		 in line by itself
		 The asm statement is detected as a null statement by PC-Lint.

		 */
		asm("nop\n");
		asm("nop\n");
		asm("nop\n");
		/*lint +e950 +e960*/

		break;
	case 0x0B:
		g_report.A = 0x0BU;
		if (1U == MSS_USBD_HID_tx_done()) {
			/*lint -e534*/
			/* 534: Warning on ignoring Return value */
			MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));
			/*lint +e534*/
		}
		break;
	case 0x0D:
		switch (g_rx_data[1]) {
		case 0x00:
			g_base_addr = (addr_t) BASE_ADDR_0;
			MC_WClearFault();
			break;
		case 0x01:
			break;
		default:
			break;
			if (1U == MSS_USBD_HID_tx_done()) {
				/*lint -e534*/
				/* 534: Warning on ignoring Return value */
				MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));
				/*lint +e534*/
			}
		}
		break;
	case 0x0E:
		switch (g_rx_data[1]) {
		case 0x00:
			g_base_addr = (addr_t) BASE_ADDR_0;
			/*g_algorithm = g_rx_data[2];*/
			g_mc_bldc_foc.i_kp_gain = ((uint32_t) g_rx_data[3] << 8)
					+ (uint32_t) g_rx_data[4];//Set Current kp
			g_mc_bldc_foc.i_ki_gain = ((uint32_t) g_rx_data[5] << 8)
					+ (uint32_t) g_rx_data[6];//Set Current ki
			g_mc_bldc_foc.speed_kp_gain = ((uint32_t) g_rx_data[7] << 8)
					+ (uint32_t) g_rx_data[8];/*Set Speed kp*/

			g_mc_bldc_foc.speed_ki_gain = ((uint32_t) g_rx_data[9] << 8)
					+ (uint32_t) g_rx_data[10]; //Set Speed ki
			g_mc_bldc_foc.seq_controller_config = g_rx_data[11];
			g_mc_bldc_foc.closed_loop_speed = ((uint32_t) g_rx_data[12] << 8)
					+ (uint32_t) g_rx_data[13];
			g_mc_bldc_foc.open_loop_current = ((uint32_t) g_rx_data[14] << 8)
					+ (uint32_t) g_rx_data[15];
			g_mc_bldc_foc.open_loop_voltage = ((uint32_t) g_rx_data[16] << 8)
					+ (uint32_t) g_rx_data[17];

			if (g_algorithm == 0u) {
				g_mc_bldc_foc.angle_kp_gain = (((uint32_t) g_rx_data[18] << 8)
						+ (uint32_t) g_rx_data[19]);
				g_mc_bldc_foc.angle_ki_gain = (((uint32_t) g_rx_data[20] << 8)
						+ (uint32_t) g_rx_data[21]);
			} else if (g_algorithm == 1u) {
				g_mc_bldc_foc.encoder_resolution = (((uint32_t) g_rx_data[18] << 8)
										+ (uint32_t) g_rx_data[19]);
				g_mc_bldc_foc.angle_kp_gain = (((uint32_t) g_rx_data[20] << 8)
										+ (uint32_t) g_rx_data[21]);
				g_mc_bldc_foc.angle_ki_gain = (((uint32_t) g_rx_data[22] << 8)
										+ (uint32_t) g_rx_data[23]);
				g_mc_bldc_foc.mode = (uint32_t) g_rx_data[24];
			} else if (g_algorithm == 2u) {
				g_hall_sensor_pol = g_rx_data[18];
			}
			else {
				//Do nothing
			}			
			MC_BLDCConstCal();
			MC_BLDCSetFabReg();
			break;
		case 0x01:
			g_base_addr = (addr_t) BASE_ADDR_1;
			g_stepper.mode = (int8_t) g_rx_data[3];
			temp_uint16 = (((uint16_t) g_rx_data[4]) << 8u);
			temp_uint16 += (uint16_t) g_rx_data[5];
			g_stepper.speed_rpm = (int16_t) temp_uint16;
			temp_uint32 = (((uint32_t) g_rx_data[6] << 8)
					+ (uint32_t) g_rx_data[7]);
			g_stepper.cmd_steps = (int32_t) temp_uint32;
			temp_uint32 = ((uint32_t) g_rx_data[8] << 8);
			temp_uint32 += (uint32_t) g_rx_data[9];
			g_stepper.i_ref = (int32_t) temp_uint32;
			g_stepper.i_kp_gain = (((uint32_t) g_rx_data[10] << 8)
					+ (uint32_t) g_rx_data[11]);
			g_stepper.i_ki_gain = (((uint32_t) g_rx_data[12] << 8)
					+ (uint32_t) g_rx_data[13]);
			temp_uint16 = ((uint16_t) g_rx_data[14] << 8);
			temp_uint16 += (uint16_t) g_rx_data[15];
			g_stepper.microstep_res = (int16_t) temp_uint16;
			temp_uint16 = ((uint16_t) g_rx_data[16] << 8);
			temp_uint16 += (uint16_t) g_rx_data[17];
			g_stepper.step_num = (int16_t) temp_uint16;
			MC_StepperConstCal();
			MC_StepperSetFabReg();
			break;
		default:
			/*Do nothing*/
			break;
			/*Acknowledging with 0x0E*/
			g_report.A = 0x0EU;
			if (1U == MSS_USBD_HID_tx_done()) {
				/*lint -e534*/
				/* 534: Warning on ignoring Return value */
				MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));
				/*lint +e534*/
			}
		}
		break;
	case 0x0F:
		switch (g_rx_data[1]) {
		case 0x00:
			g_report.A = (uint8_t) 0x0F;
			g_report.B = (uint8_t) 0x00;//Axis ID
			g_report.C = (uint8_t) g_algorithm;
			g_report.D = (uint8_t) ((uint32_t) g_mc_bldc_foc.i_kp_gain >> 8U);/*Get Current kp*/
			g_report.E = (uint8_t) (g_mc_bldc_foc.i_kp_gain);
			g_report.F = (uint8_t) ((uint32_t) g_mc_bldc_foc.i_ki_gain >> 8U);/*Get Current ki*/
			g_report.G = (uint8_t) (g_mc_bldc_foc.i_ki_gain);
			g_report.H = (uint8_t) ((uint32_t) g_mc_bldc_foc.speed_kp_gain
					>> 8U);
			g_report.I = (uint8_t) (g_mc_bldc_foc.speed_kp_gain);
			g_report.J = (uint8_t) ((uint32_t) g_mc_bldc_foc.speed_ki_gain
					>> 8U);/*Get Speed ki*/
			g_report.K = (uint8_t) (g_mc_bldc_foc.speed_ki_gain);
			g_report.L = (g_mc_bldc_foc.seq_controller_config);
			g_report.M = (uint8_t) (g_mc_bldc_foc.closed_loop_speed >> 8U);/*CL Speed*/
			g_report.N = (uint8_t) (g_mc_bldc_foc.closed_loop_speed);
			g_report.O = (uint8_t) ((uint32_t) g_mc_bldc_foc.open_loop_current
					>> 8U);/*OL I*/
			g_report.P = (uint8_t) (g_mc_bldc_foc.open_loop_current);
			g_report.Q = (uint8_t) ((uint32_t) g_mc_bldc_foc.open_loop_voltage
					>> 8U);/*OL V*/
			g_report.R = (uint8_t) (g_mc_bldc_foc.open_loop_voltage);

			if (g_algorithm == 0) {
				g_report.S = (uint8_t) ((uint32_t) g_mc_bldc_foc.angle_kp_gain
						>> 8U);//Get Angle Kp
				g_report.T = (uint8_t) (g_mc_bldc_foc.angle_kp_gain);
				g_report.U = (uint8_t) ((uint32_t) g_mc_bldc_foc.angle_ki_gain
						>> 8);//Get Angle Ki
				g_report.V = (uint8_t) (g_mc_bldc_foc.angle_ki_gain);
				g_report.W = (uint8_t) 0x00U;
				g_report.X = (uint8_t) 0x00U;
				g_report.Y = (uint8_t) 0x00U;
			} else if (g_algorithm == 1) {
				g_report.S = (uint8_t) (g_mc_bldc_foc.encoder_resolution >> 8u);//Get Angle Kp
				g_report.T = (uint8_t) (g_mc_bldc_foc.encoder_resolution);
				g_report.U = (uint8_t) (g_mc_bldc_foc.angle_kp_gain >> 8u);
				g_report.V = (uint8_t) (g_mc_bldc_foc.angle_kp_gain);
				g_report.W = (uint8_t) (g_mc_bldc_foc.angle_ki_gain >> 8u);
				g_report.X = (uint8_t) (g_mc_bldc_foc.angle_ki_gain);
				g_report.Y = (uint8_t) (g_mc_bldc_foc.mode);
			}else if (g_algorithm == 2) {
				g_report.S = (uint8_t) g_hall_sensor_pol;
				g_report.T = (uint8_t) 0x00U;
				g_report.U = (uint8_t) 0x00U;
				g_report.V = (uint8_t) 0x00U;
				g_report.W = (uint8_t) 0x00U;
				g_report.X = (uint8_t) 0x00U;
				g_report.Y = (uint8_t) 0x00U;
			}

			break;
		case 0x01:
			g_report.A = (uint8_t) 0x06;
			g_report.B = (uint8_t) (g_stepper.mode);
			g_report.C = (uint8_t) ((uint16_t) g_stepper.speed_rpm >> 8U);
			g_report.D = (uint8_t) (g_stepper.speed_rpm);
			g_report.E = (uint8_t) ((uint32_t) g_stepper.cmd_steps >> 8U);
			g_report.F = (uint8_t) (g_stepper.cmd_steps);
			g_report.G = (uint8_t) ((uint32_t) g_stepper.i_ref >> 8U);
			g_report.H = (uint8_t) (g_stepper.i_ref);
			g_report.I = (uint8_t) ((uint32_t) g_stepper.i_kp_gain >> 8U);
			g_report.J = (uint8_t) (g_stepper.i_kp_gain);
			g_report.K = (uint8_t) ((uint32_t) g_stepper.i_ki_gain >> 8U);/*Get Current ki*/
			g_report.L = (uint8_t) (g_stepper.i_ki_gain);
			g_report.M = (uint8_t) ((uint16_t) g_stepper.microstep_res >> 8U);
			g_report.N = (uint8_t) (g_stepper.microstep_res);
			g_report.O = (uint8_t) ((uint16_t) g_stepper.step_num >> 8U);
			g_report.P = (uint8_t) (g_stepper.step_num);
			g_report.Q = (uint8_t) 0x00U;
			g_report.R = (uint8_t) 0x00U;
			g_report.S = (uint8_t) 0x00U;
			g_report.T = (uint8_t) 0x00U;
			g_report.U = (uint8_t) 0x00U;
			g_report.V = (uint8_t) 0x00U;
			g_report.W = (uint8_t) 0x00U;
			g_report.X = (uint8_t) 0x00U;
			g_report.Y = (uint8_t) 0x00U;

			break;
		default:
			/*Do nothing*/
			break;
		}
		if (1U == MSS_USBD_HID_tx_done()) {
			/*lint -e534*/
			/* 534: Warning on ignoring Return value */
			MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));
			/*lint +e534*/
		}
		break;
	case 0x10:
		switch (g_rx_data[1]) {
		case 0x00:
			g_base_addr = (addr_t) BASE_ADDR_0;
			g_bldc1.dc_voltage_mV = ((uint32_t) g_rx_data[2] << 8)
					+ (uint32_t) g_rx_data[3];
			g_bldc1.current_mA = ((uint32_t) g_rx_data[4] << 8)
					+ (uint32_t) g_rx_data[5];
			g_bldc1.speed_RPM = (((uint32_t) g_rx_data[6] << 16)
					+ ((uint32_t) g_rx_data[7] << 8) + (uint32_t) g_rx_data[8]);
			g_bldc1.Npp = g_rx_data[9];
			g_bldc1.Rs_mohm = ((uint32_t) g_rx_data[10] << 8)
					+ (uint32_t) g_rx_data[11];
			g_bldc1.Ls_uhenry = ((uint32_t) g_rx_data[12] << 8)
					+ (uint32_t) g_rx_data[13];
			g_bldc1.switching_freq_kHz = ((uint32_t) g_rx_data[14] << 8)
					+ (uint32_t) g_rx_data[15];
			g_base_addr = (addr_t) BASE_ADDR_0;
			MC_BLDCConstCal();
			MC_BLDCSetFabReg();
			break;
		case 0x01:
			break;
		default:
			break;

		}
		break;
	case 0x11:
		switch (g_rx_data[1]) {
		case 0x00:
			g_report.A = (uint8_t) 0x11U;
			g_report.B = (uint8_t) 0x00U;//Axis ID
			g_report.C = (uint8_t) (g_bldc1.dc_voltage_mV >> 8u);
			g_report.D = (uint8_t) (g_bldc1.dc_voltage_mV);
			g_report.E = (uint8_t) (g_bldc1.current_mA >> 8u);
			g_report.F = (uint8_t) (g_bldc1.current_mA);
			g_report.G = (uint8_t) (g_bldc1.speed_RPM >> 16u);
			g_report.H = (uint8_t) (g_bldc1.speed_RPM >> 8u);
			g_report.I = (uint8_t) (g_bldc1.speed_RPM);
			g_report.J = (uint8_t) (g_bldc1.Npp);/*Get Speed ki*/
			g_report.K = (uint8_t) (g_bldc1.Rs_mohm >> 8u);
			g_report.L = (uint8_t) (g_bldc1.Rs_mohm);
			g_report.M = (uint8_t) (g_bldc1.Ls_uhenry >> 8u);
			g_report.N = (uint8_t) (g_bldc1.Ls_uhenry);
			g_report.O = (uint8_t) (g_bldc1.switching_freq_kHz >> 8U);/*OL V*/
			g_report.P = (uint8_t) (g_bldc1.switching_freq_kHz);
			g_report.Q = (uint8_t) 0x00U;
			g_report.R = (uint8_t) 0x00U;
			g_report.S = (uint8_t) 0x00U;
			g_report.T = (uint8_t) 0x00U;
			g_report.U = (uint8_t) 0x00U;
			g_report.V = (uint8_t) 0x00U;
			g_report.W = (uint8_t) 0x00U;
			g_report.X = (uint8_t) 0x00U;
			g_report.Y = (uint8_t) 0x00U;
			g_base_addr = (addr_t) BASE_ADDR_0;
			//MC_BLDCConstCal();
			//MC_BLDCSetFabReg();
			if (1U == MSS_USBD_HID_tx_done()) {
				/*lint -e534*/
				/* 534: Warning on ignoring Return value */
				MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));
				/*lint +e534*/
			}
			break;

		case 0x01:

			break;
		default:
			/*Do nothing*/
			break;
		}
		break;
	case 0x12:
		switch (g_rx_data[1]) {
		case 0x00:
			g_report.A = (uint8_t) 0x12U;
			g_report.B = (uint8_t) MC_GetStatus();//Fault Status
			g_report.C = (uint8_t) 0xFFU;//Fault Word - future enhancement
			g_report.D = (uint8_t) 0x00U;//Fault Word - future enhancement
			g_report.E = (uint8_t) 0x00U;//Fault Word - future enhancement
			g_report.F = (uint8_t) 0x00U;//Fault Word - future enhancement

			break;

		case 0x01:

			break;
		default:
			/*Do nothing*/
			break;
		}
		if (1U == MSS_USBD_HID_tx_done()) {
			/*lint -e534*/
			/* 534: Warning on ignoring Return value */
			MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));
			/*lint +e534*/
		}

		break;

	default:
		/*Do nothing*/
		break;
	}
}
/*******************************************************************************
 ** Service Name     : SendStreamDATA	                                       **
 **                                                                            **
 ** Syntax           : void SendStreamDATA(void)			                   **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none			                                           **
 **                                                                            **
 ** Description 	 : This procedure is used to send data streams to GUI for  **
 ** 					  plotting. 										   **
 **                                                                            **
 *******************************************************************************/
void SendStreamDATA() {
	if (1U == MSS_USBD_HID_tx_done()) {

		g_report.A = (uint8_t) 0x0EU;
		if (g_rx_data[1] == 0u)/*BLDC - Graph variables for display*/
		{
			g_report.B
					= (uint8_t) (((uint32_t) g_mc_bldc_foc.mc_graph.variable1)
							>> 8U);
			g_report.C = (uint8_t) (g_mc_bldc_foc.mc_graph.variable1);
			g_report.D
					= (uint8_t) (((uint32_t) g_mc_bldc_foc.mc_graph.variable2)
							>> 8U);
			g_report.E = (uint8_t) ((g_mc_bldc_foc.mc_graph.variable2));
			g_report.F
					= (uint8_t) (((uint32_t) g_mc_bldc_foc.mc_graph.variable3)
							>> 8U);
			g_report.G = (uint8_t) ((g_mc_bldc_foc.mc_graph.variable3));
			g_report.H = (uint8_t) ((uint32_t) g_mc_bldc_foc.mc_graph.variable4
					>> 8U);
			g_report.I = (uint8_t) (g_mc_bldc_foc.mc_graph.variable4);
			MC_BLDCGetSpdI();
			g_report.J = (uint8_t) (((uint32_t) g_mc_bldc_foc.mc_graph.rpm / 2)
					>> 8U);
			g_report.K = (uint8_t) ((g_mc_bldc_foc.mc_graph.rpm / 2));
			g_report.L = (uint8_t) ((g_mc_bldc_foc.mc_graph.current) >> 8U);
			g_report.M = (uint8_t) ((g_mc_bldc_foc.mc_graph.current));
			g_base_addr = (addr_t) BASE_ADDR_0;
			g_report.N = MC_GetStatus();
		} else /*stepper - Graph variables for display*/
		{
			g_report.B = (uint8_t) (((uint32_t) g_stepper.mc_graph.variable1)
					>> 8U);
			g_report.C = (uint8_t) (g_stepper.mc_graph.variable1);
			g_report.D = (uint8_t) (((uint32_t) g_stepper.mc_graph.variable2)
					>> 8U);
			g_report.E = (uint8_t) ((g_stepper.mc_graph.variable2));
			g_report.F = (uint8_t) (((uint32_t) g_stepper.mc_graph.variable3)
					>> 8U);
			g_report.G = (uint8_t) ((g_stepper.mc_graph.variable3));
			g_report.H = (uint8_t) ((uint32_t) g_stepper.mc_graph.variable4
					>> 8U);
			g_report.I = (uint8_t) (g_stepper.mc_graph.variable4);
			g_report.J = (uint8_t) ((uint16_t) g_stepper.speed_rpm / 2 >> 8U);
			g_report.K = (uint8_t) g_stepper.speed_rpm / 2;
			MC_StepperGetI();
			g_report.L = (uint8_t) ((g_stepper.mc_graph.current) >> 8U);
			g_report.M = (uint8_t) ((g_stepper.mc_graph.current));
			g_base_addr = (addr_t) BASE_ADDR_1;
			g_report.N = MC_GetStatus();
		}

		/*lint -e534*/
		/* 534: Warning on ignoring Return value */
		MSS_USBD_HID_tx_report(&(g_report.A), sizeof(g_report));
		/*lint +e534*/
	}

}

