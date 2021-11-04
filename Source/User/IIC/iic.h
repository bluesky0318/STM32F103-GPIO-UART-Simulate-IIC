#ifndef __IIC_H
#define __IIC_H

#include "stm32f10x_gpio.h"
#include "delay.h"

#define IIC_SCL   GPIO_Pin_6
#define IIC_SDA   GPIO_Pin_7
#define IIC_PORT  GPIOB


#define IIC_SCL_1    GPIO_SetBits(IIC_PORT, IIC_SCL)            /* SCL = 1 */
#define IIC_SCL_0    GPIO_ResetBits(IIC_PORT, IIC_SCL)        /* SCL = 0 */

#define IIC_SDA_1    GPIO_SetBits(IIC_PORT, IIC_SDA)            /* SDA = 1 */
#define IIC_SDA_0    GPIO_ResetBits(IIC_PORT, IIC_SDA)        /* SDA = 0 */

#define IIC_READ_SDA()    GPIO_ReadInputDataBit(IIC_PORT, IIC_SDA)    /* ?SDA???? */

void IIC_Init(void);
void IIC_SDA_OUT(void);
void IIC_SDA_IN(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_WaitAck(void);
void IIC_Ack(void);
void IIC_NAck(void);
void IIC_WriteByte(u8 txd);
u8 IIC_ReadByte(u8 ack);
u8 calc_crc_read(u8 slave_addr, u8 reg_addr, u16 data);
u8 calc_crc_write(u8 slave_addr, u8 reg_addr, u16 data);


#endif
