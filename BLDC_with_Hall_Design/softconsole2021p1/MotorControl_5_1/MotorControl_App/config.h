/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : config.h                                               	   **
 **                                                                            **
 **  PLATFORM  : SmartFusion2                                                  **
 **                                                                            **
 **  COMPILER  : Softconsole                                                   **
 **                                                                            **
 **  AUTHOR    : India Solutions Team			                               **
 **                                                                            **
 **  VENDOR    : Microsemi                                                     **
 **                                                                            **
 **  DESCRIPTION  :This header file includes                                   **
 **                 - Macros for Configuration parameters of the motor control **
 **                 project.												   **
 **                                                                            **
 *******************************************************************************/


#ifndef CONFIG_H_
#define CONFIG_H_
/*===========================================================================*/
/*=========================Motor Selection===================================*/
/*===========================================================================*/
#define QMOT_MOTOR  								1
#define PARKER_MOTOR 							    2
#define ULTRA_HIGH_SPEED_MOTOR 						3
#define INDUCTION_MOTOR		 						4
/*Replace the motor selection value to the motor of your choice. This will
 *select only those parameters which are necessary for this motor.*/
#define MOTOR_SELECTION QMOT_MOTOR
/*===========================================================================*/
/*=========================Algorithm Selection===================================*/
/*===========================================================================*/
#define SENSOR_LESS  								1
#define HALL_INTERFACE 							    2
#define ENCODER              						3
#define RESOLVER              						4
/*Replace the algorithm selection value to the algorithm of your choice. This will
 *select only those parameters which are necessary for this algorithm.*/
 #define ALGORITHM_SELECTION HALL_INTERFACE
/*===========================================================================*/

/*================Configuration parameter values===========================*/
#define C_CH0_CONFIG_VAL        					(0x00001840U)
#define C_CH1_CONFIG_VAL        					(0x000018C0U)
#define C_CH2_CONFIG_VAL        					(0x00001940U)
#define C_CH3_CONFIG_VAL        					(0x000019C0U)
/*=========================================================================*/
/*========================Parameters for BLDC==============================*/
/*=========================================================================*/
/*----------------ADC Parameters-----------------*/
#define ADC_CONFIG_DUAL_TRIG                    	(0x00000011U)

/*----------------PWM Parameters-----------------*/
/*PWM_DEAD_TIME is hard programmed in DRV8332 */
#define PWM_DEAD_TIME                           	(1U)
#define PWM_DELAY_TIME                          	(0U)

/*-----------------Rate Limiter------------------*/
/* Time interval in milli seconds for which ramp is updating speed output*/
#define RAMP_TI										(10U)
/*=========================================================================*/
/*======================Parameters for Stepper=============================*/
/*=========================================================================*/
/*----------------ADC Parameters-----------------*/
#define ADC_CONFIG_ST	                        	(0x00000011U)
/*----------------PWM Parameters-----------------*/
/*PWM_DEAD_TIME_ST is hard programmed in DRV8332*/
#define PWM_DEAD_TIME_ST                        	(1U)
#define PWM_DELAY_TIME_ST                       	(0U)

/*=========================================================================*/
/*========================Standard Definitions=============================*/
/*=========================================================================*/
/* System clock in MHz*/
#define SYS_CLK										(100)
/* Threshold current to register overcurrent fault*/
#define OVER_CURRENT_THRESHOLD						(98304U)
/* Number of auto restarts after which a fault is registered*/
#define C_AUTO_RESTART_NUMBER						(4U)
/*Constant value*/
#define TWO_POWER_10         						(1024)
/*Maximum value of angle (corresponding to 360 degrees)*/
#define ANGLE_MAX									(262144U)
#define ADC_RES										(12U)		// ADC bit resolution
#define ADC_REF_VOLT								(5000U)		// ADC reference voltage in mV
#define SHUNT_RES									(20U)		// Current shunt resistor value in mOhm
#define CURR_GAIN									(20U)		// Current shunt amplifier gain

/* c by f value*/
#define C_SEQ_CNTL_C_BY_F_VAL              			(1U)
/* 1 = Enable auto restart;
 * 0 = Disable auto restart*/
 /* Voltage boost in open loop startup; */
/* Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 1	*/
#define C_OLMNG_DV_VAL                  			(5U)

/* DC bus voltage in milli Volts*/
#define DC_VOLTS									(24000U)
/*===========================================================================*/
/*==========================Motor Parameters=================================*/
/*===========================================================================*/

