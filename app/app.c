/*
* Copyright (c) 2014,֣������ӿƼ����޹�˾
* All rights reserved.
* 
* �ļ����ƣ�main.c
* �ļ���ʶ������ϵͳ�����ļ�
* ժ    Ҫ�����������������ϵͳ����������Ӳ����ʼ���Ȳ�����
* 
* ��ǰ�汾��0.1
* ��    �ߣ���ѩ��
* ������ڣ�2014.03.18
*/

#include "includes.h"

/*
*********************************************************************************************************
*                                      �궨��
*********************************************************************************************************
*/


/*
*********************************************************************************************************
*                                      ��������
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
*                                     �����ջ����
*********************************************************************************************************
*/

OS_STK	App_TaskStartStk[APP_TASK_START_STK_SIZE];
OS_STK	App_TaskShellStk[APP_TASK_SHELL_STK_SIZE];


/*
*********************************************************************************************************
*                                       ��������
*********************************************************************************************************
*/
static	void	App_TaskStart(void	*p_arg);
//static	void	App_TaskShell(void	*p_arg);
/*
*********************************************************************************************************
*                                       ��������
*********************************************************************************************************
*/
static void	app_eventcreate(void);
static void app_taskcreate(void);

/*
*********************************************************************************************************
*                                      �����¼�
*********************************************************************************************************
*/
OS_EVENT *qmsg;		//dw1000 ������Ϣ���䶨��
void	*qmsggrp[10];


/*
*********************************************************************************************************
*                                                main()
*
* ��		�� : ����������
*
* ������� : none
*
* ������� : none
*********************************************************************************************************
*/
int main(void)
{
	OSInit();													//��ʼ��uC/OS-II
	
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
* ��	�� : ϵͳ��ʼ�����ڴ������У����ν���STM32�ײ�IO�ڡ�ͨѶ�����ã����ݱ�ṹ��ʼ������ΧоƬ��ʼ����
*			 			 ������Ϣ������������������������
*
* ������� : none
*
* ������� : none
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

	
	//ϵͳ�δ�װ
	OS_CPU_SysTickInit();   

	//��ʼ���ź���
	app_eventcreate();

	//��ʼ����������
	app_taskcreate();
	


	//��ʼ������Ӳ��
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
	//����log
	//Shell_SendDatas(strlen(logstr), logstr);

	//�ı��Լ����ȼ� ��Ϊ����ָʾ��
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
* ��	�� : �����ź���
*
* ������� : none
*
* ������� : none
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
* ��	�� : ������������
*
* ������� : none
*
* ������� : none
*********************************************************************************************************
*/
static void app_taskcreate(void)
{
	//����DW1000���ݽ�������	
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
* ��		�� :
*
* ������� : none
*
* ������� : none
*********************************************************************************************************
*/
void assert_failed(uint8_t* file, uint32_t line)
{
	
}
