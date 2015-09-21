
#ifndef _PLATDEFINE_H_
#define _PLATDEFINE_H_

#ifdef __cplusplus
extern "C" {
#endif


#define APP_IO_PORT		GPIOC

#define APP_CONFIG_1	GPIO_Pin_0
#define APP_CONFIG_2	GPIO_Pin_1
#define APP_CONFIG_3	GPIO_Pin_2
#define APP_CONFIG_4	GPIO_Pin_3
#define APP_CONFIG_5	GPIO_Pin_4
#define APP_CONFIG_6	GPIO_Pin_5
#define APP_LED_1		GPIO_Pin_6
#define APP_LED_2		GPIO_Pin_7
#define APP_LED_3		GPIO_Pin_8
#define APP_LED_4		GPIO_Pin_9


#define DW1000_DATA_PORT	GPIOA

#define DW1000_RSTn	GPIO_Pin_0
#define DW1000_CS	GPIO_Pin_4
#define DW1000_SCK	GPIO_Pin_5
#define DW1000_MISO	GPIO_Pin_6
#define DW1000_MOSI	GPIO_Pin_7


#define DW1000_CTRL_PORT	GPIOB

#define DW1000_WAK	GPIO_Pin_0
#define DW1000_IRQn	GPIO_Pin_5

#define DW1000_INT_PORT	GPIO_PortSourceGPIOB
#define DW1000_INT_PIN	GPIO_PinSource5
#define DW1000_EXTI_LINE	EXTI_Line5
#define DW1000_EXTI_CHANNEL EXTI9_5_IRQn


#define DW1000_SPI	SPI1

//定义DW1000 快速和慢速时的速度
#define DW1000_SLOW_SPEED	SPI_BaudRatePrescaler_32
#define DW1000_FAST_SPEED	SPI_BaudRatePrescaler_8


#ifdef __cplusplus
}
#endif

#endif
