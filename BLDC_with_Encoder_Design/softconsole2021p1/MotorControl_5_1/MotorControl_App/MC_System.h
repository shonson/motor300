/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : MC_System.h                                               	   **
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
 **                 - Macros defining APB Addresses of various parameters 	   **
 **                 used in the motor control project.					 	   **
 **                                                                            **
 *******************************************************************************/
#ifndef MC_SYSTEM_H_
#define MC_SYSTEM_H_

/******************************************************************************
 **                      Global Macro Definitions                             **
 ******************************************************************************/
/*lint e960*/

#define BASE_ADDR_0             					(addr_t)(0x30000000U)
#define BASE_ADDR_1             					(addr_t)(0x30001000U)

/* Common addresses for BLDC and Stepper */

/******* ADC Address space********/
#define C_ADC_CONFIG_ADDR                   	    (addr_t)(0x00000000U)
#define C_CH0_CONFIG_ADDR        				    (addr_t)(0x00000004U)
#define C_CH1_CONFIG_ADDR        				    (addr_t)(0x00000008U)
#define C_CH2_CONFIG_ADDR        				    (addr_t)(0x0000000CU)
#define C_CH3_CONFIG_ADDR        				    (addr_t)(0x00000010U)
/******* ADC Scaling Address space********/
#define C_ADC_SCALE_ADDR                   		    (addr_t)(0x00000020U)
#define C_OVER_CURR_THRESH_ADDR                   	(addr_t)(0x00000024U)

/******* PWM Address space********/
#define C_PWM_PERIOD_VAL_ADDR                   	(addr_t)(0x00000034U)
#define C_PWM_DEAD_TIME_ADDR                		(addr_t)(0x00000038U)
#define C_PWM_DELAY_TIME_ADDR               		(addr_t)(0x0000003CU)
#define C_PWM_GAIN_ADDR                 			(addr_t)(0x00000040U)

/*SPEED_PI,ID_PI,IQ_PI parameters address	*/
#define C_SPEED_PI_KP_ADDR                        	(addr_t)(0x00000050U)
#define C_SPEED_PI_KI_ADDR                        	(addr_t)(0x00000054U)
#define C_ID_PI_KP_ADDR                       		(addr_t)(0x00000058U)
#define C_ID_PI_KI_ADDR                       		(addr_t)(0x0000005CU)
#define C_IQ_PI_KP_ADDR                       		(addr_t)(0x00000060U)
#define C_IQ_PI_KI_ADDR                       		(addr_t)(0x00000064U)

/* Rate_limiter parameters address	*/
#define C_RATE_LIMIT_REF_ADDR                   	(addr_t)(0x00000070U)
#define C_RATE_LIMIT_SLEW_CNT_ADDR              	(addr_t)(0x00000074U)
#define C_RATE_LIMIT_RATE_CNT_ADDR              	(addr_t)(0x00000078U)