#if (MOTOR_SELECTION == QMOT_MOTOR)
/*===========================================================================*/
/*======================QMOT MOTOR PARAMETERS================================*/
/*===========================================================================*/

/* Motor current in mA*/
#define MOTOR_CURRENT 								(1790U)
/* Motor phase to phase inductance in micro Henry*/
#define MOTOR_LS									(2600U)
/* Motor phase to phase resistance in milli ohms*/
#define MOTOR_RS									(1800U)
/* Motor rated speed in RPM*/
#define MOTOR_RPM									(4000)
/* Motor number of pole pairs*/
#define MOTOR_NPP									(4U)
/* Switching frequency in kHz*/
#define SWITCHING_FREQ								(20)
/*---------------------------------------------------------------------------*
 * Algorithm Selection == Sensor less
 *---------------------------------------------------------------------------*/

#if (ALGORITHM_SELECTION == SENSOR_LESS)
/*---------------------------------------------------------------------------*
 * Initialisation values
 *---------------------------------------------------------------------------*/
/* Speed reference after reset*/
#define RESET_SPEED      							(4000)
/* Initial speed ramp in rpm/sec*/
#define MOTOR_INIT_RAMP								(1000U)
/*---------------------------------------------------------------------------*
 *Speed PI parameters
 *---------------------------------------------------------------------------*/
/* Speed PI proportional gain*/
#define SPEED_PI_PGAIN 								(3000U)
/* Speed PI integral gain*/
#define SPEED_PI_IGAIN 								(100U)

/*---------------------------------------------------------------------------*
 * Id and Iq PI parameters
 *---------------------------------------------------------------------------*/
/* Id and Iq PI proportional gain*/
#define I_PI_PGAIN 									(300U)
/* Id and Iq PI integral gain*/
#define I_PI_IGAIN 									(300U)

/*---------------------------------------------------------------------------*
 * PLL PI parameters
 *---------------------------------------------------------------------------*/
/* PLL PI proportional gain*/
#define ANGLECORRECTION_PI_PGAIN 					(10000U)
/* PLL PI integral gain*/
#define ANGLECORRECTION_PI_IGAIN 					(5000U)

/*---------------------------------------------------------------------------*
 * Filter parameters
 *---------------------------------------------------------------------------*/
/* Back emf filter time constant factor... (2^factor)*Tsamp = time constant*/
#define C_FILTER_FACTOR_BEMF_VAL        			(9U)
/* Speed filter time constant factor... (2^factor)*Tsamp = time constant*/
#define C_FILTER_FACTOR_OMEGA_VAL       			(2U)

/*---------------------------------------------------------------------------*/
/* Motor sequence management parameters*/
/*---------------------------------------------------------------------------*/
/* Closed loop switching speed where 65535 represents rated speed*/
#define C_SEQ_CNTL_CL_OMEGA_VAL            			(700U)
/* 1 = Enable current regulators in open loop;
 * 0 = Disable current regulators in open loop*/
#define C_AUTO_RESTART_EN							(1U)
/* 1 = Enable Soft Stop;
 * 0 = Disable Soft stop*/
#define	C_SOFT_STOP_EN								(0U)
/* 1 = Enable Sensor Calibration;
 * 0 = Disable Sensor Calibration*/
#define	C_SENSOR_CALIB_EN							(0U)
/* Iq reference value in open loop startup, 65535 represents rated current;
 * Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 0*/
#define C_OLMNG_IQ_REF_VAL              			(50U)
/*===========================================================================*/
#endif
/*---------------------------------------------------------------------------*
 * Algorithm Selection == Resolver
 *---------------------------------------------------------------------------*/
 
#if (ALGORITHM_SELECTION == RESOLVER)
/*---------------------------------------------------------------------------*
 * Initialisation values
 *---------------------------------------------------------------------------*/
/* Speed reference after reset*/
#define RESET_SPEED      							(4000)
/* Initial speed ramp in rpm/sec*/
#define MOTOR_INIT_RAMP								(1000U)
/*---------------------------------------------------------------------------*
 *Speed PI parameters
 *---------------------------------------------------------------------------*/
/* Speed PI proportional gain*/
#define SPEED_PI_PGAIN 								(1000U)
/* Speed PI integral gain*/
#define SPEED_PI_IGAIN 								(30U)

