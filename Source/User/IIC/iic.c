#include "iic.h"

//初始化IIC
void IIC_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		
    GPIO_InitStruct.GPIO_Pin = IIC_SCL | IIC_SDA;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_OD ;   //开漏输出
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IIC_PORT, &GPIO_InitStruct);

    IIC_Stop();
}

void IIC_SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};  
	GPIO_InitStruct.GPIO_Pin = IIC_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;	
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
}
 
void IIC_SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0}; 	
	GPIO_InitStruct.GPIO_Pin = IIC_SDA;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;	
	GPIO_Init(IIC_PORT, &GPIO_InitStruct);
	
}


//产生IIC起始信号
//SCL为高电平时SDA由高变低
/*
 SCL:	      \___
 SDA:   \_______
*/
void IIC_Start(void)
{
		IIC_SDA_OUT();
    IIC_SDA_1;
    IIC_SCL_1;
    DelayUs(5);
    IIC_SDA_0;
    DelayUs(5);
    IIC_SCL_0;
}

//产生IIC停止信号
//SCL为高电平时SDA由低变高
//IIC空闲时SCL和SDA均处于高电平
/*
SCL:
SDA:__/
*/
void IIC_Stop(void)
{
	  IIC_SDA_OUT();
    IIC_SDA_0;
    IIC_SCL_1;
    DelayUs(5);
    IIC_SDA_1;
}

//等待应答信号
//返回值:1,接受应答失败
//       0,接受应答成功
u8 IIC_WaitAck(void)
{
    u8 errCount = 0;
    u8 ack = 0;
		
	  IIC_SDA_IN();
    IIC_SDA_1;
    DelayUs(5);
    IIC_SCL_1;
    DelayUs(5);

    while(IIC_READ_SDA())
    {
        errCount++;

        if(errCount > 250)
        {
            ack = 1;
            break;
        }
    }

    IIC_SCL_0;
    return ack;
}

//产生应答ACK
//SCL为高电平时SDA为低电平时表示应答
/*
SCL:     \____
SDA:_______/
*/
void IIC_Ack(void)
{
    IIC_SDA_0;
		IIC_SDA_OUT();
    DelayUs(5);
    IIC_SCL_1;
    DelayUs(5);
    IIC_SCL_0;
    DelayUs(5);
    IIC_SDA_1;        //释放SDA
}

//产生非应答NACK
//SCL为高电平时SDA为高电平时表示非应答
/*
SCL:   \__
SDA:
*/
void IIC_NAck(void)
{
    IIC_SDA_1;
		IIC_SDA_OUT();
    DelayUs(5);
    IIC_SCL_1;
    DelayUs(5);
    IIC_SCL_0;
    DelayUs(5);
}

//IIC发送一个字节
/*
SCL:_  _/?\__  _/??\__  _/??\__  _/??\__  _/??\__  _/??\__  _/??\__  _/??\__
SDA:--  ------------  --------------  --------------  --------------  --------------  --------------  --------------  --------------
*/
void IIC_WriteByte(u8 txd)
{
    u8 i;
	  IIC_SDA_OUT();
    IIC_SCL_0;

    for(i = 0; i < 8; i++)
    {
        (txd & 0x80) ? IIC_SDA_1 : IIC_SDA_0;
        txd <<= 1;

        DelayUs(1);
        IIC_SCL_1;
        DelayUs(5);
        IIC_SCL_0;
        DelayUs(1);
    }
    IIC_SDA_1;
}

//读1个字节,ack=1时,发送ACK,ack=0,发送NACK
/*
SCL:??\__  /??\__  /??\__  /??\__  /??\__  /??\__  /??\__  /??\__
SDA:==========  ============  ============  ============  ============  ============  ============  ============
*/
u8 IIC_ReadByte(u8 ack)
{
    u8 i, rcv = 0;
	  IIC_SDA_IN();
    for(i = 0; i < 8; i++)
    {
        rcv <<= 1;
        IIC_SCL_1;
        DelayUs(5);

        if(IIC_READ_SDA())
        {
            rcv++;
        }

        IIC_SCL_0;
        DelayUs(5);
    }

    ack ? IIC_Ack() : IIC_NAck();

    return rcv;
}

u8 crc8_calc(u8 *pdata, u16 n)
{
	u8 crc = 0;
	u8 crcdata;
	u16 i, j;

	for (i = 0; i < n; i++)
	{
		crcdata = pdata[i];
		for (j = 0x80; j != 0; j >>= 1)
		{
			if ((crc & 0x80) != 0)
			{
				crc <<= 1;
				crc ^= 0x07;
			}
			else
				crc <<= 1;

			if ((crcdata & j) != 0)
				crc ^= 0x07;
		}
	}
	return crc;
}

u8 calc_crc_read(u8 slave_addr, u8 reg_addr, u16 data)
{
	u8 pdata[5];

	pdata[0] = slave_addr;
	pdata[1] = reg_addr;
	pdata[2] = (u8)(slave_addr | 0x01);
	pdata[3] = (u8)(data>>8);
	pdata[4] = (u8)(data);

	return crc8_calc(pdata, 5);
}

u8 calc_crc_write(u8 slave_addr, u8 reg_addr, u16 data)
{
	u8 pdata[4];

	pdata[0] = slave_addr; ;
	pdata[1] = reg_addr;
	pdata[2] = (u8)(data>>8);
	pdata[3] = (u8)(data);

	return crc8_calc(pdata, 4);
}

