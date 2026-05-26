/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : MCFPGAIF.c                                               	   **
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
 **                 - function definitions of Application driver               **
 **                                                                            **
 *******************************************************************************/

/*******************************************************************************
 **                      Includes                                              **
 *******************************************************************************/
#include "MCFPGAIF.h"

/*******************************************************************************
 **                      Global Macro Definitions                              **
 *******************************************************************************/

/*******************************************************************************
 **                      Global Type Definitions                               **
 *******************************************************************************/

/*******************************************************************************
 **                      Global Constant Declarations                          **
 *******************************************************************************/

/*******************************************************************************
 **                      Global Variable Declarations                          **
 *******************************************************************************/

addr_t g_base_addr;
int32_t speedbuf;
int32_t currentbuf;

/*******************************************************************************
 ** Service Name     : abs_function	                                          **
 **                                                                            **
 ** Syntax           : int32_t abs_function(int32_t data)                      **
 **                                                                            **
 ** Parameters(in)   : data                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : absolute value                                          **
 **                                                                            **
 ** Description 		: This API is used to obtain the absolute value of the **
 ** 					  data input.										   **
 **                                                                            **
 *******************************************************************************/
static int32_t abs_function(int32_t data);
/*******************************************************************************
 **                      Global Inline Function Definitions                    **
 *******************************************************************************/

/*******************************************************************************
 **                      Global function Definitions                           **
 *******************************************************************************/
/*******************************************************************************
 ** Service Name     :MotorControl_Init                                        **
 **                                                                            **
 ** Syntax           :void MotorControl_Init()                                 **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This function initializes the motor control modules in  **
 ** 					fabric.											       **
 *******************************************************************************/
void MotorControl_Init() {
	MC_BLDCInit();
	MC_BLDCConstCal();
	MC_StepperInit();
	MC_StepperConstCal();
	MC_BLDCSetFabReg();
	MC_StepperSetFabReg();
	return;
}

/*******************************************************************************
 ** Service Name     : MotorControl_Start                                 	   **
 **                                                                            **
 ** Syntax           : void MotorControl_Start()                          	   **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to Start the Motor	                   **
 **                                                                            **
 *******************************************************************************/
void MotorControl_Start() {

	HW_set_32bit_reg((g_base_addr + (addr_t) C_STOP_MOTOR_ADDR), (uint32_t) 0u);
	asm("NOP\n");
	asm("NOP\n");
	asm("NOP\n");
	HW_set_32bit_reg((g_base_addr + (addr_t) C_START_MOTOR_ADDR), (0x00000001U));

}
/*******************************************************************************
 ** Service Name     : MotorControl_Stop                                 	   **
 **                                                                            **
 ** Syntax           : void MotorControl_Stop()                          	   **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to Stop the Motor	                   **
 **                                                                            **
 *******************************************************************************/
void MotorControl_Stop() {

	/*lint -e950 -e960*/
	/*950: Non-ANSI reserved word or construct		**
	 * 		The asm keyword has been used to create exactly one delay
	 *960: Violates MISRA 2004 Required Rule 14.3, null statement not
	 in line by itself
	 The asm statement is detected as a null statement by PC-Lint.
	 */

	HW_set_32bit_reg((g_base_addr + (addr_t) C_START_MOTOR_ADDR), (uint32_t) 0u);
	asm("NOP\n");
	asm("NOP\n");
	asm("NOP\n");
	HW_set_32bit_reg((g_base_addr + (addr_t) C_STOP_MOTOR_ADDR), (uint32_t) 1u);
	speedbuf = 0;
	currentbuf = 0;
	/*lint +e950*/
}
/*******************************************************************************
 ** Service Name     : MC_WClearFault	                                 	   **
 **                                                                            **
 ** Syntax           : void MC_WClearFault()						   		   **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to Clear fault in clear fault register.**
 **                                                                            **
 *******************************************************************************/
void MC_WClearFault() {
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CLR_FAULT_ADDR),
			(uint32_t) 1u);
	asm("nop\n");
	asm("nop\n");
	asm("nop\n");
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CLR_FAULT_ADDR),
			(uint32_t) 0u);
}
/*******************************************************************************
 ** Service Name     : MC_GetStatus	         								   **
 **                                                                            **
 ** Syntax           : void MC_GetStatus()					   		   		   **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to get the FSM status from fabric.	   **
 **                                                                            **
 *******************************************************************************/