/*---------------------------------------------------------------------------*
 * Id and Iq PI parameters
 *---------------------------------------------------------------------------*/
/* Id and Iq PI proportional gain*/
#define I_PI_PGAIN 									(300U)
/* Id and Iq PI integral gain*/
#define I_PI_IGAIN 									(300U)
/*---------------------------------------------------------------------------*
 * Filter parameters
 *---------------------------------------------------------------------------*/
/* Speed filter time constant factor... (2^factor)*Tsamp = time constant*/
#define C_FILTER_FACTOR_OMEGA_VAL       			(2U)

/*---------------------------------------------------------------------------*/
/* Motor sequence management parameters*/
/*---------------------------------------------------------------------------*/
/* Closed loop switching speed where 65535 represents rated speed*/
#define C_SEQ_CNTL_CL_OMEGA_VAL            			(200U)
/* 1 = Enable current regulators in open loop;
 * 0 = Disable current regulators in open loop*/
#define C_AUTO_RESTART_EN							(1U)
/* 1 = Enable Soft Stop;
 * 0 = Disable Soft stop*/
#define	C_SOFT_STOP_EN								(0U)
/* 1 = Enable Sensor Calibration;
 * 0 = Disable Sensor Calibration*/
#define	C_SENSOR_CALIB_EN							(1U)
/* Iq reference value in open loop startup, 65535 represents rated current;
 * Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 0*/
#define C_OLMNG_IQ_REF_VAL              			(50U)
/*===========================================================================*/
/*---------------------------------------------------------------------------*/
/* Parameters required for resolver interface*/
/*---------------------------------------------------------------------------*/
/* KP value required for resolver PI controller*/
#define C_PLL_PI_KP_RI                    			   (8192U)
/* KI value required for resolver PI controller*/
#define C_PLL_PI_KI_RI                    			   (8192U)
/* Theta factor for resolver interface*/
#define C_THETA_FACTOR_RI							    (725U)
/* High frequency signal period*/
#define	C_HF_SIG_PERIOD_RI								(10000U)
/* DC filter factor*/
#define	C_DC_FILTER_FACTOR_RI							(11U)
/* AC filter factor*/
#define C_AC_FILTER_FACTOR_RI              			    (5U)
/* Pole pair ratio*/
#define C_PP_RATIO_RI                   			    (2U)
#endif

/*---------------------------------------------------------------------------*
 * Algorithm Selection == Hall interface
 *---------------------------------------------------------------------------*/
#if (ALGORITHM_SELECTION == HALL_INTERFACE)
/*---------------------------------------------------------------------------*
 * Initialisation values
 *---------------------------------------------------------------------------*/
/* Speed reference after reset*/
#define RESET_SPEED      							(4000)
/* Initial speed ramp in rpm/sec*/
#define MOTOR_INIT_RAMP								(1000U)
/*---------------------------------------------------------------------------*
 *Speed PI parameters
 *---------------------------------------------------------------------------*/
/* Speed PI proportional gain*/
#define SPEED_PI_PGAIN 								(3000U)
/* Speed PI integral gain*/
#define SPEED_PI_IGAIN 								(50U)

/*---------------------------------------------------------------------------*
 * Id and Iq PI parameters
 *---------------------------------------------------------------------------*/
/* Id and Iq PI proportional gain*/
#define I_PI_PGAIN 									(300U)
/* Id and Iq PI integral gain*/
#define I_PI_IGAIN 									(300U)

/* Speed filter time constant factor... (2^factor)*Tsamp = time constant*/
#define C_FILTER_FACTOR_OMEGA_VAL       			(8U)
/*---------------------------------------------------------------------------*/
/* Motor sequence management parameters*/
/*---------------------------------------------------------------------------*/
/* Closed loop switching speed where 65535 represents rated speed*/
#define C_SEQ_CNTL_CL_OMEGA_VAL            			(200U)
/* 1 = Enable current regulators in open loop;
 * 0 = Disable current regulators in open loop*/
#define C_AUTO_RESTART_EN							(0U)
/* 1 = Enable Soft Stop;
 * 0 = Disable Soft stop*/
#define	C_SOFT_STOP_EN								(0U)
/* 1 = Enable Sensor Calibration;
 * 0 = Disable Sensor Calibration*/
#define	C_SENSOR_CALIB_EN							(1U)
/* Iq reference value in open loop startup, 65535 represents rated current;
 * Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 0*/
