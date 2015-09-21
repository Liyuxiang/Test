
#include "includes.h"
#include "ranging.h"
#include "bspdef.h"
#include "deca_device_api.h"

extern OS_EVENT *dwt_msg;

const int8u startnow[3] = {0x55, 0x66, 0x55};
const int8u interval[3] = {0xff, 0xee, 0xff};
const int8u endtransmit[3] = {0x33, 0x22, 0x44};

int8u databuf[100];
typedef struct {
                uint8 PGdelay;

                //TX POWER
                //31:24     BOOST_0.125ms_PWR
                //23:16     BOOST_0.25ms_PWR-TX_SHR_PWR
                //15:8      BOOST_0.5ms_PWR-TX_PHR_PWR
                //7:0       DEFAULT_PWR-TX_DATA_PWR
                uint32 txPwr[2]; //
}tx_struct;
const tx_struct txSpectrumConfig[8] =
{
	//Channel 0 ----- this is just a place holder so the next array element is channel 1
    {
            0x0,   //0
            {
                    0x0, //0
                    0x0 //0
            }
    },
    //Channel 1
    {
			0xc9,   //PG_DELAY
            {
                    0x15355575, //16M prf power
                    0x07274767 //64M prf power
            }

    },
    //Channel 2
    {
            0xc2,   //PG_DELAY
            {
                    0x15355575, //16M prf power
                    0x07274767 //64M prf power
            }
    },
    //Channel 3
    {
            0xc5,   //PG_DELAY
            {
                    0x0f2f4f6f, //16M prf power
                    0x2b4b6b8b //64M prf power
            }
    },
    //Channel 4
    {
            0x95,   //PG_DELAY
            {
                    0x1f1f3f5f, //16M prf power
                    0x3a5a7a9a //64M prf power
            }
    },
    //Channel 5
    {
            0xc0,   //PG_DELAY
            {
                    0x0E082848, //16M prf power
                    0x25456585 //64M prf power
            }
    },
    //Channel 6 ----- this is just a place holder so the next array element is channel 7
    {
            0x0,   //0
            {
                    0x0, //0
                    0x0 //0
            }
    },
    //Channel 7
    {
            0x93,   //PG_DELAY
            {
                    0x32527292, //16M prf power
                    0x5171B1d1 //64M prf power
            }
    }
};

extern CDC_IF_Prop_TypeDef VCP_fops;

void reset_DW1000(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	// Enable GPIO used for DW1000 reset
	GPIO_InitStructure.GPIO_Pin = DW1000_RSTn;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DW1000_DATA_PORT, &GPIO_InitStructure);

	//drive the RSTn pin low
	GPIO_ResetBits(DW1000_DATA_PORT, DW1000_RSTn);

	//put the pin back to tri-state ... as input
	GPIO_InitStructure.GPIO_Pin = DW1000_RSTn;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DW1000_DATA_PORT, &GPIO_InitStructure);
}


extern OS_EVENT *qmsg;

