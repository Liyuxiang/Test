/*
* Copyright (c) 2014,郑州联睿电子科技有限公司
* All rights reserved.
* 
* 文件名称：os_driver.c
* 文件标识：操作系统驱动文件
* 摘    要：操作系统相关驱动程序(没有在系统文件中，需要外加的驱动)
* 
* 当前版本：0.1
* 作    者：何雪锋
* 完成日期：2014.03.17
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                         OS_CPU_SysTickClkFreq()
*
* 描	述 : 获取硬件CPU时钟
*
* 输入参数 : none.
*
* 输出参数 : CPU内核时钟
*
*********************************************************************************************************
*/

INT32U  OS_CPU_SysTickClkFreq (void)
{
	RCC_ClocksTypeDef  rcc_clocks;


	RCC_GetClocksFreq(&rcc_clocks);

	return ((CPU_INT32U)rcc_clocks.HCLK_Frequency);
}

