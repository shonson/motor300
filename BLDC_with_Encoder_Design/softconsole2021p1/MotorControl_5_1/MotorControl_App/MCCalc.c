/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : MCCalc.c                                               	   **
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
#include "MCCalc.h"

mc_bldc_foc_type1 g_mc_bldc_foc;
bldc_motor_type g_bldc1;
stepper_type g_stepper;
uint32_t g_adc_scale;
uint32_t g_pwm_period;
uint32_t g_adc_scale_st;
uint32_t g_pwm_period_st;
uint32_t g_theta_factor;
uint32_t g_pwm_gain;
uint32_t g_speed_descale;
uint32_t g_speed_scale;
uint32_t g_rate_count;
uint32_t g_slew_count;
uint32_t g_slew_count_st;
uint32_t g_rate_count_st;
uint8_t g_algorithm;
uint32_t g_LX_PWM_freq_kHz;
uint8_t g_hall_sensor_pol;
#if(ALGORITHM_SELECTION == SENSOR_LESS )
uint32_t g_resistance_s_pu;
uint32_t g_l_s_pu_by_ts;
#endif
#if(ALGORITHM_SELECTION == HALL_INTERFACE )
//uint8_t g_hall_sensor_pol;
uint32_t g_hall_speed_factor;
#endif
#if(ALGORITHM_SELECTION == ENCODER)
uint32_t g_en_angle_factor;
uint32_t g_en_angle_cnt_max;
uint32_t g_en_speed_factor;
uint32_t g_en_speed_window;
#endif
#if(ALGORITHM_SELECTION == RESOLVER)
uint32_t g_pll_pi_kp;
uint32_t g_pll_pi_ki;
uint32_t theta_factor;
uint32_t g_hf_sig_period;
uint8_t  g_dc_filter_factor;
uint8_t  g_ac_filter_factor;
uint8_t  g_pp_ratio;
#endif
/*******************************************************************************
 ** Service Name     :MC_StepperInit                                           **
 **                                                                            **
 ** Syntax           :void MC_StepperInit()                                    **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This function initializes the stepper parameters.       **
 *******************************************************************************/
void MC_StepperInit() {
	g_stepper.microstep_res = MICRO_STEPS_ST;
	g_stepper.speed_rpm = RESET_SPEED_ST;
	g_stepper.cmd_steps = (int32_t) CMD_STEPS_ST;
	g_stepper.step_num = MOTOR_STEP_NUM_ST;
	g_stepper.i_kp_gain = I_PI_PGAIN_ST;
	g_stepper.i_ki_gain = I_PI_IGAIN_ST;
	g_stepper.mode = (int8_t) INIT_MODE_ST;
	g_stepper.direction = -1;
	g_stepper.run_status = 0;
	g_stepper.i_ref = (int32_t) ID_REF_VAL_ST;

}
/*******************************************************************************
 ** Service Name     :MC_StepperConstCal                                       **
 **                                                                            **
 ** Syntax           :void MC_StepperConstCal()                                **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This function calculates constants related to the 	   **
 ** 				   Stepper motor.										   **
 *******************************************************************************/
void MC_StepperConstCal() {

	int32_t temp_int32;
	/*Stepper*/
	g_adc_scale_st
			= (uint32_t) ((((uint32_t) ADC_REF_VOLT * (uint32_t) 256000u)
					/ ((uint32_t) SHUNT_RES * (uint32_t) CURR_GAIN
							* MOTOR_CURRENT_ST)) << (16u - ADC_RES));
	g_pwm_period_st = ((uint16_t) SYS_CLK * (uint16_t) 500)
			/ (uint16_t) SWITCHING_FREQ_ST;
	g_rate_count_st = (uint32_t) ANGLE_MAX
			/ ((uint32_t) g_stepper.microstep_res * (uint32_t) 4);
	temp_int32 = (int32_t) g_stepper.speed_rpm
			* ((int32_t) g_stepper.microstep_res * (int32_t) 4)
			* (int32_t) g_stepper.step_num;
	g_slew_count_st = ((uint32_t) 240 * (uint32_t) 1000000)
			/ (uint32_t) (temp_int32);
	g_stepper.seq_controller_config = (C_SOFT_STOP_EN_ST << 3) + (C_BY_F_VAL_ST
			<< 2) + (C_SENSOR_CALIB_EN_ST << 1) + (C_AUTO_RESTART_EN_ST);
}
/*******************************************************************************
 ** Service Name     :MC_BLDCInit                                              **
 **                                                                            **
 ** Syntax           :void MC_BLDCInit()                                       **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This function initializes the BLDC parameters.          **
 *******************************************************************************/
