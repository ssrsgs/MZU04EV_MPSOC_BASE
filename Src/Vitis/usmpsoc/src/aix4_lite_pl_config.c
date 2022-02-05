#include"aix4_lite_pl_config.h"
#include"aix4_lite_pl_config.h"







#define XPAR_MYIP_BASEADDR  XPAR_MYIP_0_BASEADDR

void    Axis_Test_Write(UINTPTR Addr,u32 Value)
{
    Xil_Out32(Addr,Value);
}




u32    Axis_Test_Read(UINTPTR Addr)
{
	return Xil_In32(Addr);
}



/*
Void	Axis_Test_Config()
{
	Axis_Test_Write(PL_AXIS_TEST_ADDR,0x0);
	//Axis_Test_Write(PL_AXIS_TEST_ADDR,0x0);
	//Axis_Test_Write(PL_AXIS_TEST_ADDR,0x0);
	//Axis_Test_Write(PL_AXIS_TEST_ADDR,0x0);
}
*/
