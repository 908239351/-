#include "SHT2x.H"
#include "stdio.h"
#include "string.h"



unsigned char ack_cnt_SHT20=0;
unsigned char XINHAO_SHT20=0;
unsigned char OK_SHT20=0;



void IIC_Init_SHT20(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);  
  SCL_OUT_SHT20();
	SDA_OUT_SHT20();
  IIC_SCL_SHT20=1;
	IIC_SDA_SHT20=1;
}
//����IIC��ʼ�ź�
void IIC_Start_SHT20(void)
{
   SDA_OUT_SHT20();     //sda�����
   IIC_SDA_SHT20=1;    
   IIC_SCL_SHT20=1;
   delay_us(4);
   IIC_SDA_SHT20=0;//START:when CLK is high,DATA change form high to low 
   delay_us(4);
   IIC_SCL_SHT20=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	 delay_us(4);
}  
//����IICֹͣ�ź�
void IIC_Stop_SHT20(void)
{
	SDA_OUT_SHT20();//sda�����
	IIC_SCL_SHT20=0;
	IIC_SDA_SHT20=0;//STOP:when CLK is high DATA change form low to high
  delay_us(4);
  IIC_SCL_SHT20=1; 
	delay_us(1);
  IIC_SDA_SHT20=1;//����I2C���߽����ź�
  delay_us(4);  
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uchar IIC_Wait_Ack_SHT20(void)
{
	uchar ucErrTime=0;
	SDA_IN_SHT20();      //SDA����Ϊ����  
	IIC_SDA_SHT20=1;delay_us(1);  
	IIC_SCL_SHT20=1;delay_us(1);
	while(READ_SDA_SHT20)
	{
		ucErrTime++;
		if(ucErrTime>250)
		 {
			 IIC_Stop_SHT20();
			 return 1;
		 }
	}
	IIC_SCL_SHT20=0;//ʱ�����0

	return 0;  
} 
//����ACKӦ��
void IIC_Ack_SHT20(void)
{
	IIC_SCL_SHT20=0;
	SDA_OUT_SHT20();
	IIC_SDA_SHT20=0;
	delay_us(20);
	IIC_SCL_SHT20=1;
	delay_us(2);
	IIC_SCL_SHT20=0;
}
//������ACKӦ��    
void IIC_NAck_SHT20(void)
{
	IIC_SCL_SHT20=0;
	SDA_OUT_SHT20();
	IIC_SDA_SHT20=1;
	delay_us(5);
	IIC_SCL_SHT20=1;
	delay_us(5);
	IIC_SCL_SHT20=0;
}     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��  
void IIC_Send_Byte_SHT20(uchar txd)
{                        
    uchar t;   
    SDA_OUT_SHT20();    
    IIC_SCL_SHT20=0;//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {              
        IIC_SDA_SHT20=(txd&0x80)>>7;
        txd<<=1;  
				delay_us(2);   //��TEA5767��������ʱ���Ǳ����
				IIC_SCL_SHT20=1;
				delay_us(2);   //��TEA5767��������ʱ���Ǳ����
				IIC_SCL_SHT20=0;
				delay_us(2);   //��TEA5767��������ʱ���Ǳ����
    }  
}    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uchar IIC_Read_Byte_SHT20(unsigned char ack) //
{
    unsigned char i,receive=0;
    SDA_IN_SHT20();//SDA����Ϊ����
    for(i=0;i<8;i++ )
      {
        IIC_SCL_SHT20=0;  
				delay_us(2); 
        IIC_SCL_SHT20=1;
        receive<<=1;
        if(READ_SDA_SHT20)receive++;
				delay_us(2);
      }  
    if (!ack)
        IIC_NAck_SHT20();//����nACK
    else
        IIC_Ack_SHT20(); //����ACK  
    return receive;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////
SHT2x_data SHT20;
u8 SHT2x_Init(void)
{
	u8 err;
   IIC_Init_SHT20();
   err = SHT2x_SoftReset();
	return err;
}

u8 SHT2x_SoftReset(void) //SHT20�����λ
{
	u8 err=0;
	IIC_Start_SHT20();
  IIC_Send_Byte_SHT20(0x80);
	err = IIC_Wait_Ack_SHT20();
	IIC_Send_Byte_SHT20(0xFE);
	err = IIC_Wait_Ack_SHT20();
	IIC_Stop_SHT20();
	return err;
}
float SHT2x_GetTempPoll(void)
{
    float TEMP;
    u8 ack, tmp1, tmp2;
    u16 ST;
    u16 i=0;
    IIC_Start_SHT20();				//����IIC��ʼ�ź�
    IIC_Send_Byte_SHT20(I2C_ADR_W);			//IIC����һ���ֽ� 
		ack = IIC_Wait_Ack_SHT20();	
		IIC_Send_Byte_SHT20(TRIG_TEMP_MEASUREMENT_POLL);
		ack = IIC_Wait_Ack_SHT20();

    do {
        delay_ms(10);               
        IIC_Start_SHT20();				//����IIC��ʼ�ź�
        IIC_Send_Byte_SHT20(I2C_ADR_R);	
			  i++;
			  ack = IIC_Wait_Ack_SHT20();
			  if(i==10)break;
       } while(ack!=0);
    tmp1 = IIC_Read_Byte_SHT20(1);
    tmp2 = IIC_Read_Byte_SHT20(1);
		 IIC_Read_Byte_SHT20(0);
    IIC_Stop_SHT20();
    
    ST = (tmp1 << 8) | (tmp2 << 0);
	
    ST &= ~0x0003;
    TEMP = ((float)ST * 0.00268127) - 46.85; //T = -46.85 + 175.72 * ST/2^16

    return (TEMP);	  
}

float SHT2x_GetHumiPoll(void)
{
    float HUMI;
    u8 ack, tmp1, tmp2;
    u16 SRH;
		u16 i=0;
    
    IIC_Start_SHT20();				//����IIC��ʼ�ź�
    IIC_Send_Byte_SHT20(I2C_ADR_W);			//IIC����һ���ֽ� 
		ack = IIC_Wait_Ack_SHT20();	
		IIC_Send_Byte_SHT20(TRIG_HUMI_MEASUREMENT_POLL);
		ack = IIC_Wait_Ack_SHT20();    
    do {
       delay_ms(10 );               
        IIC_Start_SHT20();				//����IIC��ʼ�ź�
        IIC_Send_Byte_SHT20(I2C_ADR_R);	
			  i++;
			  ack = IIC_Wait_Ack_SHT20();
			  if(i==10)break;
    } while(ack!=0);
    
    tmp1 = IIC_Read_Byte_SHT20(1);
   
    tmp2 = IIC_Read_Byte_SHT20(1);
     IIC_Read_Byte_SHT20(0);
    IIC_Stop_SHT20();
    
    SRH = (tmp1 << 8) | (tmp2 << 0);
    SRH &= ~0x0003;
    HUMI = (float)(-6.0 + 125.0/65536 * (float)SRH);//RH = -6 + 125 * SRH/2^16

    return (HUMI);
}

u16 sht20_time = 0;
void read_sht20_time(void)
{
	++ sht20_time ;
}
float tem_t;
int tem_h;
void read_sht20_task(void)
{
	if(sht20_time > 1000)
	{
		sht20_time = 0;
		
		tem_t = SHT2x_GetTempPoll();//��ȡSHT20 �¶�
    tem_h = (int)SHT2x_GetHumiPoll();//��ȡSHT20 ʪ��
		if(tem_h > 99)
			tem_h = 99;
	}
}