void loca_rxcallback(const dwt_callback_data_t *rxd)
{
	
	//int8u temp;
	static int8u temp = 0;

	uint8_t *ptr;
	ptr = (uint8_t *)malloc(rxd->datalength + 1);
	*(ptr) = rxd->datalength & 0xff;

	dwt_readrxdata(ptr+1, rxd->datalength, 0);

	if (OSQPost(qmsg, ptr) != OS_ERR_NONE)
	{
		free(ptr);
	}


	
	
	//dwt_readrxdata(databuf,rxd->datalength,0);
	
//	pRecMsg temp;
//	int8u *buf;
//	if(rxd->event == DWT_SIG_RX_OKAY)
//	{
//		temp = (pRecMsg)malloc(sizeof(RecMsg));
//		if(temp != NULL)
//		{
//			buf = (int8u *)malloc(rxd->datalength);
//			if(buf != NULL)
//			{
//				dwt_readrxdata(buf, 1, 0);
				//dwt_readfromdevice(RX_FINFO_ID, 0, 4, temp->rxinfo);
				//dwt_readfromdevice(RX_FQUAL_ID, 0, 8, temp->rxqinfo);
				//dwt_readfromdevice(0x13, 0, 4, temp->rxtrckinterval);
				//dwt_readfromdevice(0x14, 0, 5, temp->rxtrckoffset);
			//	dwt_readfromdevice(0x15, 0, 14, temp->rxtimestamp);
				//dwt_readfromdevice(0x11, 0, temp->buflength, buf);
				
				/*
				VCP_fops.pIf_DataTx(startnow, 3);
				VCP_fops.pIf_DataTx((int8u *)&(rxd->datalength),2);
				
				VCP_fops.pIf_DataTx(interval, 3);
				
				VCP_fops.pIf_DataTx(temp->rxinfo, 4);
				
				VCP_fops.pIf_DataTx(interval, 3);
				
				VCP_fops.pIf_DataTx(temp->rxqinfo, 8);
				
				VCP_fops.pIf_DataTx(interval, 3);
				
				VCP_fops.pIf_DataTx(temp->rxtrckinterval, 4);
				
				VCP_fops.pIf_DataTx(interval, 3);
				
				VCP_fops.pIf_DataTx(temp->rxtrckoffset, 5);
				
				VCP_fops.pIf_DataTx(interval, 3);
				
				VCP_fops.pIf_DataTx(temp->rxtimestamp, 14);
				
				VCP_fops.pIf_DataTx(interval, 3);
				
				VCP_fops.pIf_DataTx(buf, rxd->datalength);
				VCP_fops.pIf_DataTx(endtransmit, 3);*/
				
				//temp->mesptr = buf;

				//发送邮箱
				/*if (OSMboxPost(dwt_msg,temp) != OS_ERR_NONE)
				{
					free(buf);
					free(temp);
				}*/
//				free(buf);
//			}//if(buf != NULL)

//			free(temp);
//		}//if(temp != NULL)
//	}//if(rxd->event == DWT_SIG_RX_OKAY)
//	dwt_readfromdevice(SY_STAT_ID, 1, 1, &temp);
//	temp = temp|0x40;
//	dwt_writetodevice(SY_STAT_ID, 1, 1, &temp);

	dwt_rxenable(0);
}//void loca_rxcallback(const dwt_callback_data_t *rxd)

int powertest(void)
{
	dwt_config_t    configData ;
	dwt_txconfig_t  configTx ;
	uint8_t pow;
	uint8_t msg[127] = "The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the lazy dog. The quick brown fox jumps over the l";

	//NOTE: SPI frequency must be < 3MHz
	//reduce the SPI speed before switching to XTAL
	//
	//	reset device
	//
	dwt_softreset();

	//
	//	configure channel paramters
	//
    configData.chan = 2 ;
    configData.rxCode =  3 ;
	configData.txCode = 3 ;
    configData.prf = DWT_PRF_16M ;
    configData.dataRate = DWT_BR_110K ;
    configData.txPreambLength = DWT_PLEN_1024 ;
    configData.rxPAC = DWT_PAC32 ;
    configData.nsSFD = 1 ;
	configData.smartPowerEn = 0;

	dwt_configure(&configData, DWT_LOADANTDLY | DWT_LOADXTALTRIM) ;

	configTx.PGdly = txSpectrumConfig[configData.chan].PGdelay ;

	if(configData.smartPowerEn == 0)
	{
		pow = txSpectrumConfig[configData.chan].txPwr[configData.prf - DWT_PRF_16M] & 0xFF ;
		configTx.power = 0x75757575;
	}
	else
	{
		configTx.power = txSpectrumConfig[configData.chan].txPwr[configData.prf - DWT_PRF_16M];
	}

	dwt_configuretxrf(&configTx);

	// the value here 0x1000 gives a period of 32.82 祍
	//this is setting 0x1000 as frame period (125MHz clock cycles) (time from Tx en - to next - Tx en)
	dwt_configcontinuousframemode(0x1000);

	dwt_writetxdata(127, (uint8 *)  msg, 0) ;
	dwt_writetxfctrl(127, 0);

    //to start the first frame - set TXSTRT
	dwt_starttx(DWT_START_TX_IMMEDIATE);

	//measure the power
	//Spectrum Analyser set:
	//FREQ to be channel default e.g. 3.9936 GHz for channel 2
	//SPAN to 1GHz
	//SWEEP TIME 1s
	//RBW and VBW 1MHz
	//measure channel power
	while(1)
	{
	}

	return DWT_SUCCESS ;
}

