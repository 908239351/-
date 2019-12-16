#include "key.h"
#include "delay.h"
#include "stdio.h"
#include "gc9306.h"
#include "key_task.h"








/*******************************************************************************
* Function Name  : power_key_floating
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void power_key_floating(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}
/*******************************************************************************
* Function Name  : power_vcc_en_init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void power_vcc_en_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	VCC_EN = 1;
}
/*******************************************************************************
* Function Name  : Power_Key_Init
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void Power_Key_Init(void)
//{
//	EXTI_InitTypeDef EXTI_InitStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;
//	GPIO_InitTypeDef  GPIO_InitStructure;
//	
//	power_vcc_en_init();
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
// 	GPIO_Init(GPIOB, &GPIO_InitStructure);

//	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource12);

//	EXTI_InitStructure.EXTI_Line = EXTI_Line12;
//	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
//	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
//	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
//	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���KEY1���ڵ��ⲿ�ж�ͨ��
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;					//�����ȼ�1 
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
//	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
//}
//������ʼ������
void KEY_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_12;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
}

//Ӳ���������루ΪӦ�����ж��������׼����
//��ս������ļ�Ϊ���������֧��16����KeyS_Type����Ϊu32���֧��32����
KeyS_Type GetHalKeyCode(void)
{
	KeyS_Type ktmp=0;
	if(KEY0_IN) 	ktmp|=1<<KB_KEY0;
	if(KEY1_IN) 	ktmp|=1<<KB_KEY1;
	if(KEY2_IN) 	ktmp|=1<<KB_KEY2;
	if(WKUP_IN) 	ktmp|=1<<KB_WKUP;
	return ktmp;
}

//********************************************************************************
static KeyS_Type KeyStable=0; //�����ȶ�(����������)�ļ�̬(����ǰ)

KeyS_Type Trg=0;  		//ȫ�ֱ��������б��ζ���ʱ�İ�������״̬
KeyS_Type Cont=0; 		//ȫ�ֱ��������б��ζ���ʱ��ʵʱ��̬
u16 KeyTime=0;  //ȫ�ֱ��������б��ζ���ʱ��ǰ��̬������ʱ��

/******************** �û�Ӧ�ó��򰴼��жϽӿں���(����ݾ��������޸�) *********************************/
//������GetAndSaveKey()��SYSTICK�е��ã����밴�����У���ѭ������ʱ��ʹ��Read_A_Key()
//�����ȶ��ļ�ֵ���û�����ֱ�Ӵ������ȶ���ļ�ֵ���ɡ�

//����Ӧ�İ����������£�
//��ͨ�����¼���Ч�����õȰ����ͷ�
//�������������ɿ�����Ч
//˫�������ٵ������Σ����ε���֮��ʱ�䲻����SHORT_TICKS��
//���������³����涨��ʱ��LONG_TICKS������������������ͨ����������ü��һ��ʱ�����һ�μ�ֵ
//��ϣ�˫����ϣ���ʵ������Ҳ��ͬ��ʵ�֣�
/**********************************************************************************/
//��ʹ����ϰ����������ж�ʱ�����º궨���ɾ��
#define KEY0_ON 						(0x0001<<KB_KEY0)  //�궨�壺����δ�ͷ�ֵ
#define KEY1_ON 						(0x0001<<KB_KEY1)
#define KEY2_ON 						(0x0001<<KB_KEY2)
#define WKUP_ON 						(0x0001<<KB_WKUP)
#define KEY0_PRESSED 				(Trg==KEY0_ON)  //�궨�壺��������ֵ
#define KEY1_PRESSED 				(Trg==KEY1_ON)
#define KEY2_PRESSED 				(Trg==KEY2_ON)
#define WKUP_PRESSED 				(Trg==WKUP_ON)

u8 Get_Key(void)
{
	u8 i,keyp=0;
/*�������ж������趨���ɣ�
	ȫ�ֱ���Trg�������˶�Ӧ�����Ĵ���״̬����ĳ���������º�����ֻ��һ�ζ�ȡ����ӦλΪ1;
	ȫ�ֱ���Cont�������˵�ǰ������״̬�����ڰ��µĶ�ӦλΪ1�������ɿ��Ķ�ӦλΪ0;
	��ȫ�ֱ���KeyTime���棬��¼�˵�ǰ��̬������ʱ��
*/
	
	//�����ǰ����жϣ��û��ɸ�����Ҫ������ӻ�ɾ�ģ�ע�͵��Ĳ���Ҳ�ɸ�����Ҫ��Ϊ�ο����ʹ�ã�
	
//ע�⣺����ǰ����ж��������иߵ����ȼ���һ���������㼴�̷��أ�����ִ�к�����䡣
	if((Cont==(WKUP_ON+KEY0_ON)) && KEY0_PRESSED)	{ //WKUP+KEY0��ϰ������Ȱ���WKUP�ٰ���KEY0��
		Get_Key_State(KB_CLR); //��λ״̬������ֹ�������������(��������״̬���г�ͻʱ����ô˾�)
		return WKUP_PLUSKEY0_PRES;} 

//������ʹ��״̬���õ��жϵ�����˫�������������ֵȼ���	
	for(i=0;i<KeyNumMax;i++)
	  {
			keyp=Get_Key_State(i);	
			if(keyp) return keyp; 
		}
	return keyp;
}


