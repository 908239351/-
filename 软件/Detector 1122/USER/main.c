#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "uart3.h"
#include "timer.h"
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "uart2.h"
#include "SHT2x.H"
#include "gc9306.h"
#include "key.h"
#include "lcd_task.h"
#include "adc.h"
#include "malloc.h"	
#include "sdio_sdcard.h"
#include "ff.h"  
#include "exfuns.h"  
#include "spi.h"
#include "w25qxx.h" 
#include "fontupd.h"
#include "key_task.h"







//ͨ�����ڴ�ӡSD�������Ϣ
void show_sdcard_info(void)
{
	switch(SDCardInfo.CardType)
	{
		case SDIO_STD_CAPACITY_SD_CARD_V1_1:printf("Card Type:SDSC V1.1\r\n");break;
		case SDIO_STD_CAPACITY_SD_CARD_V2_0:printf("Card Type:SDSC V2.0\r\n");break;
		case SDIO_HIGH_CAPACITY_SD_CARD:printf("Card Type:SDHC V2.0\r\n");break;
		case SDIO_MULTIMEDIA_CARD:printf("Card Type:MMC Card\r\n");break;
	}	
  printf("Card ManufacturerID:%d\r\n",SDCardInfo.SD_cid.ManufacturerID);	//������ID
 	printf("Card RCA:%d\r\n",SDCardInfo.RCA);								//����Ե�ַ
	printf("Card Capacity:%d MB\r\n",(u32)(SDCardInfo.CardCapacity>>20));	//��ʾ����
 	printf("Card BlockSize:%d\r\n\r\n",SDCardInfo.CardBlockSize);			//��ʾ���С
}  


u32 CpuID[3];
u32 Lock_Code;
char USER_ID[12];

/*******************************************************************************
* Function Name  : GetLockCode
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void GetLockCode(void)
{
	//��ȡCPUΨһID
	CpuID[0]=*(vu32*)(0x1ffff7e8);
	CpuID[1]=*(vu32*)(0x1ffff7ec);
	CpuID[2]=*(vu32*)(0x1ffff7f0);
	Lock_Code=(CpuID[0])+(CpuID[1])+(CpuID[2]>>3);
}

extern const u8 WIFI[];
/*******************************************************************************
* Function Name  : main
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u8 main_init_ok = 0;
int main(void)
{
	u8 res = 0;
	u8 err_ct = 0;
//	u16 i = 0;
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	/**************************************************************** ���ܸ���IOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);
	/***************************************************************** JTAG-DP ʧ�� + SW-DP ʹ�� */
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	power_key_floating();
	delay_ms(1000);delay_ms(1000);
	power_vcc_en_init();
	
	usb_power_init();
	USART1_Init(9600);
	TIM1_Cap_Init(0xffff,63);
	USART2_Init(9600);
	TIM3_Int_Init(1000,63);
	LCD_INIT_GC9306();
	delay_ms(1000);
	SHT2x_Init();
	Adc_Init();
	mem_init();				//��ʼ���ڴ��
	//W25QXX_Init();				//��ʼ��W25Q128
	lcd_show_main();
	KEY_Init();
	main_init_ok = 1;
	
	if(W25QXX_Init() == 0)
	{
		exfuns_init();				//Ϊfatfs��ر��������ڴ�  
		f_mount(fs[0],"0:",1); 		//����SD�� 
		f_mount(fs[1],"1:",1); 		//����FLASH.
		while(font_init()) 			//����ֿ�
		{
			printf("init ok\r\n");
			err_ct = 0;
			while(SD_Init())			//���SD��
			{
				printf("SD Card Failed!\r\n");
				delay_ms(500);
				err_ct ++;
				if(err_ct > 5)
					break;
			}
			err_ct = 0;
			printf("SD Card OK!\r\n");
			printf("Font Updating...!\r\n");
			res = update_font(20,110,16,"0:");//�����ֿ�
			while(res)//����ʧ��		
			{
				printf("Font Update Failed = %d\r\n",res);
				delay_ms(500);
				err_ct ++;
				if(err_ct > 5)
					break;
			}
			printf("Font Update Success!\r\n");
			delay_ms(1500);
		}
	}
//	GC9306_DrawPoint(40,100,0XFFFF);
//	for(i = 0;i < 256;i ++)
//	{
//		yanse_read = LCD_ReadPoint(40,100,i);
//		printf("yanse0x%x = %x\r\n",i,yanse_read);
//	}

//	GC9306_DrawPoint(10,30,0xffff);
//	yanse_read = LCD_ReadPoint(10,30);
//	printf("\r\n");
//	//printf("0x%x = %x\r\n",i,yanse_read);
//	
//	GC9306_DrawPoint(40,100,0X0000);
//	yanse_read = LCD_ReadPoint(40,100);
//	printf("\r\n");
//	//printf("0x%x = %x\r\n",i,yanse_read);
//	
//	GC9306_DrawPoint(80,200,0XFFFF);
//	yanse_read = LCD_ReadPoint(80,200);
//	printf("\r\n");
//	//printf("0x%x = %x\r\n",i,yanse_read);
//	
//	GC9306_DrawPoint(111,65,0X0000);
//	yanse_read = LCD_ReadPoint(111,65);
//	printf("\r\n");
//	//printf("0x%x = %x\r\n",i,yanse_read);
//	
//	GC9306_DrawPoint(258,120,0X6405);
//	yanse_read = LCD_ReadPoint(258,120);
//	printf("\r\n");
//	//printf("0x%x = %x\r\n",i,yanse_read);
	while(1)
	{
		key_run_main_task();
		lcd_updata_task();
		tim1_pwm_co2_task();
		read_sht20_task();
		adc_check_task();
		power_check_task();
		lcd_curve_task();
		
		
	}
}









