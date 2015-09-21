#include "includes.h"
#include "bspdef.h"
#include "bsp.h"


void BSP_Init(void)
{
	GPIO_InitTypeDef gpio_init;
	
	//使能各模块时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	//IO口初始化 
	gpio_init.GPIO_Pin = APP_LED_1 | APP_LED_2 | APP_LED_3 | APP_LED_4;
	gpio_init.GPIO_Mode = GPIO_Mode_Out_PP;
	gpio_init.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(APP_IO_PORT, &gpio_init);

}



void BSP_DWTSpiSet(int16u speed)
{
	SPI_InitTypeDef	spi_init;

	SPI_Cmd(SPI1, DISABLE);

	spi_init.SPI_Direction			= SPI_Direction_2Lines_FullDuplex;
	spi_init.SPI_Mode				= SPI_Mode_Master;
	spi_init.SPI_DataSize			= SPI_DataSize_8b;
	spi_init.SPI_CPOL				= SPI_CPOL_Low;
	spi_init.SPI_CPHA				= SPI_CPHA_1Edge;
	spi_init.SPI_NSS				= SPI_NSS_Soft;
	spi_init.SPI_BaudRatePrescaler	= speed;
	spi_init.SPI_FirstBit			= SPI_FirstBit_MSB;
	spi_init.SPI_CRCPolynomial		= 7;
	SPI_Init(SPI1, &spi_init);

	SPI_Cmd(SPI1, ENABLE);
}