void MC_BLDCInit() {
	g_mc_bldc_foc.speed_kp_gain = (uint32_t) SPEED_PI_PGAIN;
	g_mc_bldc_foc.speed_ki_gain = (uint32_t) SPEED_PI_IGAIN;
	g_mc_bldc_foc.i_kp_gain = (uint32_t) I_PI_PGAIN;
	g_mc_bldc_foc.i_ki_gain = (uint32_t) I_PI_IGAIN;
	g_mc_bldc_foc.speed_ref = (int32_t) RESET_SPEED;
	g_mc_bldc_foc.speed_ramp = (int32_t) MOTOR_INIT_RAMP;
	g_mc_bldc_foc.run_status = (int8_t) 0;
	g_mc_bldc_foc.direction = (int8_t) 1;
	g_mc_bldc_foc.open_loop_current = (uint16_t) C_OLMNG_IQ_REF_VAL;
	g_mc_bldc_foc.open_loop_voltage = (uint16_t) C_OLMNG_DV_VAL;
	g_mc_bldc_foc.closed_loop_speed = (uint16_t) C_SEQ_CNTL_CL_OMEGA_VAL;
	g_bldc1.Ls_uhenry = (uint32_t) MOTOR_LS;
	g_bldc1.Npp = (uint32_t) MOTOR_NPP;
	g_bldc1.Rs_mohm = (uint32_t) MOTOR_RS;
	g_bldc1.current_mA = (uint16_t) MOTOR_CURRENT;
	g_bldc1.dc_voltage_mV = (uint16_t) DC_VOLTS;
	g_bldc1.speed_RPM = (uint32_t) MOTOR_RPM;
	g_bldc1.switching_freq_kHz = (uint32_t) SWITCHING_FREQ;
	g_mc_bldc_foc.seq_controller_config = 0u;
	g_mc_bldc_foc.seq_controller_config = (C_SOFT_STOP_EN << 3)
			+ (C_SEQ_CNTL_C_BY_F_VAL << 2) + (C_SENSOR_CALIB_EN << 1)
			+ (C_AUTO_RESTART_EN);
#if(ALGORITHM_SELECTION == HALL_INTERFACE )
    g_hall_sensor_pol = C_HALL_SENSOR_POLARITY;
	g_algorithm = 2u;
#endif
#if(ALGORITHM_SELECTION == SENSOR_LESS)
	g_mc_bldc_foc.angle_kp_gain = (uint32_t) ANGLECORRECTION_PI_PGAIN;
	g_mc_bldc_foc.angle_ki_gain = (uint32_t) ANGLECORRECTION_PI_IGAIN;
	g_mc_bldc_foc.encoder_resolution = 0u;
	g_algorithm = 0u;
#endif
#if(ALGORITHM_SELECTION == ENCODER)
	g_mc_bldc_foc.encoder_resolution = ENCODER_RESOLUTION;
	g_mc_bldc_foc.angle_kp_gain = POSITION_PI_KP;
	g_mc_bldc_foc.angle_ki_gain = POSITION_PI_KI;
	g_mc_bldc_foc.mode = (!POSITION_PI_EN);
	g_algorithm = 1u;
#endif
#if (ALGORITHM_SELECTION == RESOLVER)
	g_mc_bldc_foc.angle_kp_gain = C_PLL_PI_KP_RI;
	g_mc_bldc_foc.angle_ki_gain = C_PLL_PI_KI_RI;
	g_theta_factor = C_THETA_FACTOR_RI;
	g_hf_sig_period = C_HF_SIG_PERIOD_RI;
	g_dc_filter_factor = C_DC_FILTER_FACTOR_RI;
	g_ac_filter_factor = C_AC_FILTER_FACTOR_RI;
	g_pp_ratio = C_PP_RATIO_RI;
#endif
	return;
}
/*******************************************************************************
 ** Service Name     :MC_BLDCConstCal                                          **
 **                                                                            **
 ** Syntax           :void MC_BLDCConstCal()                                   **
 **                                                                            **
 ** Parameters(in)   : none                                                    **
 **                                                                            **
 ** Parameters (out) : none                                                    **
 **                                                                            **
 ** Return value     : none                                                    **
 **                                                                            **
 ** Description 	 : This function calculates constants related to the BLDC  **
 ** 				   motor.												   **
 *******************************************************************************/
