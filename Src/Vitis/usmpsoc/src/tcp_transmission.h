/********************MILIANKE**************************

 *Company:Liyang Milian Electronic Technology Co., Ltd
 *Technical forum:www.uisrc.com
 *Taobao: https://milianke.taobao.com
 *Create Date: 2020/12/01
 *Module Name:tcp_transmission
 *Copyright: Copyright (c) milianke
 *Revision: 1.1
 *Description: Interrupt management

/****************************************************/


#ifndef TCP_TRANSMISSION_H_
#define TCP_TRANSMISSION_H_

#include "xparameters.h"

#include "netif/xadapter.h"
#if defined (__arm__) || defined(__aarch64__)
#include "xil_printf.h"
#endif

#include "lwip/tcp.h"
#include "xil_cache.h"

#if LWIP_IPV6==1
#include "lwip/ip.h"
#else
#if LWIP_DHCP==1
#include "lwip/dhcp.h"
#endif
#endif
#include <stdio.h>
#include "xil_types.h"
#include "Xscugic.h"

#define SEND_SIZE (4096)
#define PAKET_LENGTH (4096)

struct tcp_pcb *connected_pcb;
struct tcp_pcb *request_pcb;
volatile unsigned tcp_client_connected;
volatile int tcp_trans_done;
unsigned first_trans_start;

volatile u32 packet_index;

extern u16 *RxBufferPtr[2];

u16_t port;
ip_addr_t ipaddress;
void tcp_lwip_init();
int tcp_send_init();
void send_received_data();
err_t tcp_connected_callback(void *arg, struct tcp_pcb *tpcb, err_t err);


#endif /* TCP_TRANSMISSION_H_ */