uint8_t MC_GetStatus() {
	uint8_t temp;
	temp = HW_get_32bit_reg(g_base_addr + (addr_t) C_MC_FSM_STATE_ADDR);
	return temp;
}
/*******************************************************************************
 ** Service Name     : MC_BLDCSetDirn	                                 	   **
 **                                                                            **
 ** Syntax           : void MC_BLDCSetDirn(dirn)					   		   **
 **                                                                            **
 ** Parameters(in)   : direction                                               **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to set direction in the direction	   **
 ** 				   in the direction configuration register.				   **
 **                                                                            **
 *******************************************************************************/
void MC_BLDCSetDirn(uint32_t dirn) {
	g_base_addr = (addr_t) BASE_ADDR_0;
	HW_set_32bit_reg((g_base_addr + (addr_t) C_DIRECTION_CONFIG_ADDR), dirn);
}

/*******************************************************************************
 ** Service Name     : MC_BLDCGetSpdI	                                 	   **
 **                                                                            **
 ** Syntax           : void MC_BLDCGetSpdI()					   		   	   **
 **                                                                            **
 ** Parameters(in)   : none                                               	   **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to get motor speed and current from    **
 ** 				   fabric.  											   **
 **                                                                            **
 *******************************************************************************/
void MC_BLDCGetSpdI() {
	int32_t diff;
	int32_t y_scl;
	int32_t result;
	uint32_t temp_uint32;
	g_base_addr = (addr_t) BASE_ADDR_0;
	temp_uint32 = HW_get_32bit_reg(g_base_addr
			+ (addr_t) C_OMEGA_FILTER_OUT_ADDR);
	g_mc_bldc_foc.mc_graph.rpm = (int32_t) temp_uint32;
	g_mc_bldc_foc.mc_graph.rpm = abs_function(g_mc_bldc_foc.mc_graph.rpm);
	g_mc_bldc_foc.mc_graph.rpm *= (uint32_t) g_speed_descale;
	g_mc_bldc_foc.mc_graph.rpm /= (uint32_t) 4096u;
	diff = ((g_mc_bldc_foc.mc_graph.rpm * 1024) - speedbuf) / (1024);
	y_scl = speedbuf + diff;
	speedbuf = y_scl;
	result = y_scl / 1024;
	g_mc_bldc_foc.mc_graph.rpm = result;
	g_mc_bldc_foc.mc_graph.current = HW_get_32bit_reg(g_base_addr
			+ (addr_t) C_IQ_PARK_ADDR);
	g_mc_bldc_foc.mc_graph.current = ((uint32_t) abs_function(
			(int32_t) g_mc_bldc_foc.mc_graph.current)
			* (uint32_t) MOTOR_CURRENT) / (uint32_t) 65535u;
	//* 176u) / 1024u;
	diff = ((g_mc_bldc_foc.mc_graph.current * 1024) - currentbuf) / (1024);
	y_scl = currentbuf + diff;
	currentbuf = y_scl;
	result = y_scl / 1024;
	g_mc_bldc_foc.mc_graph.current = result;
}

/*******************************************************************************
 ** Service Name     : MC_BLDCSetFabReg	                                 	   **
 **                                                                            **
 ** Syntax           : void MC_BLDCSetFabReg()					   		   	   **
 **                                                                            **
 ** Parameters(in)   : none                                               	   **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to set fabric registers corresponding  **
 ** 				   to the BLDC motor.   								   **
 **                                                                            **
 *******************************************************************************/
