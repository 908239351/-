#include "timer.h"
#include "usart.h"
#include "lcd_task.h"
#include "uart2.h"
#include "SHT2x.H"
#include "adc.h"





/*******************************************************************************
* Function Name  : TIM3_Int_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void TIM3_Int_Init(u16 arr,u16 psc)
{
  TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʱ��ʹ��

	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  10Khz�ļ���Ƶ��  
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM3, ENABLE);
}
/*******************************************************************************
* Function Name  : TIM3_IRQHandler
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void co2_read_time(void);
void TIM3_IRQHandler(void)   //TIM3�ж�,1ms
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		
		PMinterface_receive_check();
		lcd_updata_time();
		co2_read_time();
		interface_receive_check();
		read_sht20_time();
		adc_check_time();
		power_check_time();
	}
}



void TIM1_Cap_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef  TIM1_ICInitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
  
	TIM1_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM1_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM1_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM1_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ 
	TIM1_ICInitStructure.TIM_ICFilter = 0x00;//IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM1, &TIM1_ICInitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM1,TIM_IT_Update|TIM_IT_CC1,ENABLE);//��������ж� ,����CC1IE�����ж�	
	
  TIM_Cmd(TIM1,ENABLE);
}




u8 tim1_cap_up_h = 0;
u8 tim1_cap_up_hl = 0;
u8 tim1_cap_sta = 0;
extern u8 main_driver_sta;
void TIM1_UP_IRQHandler(void)
{
	static u8 co2_hig_time = 0;
	TIM_ClearITPendingBit(TIM1, TIM_IT_Update); //����жϱ�־λ
	if(tim1_cap_sta == 1)
	{
		co2_hig_time = 0;
		tim1_cap_up_h ++;
		tim1_cap_up_hl ++;
	}
	else if(tim1_cap_sta == 2)
	{
		co2_hig_time = 0;
		tim1_cap_up_hl ++;
	}
	
	if(tim1_cap_up_hl > 20)
	{
		tim1_cap_sta = 0;
		tim1_cap_up_hl = 0;
		tim1_cap_up_h = 0;
	}
	
	if(tim1_cap_sta == 0)
	{
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8) != 0)
		{
			co2_hig_time ++;
			if(co2_hig_time > 20)
			{
				co2_hig_time = 0;
				CO2_data = 2000;
			}
		}
	}
}

u16 tim1_cap_h = 0;
u16 tim1_cap_hl = 0;
void TIM1_CC_IRQHandler(void)
{
	TIM_ClearITPendingBit(TIM1, TIM_IT_CC1); //����жϱ�־λ
	
	if(tim1_cap_sta == 0)//��һ�β���������
	{
		TIM1->CNT = 0;
		tim1_cap_sta = 1;
		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Falling);//����Ϊ�½��ز���
		tim1_cap_up_h = 0;
		tim1_cap_up_hl = 0;
	}
	else if(tim1_cap_sta == 1)//�ڶ��β����½���
	{
		tim1_cap_h = TIM1->CNT;
		tim1_cap_sta = 2;
		TIM_OC1PolarityConfig(TIM1,TIM_ICPolarity_Rising);//����Ϊ�����ز���
	}
	else if(tim1_cap_sta == 2)//����������
	{
		tim1_cap_hl = TIM1->CNT;
		tim1_cap_sta = 3;
	}
}
/*******************************************************************************
* Function Name  : co2_read_time_ct
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 co2_read_ct = 0;
void co2_read_time(void)
{
	co2_read_ct ++;
}
/*******************************************************************************
* Function Name  : tim1_pwm_co2_task
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 CO2_data;
u8 co2_sensor_type = 0;
void tim1_pwm_co2_task(void)
{
	u32 pwm_h = 0;
	u32 pwm_hl = 0;
	float ph = 0;
	float phl = 0;
	float pwm_co2 = 0;
	
	if(co2_read_ct > 500)
	{
		co2_read_ct = 0;
		if(tim1_cap_sta == 3)
		{
			if(tim1_cap_up_hl > 10)
			{
				co2_sensor_type = 1;//�ʢ�Ķ�����̼
			}
			pwm_hl = (tim1_cap_up_hl * 65536) + tim1_cap_hl;
			pwm_h = (tim1_cap_up_h * 65536) + tim1_cap_h;
			ph = (float)pwm_h;
			phl = (float)pwm_hl;
			if(co2_sensor_type == 0)
				pwm_co2 = (ph / phl) * 2000;
			else
				pwm_co2 = (ph / phl) * 5000;
			
			CO2_data = (u16)pwm_co2;
			
			tim1_cap_up_h = 0;
			tim1_cap_up_hl = 0;
			tim1_cap_sta = 0;
		}
	}
}








