#include "adc.h"
#include "stdio.h"
#include "lcd.h"

/*******************************************************************************
* Function Name  : usb_power_init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void usb_power_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : power_Read_Input
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 power_Read_Input(void)
{
  if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9))
    return 1;
  else
    return 0;
}
/*******************************************************************************
* Function Name  : Adc_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Adc_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1,ENABLE);//ʹ��ADC1ͨ��ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		//ģ����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	RCC->APB2ENR|=1<<9;    //ADC1ʱ��ʹ��	  
	RCC->APB2RSTR|=1<<9;   //ADC1��λ
	RCC->APB2RSTR&=~(1<<9);//��λ����	    
	RCC->CFGR&=~(3<<14);   //��Ƶ��������	
	//SYSCLK/DIV2=12M ADCʱ������Ϊ12M,ADC���ʱ�Ӳ��ܳ���14M!
	//���򽫵���ADC׼ȷ���½�! 
	RCC->CFGR|=2<<14;      	 

	ADC1->CR1&=0XF0FFFF;   //����ģʽ����
	ADC1->CR1|=0<<16;      //��������ģʽ  
	ADC1->CR1&=~(1<<8);    //��ɨ��ģʽ	  
	ADC1->CR2&=~(1<<1);    //����ת��ģʽ
	ADC1->CR2&=~(7<<17);	   
	ADC1->CR2|=7<<17;	   //�������ת��  
	ADC1->CR2|=1<<20;      //ʹ�����ⲿ����(SWSTART)!!!	����ʹ��һ���¼�������
	ADC1->CR2&=~(1<<11);   //�Ҷ���	 
	ADC1->SQR1&=~(0XF<<20);
	ADC1->SQR1&=0<<20;     //1��ת���ڹ��������� Ҳ����ֻת����������1 		
	
	//����ͨ��1�Ĳ���ʱ��
 ADC1->SMPR2&=0X00000000;//ͨ��0,1,2,3,4,5,6,7,8,9����ʱ�����
 
 ADC1->SMPR2|=7<<27; //ͨ��9 239.5����,��߲���ʱ�������߾�ȷ�� 
 ADC1->SMPR2|=7<<24; //ͨ��8 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR2|=7<<21; //ͨ��7 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR2|=7<<18; //ͨ��6 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR2|=7<<15; //ͨ��5 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR2|=7<<12; //ͨ��4 239.5����,��߲���ʱ�������߾�ȷ��
// ADC1->SMPR2|=7<<9; //ͨ��3 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR2|=7<<6; //ͨ��2 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR2|=7<<3; //ͨ��1 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR2|=7<<0; //ͨ��0 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR1&=0XFFFC0000;//ͨ��10,11,12,13,14,15����ʱ�����
// ADC1->SMPR1|=7<<15; //ͨ��15 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR1|=7<<12; //ͨ��14 239.5����,��߲���ʱ�������߾�ȷ��
// ADC1->SMPR1|=7<<9; //ͨ��13 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR1|=7<<6; //ͨ��12 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR1|=7<<3; //ͨ��11 239.5����,��߲���ʱ�������߾�ȷ�� 
// ADC1->SMPR1|=7<<0; //ͨ��10 239.5����,��߲���ʱ�������߾�ȷ��

	ADC1->CR2|=1<<0;	    //����ADת����	 
	ADC1->CR2|=1<<3;        //ʹ�ܸ�λУ׼  
	while(ADC1->CR2&1<<3);  //�ȴ�У׼���� 			 
  //��λ��������ò���Ӳ���������У׼�Ĵ�������ʼ�����λ��������� 		 
	ADC1->CR2|=1<<2;        //����ADУ׼	   
	while(ADC1->CR2&1<<2);  //�ȴ�У׼����
}
/*******************************************************************************
* Function Name  : Get_Adc
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 Get_Adc(u8 ch)   
{
  //����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);//ADC1,ADCͨ��,����ʱ��Ϊ239.5����
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);//ʹ��ָ����ADC1�����ת����������	
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));//�ȴ�ת������
	return ADC_GetConversionValue(ADC1);//�������һ��ADC1�������ת�����
}
/*******************************************************************************
* Function Name  : Get_Adc_Average
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val += Get_Adc(ch);
		delay_ms(10);
	}
	return temp_val/times;
}
//���ADCֵ
//ch:ͨ��ֵ 11
u16 VOC_AD = 0,VOC_data = 0,VOC_ZERO = 500;
float VOC_fdata = 0;
void Get_Adc_voc(void)   
{
	u8 i,k;
	static u16 tempunm[10],tempsum[10],temp;
//	//����ת������	  		 
//	ADC1->SQR3&=0XFFFFFFE0;//��������1 ͨ��ch
//	ADC1->SQR3|=0;		  			    
//	ADC1->CR2|=1<<22;       //��������ת��ͨ�� 
//	while(!(ADC1->SR&1<<1));//�ȴ�ת������	 

	for(i=0;i<9;i++)
	{
		tempunm[i]=tempunm[i+1];
	}
	tempunm[9] = Get_Adc(9);	
	for(i=0;i<10;i++)
	{
		tempsum[i]=tempunm[i];
	}
	for(k=0;k<9;k++)
	{
		for(i=0;i<9-k;i++)
		{
			if(tempsum[i]>tempsum[i+1])
			{
				temp=tempsum[i];
				tempsum[i]=tempsum[i+1];
				tempsum[i+1]=temp;
			}
		}
	}
	VOC_AD=(tempsum[4]+tempsum[5])/2;	
	if(VOC_AD > VOC_ZERO)
		VOC_data=(float)(VOC_AD - VOC_ZERO)/8;	
	else
		VOC_data=0;

	if(VOC_data>999)
		VOC_data=999;
	VOC_fdata = (float)VOC_data / 100;
}
/*******************************************************************************
* Function Name  : power_check_time
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 adc_voc_time = 500;
void adc_check_time(void)
{
	adc_voc_time ++;
}
/*******************************************************************************
* Function Name  : power_check_task
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void adc_check_task(void)
{
	if(adc_voc_time > 2000)
	{
		adc_voc_time = 0;
		Get_Adc_voc();
		//adcc = Get_Adc_Average(4,10);
		//printf("ad=%d \r\n",adcc);
	}
}

/*******************************************************************************
* Function Name  : power_check_time
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 power_get_ct = 500;
void power_check_time(void)
{
	power_get_ct ++;
}
/*******************************************************************************
* Function Name  : power_check_task
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
extern const u8 BAT0[];
extern const u8 BAT1[];
extern const u8 BAT2[];
extern const u8 BAT3[];
extern const u8 BAT4[];
extern const u8 BAT5[];
float powerbuf[20] = {3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8,3.8};
void power_check_task(void)
{
	u8 power_updata_flag = 0;
	u8 i = 0;
	float power_aver = 0;
	static u8 power_ct = 0;
	u16 adc_dat = 0;
	float power = 0;
	
	//if((lcd_yemian == 0) || (lcd_yemian == 3))
	{
		if(power_get_ct > 1000)
		{
			power_get_ct = 0;
			adc_dat = Get_Adc_Average(8,10);
			power = adc_dat;
			power = ((power / 4096) * 6.6);
			
			powerbuf[power_ct] = power;
			power_ct ++;
			if(power_ct > 19)
				power_ct = 0;
			
			for(i = 0;i < 20;i ++)
			{
				power_aver += powerbuf[i];
			}
			power = power_aver / 20;
			
			if(power_Read_Input() == 1)
			{
				power_updata_flag = 5;
			}
			else
			{
				if(power > 3.9)	{power_updata_flag = 4;}
				else 	if(power > 3.8)	{power_updata_flag = 3;}
				else 	if(power > 3.7)	{power_updata_flag = 2;}
				else  if(power > 3.6){power_updata_flag = 1;}
				else 	{power_updata_flag = 0;}
			}
			
			if(power_updata_flag == 5)
				Draw_pic(270,4,48,2, BAT5,WHITE,BLACK);
			else if(power_updata_flag == 4)
				Draw_pic(270,4,48,2, BAT4,WHITE,BLACK);
			else if(power_updata_flag == 3)
				Draw_pic(270,4,48,2, BAT3,WHITE,BLACK);
			else if(power_updata_flag == 2)
				Draw_pic(270,4,48,2, BAT2,WHITE,BLACK);
			else if(power_updata_flag == 1)
				Draw_pic(270,4,48,2, BAT1,WHITE,BLACK);
			else if(power_updata_flag == 0)
				Draw_pic(270,4,48,2, BAT0,WHITE,BLACK);
		}
	}
}