/*OLMNG and Sequence Controller parameters address	*/
#define C_DIRECTION_CONFIG_ADDR                    	(addr_t)(0x00000090U)
#define C_START_MOTOR_ADDR                    		(addr_t)(0x00000094U)
#define C_OLMNG_THETA_FACTOR_ADDR               	(addr_t)(0x00000098U)
#define C_SEQ_CNTL_CONFIG_ADDR                     	(addr_t)(0x0000009CU)
#define C_OLMNG_DV_ADDR                         	(addr_t)(0x000000A0U)
#define C_OLMNG_IQ_REF_ADDR                     	(addr_t)(0x000000A4U)
#define C_SEQ_CNTL_CL_OMEGA_ADDR                   	(addr_t)(0x000000A8U)
#define C_STOP_MOTOR_ADDR		                   	(addr_t)(0x000000ACU)
#define C_MC_FSM_STATE_ADDR		                   	(addr_t)(0x000000B0U)
#define C_AUTO_RESTART_NO_ADDR                     	(addr_t)(0x000000B4U)
#define C_CLR_FAULT_ADDR                     		(addr_t)(0x000000B8U)
/* END of common addresses */
#if 1
/*Position Estimator Address Space*/
#define C_ANGLE_LB_FIRST_CONST_ADDR           		(addr_t)(0x00000200U)
#define C_ANGLE_LB_SECOND_CONST_ADDR          		(addr_t)(0x00000204U)
#define C_ANGLE_KP_ADDR                       		(addr_t)(0x00000208U)
#define C_ANGLE_KI_ADDR                       		(addr_t)(0x0000020CU)
#define C_FILTER_FACTOR_BEMF_ADDR            		(addr_t)(0x00000210U)
#define C_FILTER_FACTOR_OMEGA_ADDR           		(addr_t)(0x00000214U)
#define C_INIT_THETA_ADDR                  			(addr_t)(0x00000218U)
#endif
#if 1
#define C_EN_SPEED_FACTOR_ADDR          			(addr_t)(0x00000300U)
#define C_EN_ANGLE_FACTOR_ADDR          			(addr_t)(0x00000304U)
#define C_EN_ANGLE_CNT_MAX_ADDR          			(addr_t)(0x00000308U)
#define C_EN_SPD_FILTER_FACTOR_ADDR          		(addr_t)(0x0000030CU)
#define C_EN_SPEED_WINDOW_ADDR		          		(addr_t)(0x00000310U)
#define C_EN_POSITION_PI_KP			          		(addr_t)(0x00000314U)
#define C_EN_POSITION_PI_KI			          		(addr_t)(0x00000318U)
#define C_EN_POSITION_PI_EN			          		(addr_t)(0x0000031CU)
#define C_EN_POSITION_COUNT			          		(addr_t)(0x00000320U)
#endif
#if 1
/*Hall Theta Gen Address Space*/
#define C_HALL_THT_INTRP_FACTOR_ADDR             	(addr_t)(0x00000400U)
#define C_HALL_THETA_STEP_ADDR                  	(addr_t)(0x00000404U)
#define C_HALL_ANGLE_OFFSET_ADDR                  	(addr_t)(0x00000408U)
#define C_HALL_FILTER_FACTOR_ADDR                  	(addr_t)(0x0000040CU)
#define C_HALL_SPEED_FACTOR_ADDR                  	(addr_t)(0x00000410U)
#endif
#if 1
/*Resolver Parameters*/
#define C_PLL_PI_KP_ADDR						    (addr_t) (0x00000920u)
#define C_PLL_PI_KI_ADDR						    (addr_t) (0x00000924u)
#define C_THETA_FACTOR_ADDR						    (addr_t) (0x00000928u)
#define C_HF_SIG_PERIOD_ADDR						(addr_t) (0x0000092Cu)
#define C_DC_FILTER_FACTOR_ADDR						(addr_t) (0x00000930u)
#define C_AC_FILTER_FACTOR_ADDR						(addr_t) (0x00000934u)
#define C_PP_RATIO_ADDR    						    (addr_t) (0x00000938u)
#endif
#if 1
/*Hall Theta Gen Address Space*/
#define C_HALL_THT_INTRP_FACTOR_ADDR             	(addr_t)(0x00000400U)
#define C_HALL_THETA_STEP_ADDR                  	(addr_t)(0x00000404U)
#define C_HALL_SENSOR_POLARITY_ADDR                 (addr_t)(0x00000408U)
#define C_HALL_FILTER_FACTOR_ADDR                  	(addr_t)(0x0000040CU)
#define C_HALL_SPEED_FACTOR_MUL_ADDR                (addr_t)(0x00000410U)
#endif
// Stepper motor addresses
#define C_THETA_GEN_SLEW_COUNT_ADDR    				(addr_t)(0x00000700U)
#define C_THETA_GEN_RATE_LIMIT_ADDR   				(addr_t)(0x00000704U)
#define C_THETA_GEN_CMD_STEP_NO_ADDR       			(addr_t)(0x00000708U)
#define C_THETA_GEN_MODE_ADDR            			(addr_t)(0x0000070CU)
#define C_ID_REF_ADDR            					(addr_t)(0x00000710U)

/* REGISTERS FOR PLOTTING WAVEFORMS*/
/*BLDC parameters for plotting*/
#define C_SPEED_PI_OUT_ADDR                   		(addr_t)(0x00000824U)
#define C_IQ_PARK_ADDR        					    (addr_t)(0x00000814U)
#define C_IQ_PI_OUT_ADDR                      		(addr_t)(0x0000082CU)
#define C_OMEGA_FILTER_OUT_ADDR               		(addr_t)(0x00000918U)
#define C_OMEGA_PI_OUT_ADDR                   		(addr_t)(0x00000914U)
/*Stepper parameters for plotting*/
#define C_ID_PARK_ADDR    						    (addr_t)(0x00000810U)
#define C_THETA_OUT_ADDR	            			(addr_t)(0x00000E04U)
#define C_ID_PI_OUT_ADDR                      		(addr_t)(0x00000828U)

