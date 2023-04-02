/*****************************************************************************/
/**
* @file TongueCtrlGPIO.h
* @brief 舌苔控制GPIO定义
*
* 舌苔控制GPIO定义
*
* @author wangwei
* @date 2016/06/07 wangwei 初始版本
*/
/* Copyright (C) 2016 Uben Health Co.Ltd. All rights reserved.  */
/*****************************************************************************/

#ifndef TONGUECTRLUTIL_TONGUECTRLGPIO_H_
#define TONGUECTRLUTIL_TONGUECTRLGPIO_H_

#include <stdint-gcc.h>
#include <inttypes.h>

/*GPIO定义*/
typedef enum GPIO_NUM_EN
{
	GPIO0 = 0,
	GPIO1,
	GPIO2,
	GPIO3,
	GPIO4,
	GPIO5,
	GPIO6,
	GPIO7,
	GPIO8,
	GPIO9,
	GPIO10,
	GPIO11,
	GPIO12,
	GPIO13,
	GPIO14,
	GPIO15,
	GPIO16,
	GPIO17,
	GPIO18,
	GPIO19,
	GPIO20,
	GPIO21,
	GPIO22,
	GPIO23,
	GPIO24,
	GPIO25,
	GPIO26,
	GPIO27,
	GPIO_NUM
} GPIO_NUMBER;

/* LOW NOTE */
#define c 262	// L1
#define d 294	// L2
#define e 330	// L3
#define f 349	// L4
#define g 392	// L5
#define a1 440	// L6
#define b1 494	// L7

/* MIDDLE NOTE */
#define c1 523	// M1
#define d1 587	// M2
#define e1 659	// M3
#define f1 698	// M4
#define g1 784	// M5
#define a2 880	// M6
#define b2 988	// M7

/* HIGH NOTE */
#define c2 1047	// H1
#define d2 1175	// H2
#define e2 1319	// H3
#define f2 1397	// H4
#define g2 1568	// H5
#define a3 1760	// H6
#define b3 1976	// H7

/* BEATS */
#define T 500

/* Register Address*/
#define RPI_REG_BASE	 0x20000000
#define RPI_GPIO_BASE	 (RPI_REG_BASE + 0x200000)
#define RPI_BLOCK_SIZE	4096

/* GPIO Register Address */
#define RPI_GPFSEL0_OFFSET	0x00
#define RPI_GPFSEL1_OFFSET	0x04
#define RPI_GPFSEL2_OFFSET	0x08
#define RPI_GPFSEL3_OFFSET	0x0c

#define RPI_GPSET0_OFFSET	0x1c
#define RPI_GPCLR0_OFFSET	0x28
#define RPI_GPLEV0_OFFSET	0x34
#define RPI_GPPUD_OFFSET	0x94
#define RPI_GPPUDCLK0_OFFSET	0x98

/* GPIO Register Index */
#define RPI_GPFSEL0_INDEX	0
#define RPI_GPFSEL1_INDEX	1
#define RPI_GPFSEL2_INDEX	2
#define RPI_GPFSEL3_INDEX	3

#define RPI_GPSET0_INDEX	7
#define RPI_GPCLR0_INDEX	10
#define RPI_GPLEV0_INDEX	13
#define RPI_GPPUD_INDEX	37
#define RPI_GPPUDCLK0_INDEX	38

/* GPIO Functions	*/
#define	RPI_GPF_INPUT	0x00
#define	RPI_GPF_OUTPUT	0x01
#define	RPI_GPF_ALT0	0x04
#define	RPI_GPF_ALT1	0x05
#define	RPI_GPF_ALT2	0x06
#define	RPI_GPF_ALT3	0x07
#define	RPI_GPF_ALT4	0x03
#define	RPI_GPF_ALT5	0x02

/* PULLUP/PULLDOWN */
#define RPI_GPIO_PULLNONE	0x00
#define RPI_GPIO_PULLDOWN	0x01
#define	RPI_GPIO_PULLUP		0x02

/* GPIO Mask */
#define RPI_GPIO_P1MASK	(uint32_t) ((0x01<<2) | (0x01<<3) | (0x01<<4) | \
									(0x01<<7) | (0x01<<8) | (0x01<<9) | \
									(0x01<<10)| (0x01<<11)| (0x01<<14)| \
									(0x01<<15)| (0x01<<17)| (0x01<<18)| \
									(0x01<<22)| (0x01<<23)| (0x01<<24)| \
									(0x01<<25)| (0x01<<27)\
								   )
#define RPI_GPIO_P5MASK	(uint32_t) ((0x01<<28)| (0x01<<29)| 0x01<<30) | (0x01<<31))


/*GPIO初始化*/
void InitGPIO();
/*GPIO检测*/
void CheckGPIO();

int RpiGpioInit(void);
int RpiGpioDeInit(void);
int RpiGpioFunctionSet(int , uint32_t );
int RpiGpioPullControl(int, uint32_t);
void RpiGpioSet32( uint32_t, uint32_t);
uint32_t RpiGpioGet32( uint32_t );
void RpiGpioClear32( uint32_t, uint32_t );
void RpiGpioSetpin( int );
uint32_t RpiGpioGetpin( int );
void RpiGpioClearpin( int );


extern void DoGPIOCmd(int in_cmd, ...);
extern int SwitchLightOnOff(int in_category, int in_onOff);
extern int TurnOn(int in_iBrightness);
extern void Light(int pattern,int state);
#endif /* TONGUECTRLUTIL_TONGUECTRLGPIO_H_ */
