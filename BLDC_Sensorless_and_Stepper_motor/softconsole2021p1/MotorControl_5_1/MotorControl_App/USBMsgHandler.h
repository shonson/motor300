/*******************************************************************************
 ** (c) Copyright 2016 Microsemi Corporation All rights reserved.              **
 **                                                                            **
 ********************************************************************************
 **                                                                            **
 **  FILENAME  : USBMsgHandler.h                                           	   **
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

#ifndef USBMSGHANDLER_H_
#define USBMSGHANDLER_H_
/*******************************************************************************
 **                      Includes                                             **
 *******************************************************************************/
#include "MCFPGAIF.h"

extern volatile uint8_t g_streamdata;
extern volatile uint8_t g_streamaxis;
/*******************************************************************************
 ** Service Name     : USB_init	                                      	      **
 **                                                                           **
 ** Syntax           : void USB_init(void)				                      **
 **                                                                           **
 ** Parameters(in)   : none                                                   **
 **                                                                           **
 ** Parameters (out) : none                                                   **
 **                                                                           **
 ** Return value     : none			                                          **
 **                                                                           **
 ** Description 		: This procedure is used to initialize USB.			  **
 **                                                                           **
 *******************************************************************************/
extern void USB_init(void);
/*******************************************************************************
 ** Service Name     : SendStreamDATA	                                      **
 **                                                                           **
 ** Syntax           : void SendStreamDATA(void)			                  **
 **                                                                           **
 ** Parameters(in)   : none                                                   **
 **                                                                           **
 ** Parameters (out) : none                                                   **
 **                                                                           **
 ** Return value     : none			                                          **
 **                                                                           **
 ** Description 	: This procedure is used to send data streams to GUI for  **
 ** 					  plotting. 										  **
 **                                                                           **
 *******************************************************************************/
extern void SendStreamDATA(void);
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
extern void USBMsgHandler(void);

#endif
