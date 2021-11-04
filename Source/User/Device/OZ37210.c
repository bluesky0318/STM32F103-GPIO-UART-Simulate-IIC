#include "OZ37210.h"

void WriteWord(u8 addr ,u16 udata)
{
	u8 temp = 0;
	IIC_Start();
	IIC_WriteByte(0x52);//写操作时地址为0xA0
	IIC_WaitAck();
	IIC_WriteByte(addr);
	IIC_WaitAck();
	IIC_WriteByte((u8)(udata>>8));
	IIC_WaitAck();
	IIC_WriteByte((u8)udata);
	IIC_WaitAck();
	temp = calc_crc_write(0x52,addr,udata);	
	IIC_WriteByte(temp);
	IIC_Stop();
	DelayMs(5);//注意这里延时！一定要调试
}

u16 ReadWord(u8 addr)
{
	u8 bh = 0,bl = 0,crc =0 ;
	u16 temp=0;
	IIC_Start();
	IIC_WriteByte(0x52);//写地址
	IIC_WaitAck();
	IIC_WriteByte(addr);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_WriteByte(0x53);//读地址
	IIC_WaitAck(); //一定要等待
	bh=IIC_ReadByte(1);//需要应答
	bl=IIC_ReadByte(1);//需要应答
	crc=IIC_ReadByte(0);//需要应答
	IIC_Stop();
	DelayMs(5);//注意这里延时！一定要调试
	temp = (u16)(bh << 8);
	temp |= bl;
	return temp;
}