void loca_txcallback(const dwt_callback_data_t *txd)
{
	
}

loca_status dw1000_init(void)
{
	int32s result;
	int32u devid;
	
	dw1000_ioset();
	dw1000_intset();
	//首先硬件复位，
	
	BSP_DWTSpiSet(DW1000_SLOW_SPEED);
	
	devid = dwt_readdevid();
	
	if(DWT_DEVICE_ID != devid)
	{
		GPIO_ResetBits(DW1000_DATA_PORT, DW1000_CS);
		OSTimeDlyHMSM(0, 0, 0, 2);
		GPIO_SetBits(DW1000_DATA_PORT, DW1000_CS);
		OSTimeDlyHMSM(0, 0, 0, 10);
		
		devid = dwt_readdevid();
		if(DWT_DEVICE_ID != devid)
			return LOCA_ERROR;
		dwt_softreset();
	}
	
	reset_DW1000();
	
	result = dwt_initialise(DWT_LOADUCODE | DWT_LOADTXCONFIG | DWT_LOADANTDLY| DWT_LOADXTALTRIM) ;
	dwt_setinterrupt( DWT_INT_RFCG | DWT_INT_TFRS, 1);
	//dwt_setinterrupt( DWT_INT_RFCG | DWT_INT_TFRS, 1);
	dwt_setleds(2);
	
	dwt_configeventcounters(1);
	
	if (DWT_SUCCESS != result)
		return LOCA_ERROR;
	
	dwt_setcallbacks(loca_txcallback, loca_rxcallback);
	
	dwt_readfromdevice(EUI_64_ID, 0, 8, databuf);
	
	BSP_DWTSpiSet(DW1000_FAST_SPEED);
	devid = dwt_readdevid();
	if(DWT_DEVICE_ID != devid)
			return LOCA_ERROR;
	dw1000_intenable();
//	powertest();
	return LOCA_OK;
	
	/*
	reset_DW1000();
	
	//设置为低速
	BSP_DWTSpiSet(DW1000_SLOW_SPEED);
	//dwt_softreset();
	
	result = dwt_initialise(DWT_LOADNONE);
	
	
	//dwt_setinterrupt(SY_STAT_RFCG, ENABLE);
	
	dwt_setleds(2);
	
	if (DWT_SUCCESS != result)
		return LOCA_ERROR;
	dwt_setcallbacks(loca_txcallback, loca_rxcallback);
	
	dw1000_intenable();
	BSP_DWTSpiSet(DW1000_FAST_SPEED);
	return LOCA_OK;*/
}

void dw1000_config(const pdwtconf config_t)
{
	dwt_txconfig_t temp;
	int8u eui[8] = {0XA6,0x0F,0XFB,0X5D,0X20,0X29,0X01,0X10};
	int16u panid = 0x0523;
	int16u addr16 = 0x0223;
	temp.PGdly = 0xc9;
	temp.power = 0x75757575;
	dwt_configure(&(config_t->config), 3);
	dwt_setsmarttxpower(0);
	dwt_configuretxrf(&temp);
	

	
	OSTimeDlyHMSM(0, 0, 0, 5);
	
	dwt_enableframefilter(DWT_FF_NOTYPE_EN);
//	dwt_enableframefilter(DWT_FF_RSVD4_EN);
	dwt_seteui(eui);
	dwt_setpanid(panid);
	dwt_setaddress16(addr16);
//	dwt_enableframefilter(DWT_FF_RSVD4_EN);
//	 dwt_enableframefilter(DWT_FF_DATA_EN);
		
	dwt_setrxaftertxdelay(0);
	dwt_setautorxreenable(1);
	dwt_setrxtimeout(0);
	
	dwt_setrxmode(DWT_RX_NORMAL,0,0);
	
}