#define C_OLMNG_IQ_REF_VAL              			(50U)
/*---------------------------------------------------------------------------*/
#define C_HALL_SENSOR_POLARITY						(1)//1-> ACTIVE HIGH; 0-> ACTIVE LOW
/*===========================================================================*/
#endif

#endif
#if (MOTOR_SELECTION == PARKER_MOTOR)
/*===========================================================================*/
/*ANAHEIM MOTOR PARAMETERS*/
/*===========================================================================*/

/* Motor current in mA*/
#define MOTOR_CURRENT 								(1790U)
/* Motor phase to phase inductance in microhenry*/
#define MOTOR_LS									(2600U)
/* Motor phase to phase resistance in milliohms*/
#define MOTOR_RS									(1800U)
/* Motor rated speed in RPM*/
#define MOTOR_RPM									(4000)
/* Motor number of pole pairs*/
#define MOTOR_NPP									(4U)
/* Switching frequency in kHz*/
#define SWITCHING_FREQ								(20)
/*---------------------------------------------------------------------------*
 * Algorithm Selection == Encoder
 *---------------------------------------------------------------------------*/
 #if (ALGORITHM_SELECTION == ENCODER)
/*---------------------------------------------------------------------------*/
/* Initialisation values*/
/*---------------------------------------------------------------------------*/
/* Speed reference after reset*/
#define RESET_SPEED      							(4000)
/* Initial speed ramp in rpm/sec*/
#define MOTOR_INIT_RAMP								(300U)
/*---------------------------------------------------------------------------*/
/*Speed PI parameters*/
/*---------------------------------------------------------------------------*/
/* Speed PI proportional gain*/
#define SPEED_PI_PGAIN 								(3000)
/* Speed PI integral gain*/
#define SPEED_PI_IGAIN 								(100)
/*---------------------------------------------------------------------------*/
/* Id and Iq PI parameters*/
/*---------------------------------------------------------------------------*/
/* Id and Iq PI proportional gain*/
#define I_PI_PGAIN 									(500U)
/* Id and Iq PI integral gain*/
#define I_PI_IGAIN 									(500U)
/*---------------------------------------------------------------------------*/
/*PLL PI parameters*/
/*---------------------------------------------------------------------------*/
/* PLL PI proportional gain*/
#define ANGLECORRECTION_PI_PGAIN 					(10000U)
/* PLL PI integral gain*/
#define ANGLECORRECTION_PI_IGAIN 					(5000U)
/*---------------------------------------------------------------------------*/
/* Filter parameters*/
/*---------------------------------------------------------------------------*/
/* Back emf filter time constant factor... (2^factor)*Tsamp = time constant*/
#define C_FILTER_FACTOR_BEMF_VAL        			(9U)
/* Speed filter time constant factor... (2^factor)*Tsamp = time constant*/
#define C_FILTER_FACTOR_OMEGA_VAL       			(8U)

/*---------------------------------------------------------------------------*/
/* Motor sequence management parameters*/
/*---------------------------------------------------------------------------*/
/* Closed loop switching speed where 65535 represents rated speed*/
#define C_SEQ_CNTL_CL_OMEGA_VAL            			(0U)
/* 1 = Enable current regulators in open loop;
 * 0 = Disable current regulators in open loop*/
#define C_AUTO_RESTART_EN							(0U)
/* 1 = Enable soft stop;
 * 0 = Disable soft stop*/
#define C_SOFT_STOP_EN								(0U)
/* 1 = Enable initial sensor calibration;
 * 0 = Disable initial sensor calibration*/
#define C_SENSOR_CALIB_EN							(1U)
/* Iq reference value in open loop startup, 65535 represents rated current;
 * Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 0*/
#define C_OLMNG_IQ_REF_VAL              			(20)//(10000U)
/* Voltage boost in open loop startup;
 * Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 1	*/
//===========================================================================
//===========================================================================
/*---------------------------------------------------------------------------*/
/*Encoder Parameters*/
/*---------------------------------------------------------------------------*/
/* Encoder Resolution */
#define ENCODER_RESOLUTION							(2000)
/* Encoder - Number of Edges */
#define ENCODER_EDGES								(4)
/* Encoder - Speed Window in milli seconds*/
#define ENCODER_SPDWIN								(3)

#define POSITION_PI_KP								(5000)
#define POSITION_PI_KI								(1000)
#define POSITION_PI_EN								(1)
#define POSITION_COUNT								(32000)
//===========================================================================
#endif
#endif

