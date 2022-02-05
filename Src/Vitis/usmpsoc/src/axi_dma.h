#ifndef _AXI_DMA_
#define _AXI_DMA_







#define AXI_DMA_ADDR	XPAR_AXI_DMA_0_BASEADDR


typedef struct
{
	u32 next_desc; //00h
	u32 next_desc_msb; //04h
	u32 buffer_addr; //08h
	u32 buffer_addr_msb; //0ch
	u32 not_used[2]; //10h & 14h
	u32 ctrl; //18h
	u32 status; //1ch
	u32 app[5]; //20h 24h 28h 2ch 30h
	u32 aligned[3];
}SG_Desc; //aligned to 64

volatile u32 data0[16];
volatile u32 data1[32];
volatile u32 data2[64];
volatile u32 data3[128];


volatile u32 S2MM_data0[64];
volatile u32 S2MM_data1[64];
volatile u32 S2MM_data2[64];
volatile u32 S2MM_data3[64];

SG_Desc DMA_DescMm2s[4] __attribute__ ((aligned (64)));
SG_Desc DMA_DescS2mm[4] __attribute__ ((aligned (64)));



void 	AxiDma_Mm2s_TestData(void);
void 	AxiDma_Mm2s_DescInit(void);
void 	AxiDma_Mm2s_Crtl(void);
u32 	AxiDma_Mm2s_Status(void);
void 	AxiDma_Mm2s(void);

void	AxiDma_S2mm_DescInit(void);
void	AxiDma_S2mm_Crtl(void);
u32 	AxiDma_S2mm_Status(void);
void 	AxiDma_S2mm(void);


void	AxiDma_Reset(void);




#endif /*_AXI_DMA*/



