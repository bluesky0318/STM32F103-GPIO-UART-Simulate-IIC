//ͷ�ļ�����
#include "usually.h"
#include "usart.h"
#include "OZ37210.h"

//��������

//��������
void Init_LED(void);
void Init_NVIC(void);
void Delay_Ms(uint16_t time);
void Delay_Us(uint16_t time);
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: main
** ��������: ���������
** �������������������ʾ�˴����շ����� �������ǲ����жϷ�ʽ
				�������������ϵ�����ֵ ���Գ����ն˽���ʮ����������������ֵ
				���磺1��Ӧʮ������49��2��Ӧ��50��a��Ӧ��97������㶮������
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
int main(void)
{
		u16 tmp = 0;
    SystemInit();					//ϵͳʱ������
    Init_NVIC();					//�ж�������ע�ắ��
    Init_LED();						//����������������
    Init_Usart();					//������������
		DelayInit();
		IIC_Init();           //IIC inital
    Usart_Configuration(115200);	//�������� ���ò�����Ϊ115200
		printf("Beging to read the OZ37210 \n\r");
    while(1)
    {
			  tmp |= 0x8000;
				WriteWord(0x18,tmp);
				tmp = ReadWord(0x18);
				printf("Addr = 0x18,0x%04x\n\r",tmp);
			  tmp |= 0x8001;
				WriteWord(0x18,tmp);
				tmp = ReadWord(0x18);
				printf("Addr = 0x18,0x%04x\n\r",tmp);
        LED1 = ~LED1;
        Delay_Ms(1000);				 //LED1��˸��ϵͳ��������
    }
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_LED
** ��������: LED IO��������
** ������������
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_LED(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE);	//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;										//����LED�˿ڹҽӵ�6��12��13�˿�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//ͨ���������
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
    GPIO_Init(GPIOC, &GPIO_InitStructure);				   	//���ݲ�����ʼ��GPIOD�Ĵ���
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Init_NVIC
** ��������: ϵͳ�ж�����
** ������������
** ��  ����: Dream
** �ա�  ��: 2011��5��14��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Init_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;			//����һ��NVIC������ṹ�����

    #ifdef  VECT_TAB_RAM  							//���������ַѡ��

    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);  	//��0x20000000��ַ��Ϊ���������ַ(RAM)
    #else

    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0); //��0x08000000��ַ��Ϊ���������ַ(FLASH)
    #endif

    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж��� Ϊ2

    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;			//���ô���1Ϊ�ж�Դ
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//����ռ�����ȼ�Ϊ2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  	//���ø����ȼ�Ϊ0
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  	//ʹ�ܴ���1�ж�
    NVIC_Init(&NVIC_InitStructure);							  	//���ݲ�����ʼ���жϼĴ���
}

/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Delay_Ms_Ms
** ��������: ��ʱ1MS (��ͨ���������ж�����׼ȷ��)
** ����������time (ms) ע��time<65535
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Ms(uint16_t time)  //��ʱ����
{
    uint16_t i, j;

    for(i = 0; i < time; i++)
        for(j = 0; j < 10260; j++);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
** ��������: Delay_Ms_Us
** ��������: ��ʱ1us (��ͨ���������ж�����׼ȷ��)
** ����������time (us) ע��time<65535
** ��  ����: Dream
** �ա�  ��: 2011��6��20��
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
void Delay_Us(uint16_t time)  //��ʱ����
{
    uint16_t i, j;

    for(i = 0; i < time; i++)
        for(j = 0; j < 9; j++);
}
/*:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
End:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D:-D
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::*/
