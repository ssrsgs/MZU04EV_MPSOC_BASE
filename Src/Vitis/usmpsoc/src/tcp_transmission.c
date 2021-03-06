
/********************MILIANKE**************************

 *Company:Liyang Milian Electronic Technology Co., Ltd
 *Technical forum:www.uisrc.com
 *Taobao: https://milianke.taobao.com
 *Create Date: 2020/12/01
 *Module Name:tcp_transmiision
 *Copyright: Copyright (c) milianke
 *Revision: 1.1
 *Description:

/****************************************************/


#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwip/init.h"
#include "lwipopts.h"
#include "netif/xadapter.h"
#include "lwipopts.h"
#include "xaxidma.h"
#include "xil_cache.h"
#include "xil_printf.h"
#include "sleep.h"
#include "tcp_transmission.h"
#include "dma_intr.h"

volatile int packet_trans_done;

void send_received_data()
{
#if __arm__
	int copy = 3;
#else
	int copy = 0;
#endif
	err_t err;
	int Status;
	struct tcp_pcb *tpcb = connected_pcb;

	/*initial the first axdma transmission, only excuse once*/
	if(!first_trans_start)
	{
		Status = XAxiDma_SimpleTransfer(&AxiDma, (u32)RxBufferPtr[0],
				(u32)(PAKET_LENGTH), XAXIDMA_DEVICE_TO_DMA);
		if (Status != XST_SUCCESS)
		{
			xil_printf("axi dma failed! 0 %d\r\n", Status);
			return;
		}
		/*set the flag, so this part of code will not excuse again*/
		first_trans_start = 1;
	}

	/*if the last axidma transmission is done, the interrupt triggered, then start TCP transmission*/
	if(packet_trans_done)
	{

		if (!connected_pcb)
			return;

		/* if tcp send buffer has enough space to hold the data we want to transmit from PL, then start tcp transmission*/
		if (tcp_sndbuf(tpcb) > SEND_SIZE)
		{
			/*transmit received data through TCP*/
			Xil_DCacheInvalidateRange((u32)RxBufferPtr[packet_index & 1], SEND_SIZE);
			err = tcp_write(tpcb, RxBufferPtr[packet_index & 1], SEND_SIZE, copy);
			if (err != ERR_OK) {
				xil_printf("txperf: Error on tcp_write: %d\r\n", err);
				connected_pcb = NULL;
				return;
			}
			err = tcp_output(tpcb);
			if (err != ERR_OK) {
				xil_printf("txperf: Error on tcp_output: %d\r\n",err);
				return;
			}

			packet_index++;
			/*clear the axidma done flag*/
			packet_trans_done = 0;

			/*initial the other axidma transmission when the current transmission is done*/
			Status = XAxiDma_SimpleTransfer(&AxiDma, (u32)RxBufferPtr[(packet_index + 1)&1],
						(u32)(PAKET_LENGTH), XAXIDMA_DEVICE_TO_DMA);
			if (Status != XST_SUCCESS)
			{
				xil_printf("axi dma %d failed! %d \r\n", (packet_index + 1), Status);
				return;
			}

		}
	}
}

/*this fuction just used to count the tcp transmission times*/
static err_t
tcp_sent_callback(void *arg, struct tcp_pcb *tpcb, u16_t len)
{

	err_t err;
	tcp_trans_done = 1;
	err = tcp_output(tpcb);
	if (err != ERR_OK) {
		xil_printf("txperf: Error on tcp_output: %d\r\n",err);
		return -1;
	}

	return ERR_OK;
}

err_t
tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	xil_printf("txperf: Connected to iperf server\r\n");

	/* store state */
	connected_pcb = tpcb;

	/* set callback values & functions */
	tcp_arg(tpcb, NULL);
	tcp_sent(tpcb, tcp_sent_callback);

	/* disable nagle algorithm to ensure
	 * the last small segment of a ADC packet will be sent out immediately
	 * with no delay
	 * */
	tcp_nagle_disable(tpcb);

	if(!tcp_nagle_disabled(tpcb))
		xil_printf("tcp nagle disable failed!\r\n");

	tcp_client_connected = 1;

	/* initiate data transfer */
	return ERR_OK;
}


int tcp_send_init()
{

	err_t err;

	/* create new TCP PCB structure */
	request_pcb = tcp_new();
	if (!request_pcb) {
		xil_printf("txperf: Error creating PCB. Out of Memory\r\n");
		return -1;
	}

	/* connect to iperf tcp server */
	IP4_ADDR(&ipaddress,  192, 168, 137, 209);		/* iperf server address */

	port = 5001;					/* iperf default port */

    tcp_client_connected = 0;
    first_trans_start = 0;
    packet_trans_done = 0;
    packet_index = 0;
    tcp_trans_done = 1;
    connected_pcb = NULL;

	err = tcp_connect(request_pcb, &ipaddress, port, tcp_connected_callback);
	if (err != ERR_OK) {
		xil_printf("txperf: tcp_connect returned error: %d\r\n", err);
		return err;
	}


	return 0;
}



