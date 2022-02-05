#ifndef _AXI4_LITE_PL_CONFIG_
#define _AXI4_LITE_PL_CONFIG_


#include "xparameters.h"
#include "xil_io.h"
#include "xil_types.h"







/*axis test mode base address*/
#define AXIS_TEST_MODE_BASE_ADDR   XPAR_AXIS_TEST_MODE_0_BASEADDR



/*register effect address*/
#define PL_AXIS_TEST_ADDR         ( AXIS_TEST_MODE_BASE_ADDR + 0X00 )
//#define PL_AXIS_TEST_ADDR           0X04
//#define PL_AXIS_TEST_ADDR           0X08
//#define PL_AXIS_TEST_ADDR           0X0C



/*aix4 test mode write */
void    Axis_Test_Write(UINTPTR Addr,u32 Value);
/* aix4 test mode read */
u32    Axis_Test_Read(UINTPTR Addr);






#endif /*_AXI4_LITE_PL_CONFIG_*/
