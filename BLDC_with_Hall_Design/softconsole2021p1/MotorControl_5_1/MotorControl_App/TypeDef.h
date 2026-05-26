/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : TypeDef.h                                               	   **
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
 **                 - macros and type definitions used in this project         **
 **                                                                            **
 *******************************************************************************/

#ifndef TYPEDEF_H
#define TYPEDEF_H
/******************************************************************************
 **                      Includes                                             **
 ******************************************************************************/

#include <stdint.h>
#include "mss_gpio.h"


/******************************************************************************
 **                      Global Type Definitions                              **
 ******************************************************************************/
/*Type definition of user defined types */

typedef struct mc_graph_type1 mc_graph_type;
struct mc_graph_type1{
	int32_t variable1;
	int32_t variable2;
	int32_t variable3;
	int32_t variable4;
	int32_t rpm;
	int32_t current;

};

typedef struct MC_BLDC_FOC_type1 mc_bldc_foc_type1;
struct MC_BLDC_FOC_type1{
	int32_t speed_ramp;
	int32_t speed_ref;
	uint32_t speed_kp_gain;
	uint32_t speed_ki_gain;
	uint32_t i_kp_gain;
	uint32_t i_ki_gain;
	uint32_t angle_kp_gain;
	uint32_t angle_ki_gain;
	uint16_t open_loop_voltage;
	uint16_t open_loop_current;
	uint16_t closed_loop_speed;
	int8_t direction;
	int8_t run_status;
	uint8_t seq_controller_config;
	uint16_t encoder_resolution;
	uint16_t position_kp;
	uint16_t position_ki;
	int8_t mode;
	mc_graph_type mc_graph;

};

typedef struct stepper_type1 stepper_type;
struct stepper_type1{

	int8_t mode;
	int8_t direction;
	int8_t run_status;
	int16_t microstep_res;
	int16_t speed_rpm;
	int16_t step_num;
	int32_t cmd_steps;
	int32_t i_ref;
	uint32_t i_kp_gain;
	uint32_t i_ki_gain;
	uint8_t seq_controller_config;
	mc_graph_type mc_graph;

} ;


typedef struct bldc_motor1 bldc_motor_type;
struct bldc_motor1{
	uint32_t speed_RPM;
	uint32_t Npp;
	uint32_t Rs_mohm;
	uint32_t Ls_uhenry;
	uint32_t switching_freq_kHz;
	uint16_t dc_voltage_mV;
	uint16_t current_mA;
};

#endif
