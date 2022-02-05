
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

volatile u32 TcpTmrFlag ;

static void TickHandler(void *CallBackRef, u32 StatusEvent);

int TTC_init(XTtcPs *TTCInstancePtr, u32 DeviceId)
{
	int Status;
	XTtcPs_Config *Config;
	TmrCntrSetup *TimerSetup;
	TimerSetup = &SettingsTable;
	/*
	 * Set up appropriate options for Ticker: interval mode without
	 * waveform output.
	 */
	TimerSetup->Options |= (XTTCPS_OPTION_INTERVAL_MODE |XTTCPS_OPTION_WAVE_DISABLE);
	/*
	 * Look up the configuration based on the device identifier
	 */
	Config = XTtcPs_LookupConfig(DeviceId);
	if (NULL == Config) {
		return XST_FAILURE;
	}
	/*
	 * Initialize the device
	 */
	Status = XTtcPs_CfgInitialize(TTCInstancePtr, Config, Config->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	/*
	 * Set the options
	 */
	XTtcPs_SetOptions(TTCInstancePtr, TimerSetup->Options);

	/*
	 * Timer frequency is preset in the TimerSetup structure,
	 * however, the value is not reflected in its other fields, such as
	 * IntervalValue and PrescalerValue. The following call will map the
	 * frequency to the interval and prescaler values.
	 */
	XTtcPs_CalcIntervalFromFreq(TTCInstancePtr, TimerSetup->OutputHz,
		&(TimerSetup->Interval), &(TimerSetup->Prescaler));

	/*
	 * Set the interval and prescale
	 */
	XTtcPs_SetInterval(TTCInstancePtr, TimerSetup->Interval);
	XTtcPs_SetPrescaler(TTCInstancePtr, TimerSetup->Prescaler);

	return XST_SUCCESS;
}

void TTC_Setup_Intr_System(XScuGic *GicInstancePtr,XTtcPs *TTCInstancePtr, u16 TTCIntrId)
{
	int Status;
	/*
	 * Connect to the interrupt controller
	 */
	Status = XScuGic_Connect(GicInstancePtr, TTCIntrId,(Xil_InterruptHandler)XTtcPs_InterruptHandler, (void *)TTCInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XTtcPs_SetStatusHandler(TTCInstancePtr, TTCInstancePtr,(XTtcPs_StatusHandler) TickHandler);
	/*
	 * Enable the interrupt for the Timer counter
	 */
	XScuGic_Enable(GicInstancePtr, TTC_TICK_INTR_ID);

	/*
	 * Enable the interrupts for the tick timer/counter
	 * We only care about the interval timeout.
	 */
	XTtcPs_EnableInterrupts(TTCInstancePtr, XTTCPS_IXR_INTERVAL_MASK);

}







static void TickHandler(void *CallBackRef, u32 StatusEvent)
{

	if (0 != (XTTCPS_IXR_INTERVAL_MASK & StatusEvent)) {
		TickCount++;

		if (TICKS_PER_CHANGE_PERIOD == TickCount) {
			TickCount = 0;
			TcpTmrFlag = 1;

		}

	}
	else {
		/*
		 * The Interval event should be the only one enabled. If it is
		 * not it is an error
		 */
		ErrorCount++;
	}
}
