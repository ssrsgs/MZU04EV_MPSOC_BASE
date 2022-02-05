
/********************MILIANKE**************************

 *Company:Liyang Milian Electronic Technology Co., Ltd
 *Technical forum:www.uisrc.com
 *Taobao: https://milianke.taobao.com
 *Create Date: 2020/12/01
 *Module Name:ttc_intr
 *Copyright: Copyright (c) milianke
 *Revision: 1.1
 *Description: Interrupt management

/****************************************************/

#ifndef TTC_INTR_H_
#define TTC_INTR_H_

#include <stdio.h>
#include <stdlib.h>
#include "xparameters.h"
#include "xstatus.h"
#include "xil_io.h"
#include "xil_exception.h"
#include "xttcps.h"
#include "xscugic.h"
#include "xil_printf.h"
#include "ttc_intr.h"

/************************** Constant Definitions *****************************/
#define TTC_TICK_DEVICE_ID	XPAR_XTTCPS_0_DEVICE_ID
#define TTC_TICK_INTR_ID	XPAR_XTTCPS_0_INTR
#define INTC_DEVICE_ID		XPAR_SCUGIC_SINGLE_DEVICE_ID

#define	TICK_TIMER_FREQ_HZ	4  /* Tick timer counter's output frequency */
#define TICKS_PER_CHANGE_PERIOD	1 /* Tick signals per update */

/**************************** Type Definitions *******************************/
typedef struct {
u32 OutputHz;	/* Output frequency */
XInterval Interval;	/* Interval value */
u8 Prescaler;	/* Prescaler value */
u16 Options;	/* Option settings */
} TmrCntrSetup;

/************************** Function Prototypes ******************************/
static XTtcPs TtcPsInst;  /* Timer counter instance */
static TmrCntrSetup SettingsTable = {TICK_TIMER_FREQ_HZ, 0, 0, 0};	/* Ticker timer counter initial setup,only output freq */
static u32 TickCount;		/* Ticker interrupts between seconds change */
extern volatile u32 TcpTmrFlag;;
u32 ErrorCount;
int  TTC_init(XTtcPs *TTCInstancePtr, u32 DeviceId);
void TTC_Setup_Intr_System(XScuGic *GicInstancePtr,XTtcPs *TTCInstancePtr, u16 TTCIntrId);
void Timer_start(XTtcPs *TTCInstancePtr);

#endif /* SYS_INTR_H_ */