//
//����Ԥ�������:  �������ǿʵʱ����Ҫ�Ľ�Ҫ���������Ŷ�����ִ�У���Ч���е�����ƻص�����
//����������ʵ����Ŀ������������־���ʵ��ģʽѡ��
//ģʽһ�������������ռ��ʱ��̣�����ʱ��䡢��ѭ���ȣ�������Ҫ��ǿʵʱ��������԰����еİ���������������
//        ������ѭ�������봦�����ˣ��൱��ʹ���жϷ���ķ�ʽ����ȫ��������
//ģʽ�����԰�������ʵʱ��Ҫ�󲻸ߣ��ܹ�����һ������İ�����Ӧʱ�ӣ����԰����а������������ѭ���в�ѯ��Ӧ��
//        ��ʱ���������Լ�Ϊreturn Get_Key();
//ģʽ����ǰ���ֵ����з�������ǿʵʱ����Ҫ��������ļ���ֱ��������дִ�д��룬����������ʱ�ļ�������ѭ����ѯ������ʽ��������ʾ��
u8 Key_PrePro(void)
{
	//return Get_Key(); //ģʽ��ʱ���������򻯵�ֻ����һ�䣬���¿�ȫ��ɾ����
	u8 newkeytmp,ret=0;
	newkeytmp=Get_Key();
	switch(newkeytmp)
	{
		case KEY_EVENT(KB_KEY1,DOUBLE_CLICK)://KEY1˫����ִ������ͬʱ��ת������Ϊʾ����
			//LED0=!LED0;LED1=!LED1; //�������Ʒ�ת
      break;
    default:
			ret=newkeytmp;
	}
	return ret;
}






//**********************  ����Ϊͨ�ú�����һ�㲻Ҫ�޸�  ****************************
u8 New_KeyBuff[KEYBUFFSIZE];
u8 pNewKey=0;

void GetAndSaveKey(void)//��������SYSTICK���ã��ں�̨����������м�ֵ����밴��������
{
	u8 newkeytmp;
	if(KeyTime>=LONG_TICKS && KEY_RELEASED)
		{//���̳�ʱ�����ã�ֱ�ӷ��أ����󲿷�ʱ�������������״̬���˾ٽ�����ʡCPU��Դ��
			KeyTime=LONG_TICKS;//�˾��ֹKeyTime���(KeyTime��ɨ����������)
			return; 
	  }
	Trg=KeyStable & (KeyStable ^ Cont); //�������ж�������,��ʵ����ֻ�д��У�ʹ��Trg��ĳ���������º�����ֻ��һ�ζ�ȡ����ӦλΪ1;
	Cont=KeyStable;
	newkeytmp=Key_PrePro();//�Ӽ�Ԥ��������ж���ֵ
	if(newkeytmp)//������µļ�ֵ
	{
		New_KeyBuff[pNewKey++]=newkeytmp;//���밴��������(pNewKey��Զָ����һ��λ��)
		if(pNewKey==KEYBUFFSIZE)pNewKey=0;//����������ѭ��ʹ��
	}
}

//��ȡ����ֵ������ѭ�����á�
//�Ӱ��������ж�ȡ����ֵ���޼��򷵻�0
u8 Read_A_Key(void)
{
	static u8 pReadKey=0;//����ָ��
	if(pReadKey==KEYBUFFSIZE)pReadKey=0;//����������ѭ��ʹ��
	if(pReadKey==pNewKey) return 0;//���Ѿ�ȡ��������0
	return New_KeyBuff[pReadKey++];
}


