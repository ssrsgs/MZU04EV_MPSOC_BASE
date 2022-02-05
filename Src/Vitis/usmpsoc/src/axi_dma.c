

#include "stdio.h"
#include "xparameters.h"
#include "sleep.h"
#include "xil_cache.h"
#include "xaxidma.h"
#include "xaxidma_hw.h"


#include "axi_dma.h"






void AxiDma_Mm2s_TestData(void)
{
	int i;
	for(i=0; i<16; i++)
	{
		data0[i] = i;
	}
	for(i=16; i<16+32; i++)
	{
		data1[i-16] = i;
	}

	for(i=16+32; i<16+32+64; i++)
	{
		data2[i-16-32] = i;
	}
	for(i=16+32+64; i<16+32+64+128; i++)
	{
		data3[i-16-32-64] = i;
	}
	Xil_DCacheFlushRange((INTPTR)data0, sizeof(data0));
	Xil_DCacheFlushRange((INTPTR)data1, sizeof(data1));
	Xil_DCacheFlushRange((INTPTR)data2, sizeof(data2));
	Xil_DCacheFlushRange((INTPTR)data3, sizeof(data3));
}








void AxiDma_Mm2s_DescInit(void)
{
	memset(DMA_DescMm2s, 0, sizeof(DMA_DescMm2s));
	//
	//start ddr to pl
	DMA_DescMm2s[0].next_desc = (INTPTR)(&(DMA_DescMm2s[1]));
	DMA_DescMm2s[0].buffer_addr = (INTPTR)(S2MM_data0);
	DMA_DescMm2s[0].ctrl = sizeof(S2MM_data0)|XAXIDMA_BD_CTRL_TXSOF_MASK;

	DMA_DescMm2s[1].next_desc = (INTPTR)(&(DMA_DescMm2s[2]));


	DMA_DescMm2s[1].buffer_addr = (INTPTR)(S2MM_data1);
	DMA_DescMm2s[1].ctrl = sizeof(S2MM_data1);

	DMA_DescMm2s[2].next_desc = (INTPTR)(&(DMA_DescMm2s[3]));
	DMA_DescMm2s[2].buffer_addr = (INTPTR)(S2MM_data2);
	DMA_DescMm2s[2].ctrl = sizeof(S2MM_data2);
	//end
	DMA_DescMm2s[3].next_desc = (INTPTR)(&(DMA_DescMm2s[0]));
	DMA_DescMm2s[3].buffer_addr = (INTPTR)(S2MM_data3);
	DMA_DescMm2s[3].ctrl = sizeof(S2MM_data3)|XAXIDMA_BD_CTRL_TXEOF_MASK;

	Xil_DCacheFlushRange((INTPTR)DMA_DescMm2s, sizeof(DMA_DescMm2s));
}





void AxiDma_Mm2s_Crtl(void)
{
	// reset
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_CR_OFFSET, XAXIDMA_CR_RESET_MASK);
	usleep(1000);

	//disable all interrupt
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_CR_OFFSET, Xil_In32(AXI_DMA_ADDR+XAXIDMA_CR_OFFSET)&(~XAXIDMA_IRQ_ALL_MASK));
	//It can be ignored due to system reset;

	//set DMA_DescMm2s address
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_CDESC_OFFSET, (UINTPTR)(&(DMA_DescMm2s[0])));

	//start
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_CR_OFFSET, Xil_In32(AXI_DMA_ADDR+XAXIDMA_CR_OFFSET)|XAXIDMA_CR_RUNSTOP_MASK);
	while(1)
	{
		if((Xil_In32(AXI_DMA_ADDR+XAXIDMA_SR_OFFSET)&XAXIDMA_HALTED_MASK)==0)
			break;
	}
	//trigger
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_TDESC_OFFSET, (UINTPTR)(&(DMA_DescMm2s[3])));
}




u32 AxiDma_Mm2s_Status(void)
{
	return Xil_In32(AXI_DMA_ADDR+XAXIDMA_SR_OFFSET)&XAXIDMA_IDLE_MASK;
}










//void	AxiDma_Mm2s_TestData(void)