#if (MOTOR_SELECTION == ULTRA_HIGH_SPEED_MOTOR)
/*===========================================================================*
 *HIGH SPEED MOTOR PARAMETERS
 *===========================================================================*/

#define MOTOR_CURRENT 								(2640U)		// Motor current in mA
#define MOTOR_LS									(26U)		// Motor phase to phase inductance in micor Henry
#define MOTOR_RS									(513U)		// Motor phase to phase resistance in milli ohms
#define MOTOR_RPM									(70000)		// Motor rated speed in RPM
#define MOTOR_NPP									(1U)		// Motor number of pole pairs
#define SWITCHING_FREQ								(350U)		// Switching frequency in kHz
/*---------------------------------------------------------------------------*
 * Algorithm Selection == SENSOR_LESS
 *---------------------------------------------------------------------------*/
 #if (ALGORITHM_SELECTION == SENSOR_LESS)
/*---------------------------------------------------------------------------*
 * Initialisation values
 *---------------------------------------------------------------------------*/
/* Speed reference after reset*/
#define RESET_SPEED      							(20000)		
/* Initial speed ramp in rpm/sec*/
#define MOTOR_INIT_RAMP								(10000U)	
/*---------------------------------------------------------------------------*
 *Speed PI parameters
 *---------------------------------------------------------------------------*/
/* Speed PI proportional gain*/
#define SPEED_PI_PGAIN 								(2000U)		
/* Speed PI integral gain*/
#define SPEED_PI_IGAIN 								(10U)		
/*---------------------------------------------------------------------------*
 * Id and Iq PI parameters
 *---------------------------------------------------------------------------*/
/* Id and Iq PI proportional gain*/
#define I_PI_PGAIN 									(100U)		
/* Id and Iq PI integral gain*/
#define I_PI_IGAIN 									(100U)		
/*---------------------------------------------------------------------------*
 * PLL PI parameters
 *---------------------------------------------------------------------------*/
/* PLL PI proportional gain*/
#define ANGLECORRECTION_PI_PGAIN 					(5000U)		
/* PLL PI integral gain*/
#define ANGLECORRECTION_PI_IGAIN 					(200U)		
/*---------------------------------------------------------------------------*
 * Filter parameters
 *---------------------------------------------------------------------------*/
/* Back emf filter time constant factor... (2^factor)*Tsamp = time constant*/
#define C_FILTER_FACTOR_BEMF_VAL        			(11U)		
/* Speed filter time constant factor... (2^factor)*Tsamp = time constant*/
#define C_FILTER_FACTOR_OMEGA_VAL       			(10U)		

/*---------------------------------------------------------------------------*/
/* Motor sequence management parameters*/
/*---------------------------------------------------------------------------*/
/* Closed loop switching speed where 65535 represents rated speed*/
#define C_SEQ_CNTL_CL_OMEGA_VAL            			(10000U)
/* 1 = Enable auto restart;
 * 0 = Disable auto restart*/
#define C_AUTO_RESTART_EN							(1U)
/* 1 = Enable Soft Stop;
 * 0 = Disable Soft stop*/
#define	C_SOFT_STOP_EN								(0U)
/* 1 = Enable Sensor Calibration;
 * 0 = Disable Sensor Calibration*/
#define	C_SENSOR_CALIB_EN							(0U)
/* Iq reference value in open loop startup, 65535 represents rated current;
 * Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 0*/
#define C_OLMNG_IQ_REF_VAL              			(50U)
/*===========================================================================*/
#endif
#endif
#if (MOTOR_SELECTION == INDUCTION_MOTOR)
/*===========================================================================*
 *HIGH SPEED MOTOR PARAMETERS
 *===========================================================================*/
