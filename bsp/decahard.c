#include "includes.h"
#include "deca_device_api.h"
#include "bspdef.h"

#define dw_set_device_select()	GPIO_ResetBits(DW1000_DATA_PORT, DW1000_CS)
#define dw_reset_device_select()	GPIO_SetBits(DW1000_DATA_PORT, DW1000_CS)

/*! ------------------------------------------------------------------------------------------------------------------
 * Function: writetospi()
 *
 * Low level abstract function to write to the SPI
 * Takes two separate byte buffers for write header and write data
 * returns 0 for success, or -1 for error
 */
int writetospi
(
    uint16       headerLength,
    const uint8 *headerBuffer,
    uint32       bodylength,
    const uint8 *bodyBuffer
)
{

	int i=0;

    decaIrqStatus_t  stat ;

    stat = decamutexon() ;

    /* Wait for SPIx Tx buffer empty */
    //while (port_SPIx_busy_sending());

	dw_set_device_select();		//cs low

    for(i=0; i<headerLength; i++)
    {
			while (SPI_I2S_GetFlagStatus(DW1000_SPI, SPI_I2S_FLAG_TXE) == RESET){}
			SPI_I2S_SendData(DW1000_SPI, headerBuffer[i]);
    	//port_SPIx_send_data(headerBuffer[i]); //send data on the SPI
			while (SPI_I2S_GetFlagStatus(DW1000_SPI, SPI_I2S_FLAG_RXNE) == RESET){}
			SPI_I2S_ReceiveData(DW1000_SPI);
    }

    for(i=0; i<bodylength; i++)
    {
			while (SPI_I2S_GetFlagStatus(DW1000_SPI, SPI_I2S_FLAG_TXE) == RESET){}
			SPI_I2S_SendData(DW1000_SPI, bodyBuffer[i]);
		//port_SPIx_send_data(bodyBuffer[i]); //send data on the SPI
			while (SPI_I2S_GetFlagStatus(DW1000_SPI, SPI_I2S_FLAG_RXNE) == RESET){}
			SPI_I2S_ReceiveData(DW1000_SPI);
		//while (port_SPIx_no_data()); //wait for rx buffer to fill

		//port_SPIx_receive_data(); //this clears RXNE bit
	}

    dw_reset_device_select();  //CS high

    decamutexoff(stat) ;

    return 0;
} // end writetospi()


/*! ------------------------------------------------------------------------------------------------------------------
 * Function: readfromspi()
 *
 * Low level abstract function to read from the SPI
 * Takes two separate byte buffers for write header and read data
 * returns the offset into read buffer where first byte of read data may be found,
 * or returns -1 if there was an error
 */
int readfromspi
(
    uint16       headerLength,
    const uint8 *headerBuffer,
    uint32       readlength,
    uint8       *readBuffer
)
{

	int i=0;

    decaIrqStatus_t  stat ;

    stat = decamutexon() ;

    /* Wait for SPIx Tx buffer empty */
    //while (port_SPIx_busy_sending());

    dw_set_device_select();  //CS low

    for(i=0; i<headerLength; i++)
    {
			while (SPI_I2S_GetFlagStatus(DW1000_SPI, SPI_I2S_FLAG_TXE) == RESET){}
			SPI_I2S_SendData(DW1000_SPI, headerBuffer[i]);
    	//port_SPIx_send_data(headerBuffer[i]); //send data on the SPI
			while (SPI_I2S_GetFlagStatus(DW1000_SPI, SPI_I2S_FLAG_RXNE) == RESET){}
			SPI_I2S_ReceiveData(DW1000_SPI);
				
    }

    for(i=0; i<readlength; i++)
    {
			while (SPI_I2S_GetFlagStatus(DW1000_SPI, SPI_I2S_FLAG_TXE) == RESET){}
			SPI_I2S_SendData(DW1000_SPI, 0);
    	//port_SPIx_send_data(headerBuffer[i]); //send data on the SPI
			while (SPI_I2S_GetFlagStatus(DW1000_SPI, SPI_I2S_FLAG_RXNE) == RESET){}
			readBuffer[i] = SPI_I2S_ReceiveData(DW1000_SPI);
    }

    dw_reset_device_select();  //CS high

    decamutexoff(stat) ;

    return 0;
} // end readfromspi()

void Reset_DW1000(void)
{
	GPIO_ResetBits(DW1000_DATA_PORT, DW1000_RSTn);
}

void Set_DW1000(void)
{
	GPIO_SetBits(DW1000_DATA_PORT, DW1000_RSTn);
}

void dw1000_ioset(void)
{
	GPIO_InitTypeDef gpio_init;
	
	//dw1000 相关引脚初始化
	gpio_init.GPIO_Pin = DW1000_RSTn;				//DW1000复位引脚
	gpio_init.GPIO_Mode = GPIO_Mode_AIN;
	gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DW1000_DATA_PORT, &gpio_init);
	//GPIO_SetBits(DW1000_DATA_PORT, DW1000_RSTn);

	gpio_init.GPIO_Pin = DW1000_CS;					//DW1000片选引脚
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(DW1000_DATA_PORT, &gpio_init);
	GPIO_SetBits(DW1000_DATA_PORT, DW1000_CS);

	gpio_init.GPIO_Pin = DW1000_SCK | DW1000_MOSI;	//DW1000 SPI时钟和MOSI引脚
	gpio_init.GPIO_Mode = GPIO_Mode_AF_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DW1000_DATA_PORT, &gpio_init);

	gpio_init.GPIO_Pin = DW1000_MISO;				//DW1000 MISO引脚
	gpio_init.GPIO_Mode = GPIO_Mode_AF_OD;
	gpio_init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(DW1000_DATA_PORT, &gpio_init);

	gpio_init.GPIO_Pin = DW1000_WAK;				//DW1000唤醒引脚
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(DW1000_CTRL_PORT, &gpio_init);
	GPIO_ResetBits(DW1000_DATA_PORT, DW1000_WAK);

	gpio_init.GPIO_Pin = DW1000_IRQn;				//DW1000中断引脚
	gpio_init.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	gpio_init.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(DW1000_CTRL_PORT, &gpio_init);
	GPIO_EXTILineConfig(DW1000_INT_PORT, DW1000_INT_PIN);

	//SPI初始化
	BSP_DWTSpiSet(DW1000_SLOW_SPEED);
	SPI_NSSInternalSoftwareConfig(SPI1, SPI_NSSInternalSoft_Set);
	
}

void dw1000_intset(void)
{

	NVIC_InitTypeDef nvic_init;
		
	nvic_init.NVIC_IRQChannel = DW1000_EXTI_CHANNEL;
	nvic_init.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_init.NVIC_IRQChannelSubPriority = 0;
	nvic_init.NVIC_IRQChannelCmd = ENABLE;
	
	NVIC_Init(&nvic_init);
}

void dw1000_intenable(void)
{
	EXTI_InitTypeDef exti_init;	
	
	exti_init.EXTI_Line = DW1000_EXTI_LINE;
	exti_init.EXTI_Mode = EXTI_Mode_Interrupt;
	exti_init.EXTI_Trigger = EXTI_Trigger_Rising;
	exti_init.EXTI_LineCmd = ENABLE;
	
	EXTI_Init(&exti_init);
}

void Sleep(int16u cnt)
{
	OSTimeDlyHMSM(0, 0, 0, cnt);
}

void decamutexoff(decaIrqStatus_t s)
{
}

decaIrqStatus_t decamutexon(void)
{
	return 0;
}