void	AxiDma_S2mm_DescInit(void)
{
	memset(DMA_DescS2mm, 0, sizeof(DMA_DescS2mm));

	DMA_DescS2mm[0].next_desc = (INTPTR)(&(DMA_DescS2mm[1]));
	DMA_DescS2mm[0].buffer_addr = 	 (INTPTR)(S2MM_data0);
	DMA_DescS2mm[0].ctrl = sizeof(S2MM_data0)|XAXIDMA_BD_CTRL_TXSOF_MASK;

	DMA_DescS2mm[1].next_desc = (INTPTR)(&(DMA_DescS2mm[2]));
	DMA_DescS2mm[1].buffer_addr = 	 (INTPTR)(S2MM_data1);
	DMA_DescS2mm[1].ctrl = sizeof(S2MM_data1);

	DMA_DescS2mm[2].next_desc = (INTPTR)(&(DMA_DescS2mm[3]));
	DMA_DescS2mm[2].buffer_addr = 	 (INTPTR)(S2MM_data2);
	DMA_DescS2mm[2].ctrl = sizeof(S2MM_data2);

	DMA_DescS2mm[3].next_desc = (INTPTR)(&(DMA_DescS2mm[0]));
	DMA_DescS2mm[3].buffer_addr = 	 (INTPTR)(S2MM_data3);
	DMA_DescS2mm[3].ctrl = sizeof(S2MM_data3)|XAXIDMA_BD_CTRL_TXEOF_MASK;

	Xil_DCacheFlushRange((INTPTR)DMA_DescS2mm, sizeof(DMA_DescS2mm));

}




void	AxiDma_S2mm_Crtl(void)
{
	// reset
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_CR_OFFSET, XAXIDMA_CR_RESET_MASK);
	usleep(1000);



	//set DMA_DescMm2s address
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_CDESC_OFFSET, (UINTPTR)(&(DMA_DescS2mm[0])));

	//start
	
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_CR_OFFSET, Xil_In32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_CR_OFFSET)|XAXIDMA_CR_RUNSTOP_MASK);
	while(1)
	{
		if((Xil_In32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_SR_OFFSET)&XAXIDMA_HALTED_MASK)==0)
			break;
	}

	//disable all interrupt
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_CR_OFFSET, Xil_In32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_CR_OFFSET)&(~XAXIDMA_IRQ_ALL_MASK));
	//It can be ignored due to system reset;


	//trigger
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_TDESC_OFFSET, (UINTPTR)(&(DMA_DescS2mm[3])));
	

}








u32 AxiDma_S2mm_Status(void)
{

	return Xil_In32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_SR_OFFSET)&XAXIDMA_IDLE_MASK;

}








void AxiDma_S2mm(void)
{
	u32 Status_register;
	//printf("Dma_S2mm is runing~!\r\n");
	AxiDma_S2mm_DescInit();
	AxiDma_S2mm_Crtl();
	/*
	while(1)
	{

		if(AxiDma_S2mm_Status())
		{
			//printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~S2mm is done ~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
			break;
		}
		//atus_register = Xil_In32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_SR_OFFSET);
		//printf("Status_register is = %X\r\n",Xil_In32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_SR_OFFSET));
		usleep(1);
	}
	*/
}

 








void AxiDma_Mm2s(void)
{
	//printf("Dma_Mm2s is runing~!\r\n");
	AxiDma_Mm2s_DescInit();
	AxiDma_Mm2s_Crtl();
	while(1)
	{

		if(AxiDma_Mm2s_Status())
		{
			//printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~Mm2s is done ~~~~~~~~~~~~~~~~~~~~~~~~~~\r\n");
			break;
		}
		usleep(10000);
	}

	
}



void	AxiDma_Reset(void)
{
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_CR_OFFSET, ~XAXIDMA_CR_RESET_MASK);
	usleep(1000);
	Xil_Out32(AXI_DMA_ADDR+XAXIDMA_RX_OFFSET + XAXIDMA_CR_OFFSET, ~XAXIDMA_CR_RESET_MASK);
	usleep(1000);
}