#define DC_VOLTS									(24000U)	// DC bus voltage in milli Volts
#define MOTOR_CURRENT 								(1790U)		// Motor current in mA
#define MOTOR_LS									(2600U)		// Motor phase to phase inductance in micor Henry
#define MOTOR_RS									(1800U)		// Motor phase to phase resistance in milli ohms
#define MOTOR_RPM									(4000)		// Motor rated speed in RPM
#define MOTOR_NPP									(2U)		// Motor number of pole pairs
#define SWITCHING_FREQ								(20U)		// Switching frequency in kHz
// Initialisation values
#define RESET_SPEED      							(4000)		// Speed reference after reset
#define MOTOR_INIT_RAMP								(1000U)	// Initial speed ramp in rpm/sec
//Speed PI parameters
#define SPEED_PI_PGAIN 								(3000U)		// Speed PI proportional gain
#define SPEED_PI_IGAIN 								(10U)		// Speed PI integral gain
// Id and Iq PI parameters
#define I_PI_PGAIN 									(300U)		// Id and Iq PI proportional gain
#define I_PI_IGAIN 									(300U)		// Id and Iq PI integral gain
//PLL PI parameters
#define ANGLECORRECTION_PI_PGAIN 					(5000U)		// PLL PI proportional gain
#define ANGLECORRECTION_PI_IGAIN 					(200U)		// PLL PI integral gain
// Filter parameters
#define C_FILTER_FACTOR_BEMF_VAL        			(8U)		// Back emf filter time constant factor... (2^factor)*Tsamp = time constant
#define C_FILTER_FACTOR_OMEGA_VAL       			(7U)		// Speed filter time constant factor... (2^factor)*Tsamp = time constant
// Motor sequence management parameters
#define C_SEQ_CNTL_CL_OMEGA_VAL            			(10000U)	// Closed loop switching speed where 65535 represents rated speed
#define C_SEQ_CNTL_C_BY_F_VAL              			(1U)		// 1 = Enable current regulators in open loop; 0 = Disable current regulators in open loop
#define C_OLMNG_IQ_REF_VAL              			(32000U)	// Iq reference value in open loop startup, 65535 represents rated current; Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 1
#define C_OLMNG_DV_VAL                  			(10000U)	// Voltage boost in open loop startup; Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 0
/*===========================================================================*/
#endif

/*===========================================================================*/
/*STEPPER MOTOR PARAMETERS*/
/*===========================================================================*/
/*Mode Definitions*/
#define SPEED_MODE 									(1U)
#if 0
#define POSITION_MODE								(0U)
#endif

#define ADC_SCALE_ST 								(51200U)	// ADC scale value for 12 bit ADC
#define MOTOR_CURRENT_ST 							(1000U)		// Motor current in mA
#if 0
#define DC_VOLTS_ST									(24000U)	// DC bus voltage in milli Volts
/* Motor phase to phase inductance in micro Henry*/
#define MOTOR_LS_ST									(2600U)
/* Motor phase to phase resistance in milli ohms*/
#define MOTOR_RS_ST									(1800U)
/* Motor step number*/
#endif
#define MOTOR_STEP_NUM_ST							(200)
/* Switching frequency in kHz*/
#define SWITCHING_FREQ_ST							(20)

/*---------------------------------------------------------------------------*
 * Initialization values
 *---------------------------------------------------------------------------*/
/* Microsteps at Reset*/
#define MICRO_STEPS_ST     							(64)
// Speed reference after reset
#define RESET_SPEED_ST     							(100)
// Initial command steps
#define CMD_STEPS_ST								(100U)
//Speed or Position Mode
#define INIT_MODE_ST								(SPEED_MODE)
/*---------------------------------------------------------------------------*/
/* Id and Iq PI parameters*/
/*---------------------------------------------------------------------------*/
/* Id and Iq PI proportional gain*/
#define I_PI_PGAIN_ST								(300U)
/* Id and Iq PI integral gain*/
#define I_PI_IGAIN_ST								(328U)
/*---------------------------------------------------------------------------*/
/* Motor sequence management parameters*/
/*---------------------------------------------------------------------------*/
/* 1 = Enable current regulators in open loop;
 * 0 = Disable current regulators in open loop*/
#define C_BY_F_VAL_ST              					(1U)
/* 1 = Enable auto restart;
 * 0 = Disable auto restart*/
#define C_AUTO_RESTART_EN_ST						(0U)
/* 1 = Enable Soft Stop;
 * 0 = Disable Soft stop*/
#define	C_SOFT_STOP_EN_ST							(0U)
/* 1 = Enable Sensor Calibration;
 * 0 = Disable Sensor Calibration*/
#define	C_SENSOR_CALIB_EN_ST						(0U)
/* Id reference value, represents current in mA;
 *Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 1*/
#define ID_REF_VAL_ST              					(500U)
/* Voltage boost in open loop startup;
 * Applicable only when C_SEQ_CNTL_C_BY_F_VAL is 0*/
#define VD_REF_VAL_ST              					(7000U)
/*========= END STEPPER PARAMETERS ======================================*/

#endif /* CONFIG_H_ */
