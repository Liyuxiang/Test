/*
* Copyright (c) 2014,郑州联睿电子科技有限公司
* All rights reserved.
* 
* 文件名称：main.c
* 文件标识：操作系统驱动文件
* 摘    要：软件主函数，操作系统启动引导，硬件初始化等操作。
* 
* 当前版本：0.1
* 作    者：何雪锋
* 完成日期：2014.03.18
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                      宏定义
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      常量定义
*********************************************************************************************************
*/
const char *logstr = "\
					 *****************************************************************\r\n\
					 *                                                               *\r\n\
					 *                                                               *\r\n\
					 *                                                               *\r\n\
					 *                                                               *\r\n\
					 *                                                               *\r\n\
					 *                                                               *\r\n\
					 *                                                               *\r\n\
					 *       @Copyright (c) 2014, Locaris Technology Co., Ltd.       *\r\n\
					 *                       All Rights Reserved                     *\r\n\
					 *                                                               *\r\n\
					 *****************************************************************";

__ALIGN_BEGIN USB_OTG_CORE_HANDLE    USB_OTG_dev __ALIGN_END ;
extern CDC_IF_Prop_TypeDef VCP_fops;

/*
*********************************************************************************************************
*                                     任务堆栈定义
*********************************************************************************************************
*/

OS_STK	App_TaskStartStk[APP_TASK_START_STK_SIZE];
OS_STK	App_TaskShellStk[APP_TASK_SHELL_STK_SIZE];


/*
*********************************************************************************************************
*                                       任务声明
*********************************************************************************************************
*/
static	void	App_TaskStart(void	*p_arg);
//static	void	App_TaskShell(void	*p_arg);
/*
*********************************************************************************************************
*                                       函数声明
*********************************************************************************************************
*/
static void	app_eventcreate(void);
static void app_taskcreate(void);

/*
*********************************************************************************************************
*                                      定义事件
*********************************************************************************************************
*/
OS_EVENT *qmsg;		//dw1000 接收消息邮箱定义
void	*qmsggrp[10];


/*
*********************************************************************************************************
*                                                main()
*
* 描		述 : 程序主函数
*
* 输入参数 : none
*
* 输出参数 : none
*********************************************************************************************************
*/
int main(void)
{
	OSInit();													//初始化uC/OS-II
	
	OSTaskCreateExt((void (*)(void *)) App_TaskStart,           /* Create the start task                                    */
		(void           *) 0,
		(OS_STK         *)&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
		(INT8U           ) APP_TASK_START_PRIO,
		(INT16U          ) APP_TASK_START_PRIO,
		(OS_STK         *)&App_TaskStartStk[0],
		(INT32U          ) APP_TASK_START_STK_SIZE,
		(void           *) 0,
		(INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

	OSStart();
}


/*
*********************************************************************************************************
*                                          App_TaskStart(void *p_arg)
*
* 描	述 : 系统起始任务，在此任务中，依次进行STM32底层IO口、通讯口配置，数据表结构初始化，外围芯片初始化，
*			 			 创建消息，创建功能任务，最后挂起本任务。
*
* 输入参数 : none
*
* 输出参数 : none
*********************************************************************************************************
*/
int8u msg[127] = "The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the l";
static	void	App_TaskStart(void *p_arg)
{
	volatile  uint32_t status;
	int8u bufp[7] = {0xc4,0x01,0x01,0x00,0x00,0x00,0x55};
	uint8_t *ptr;
	uint8_t err;
	
	(void) p_arg;

	
	//系统滴答安装
	OS_CPU_SysTickInit();   

	//初始化信号量
	app_eventcreate();

	//初始化其他任务
	app_taskcreate();
	


	//初始化基础硬件
	BSP_Init();
	
	dw1000_init();

{
	dwtconf temp;
	temp.config.chan = 3;
	temp.config.prf = DWT_PRF_16M;
	
	temp.config.txPreambLength = DWT_PLEN_256;
	temp.config.rxPAC = DWT_PAC16;
	
	temp.config.rxCode = 5;
	temp.config.txCode = 5;
	temp.config.nsSFD = 1;
	
	temp.config.dataRate = DWT_BR_850K;
	temp.config.smartPowerEn = 0;
	
	temp.config.phrMode = 0;
	
	temp.mode = LOCA_LISTENER;
	dw1000_config(&temp);

}
dwt_rxenable(0);

	  USBD_Init(&USB_OTG_dev,
            USB_OTG_FS_CORE_ID, 
            &USR_desc, 
            &USBD_CDC_cb, 
            &USR_cb);
//dw1000_intenable();
	//发送log
	//Shell_SendDatas(strlen(logstr), logstr);

	//改变自己优先级 变为工作指示灯
	//OSTaskDel(OS_PRIO_SELF);
	OSTaskChangePrio(OS_PRIO_SELF, OS_LOWEST_PRIO - 4);
	
	while(TURE)
	{
	//	OSTimeDlyHMSM(0, 0, 0, 10);
		//dwt_writetxdata(127,msg,0);
		//dwt_writetxfctrl(127,0);
		//dwt_starttx(DWT_START_TX_IMMEDIATE);
	//	bufp[1] ++;
		//status = dwt_read32bitreg(SYS_STATUS_ID);


		ptr = OSQPend(qmsg, 0, &err);
		if (err == OS_ERR_NONE)
		{
		VCP_fops.pIf_DataTx(ptr+1, ptr[0]);

		free(ptr);
		}
	}
}


/*
*********************************************************************************************************
*                                         app_eventcreate(void)
*
* 描	述 : 创建信号量
*
* 输入参数 : none
*
* 输出参数 : none
*********************************************************************************************************
*/
static void	app_eventcreate(void)
{
	//dwt_msg = (OS_EVENT *)OSMBoxCreate((void *)0);
	qmsg = (OS_EVENT*)OSQCreate(&qmsggrp[0], 10);
}

/*
*********************************************************************************************************
*                                         app_taskcreate(void)
*
* 描	述 : 创建其他任务
*
* 输入参数 : none
*
* 输出参数 : none
*********************************************************************************************************
*/
static void app_taskcreate(void)
{
	//创建DW1000数据接收任务	
	OSTaskCreateExt((void (*)(void *)) App_TaskStart,           /* Create the start task                                    */
		(void           *) 0,
		(OS_STK         *)&App_TaskStartStk[APP_TASK_START_STK_SIZE - 1],
		(INT8U           ) APP_TASK_START_PRIO,
		(INT16U          ) APP_TASK_START_PRIO,
		(OS_STK         *)&App_TaskStartStk[0],
		(INT32U          ) APP_TASK_START_STK_SIZE,
		(void           *) 0,
		(INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

}

/*
*********************************************************************************************************
*                                     assert_failed(uint8_t* file, uint32_t line)
*
* 描		述 :
*
* 输入参数 : none
*
* 输出参数 : none
*********************************************************************************************************
*/
void assert_failed(uint8_t* file, uint32_t line)
{
	
}