void MC_BLDCSetFabReg() {
	uint32_t temp_scl;
	g_base_addr = (addr_t) BASE_ADDR_0;
	HW_set_32bit_reg((g_base_addr + (addr_t) C_SPEED_PI_KP_ADDR),
			(uint32_t) g_mc_bldc_foc.speed_kp_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_SPEED_PI_KI_ADDR),
			(uint32_t) g_mc_bldc_foc.speed_ki_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ID_PI_KP_ADDR),
			(uint32_t) g_mc_bldc_foc.i_kp_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ID_PI_KI_ADDR),
			(uint32_t) g_mc_bldc_foc.i_ki_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_IQ_PI_KP_ADDR),
			(uint32_t) g_mc_bldc_foc.i_kp_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_IQ_PI_KI_ADDR),
			(uint32_t) g_mc_bldc_foc.i_ki_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_DIRECTION_CONFIG_ADDR),
			(uint32_t) ((int32_t) g_mc_bldc_foc.direction));
	HW_set_32bit_reg((g_base_addr + (addr_t) C_RATE_LIMIT_SLEW_CNT_ADDR),
			(uint32_t) g_slew_count);
	temp_scl = (uint32_t) g_mc_bldc_foc.speed_ramp;
	temp_scl *= g_rate_count;
	temp_scl /= TWO_POWER_10;
	HW_set_32bit_reg((g_base_addr + (addr_t) C_RATE_LIMIT_RATE_CNT_ADDR),
			temp_scl);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_OLMNG_THETA_FACTOR_ADDR),
			g_theta_factor);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_SEQ_CNTL_CONFIG_ADDR),
			g_mc_bldc_foc.seq_controller_config);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_OLMNG_DV_ADDR),
			(uint32_t) (((uint32_t) g_mc_bldc_foc.open_loop_voltage
					* (uint32_t) 65535u) / 100u));
	HW_set_32bit_reg((g_base_addr + (addr_t) C_OLMNG_IQ_REF_ADDR),
			((uint32_t) g_mc_bldc_foc.open_loop_current * (uint32_t) 65535u)
					/ 100u);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_SEQ_CNTL_CL_OMEGA_ADDR),
			((g_mc_bldc_foc.closed_loop_speed * 65535u / g_bldc1.speed_RPM)));
	HW_set_32bit_reg((g_base_addr + (addr_t) C_AUTO_RESTART_NO_ADDR),
			C_AUTO_RESTART_NUMBER);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ADC_SCALE_ADDR), g_adc_scale);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_OVER_CURR_THRESH_ADDR),
			OVER_CURRENT_THRESHOLD);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_PWM_PERIOD_VAL_ADDR),
			g_pwm_period);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ADC_CONFIG_ADDR),
			ADC_CONFIG_DUAL_TRIG);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_PWM_DEAD_TIME_ADDR),
			PWM_DEAD_TIME);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_PWM_DELAY_TIME_ADDR),
			PWM_DELAY_TIME);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_PWM_GAIN_ADDR), g_pwm_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CH0_CONFIG_ADDR),
			C_CH0_CONFIG_VAL);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CH1_CONFIG_ADDR),
			C_CH1_CONFIG_VAL);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CH2_CONFIG_ADDR),
			C_CH2_CONFIG_VAL);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CH3_CONFIG_ADDR),
			C_CH3_CONFIG_VAL);

#if (ALGORITHM_SELECTION == SENSOR_LESS)
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ANGLE_KP_ADDR),
			(uint32_t) g_mc_bldc_foc.angle_kp_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ANGLE_KI_ADDR),
			(uint32_t) g_mc_bldc_foc.angle_ki_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ANGLE_LB_FIRST_CONST_ADDR),
			g_l_s_pu_by_ts);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ANGLE_LB_SECOND_CONST_ADDR),
			g_resistance_s_pu);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_FILTER_FACTOR_BEMF_ADDR),
			C_FILTER_FACTOR_BEMF_VAL);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_FILTER_FACTOR_OMEGA_ADDR),
			C_FILTER_FACTOR_OMEGA_VAL);
#endif
#if (ALGORITHM_SELECTION == ENCODER)
	HW_set_32bit_reg((g_base_addr + (addr_t) C_EN_SPEED_FACTOR_ADDR),
			g_en_speed_factor);

	HW_set_32bit_reg((g_base_addr + (addr_t) C_EN_ANGLE_FACTOR_ADDR),
			(uint32_t)g_en_angle_factor);

	HW_set_32bit_reg((g_base_addr + (addr_t) C_EN_ANGLE_CNT_MAX_ADDR),
			g_en_angle_cnt_max);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_EN_SPD_FILTER_FACTOR_ADDR),
			C_FILTER_FACTOR_OMEGA_VAL);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_EN_SPEED_WINDOW_ADDR),
			g_en_speed_window);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_EN_POSITION_PI_KP),
			g_mc_bldc_foc.angle_kp_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_EN_POSITION_PI_KI),
			g_mc_bldc_foc.angle_ki_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_EN_POSITION_PI_EN),
			g_mc_bldc_foc.mode);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_EN_POSITION_COUNT),
			g_mc_bldc_foc.speed_ref);
