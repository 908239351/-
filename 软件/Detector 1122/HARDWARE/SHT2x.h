#ifndef __SHT20_H__
#define __SHT20_H__



#include "stm32f10x_gpio.h"
#include "sys.h"
#include "delay.h"



#define uchar unsigned char
#define uint  unsigned  int
#define ulint unsigned int

extern unsigned char ack_cnt_SHT20;
extern unsigned char OK_SHT20;

#define delay_nus 2
//IO��������
//#define SDA_IN_SHT20()  {GPIOA->CRH&=0XFFF0FFFF;GPIOA->CRH|=8<<16;}
//#define SDA_OUT_SHT20() {GPIOA->CRH&=0XFFF0FFFF;GPIOA->CRH|=3<<16;}
//#define SCL_OUT_SHT20() {GPIOB->CRL&=0XFFF0FFFF;GPIOB->CRL|=3<<4*4;}

////IO��������  
//#define IIC_SCL_SHT20    PBout(4) //SCL
//#define IIC_SDA_SHT20    PAout(12) //SDA  
//#define READ_SDA_SHT20   PAin(12)  //����SDA 

#define SDA_IN_SHT20()  {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=8<<24;}
#define SDA_OUT_SHT20() {GPIOB->CRH&=0XF0FFFFFF;GPIOB->CRH|=3<<24;}

#define SCL_OUT_SHT20() {GPIOB->CRH&=0X0FFFFFFF;GPIOB->CRH|=3<<4*7;}

//IO��������  
#define IIC_SCL_SHT20    PBout(15) //SCL
#define IIC_SDA_SHT20    PBout(14) //SDA  
#define READ_SDA_SHT20   PBin(14)  //����SDA 

#define  I2C_ADR_W 0x80  //SHT2X IIC ����ַ
#define  I2C_ADR_R 0x81  //SHT2X IIC д��ַ

typedef enum {
    TRIG_TEMP_MEASUREMENT_HM   = 0xE3, // ���� �¶� ���� ����������   ����
    TRIG_HUMI_MEASUREMENT_HM   = 0xE5, // ���� ʪ�� ���� ����������   ����
    TRIG_TEMP_MEASUREMENT_POLL = 0xF3, // ���� �¶� ���� ���Ǳ������� ����
    TRIG_HUMI_MEASUREMENT_POLL = 0xF5, // ���� ʪ�� ���� ���Ǳ������� ����
    USER_REG_W                 = 0xE6, // д�Ĵ�������
    USER_REG_R                 = 0xE7, // ���Ĵ�������
    SOFT_RESET                 = 0xFE  // �����λ����
} SHT2xCommand;

typedef enum {
    SHT2x_RES_12_14BIT         = 0x00, //RH=12bit, T=14bit ����Ĭ�ϵ�ֵ   ���ǲ����޸�
    SHT2x_RES_8_12BIT          = 0x01, //RH= 8bit, T=12bit
    SHT2x_RES_10_13BIT         = 0x80, //RH=10bit, T=13bit
    SHT2x_RES_11_11BIT         = 0x81, //RH=11bit, T=11bit
    SHT2x_RES_MASK             = 0x81  //Mask for res. bits (7,0) in user reg.
} SHT2xResolution;

typedef enum {
    SHT2x_HEATER_ON            = 0x04, //heater on
    SHT2x_HEATER_OFF           = 0x00, //heater off
    SHT2x_HEATER_MASK          = 0x04  //Mask for Heater bit(2) in user reg.
} SHT2xHeater;

typedef struct{
    float TEMP_POLL;
    float HUMI_POLL;    
} SHT2x_data;

extern SHT2x_data SHT20;


extern float tem_t;
extern int tem_h;

u8 SHT2x_Init(void); 
u8    SHT2x_SoftReset(void);  //SHT20�����λ
float SHT2x_GetTempPoll(void);//��ȡSHT20 �¶�
float SHT2x_GetHumiPoll(void);//��ȡSHT20 ʪ��
void read_sht20_time(void);
void read_sht20_task(void);



#endif