void MC_BLDCConstCal() {
	uint16_t Zbase;
	uint16_t Motor_freq;
	uint32_t temp_uint32;
	g_adc_scale
			= (uint32_t) ((((uint32_t) ADC_REF_VOLT * (uint32_t) 256000u)
					/ ((uint32_t) SHUNT_RES * (uint32_t) CURR_GAIN
							* g_bldc1.current_mA)) << (16u - ADC_RES));
	g_pwm_period = ((uint16_t) SYS_CLK * (uint16_t) 500u)
			/ (uint16_t) g_bldc1.switching_freq_kHz;
	g_pwm_gain = ((uint32_t) g_pwm_period * (uint32_t) 1182u)
			/ (uint32_t) 1024u;
	Zbase = ((uint32_t) g_bldc1.dc_voltage_mV * (uint16_t) 577u)
			/ ((uint16_t) g_bldc1.current_mA);
	Motor_freq = ((uint32_t) g_bldc1.speed_RPM * (uint16_t) g_bldc1.Npp)
			/ (uint16_t) 60u;
	if (g_pwm_period > 267)
		g_theta_factor = ((uint32_t) Motor_freq * (uint32_t) 134u)
				/ ((uint32_t) g_bldc1.switching_freq_kHz * (uint32_t) 4u);
	else if (g_pwm_period > 133)
		g_theta_factor = ((uint32_t) Motor_freq * (uint32_t) 268u)
				/ ((uint32_t) g_bldc1.switching_freq_kHz * (uint32_t) 4u);
	else
		g_theta_factor = ((uint32_t) Motor_freq * (uint32_t) 402u)
				/ ((uint32_t) g_bldc1.switching_freq_kHz * (uint32_t) 4u);
	temp_uint32 = (((uint32_t) 65536u * (uint32_t) 1024u))
			/ (uint32_t) g_bldc1.speed_RPM;
	g_speed_scale = (uint16_t) temp_uint32;
	g_speed_descale = ((uint32_t) g_bldc1.speed_RPM) / (uint16_t) 16u;
	g_slew_count = (uint16_t) RAMP_TI * (uint16_t) g_bldc1.switching_freq_kHz;
	temp_uint32 = ((uint32_t) 67110u * (uint32_t) RAMP_TI)
			/ (uint32_t) g_bldc1.speed_RPM;
	g_rate_count = (uint16_t) temp_uint32;
#if(ALGORITHM_SELECTION == HALL_INTERFACE )
    //g_hall_speed_factor = (384u * (uint32_t)(g_bldc1.speed_RPM) * (uint32_t)(g_bldc1.Npp))/(1600u);
	g_hall_speed_factor = (376u * (uint32_t)(g_bldc1.speed_RPM))/(100u * (uint32_t)(g_bldc1.Npp));
#endif
#if (ALGORITHM_SELECTION == SENSOR_LESS)
	g_resistance_s_pu = ((uint32_t) g_bldc1.Rs_mohm * (uint32_t) 512u)
			/ (uint32_t) Zbase;
	g_l_s_pu_by_ts = ((uint32_t) g_bldc1.Ls_uhenry
			* (uint32_t) g_bldc1.switching_freq_kHz * (uint32_t) 512u)
			/ (uint32_t) Zbase;
#endif
#if(ALGORITHM_SELECTION == ENCODER)
		/*Encoder*/
		temp_uint32 = ((uint32_t)ENCODER_SPDWIN *
				(uint32_t)g_bldc1.speed_RPM * (uint32_t)g_mc_bldc_foc.encoder_resolution)/60u;
		g_en_speed_factor = (((uint32_t)(1u << 22u)* 1000u)/ temp_uint32) ;
		g_en_angle_factor = (256 * ANGLE_MAX * g_bldc1.Npp) / (g_mc_bldc_foc.encoder_resolution * 4u);
		g_en_angle_cnt_max = (4 * g_mc_bldc_foc.encoder_resolution) - 1;
		g_en_speed_window = ENCODER_SPDWIN * 100;
#endif
}