//����ɨ�躯����һ����Systick�жϷ��������5msһ�ε�ʱ����ĵ��ô˺���
//�����˼�������Ӧ��Ƶɨ���ʩ���ڼ��������ȶ��ڼ䣨�������ڼ䣩ɨ��Ƶ�ʽ����Լ���CPU��Դռ��
//�ú�����Ӱ��ȫ�ֱ�����������������ȶ���ֵ̬KeyStable���ۼ�ʱ��KeyTime
void Key_Scan_Stick(void)
{
	KeyS_Type KeyValTemp;
	static KeyS_Type KeyValTempOld=0;
	static u16 debounce_cnt=0;
	static u16 debouncing=0;
	
	KeyTime++;//���ȶ���̬�������޼���״̬�£�ȫ�ֱ���KeyTime�ǳ������ӵ�
	if((!debouncing) && (KeyTime%NORMAL_SCAN_FREQ))//�������ڼ����ۼƼ�ʱ����6�ı���(��6*5��30ms��ɨ��һ��)
		return;	//��ɨ�����ֱ�ӷ��أ�����ɵ���NORMAL_SCAN_FREQΪ���������������40��120msɨ��һ�ζ����ⲻ��ġ�
	
	KeyValTemp=GetHalKeyCode();//ɨ����̣��õ�ʵʱ��ֵ���ϲ������ɴ�16����ֵ��������ӦλΪ1�ɿ�Ϊ0��;
	
	if(KeyValTemp!=KeyStable) //�����ǰֵ�����ھɴ�ֵ������ֵ�б仯
	{
		debouncing=1;//��ʾΪ������
		if(!(KeyValTemp^KeyValTempOld))//�����ʱֵ���ȶ������¼�ֵ�б仯��
		{
			debounce_cnt=0;
			KeyValTempOld=KeyValTemp;
		}
		else//��ʱֵ�ȶ�
		{
		 if(++debounce_cnt >= DEBOUNCE_TICKS) 
		 {
			KeyStable = KeyValTemp;//��ֵ����Ϊ��ǰֵ.
			debounce_cnt = 0;//����λ����������.
			KeyTime=1; //�¼�ֵ�ۼ�ʱ����λΪ1��ʱ�䵥λ
			debouncing=0;//�����ڽ���
		 }
	  } 
	} 
	else //�����ֵ�Ե��ھɴ�ֵ��
	{ //��λ������������ע�⣺ֻҪ������;����һ�μ�ֵ���ھɴ�ֵ����������������0��ʼ���¼�����.
		debounce_cnt = 0;
		KeyValTempOld=KeyValTemp;
	}
}

//***************************************************************
//  ���뵥����˫�����������������ֵȹ��ܼ��Ŀ���ɾ�����º���
//***************************************************************
/**���๦�ܰ���״̬��
  * ��ڲ�����ʵ�尴����ţ�����ΪKEYCLR���ڸ�λ״̬����
  * ���أ���ֵ�������¼�ֵ����(KeyNum+2)*10+���¼�ֵ; ��������0.
  */
#define THE_KEY_IS_OFF			(!(Cont & KeyOnCode))
#define THE_KEY_IS_ON				(Cont & KeyOnCode)
#define THE_KEY_PRESSED			((Trg & KeyOnCode) && (Cont & KeyOnCode))