#if 1
/*List of available parameters that can be used for plotting*/
#define C_IA_ADDR        						    (addr_t)(0x00000800U)
#define C_IB_ADDR        						    (addr_t)(0x00000804U)
#define C_IALPHA_ADDR    						    (addr_t)(0x00000808U)
#define C_IBETA_ADDR           						(addr_t)(0x0000080CU)
#define C_ID_PARK_ADDR     						    (addr_t)(0x00000810U)
#define C_IQ_PARK_ADDR        					    (addr_t)(0x00000814U)
#define C_VALPHA_IPARK_ADDR						    (addr_t)(0x00000818U)
#define C_VBETA_IPARK_ADDR 						    (addr_t)(0x0000081CU)
#define C_RATE_LIMITER_OUT_ADDR               		(addr_t)(0x00000820U)
#define C_SPEED_PI_OUT_ADDR                   		(addr_t)(0x00000824U)
#define C_ID_PI_OUT_ADDR                      		(addr_t)(0x00000828U)
#define C_IQ_PI_OUT_ADDR                      		(addr_t)(0x0000082CU)
#define C_OLMNG_CL_OMEGA_ADDR                		(addr_t)(0x00000830U)
#define C_CH0_DATA_ADDR          				    (addr_t)(0x00000838U)
#define C_CH1_DATA_ADDR          				    (addr_t)(0x0000083CU)
#define C_CH2_DATA_ADDR          			    	(addr_t)(0x00000840U)
#define C_CH3_DATA_ADDR          			    	(addr_t)(0x00000844U)
#define C_ADC_RESULTS_READY_ADDR      				(addr_t)(0x00000848U)
/*Position Estimator parameters for plotting*/
#define C_EALPHA_OUT_ADDR                     		(addr_t)(0x00000900U)
#define C_EBETA_OUT_ADDR                      		(addr_t)(0x00000904U)
#define C_EALPHA_FILTER_OUT_ADDR              		(addr_t)(0x00000908U)
#define C_EBETA_FILTER_OUT_ADDR               		(addr_t)(0x0000090CU)
#define C_PLL_THETA_OUT_ADDR                  		(addr_t)(0x00000910U)
#define C_OMEGA_PI_OUT_ADDR                   		(addr_t)(0x00000914U)
#define C_OMEGA_FILTER_OUT_ADDR               		(addr_t)(0x00000918U)
/*Encoder parameters for plotting*/
#define C_EN_SPEED_ADDR								(addr_t)(0x00000A00U)
#define C_EN_ANGLE_ADDR								(addr_t)(0x00000A04U)
#if 1
/*Resolver Parameters*/
#define C_PLL_PI_KP_ADDR						    (addr_t) (0x00000920u)
#define C_PLL_PI_KI_ADDR						    (addr_t) (0x00000924u)
#define C_THETA_FACTOR_ADDR						    (addr_t) (0x00000928u)
#define C_HF_SIG_PERIOD_ADDR						(addr_t) (0x0000092Cu)
#define C_DC_FILTER_FACTOR_ADDR						(addr_t) (0x00000930u)
#define C_AC_FILTER_FACTOR_ADDR						(addr_t) (0x00000934u)
#define C_PP_RATIO_ADDR    						    (addr_t) (0x00000938u)
#endif
/* Stepper Specific*/
#if 1
#define C_STEP_CNT_ADDR	            				(addr_t)(0x00000E00U)
#define C_THETA_O_ADDR	            				(addr_t)(0x00000E04U)
#endif
#if 1
#define C_HALL_ANGLE_RAW_ADDR  						(addr_t)(0x00000B00U)
#define C_HALL_THETA_OUT_ADDR            			(addr_t)(0x00000B04U)
#define C_HALL_OMEGA_OUT_ADDR                 		(addr_t)(0x00000B08U)
#define C_HALL_OMEGA_FILT_OUT_ADDR                 	(addr_t)(0x00000B0CU)
#define C_HALL_SPD_WINDOW_ADDR                 		(addr_t)(0x00000B10U)
#endif
#endif

#endif /* MC_SYSTEM_H_ */
