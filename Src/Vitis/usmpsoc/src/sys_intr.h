/********************MILIANKE**************************

 *Company:Liyang Milian Electronic Technology Co., Ltd
 *Technical forum:www.uisrc.com
 *Taobao: https://milianke.taobao.com
 *Create Date: 2020/12/01
 *Module Name:sys_intr
 *Copyright: Copyright (c) milianke
 *Revision: 1.1
 *Description: Interrupt management

********************************************************/



#ifndef SYS_INTR_H_
#define SYS_INTR_H_

#include "xparameters.h"
#include "xil_exception.h"
#include "xdebug.h"
#include "xscugic.h"


#define INTC_DEVICE_ID          XPAR_SCUGIC_SINGLE_DEVICE_ID

XScuGic Intc; //GIC

int Init_Intr_System(XScuGic * IntcInstancePtr);
void Setup_Intr_Exception(XScuGic * IntcInstancePtr);

#endif /* SYS_INTR_H_ */