u8 Get_Key_State(u8 KeyNum)
{
	//��������״̬(ÿ�ֽڵ���λ��state����4λ��repeat)
	static u8 KeyState[KeyNumMax];
	
	KeyS_Type KeyOnCode;
	u8 i,state,repeat,event=0;
	if(KeyNum==KB_CLR) //����ΪKB_CLRʱ�����������а�������״̬
	{
		for(i=0;i<KeyNumMax;i++) KeyState[i]=0;
		return 0;
	}
	KeyOnCode=(KeyS_Type)1<<KeyNum;
	state=KeyState[KeyNum]&0x0f; //ȡ��Ӧ�ļ���״ֵ̬
	repeat=KeyState[KeyNum]>>4;
	
	if(Trg && (Trg!=KeyOnCode)) state=0; //��������������״̬��0
	
	switch (state) 
	{
	case 0://״̬0������ȫ�ɿ�
		if(THE_KEY_PRESSED) 
			{	//���ΰ�����������Ч
			event = (u8)PRESS_DOWN;
			repeat = 1;
			state = 1;//���ΰ�����Ч�����״̬1
			} 
			else //��Ч��ƽ��������Ϊ�ɿ�״̬
			  event = (u8)NONE_PRESS;
	  break;

	case 1://״̬1�����ΰ�����������Ч
		if(THE_KEY_IS_OFF) { //��⵽�����ɿ�		
			event = (u8)PRESS_UP;
			state = 2;//�������º��ɿ������״̬2
      }
		 else if(KeyTime > LONG_TICKS) {//����δ�ɿ����ҳ���ʱ���Ѿ�����LONG_TICKS
			event = (u8)LONG_RRESS_START;
			state = 5;//�������������������״̬5
		  }
		break;

	case 2://״̬2���������º����ɿ�
		if(THE_KEY_PRESSED) { //�ٴμ�⵽����   
			event = (u8)PRESS_DOWN;
			repeat++;//�ذ������ۼ�
			if(repeat == 2) state = 3;//����ذ���������2,����״̬3
			} 
		else //�����ɿ�
      {
		   if(KeyTime > SHORT_TICKS)  
			  {//����ɿ�ʱ�䳬��SHORT_TICKS����һ�ΰ�������
				 state = 0;//�򰴼��ɿ�ʱ�䳬��SHORT_TICKS����λ��״̬0	
				 if(repeat==1) event=(u8)SINGLE_CLICK;//����Ϊ1������´��������¼�
			   else if(repeat==2) event=(u8)DOUBLE_CLICK;//�ذ�����Ϊ2������´���˫���¼�
			  }
			} //����������ɿ�ʱ�仹û�г���SHORT_TICKS����Ȼά��״̬2���д������ж�		
		break;

	case 3://״̬3�����¡��ɿ����ְ��£����ڶ��ΰ��£�				
		if(THE_KEY_IS_OFF)  //��⵽�����ɿ�
			{
				event = (u8)PRESS_UP;
			  if(KeyTime < SHORT_TICKS) state = 2; //�ɿ�ʱ��С��SHORT_TICKS���ص�״̬2 
			  else state = 0;//�ɿ�ʱ�����SHORT_TICKS������״̬0
		  }//����������԰�����ͣ����״̬3�ȴ��ɿ����ڶ��ΰ���û�г���֮˵��
		break;

	case 5://״̬5�����������Ѿ�����
		if(THE_KEY_IS_ON)  //��������Գ�������				
	     event = (u8)LONG_PRESS_HOLD;//���������ְ����¼�����
		else { //��������ɿ�
			event = (u8)PRESS_UP;
			state = 0; //��ָ���״̬0
		  }
		break;
	}
	KeyState[KeyNum]=state; //������Ӧ�ļ���״ֵ̬
	KeyState[KeyNum]+= repeat<<4;
	if(event>=(u8)PRESS_DOWN) //�趨ֻ������⹦�ܼ����޸Ĵ˴����������/�ɿ���һ���¼���
//	if(event) //��������¼�		
		return KEYOUT_BASE_DEF+event;
	else return 0;
}
u8 Flag300ms = 0;//300ms���ı�־

void key_run_main_task(void)
{
	vu8 key = 0;
	key = Read_A_Key();	//�õ���ֵ
	  				   
	switch(key)
	{				 
		//����һ�㰴�����ԣ����¼��������ã���
		case KEY_EVENT(KB_KEY0,PRESS_DOWN):	//KEY2���¼���Ч������LED0��ת	
			//LCD_ShowString(10,120,24,"1",BLACK,WHITE);
			key1_short_event();
			break;
		case KEY_EVENT(KB_KEY1,PRESS_DOWN):	//KEY0���¼���Ч������LED1��ת	 
			//LCD_ShowString(10,120,24,"2",BLACK,WHITE);
			key2_short_event();
			break;
		case KEY_EVENT(KB_KEY2,PRESS_DOWN):	//KEY0���¼���Ч������LED1��ת	 
			//LCD_ShowString(10,120,24,"3",BLACK,WHITE);
			key3_short_event();
			break;
		
		//������������������������ԣ����磨������������
		case KEY_EVENT(KB_WKUP,SINGLE_CLICK):	//KEY1�����������ٰ������ɿ���	 
			//LCD_ShowString(10,120,24,"4",BLACK,WHITE);
			key_up_short_event();
			break;
		case KEY_EVENT(KB_WKUP,LONG_RRESS_START):
			key_up_long_event();
			
			break;
//		case KEY_EVENT(KB_WKUP,DOUBLE_CLICK):	//WKUP��˫���������������Σ�
//			printf("KEY4 double kell\r\n");
//			break;
//		case WKUP_PLUSKEY0_PRES:	//WKUP+KEY0��ϰ������Ȱ���WKUP�ٰ���KEY0��
//			printf("KEY0 + KEY4\r\n");
//			break;
//		case KEY_EVENT(KB_WKUP,LONG_RRESS_START):	//������WKUP
//			printf("KEY4 long\r\n");
//			break;
//		case KEY_EVENT(KB_WKUP,LONG_PRESS_HOLD):	//������KEY1��ÿ300ms����һ�Σ�ʵ�����Ƶ��ӱ�ĳ���"+"ʱ�������ٵ���ʱ��Ĺ��ܣ�
//			if(Flag300ms)
//			{
//				Flag300ms=0;
//				//LCD_ShowString(105,107,24,"POWER OFF",WHITE,BLACK);;
//			} //����LED0ÿ300ms��תһ��
//			break;
	}
}