#endif
#if (ALGORITHM_SELECTION == RESOLVER)
	HW_set_32bit_reg((g_base_addr + (addr_t) C_PLL_PI_KP_ADDR),
			(uint32_t)g_mc_bldc_foc.angle_kp_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_PLL_PI_KI_ADDR),
			(uint32_t)g_mc_bldc_foc.angle_ki_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_THETA_FACTOR_ADDR),
			(uint32_t)C_THETA_FACTOR_RI);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_HF_SIG_PERIOD_ADDR),
			(uint32_t)C_HF_SIG_PERIOD_RI);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_DC_FILTER_FACTOR_ADDR),
			(uint32_t)C_DC_FILTER_FACTOR_RI);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_AC_FILTER_FACTOR_ADDR),
			(uint32_t)C_AC_FILTER_FACTOR_RI);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_PP_RATIO_ADDR),
			(uint32_t)C_PP_RATIO_RI);
#endif
#if (ALGORITHM_SELECTION == HALL_INTERFACE)
	HW_set_32bit_reg((g_base_addr + (addr_t) C_HALL_SENSOR_POLARITY_ADDR),
			g_hall_sensor_pol);//1000//4754
	HW_set_32bit_reg((g_base_addr + (addr_t) C_HALL_SPEED_FACTOR_MUL_ADDR),
			g_hall_speed_factor);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_HALL_FILTER_FACTOR_ADDR),
			C_FILTER_FACTOR_OMEGA_VAL);
#endif

	temp_scl = (uint32_t) g_mc_bldc_foc.speed_ref;
	temp_scl *= g_speed_scale;
	temp_scl /= TWO_POWER_10;
	HW_set_32bit_reg((g_base_addr + (addr_t) C_RATE_LIMIT_REF_ADDR), temp_scl);
}
/*******************************************************************************
 ** Service Name     : MC_StepperSetDirn                                 	   **
 **                                                                            **
 ** Syntax           : void MC_StepperSetDirn(dirn)					   		   **
 **                                                                            **
 ** Parameters(in)   : direction                                               **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to set the stepper motor direction.    **
 **                                                                            **
 *******************************************************************************/
void MC_StepperSetDirn(int32_t dirn) {

	int32_t temp_int32;
	g_base_addr = (addr_t) BASE_ADDR_1;
	temp_int32 = (g_stepper.cmd_steps * g_stepper.microstep_res
			* (int32_t) dirn);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_THETA_GEN_CMD_STEP_NO_ADDR),
			(uint32_t) temp_int32);
}
/*******************************************************************************
 ** Service Name     : MC_StepperGetI	                                 	   **
 **                                                                            **
 ** Syntax           : void MC_StepperGetI()					   		   	   **
 **                                                                            **
 ** Parameters(in)   : none	                                                   **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to get the stepper motor current.      **
 **                                                                            **
 *******************************************************************************/
void MC_StepperGetI() {
	g_base_addr = BASE_ADDR_1;
	g_stepper.mc_graph.current = ((HW_get_32bit_reg(g_base_addr
			+ (addr_t) C_ID_PARK_ADDR)) * (uint32_t) MOTOR_CURRENT_ST)
			/ (uint32_t) 65535u;
}

/*******************************************************************************
 ** Service Name     : BLDC_SetSpeedSRamp                                	   **
 **                                                                            **
 ** Syntax           : void BLDC_SetSpeedSRamp()				   		   	   **
 **                                                                            **
 ** Parameters(in)   : none                                               	   **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to set the speed and rate count fabric **
 ** 				   registers corresponding to the BLDC motor.			   **
 **                                                                            **
 *******************************************************************************/
