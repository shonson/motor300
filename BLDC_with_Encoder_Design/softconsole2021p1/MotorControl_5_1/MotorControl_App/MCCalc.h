/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : MCCalc.h                                               	   **
 **                                                                            **
 **  PLATFORM  : SmartFusion2                                                  **
 **                                                                            **
 **  COMPILER  : Softconsole                                                   **
 **                                                                            **
 **  AUTHOR    : India Solutions Team			                               **
 **                                                                            **
 **  VENDOR    : Microsemi                                                     **
 **                                                                            **
 **  DESCRIPTION  : This header file includes                                  **
 **                 - function declarations of Application drivers             **
 **                                                                            **
 *******************************************************************************/

#ifndef MCCALC_H_
#define MCCALC_H_

#include "config.h"
#include "TypeDef.h"

extern mc_bldc_foc_type1 g_mc_bldc_foc;
extern bldc_motor_type g_bldc1;
extern stepper_type g_stepper;
extern uint32_t g_adc_scale;
extern uint32_t g_pwm_period;
extern uint32_t g_adc_scale_st;
extern uint32_t g_pwm_period_st;
extern uint32_t g_resistance_s_pu;
extern uint32_t g_l_s_pu_by_ts;
extern uint32_t g_theta_factor;
extern uint32_t g_pwm_gain;
extern uint32_t g_speed_descale;
extern uint32_t g_speed_scale;
extern uint32_t g_rate_count;
extern uint32_t g_slew_count;
extern uint32_t g_slew_count_st;
extern uint32_t g_rate_count_st;
extern uint32_t g_en_angle_factor;
extern uint32_t g_en_angle_cnt_max;
extern uint32_t g_en_speed_factor;
extern uint32_t g_en_speed_window;
extern uint8_t g_algorithm;
extern uint32_t g_rs_filter_factor;
extern uint32_t g_rs_angle_factor;
extern uint32_t g_rs_speed_factor;
extern uint8_t g_hall_sensor_pol;
extern uint32_t g_hall_speed_factor;

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
extern void MC_StepperInit(void);

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
extern void MC_BLDCInit(void);

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
extern void MC_BLDCConstCal(void);

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
extern void MC_StepperConstCal(void);


#endif /* MCCALC_H_ */
