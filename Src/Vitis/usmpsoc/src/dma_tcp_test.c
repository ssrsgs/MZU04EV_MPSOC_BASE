
/********************MILIANKE**************************

 *Company:Liyang Milian Electronic Technology Co., Ltd
 *Technical forum:www.uisrc.com
 *Taobao: https://milianke.taobao.com
 *Create Date: 2020/12/01
 *Module Name:dma_tcp_test
 *Copyright: Copyright (c) milianke
 *Revision: 1.1
 *Description:

/****************************************************/

#include "dma_intr.h"
#include "ttc_intr.h"
#include "sys_intr.h"
#include "xgpio.h"

#include "lwip/err.h"
#include "lwip/tcp.h"
#include "lwipopts.h"
#include "netif/xadapter.h"
#include "lwipopts.h"
#include "tcp_transmission.h"

XAxiDma AxiDma;
u16 	*RxBufferPtr[2];  /* ping pong buffers*/

volatile u32 RX_success;
volatile u32 RX_ready=1;

static XGpio Gpio;

#define AXI_GPIO_DEV_ID	        XPAR_AXI_GPIO_0_DEVICE_ID

int init_intr_sys(void)
{
	DMA_Intr_Init(&AxiDma,0);//initial interrupt system
	TTC_init(&TtcPsInst,TTC_TICK_DEVICE_ID);
	Init_Intr_System(&Intc); // initial DMA interrupt system
	Setup_Intr_Exception(&Intc);
	DMA_Setup_Intr_System(&Intc,&AxiDma,RX_INTR_ID);//setup dma interrpt system
	TTC_Setup_Intr_System(&Intc,&TtcPsInst, TTC_TICK_INTR_ID);
	DMA_Intr_Enable(&Intc,&AxiDma);

}

int main(void)
{

	int Status;
	struct netif *netif, server_netif;
    ip_addr_t ipaddr, netmask, gw;
	err_t err;

	/* the mac address of the board. this should be unique per board */
	unsigned char mac_ethernet_address[] = { 0x00, 0x0a, 0x35, 0x00, 0x01, 0x02 };

	/* Initialize the ping pong buffers for the data received from axidma */
	RxBufferPtr[0] = (u16 *)RX_BUFFER0_BASE;
	RxBufferPtr[1] = (u16 *)RX_BUFFER1_BASE;

	XGpio_Initialize(&Gpio, AXI_GPIO_DEV_ID);//initialize gpio use gpio for dma reset
	XGpio_SetDataDirection(&Gpio, 1, 0);

	init_intr_sys();//initialize interrupts
	TcpTmrFlag = 0;

	netif = &server_netif;//as server

	//set board ip address
	IP4_ADDR(&ipaddr,  192, 168,   137,  10);
	IP4_ADDR(&netmask, 255, 255, 255,  0);
	IP4_ADDR(&gw,      192, 168,   137,  10);

	/*lwip library init*/
	lwip_init();
	/* Add network interface to the netif_list, and set it as default */
	if (!xemac_add(netif, &ipaddr, &netmask, &gw, mac_ethernet_address, XPAR_XEMACPS_0_BASEADDR)) {
		xil_printf("Error adding N/W interface\r\n");
		return -1;
	}
	netif_set_default(netif);

	netif_set_up(netif);/* specify that the network if is up */

	tcp_send_init();/* initialize tcp pcb */

	XGpio_DiscreteWrite(&Gpio, 1, 1);//reset done

	XTtcPs_Start(&TtcPsInst);//start ttc timer ttc will interrupt every 250ms


	while (1)
	{
		/* call tcp timer every 250ms */
		if(TcpTmrFlag)
		{
			if(request_pcb->state == CLOSED || (request_pcb->state == SYN_SENT && request_pcb->nrtx == TCP_SYNMAXRTX))//check conditions for create new tcp connection
			{
				request_pcb = tcp_new();//create new tcp connetion
				if (!request_pcb) {
					xil_printf("txperf: Error creating PCB. Out of Memory\r\n");
					return -1;
				}

				//ip_set_option(request_pcb, SOF_REUSEADDR);
				err = tcp_connect(request_pcb, &ipaddress, port, tcp_connected_callback);//set tcp_connected_callback function when tcp connection is in request
				if (err != ERR_OK) {
					xil_printf("txperf: tcp_connect returned error: %d\r\n", err);
					return err;
				}
			}
			TcpTmrFlag = 0;
		}
		/*receive input packet from emac*/
		xemacif_input(netif);
		/* if connected to the server, start receive data from PL through axidma, then transmit the data to the PC software by TCP*/
		if(tcp_client_connected)//if tcp connection is setup
			send_received_data();//call send_received_data() function sent data from ddr
	}
	return 0;

}


