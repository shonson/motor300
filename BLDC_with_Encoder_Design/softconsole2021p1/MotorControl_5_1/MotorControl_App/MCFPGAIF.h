/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : MCFPGAIF.h                                               	   **
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
 **                 - function declarations of Application drivers             **
 **                                                                            **
 *******************************************************************************/

#ifndef MCFPGAIF_H_
#define MCFPGAIF_H_

/*******************************************************************************
**                      Includes                                              **
*******************************************************************************/

#include "MCCalc.h"
#include "MC_System.h"
#include "hal.h"

/*******************************************************************************
**                      Global Macro Definitions                              **
*******************************************************************************/


/*******************************************************************************
**                      Global Type Definitions                               **
*******************************************************************************/

extern addr_t g_base_addr;
/*******************************************************************************
**                      Global function Declarations                          **
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
** Return value     : result of sine value                                    **
**                                                                            **
** Description 		: This function initializes the motor control module      **
*******************************************************************************/
void MotorControl_Init(void);

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
extern void MC_BLDCSetFabReg(void);

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
extern void MC_StepperSetFabReg(void);

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
extern void BLDC_SetSpeedSRamp(void);

/*******************************************************************************
** Service Name     :MotorControl_Start                                       **
**                                                                            **
** Syntax           :void MotorControl_Start()                                **
**                                                                            **
** Parameters(in)   : none                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description 		: This API is used to run the Stepper motor               **
**                                                                            **
*******************************************************************************/
extern void MotorControl_Start(void);

/*******************************************************************************
** Service Name     :MotorControl_Stop                                        **
**                                                                            **
** Syntax           :void MotorControl_Stop(mss_gpio_id_t GPIO_X)             **
**                                                                            **
** Parameters(in)   : none                                                    **
**                                                                            **
** Parameters (out) : none                                                    **
**                                                                            **
** Return value     : none                                                    **
**                                                                            **
** Description 		: This API is used to stop the motor(s)                   **
**                                                                            **
*******************************************************************************/
extern void MotorControl_Stop(void);

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
extern void MC_BLDCSetDirn(uint32_t);

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
extern void MC_StepperSetDirn(int32_t);

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
extern void MC_WClearFault(void);

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
extern void MC_BLDCGetSpdI(void);

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
extern uint8_t MC_GetStatus(void);

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
extern void MC_StepperGetI();


#endif /* MCFPGAIF_H_ */