void BLDC_SetSpeedSRamp() {
	uint32_t temp_scl;
	g_base_addr = BASE_ADDR_0;
	temp_scl = (uint32_t) g_mc_bldc_foc.speed_ref;
	temp_scl *= (g_speed_scale);
	temp_scl /= (uint32_t) TWO_POWER_10;
	HW_set_32bit_reg((g_base_addr + (addr_t) C_RATE_LIMIT_REF_ADDR), temp_scl);
	temp_scl = (uint32_t) g_mc_bldc_foc.speed_ramp;
	temp_scl *= g_rate_count;
	temp_scl /= (uint32_t) TWO_POWER_10;
	HW_set_32bit_reg((g_base_addr + (addr_t) C_RATE_LIMIT_RATE_CNT_ADDR),
			temp_scl);
}

/*******************************************************************************
 ** Service Name     : MC_StepperSetFabReg                                	   **
 **                                                                            **
 ** Syntax           : void MC_StepperSetFabReg()				   		   	   **
 **                                                                            **
 ** Parameters(in)   : none                                               	   **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This API is used to set fabric registers corresponding  **
 ** 				   to the stepper motor.   								   **
 **                                                                            **
 *******************************************************************************/
void MC_StepperSetFabReg() {
	uint32_t temp_scl;
	g_base_addr = BASE_ADDR_1;
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ID_PI_KP_ADDR),
			(uint32_t) g_stepper.i_kp_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ID_PI_KI_ADDR),
			(uint32_t) g_stepper.i_ki_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_IQ_PI_KP_ADDR),
			(uint32_t) g_stepper.i_kp_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_IQ_PI_KI_ADDR),
			(uint32_t) g_stepper.i_ki_gain);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_SEQ_CNTL_CONFIG_ADDR),
			g_stepper.seq_controller_config);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_OLMNG_DV_ADDR), VD_REF_VAL_ST);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_OLMNG_IQ_REF_ADDR),
			ID_REF_VAL_ST);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ADC_SCALE_ADDR),
			(uint32_t) g_adc_scale_st);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_OVER_CURR_THRESH_ADDR),
			OVER_CURRENT_THRESHOLD);
	temp_scl = ((uint32_t) g_stepper.i_ref * 65535u) / MOTOR_CURRENT_ST;
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ID_REF_ADDR), temp_scl);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_PWM_PERIOD_VAL_ADDR),
			(uint32_t) g_pwm_period_st);
	/*lint -e835*/
	/*Suppressing Note 835 - C_ADC_CONFIG_ADDR is equal to 0,
	 * but refers to an address.*/
	HW_set_32bit_reg((g_base_addr + (addr_t) C_ADC_CONFIG_ADDR), ADC_CONFIG_ST);
	/*lint +e835*/

	HW_set_32bit_reg((g_base_addr + (addr_t) C_PWM_DEAD_TIME_ADDR),
			PWM_DEAD_TIME_ST);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_PWM_DELAY_TIME_ADDR),
			PWM_DELAY_TIME_ST);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CH0_CONFIG_ADDR),
			C_CH0_CONFIG_VAL);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CH1_CONFIG_ADDR),
			C_CH2_CONFIG_VAL);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CH2_CONFIG_ADDR),
			C_CH1_CONFIG_VAL);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_CH3_CONFIG_ADDR),
			C_CH3_CONFIG_VAL);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_THETA_GEN_SLEW_COUNT_ADDR),
			g_slew_count_st);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_THETA_GEN_RATE_LIMIT_ADDR),
			(uint32_t) g_rate_count_st);
	HW_set_32bit_reg((g_base_addr + (addr_t) C_THETA_GEN_CMD_STEP_NO_ADDR),
			(uint32_t) (g_stepper.cmd_steps * g_stepper.microstep_res
					* (int32_t) g_stepper.direction));
	HW_set_32bit_reg((g_base_addr + (addr_t) C_THETA_GEN_MODE_ADDR),
			(uint32_t) ((int32_t) g_stepper.mode));
}
/*******************************************************************************
 ** Service Name     : abs_function	                                           **
 **                                                                            **
 ** Syntax           : int32_t abs_function(int32_t data)                      **
 **                                                                            **
 ** Parameters(in)   : data                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : absolute value                                          **
 **                                                                            **
 ** Description 		: This API is used to obtain the absolute value of the **
 ** 					  data input.										   **
 **                                                                            **
 *******************************************************************************/
static int32_t abs_function(int32_t data) {
	int32_t t;
	if (data < 0) {
		t = -(data);
	} else {
		t = data;
	}
	return (t);
}
