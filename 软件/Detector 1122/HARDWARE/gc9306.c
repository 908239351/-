#include "gc9306.h"
#include "delay.h"
#include "font.h"
#include "stdio.h"



//����LCD��Ҫ����
//Ĭ��Ϊ����
_lcd_dev lcddev;





/*******************************************************************************
* Function Name  : LCD_GPIOInit
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_PD01, ENABLE);  
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_1);
	//GPIO_SetBits(GPIOB,GPIO_Pin_13);
	GPIO_SetBits(GPIOC,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_14|GPIO_Pin_15);
	GPIO_SetBits(GPIOD,GPIO_Pin_0|GPIO_Pin_1);
	
}
/*******************************************************************************
* Function Name  : LCD_GPIOInit
* Description    : д�Ĵ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WrCom(u16 data)
{
	LCD_CS_CLR;
	LCD_RD_SET;
	
	LCD_RS_CLR;
	LCDDATA_ODR_Addr(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
}
/*******************************************************************************
* Function Name  : LCD_WR_REG
* Description    : д�Ĵ�������
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WR_REG(u16 data)
{
	LCD_CS_CLR;
	LCD_RD_SET;
	
	LCD_RS_CLR;
	LCDDATA_ODR_Addr(data>>8);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCDDATA_ODR_Addr(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
}
/*******************************************************************************
* Function Name  : LCD_GPIOInit
* Description    : д���ݺ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WrDat(u16 data)
{
	LCD_CS_CLR;
	LCD_RD_SET;
	
	LCD_RS_SET;
	LCDDATA_ODR_Addr(data);
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
}
/*******************************************************************************
* Function Name  : LCD_WR_DATAX
* Description    : д���ݺ���
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WR_DATAX(u16 x_data) 
{
	LCD_CS_CLR;
	LCD_RD_SET;
	
	LCD_RS_SET;
	LCDDATA_ODR_Addr(x_data>>8);
	LCD_WR_CLR;
	LCD_WR_SET;
	LCDDATA_ODR_Addr(x_data);
	LCD_WR_CLR;
	LCD_WR_SET;
	
	LCD_CS_SET;
}
/*******************************************************************************
* Function Name  : LCD_WriteReg
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteReg(u16 LCD_Reg,u16 LCD_RegValue)
{	
	LCD_WR_REG(LCD_Reg);  
	LCD_WR_DATAX(LCD_RegValue);	    		 
}
/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_SetCursor(u16 Xstar,u16 Xend, u16 Ystar,u16 Yend)
{
	LCD_WR_REG(lcddev.setxcmd);  
	LCD_WR_DATAX(Xstar);
	LCD_WR_DATAX(Xend);
	LCD_WR_REG(lcddev.setycmd);  
	LCD_WR_DATAX(Ystar);
	LCD_WR_DATAX(Yend);
	LCD_WrCom(lcddev.wramcmd);
}
/*******************************************************************************
* Function Name  : LCD_WriteRAM_Prepare
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
	LCD_WrCom(lcddev.wramcmd);
}
/*******************************************************************************
* Function Name  : LCD_Clear
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Clear(u16 color)
{
	u32 index = 0;      
	u32 totalpoint = lcddev.width;
	totalpoint *= lcddev.height; 			//�õ��ܵ���
	LCD_SetCursor(0,319,0,239);	//���ù��λ�� 	
	for(index=0;index<totalpoint;index++)
	{
		LCD_WR_DATAX(color);
	}
}
void LCD_Clear1(u16 color)
{
	u32 index = 0;      
	u32 totalpoint = lcddev.width;
	totalpoint *= lcddev.height; 			//�õ��ܵ���
	LCD_SetCursor(0,319,0,239);	//���ù��λ�� 	
	for(index=0;index<totalpoint;index++)
	{
		LCD_WR_DATAX(color);
		delay_us(10);
	}
}
/*******************************************************************************
* Function Name  : LCD_Fill
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_Fill(u16 x,u16 y,u16 w,u16 h,u16 color)
{
	u32 i=0;

	LCD_SetCursor(x,x+w-1,y,y+h-1);
	for(i=0;i<w*h;i++)
	{
		LCD_WR_DATAX(color);
	}
}

void GC9306_enter_sleep(void)
{
	LCD_WrCom(0xfe);
	LCD_WrCom(0xef);
	LCD_WrCom(0x28);
	delay_ms(240);
	LCD_WrCom(0x10);
	delay_ms(300);
}
void GC9306_exit_sleep(void)
{
	LCD_WrCom(0xfe);
	LCD_WrCom(0xef);
	LCD_WrCom(0x11);
	delay_ms(240) ;
	LCD_WrCom(0x29);
}
/*******************************************************************************
* Function Name  : LCD_INIT_ILI9320_032
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_INIT_GC9306(void)
{
	LCD_GPIOInit();
	
	//------------- Reset Sequence---------------//
	LCD_RST_SET;
	delay_ms(240);
	LCD_RST_CLR;
	delay_ms(200); // delay 10ms This delay time is necessary
	LCD_RST_SET;
	delay_ms(240);
	//------------end Reset Sequence-----------//
	LCD_WrCom(0xfe); 
		
	LCD_WrCom(0xef);  
	 
	LCD_WrCom(0x36);//��ת��Ļ
	LCD_WrDat(0xc8);//����λ�ǿ�����Ļ�����
	 
	LCD_WrCom(0x3a);   
	LCD_WrDat(0x05);  
	 
	LCD_WrCom(0xad); 
	LCD_WrDat(0x33); 

	LCD_WrCom(0xaf); 
	LCD_WrDat(0x55); 

	LCD_WrCom(0xae); 
	LCD_WrDat(0x2b); 
	//----------------------------------end    display    control setting--------------------------------// 
	//----------------------------------Power Control Registers Initial--------------------------------// 
	LCD_WrCom(0xa4); 
	LCD_WrDat(0x44); 
	LCD_WrDat(0x44); 

	LCD_WrCom(0xa5); 
	LCD_WrDat(0x42); 
	LCD_WrDat(0x42); 

	LCD_WrCom(0xaa); 
	LCD_WrDat(0x88); 
	LCD_WrDat(0x88); 
	 
	LCD_WrCom(0xae); 
	LCD_WrDat(0x2b); 
	 
	LCD_WrCom(0xe8); 
	LCD_WrDat(0x11); 
	LCD_WrDat(0x0b); 

	LCD_WrCom(0xe3); 
	LCD_WrDat(0x01); 
	LCD_WrDat(0x10); 
		
	LCD_WrCom(0xff); 
	LCD_WrDat(0x61);   
	 
	LCD_WrCom(0xAC); 
	LCD_WrDat(0x00); 

	LCD_WrCom(0xAf); 
	LCD_WrDat(0x67); 

	LCD_WrCom(0xa6); 
	LCD_WrDat(0x2a); 
	LCD_WrDat(0x2a); 

	LCD_WrCom(0xa7); 
	LCD_WrDat(0x2b); 
	LCD_WrDat(0x2b);
	 
	LCD_WrCom(0xa8); 
	LCD_WrDat(0x18); 
	LCD_WrDat(0x18); 

	LCD_WrCom(0xa9); 
	LCD_WrDat(0x2a); 
	LCD_WrDat(0x2a); 
	//---------------------------------display window 240X320-------------------------------------// 
	LCD_WrCom(0x2a); 
	LCD_WrDat(0x00);   
	LCD_WrDat(0x00);   
	LCD_WrDat(0x00);   
	LCD_WrDat(0xef); 
		
	LCD_WrCom(0x2b); 
	LCD_WrDat(0x00);   
	LCD_WrDat(0x00);   
	LCD_WrDat(0x01);   
	LCD_WrDat(0x3f); 

	LCD_WrCom(0x2c); 
	//------------------------------------end display window ------------------------------------------// 
	//----------------------------------------gamma setting----------------------------------------------// 
	LCD_WrCom(0xf0); 
	LCD_WrDat(0x02); 
	LCD_WrDat(0x00); 
	LCD_WrDat(0x00); 
	LCD_WrDat(0x1b); 
	LCD_WrDat(0x1f); 
	LCD_WrDat(0xb); 

	LCD_WrCom(0xf1); 
	LCD_WrDat(0x01); 
	LCD_WrDat(0x3);  
	LCD_WrDat(0x00); 
	LCD_WrDat(0x28); 
	LCD_WrDat(0x2b); 
	LCD_WrDat(0xe); 

	LCD_WrCom(0xf2); 
	LCD_WrDat(0xb); 
	LCD_WrDat(0x08); 
	LCD_WrDat(0x3b); 
	LCD_WrDat(0x04); 
	LCD_WrDat(0x03); 
	LCD_WrDat(0x4c); 

	LCD_WrCom(0xf3); 
	LCD_WrDat(0xe); 
	LCD_WrDat(0x7); 
	LCD_WrDat(0x46); 
	LCD_WrDat(0x4); 
	LCD_WrDat(0x5); 
	LCD_WrDat(0x51); 

	LCD_WrCom(0xf4); 
	LCD_WrDat(0x08); 
	LCD_WrDat(0x15); 
	LCD_WrDat(0x15); 
	LCD_WrDat(0x1f); 
	LCD_WrDat(0x22); 
	LCD_WrDat(0x0F); 

	LCD_WrCom(0xf5); 
	LCD_WrDat(0xb); 
	LCD_WrDat(0x13); 
	LCD_WrDat(0x11); 
	LCD_WrDat(0x1f); 
	LCD_WrDat(0x21); 
	LCD_WrDat(0x0F); 
	//------------------------------------end gamma setting------------------------------------------// 
	LCD_WrCom(0x11);   
	delay_ms(120);   
	LCD_WrCom(0x29);

	lcddev.dir = 1;	//����
	lcddev.width = 320;
	lcddev.height = 240;
	lcddev.wramcmd = 0X2C;
	lcddev.setxcmd = 0X2B;
	lcddev.setycmd = 0X2A;
	LCD_Clear(BLACK);
	LCD_LED = 1;
}

/*******************************************************************************
* Function Name  : LCD_SetCursor
* Description    : ����
* Input          : None RGB_Code:�˵����ɫ
* Output         : None
* Return         : None
*******************************************************************************/
void GC9306_DrawPoint(u16 Xpos,u16 Ypos,u16 RGB_Code)
{
	LCD_SetCursor(Xpos,Xpos,Ypos,Ypos);
  LCD_WR_DATAX(RGB_Code);		
}
/*******************************************************************************
* Function Name  : LCD_ReadPoint
* Description    : ����
* Input          : None x,y:����
* Output         : None
* Return         : None����ֵ:�˵����ɫ
*******************************************************************************/
//u16 LCD_ReadPoint(u16 x,u16 y)
//{
// 	u16 r,g,b,x1;
//	u8 r1,b1;
//	if(x>=lcddev.width||y>=lcddev.height)	return 0;	//�����˷�Χ,ֱ�ӷ���		   
////	LCD_SetCursor(x,y,x,y);
//	LCD_WR_REG(lcddev.setxcmd);  
//	LCD_WR_DATAX(x);
//	LCD_WR_DATAX(x);
//	LCD_WR_REG(lcddev.setycmd);  
//	LCD_WR_DATAX(y);
//	LCD_WR_DATAX(y);
//	LCD_WR_REG(0X2E);	//���Ͷ�GRAMָ��
//	
//	GPIOC->CRL = 0X88888888; //PB0-7  ��������
//	GPIOC->ODR |= 0X00FF;     //ȫ�������

//	LCD_RS_SET;
//	LCD_CS_CLR;	    
//	//��ȡ����(��GRAMʱ,��һ��Ϊ�ٶ�)	
////	LCD_RD_CLR;	
////  delay_us(1);//��ʱ1us

////	LCD_RD_SET;
// 
//	LCD_RD_CLR;					   
//	delay_us(1);//��ʱ1us					   
// 	r1 = DATAIN;  	//ʵ��������ɫ
//	r = r1;
//	r <<= 8;
//	LCD_RD_SET;
//	
//	LCD_RD_CLR;					   
//	delay_us(1);//��ʱ1us					   
// 	r1 = DATAIN;  	//ʵ��������ɫ
//	r |= r1;
//	LCD_RD_SET;
//	 
//	LCD_RD_CLR;					   
//	b1=DATAIN;//��ȡ��ɫֵ 
//	b = b1;
//	b <<= 8;
//	LCD_RD_SET;
//	
//	LCD_RD_CLR;					   
//	b1=DATAIN;//��ȡ��ɫֵ 
//	b |= b1;
//	LCD_RD_SET;
//	
//	g=r&0XFF;//����9341,��һ�ζ�ȡ����RG��ֵ,R��ǰ,G�ں�,��ռ8λ
//	g<<=8;
//	
//	GPIOC->CRL = 0X33333333; 		//PB0-7  �������
//	GPIOC->ODR |= 0X00FF;    		//ȫ�������  
//	
//	x1 = (((r>>11)<<11)|((g>>10)<<5)|(b>>11));
//	return x1;
//}

//��ȡ��ĳ�����ɫֵ?? ??
//x,y:����
//����ֵ:�˵����ɫ
//��ȡ��ĳ�����ɫֵ?? ??
//x,y:����
//����ֵ:�˵����ɫ
//u16 LCD_ReadPoint(u16 x,u16 y)
//{
//	u16 r1,r2,r3,r4,r5;
//	u16 x1;
//	if(x>=lcddev.width||y>=lcddev.height)	return 0;
//	LCD_WR_REG(lcddev.setxcmd);  
//	LCD_WR_DATAX(x);
//	LCD_WR_DATAX(x);
//	LCD_WR_REG(lcddev.setycmd);  
//	LCD_WR_DATAX(y);
//	LCD_WR_DATAX(y);
//	//LCD_WR_REG(0x2c);//���Ͷ�GRAMָ��
//	
//	GPIOC->CRL = 0X88888888; //PB0-7��������
//	GPIOC->ODR |= 0X00FF;//ȫ�������
//	LCD_RS_SET;
//	LCD_CS_CLR;
//	LCD_WR_SET;
//	//��ȡ����(��GRAMʱ,��һ��Ϊ�ٶ�)
//	LCD_RD_CLR;
//	delay_us(1);//��ʱ1us
//	
//	LCD_RD_SET;
//	r1 = (DATAIN & 0x00ff);printf("r1 = %x  ",r1);
//	//�ڶ��ζ�����
//	LCD_RD_CLR;
//	delay_us(1);//��ʱ1us
//	
//	LCD_RD_SET;
//	r2 = (DATAIN & 0x00ff);printf("r2 = %x  ",r2);
//	//�����ζ�����
//	LCD_RD_CLR;
//	delay_us(1);//��ʱ1us
//	
//	LCD_RD_SET;
//	r3 = (DATAIN & 0x00ff);printf("r3 = %x  ",r3);
//	//���Ĵζ�����
//	LCD_RD_CLR;
//	
//	LCD_RD_SET;
//	r4 = (DATAIN & 0x00ff);printf("r4 = %x  ",r4);
//	//����ζ�����
//	LCD_RD_CLR;
//	
//	LCD_RD_SET;
//	r5 = (DATAIN & 0x00ff);printf("r5 = %x  ",r5);
//	
//	GPIOC->CRL = 0X33333333;//PB0-7�������
//	GPIOC->ODR |= 0X00FF;//ȫ�������
//	return x1;
//}
/*******************************************************************************
* Function Name  : LCD_DrawLine
* Description    : ����
* Input          : None RGB_Code:�˵����ɫ
* Output         : None
* Return         : None
*******************************************************************************/
void LCD_DrawLine(u16 sx, u16 sy, u16 ex, u16 ey,u16 cala)
{
	u16 t; 
	int xerr = 0,yerr =  0,delta_x,delta_y,distance; 
	int incx,incy,uRow,uCol;
	
	delta_y = ey - sy; //������������ 
	delta_x = ex - sx; 
	uRow = sx;
	uCol = sy;
	
	if(delta_x > 0)
		incx = 1; //���õ������� 
	else if(delta_x==0)
		incx = 0;//��ֱ�� 
	else
	{
		incx = -1;
		delta_x = -delta_x;
	} 
	if(delta_y > 0)
		incy=1; 
	else if(delta_y==0)
		incy=0;//ˮƽ�� 
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	
	if(delta_x > delta_y)
		distance = delta_x; //ѡȡ�������������� 
	else
		distance = delta_y;
	
	for(t = 0;t <= distance + 1;t ++)//������� 
	{
		GC9306_DrawPoint(uRow,uCol,cala);//���� 
		xerr += delta_x ; 
		yerr += delta_y ; 
		if(xerr > distance) 
		{ 
			xerr -= distance; 
			uRow += incx; 
		} 
		if(yerr > distance) 
		{ 
			yerr -= distance; 
			uCol += incy; 
		} 
	}  
}
/*******************************************************************************
* Function Name  : Draw_pic
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Draw_pic(u16 x, u16 y, u16 w, u16 h,const u8 *pHz, u16 LineColor,u16 FillColor)
{
	u16 j=0,i=0,k=0;
	u8 str;
	for (i=0;i<w;i++)
	{
		LCD_SetCursor(x+i,x+i,y,y+h*8);
		for (j=0;j<h;j++)
		{
			str = pHz[w*j+i];
			for (k=0;k<8;k++)
			{
				if(str & (0x80 >> k))  LCD_WR_DATAX(LineColor);
				else	LCD_WR_DATAX(FillColor);
			}
		}
		LCD_WR_DATAX(FillColor);
	}
}
void Draw_pic1(u16 x, u16 y, u16 w, u16 h,const u8 *pHz, u16 LineColor,u16 FillColor)
{
	u16 j=0,i=0,k=0;
	u8 str;
	for (i=0;i<w;i++)
	{
		LCD_SetCursor(x+i,x+i,y,y+h*8);
		for (j=0;j<h;j++)
		{
			str = pHz[w*j+i];
			for (k=0;k<8;k++)
			{
				if(str & (0x80 >> k))  LCD_WR_DATAX(LineColor);
				else	LCD_WR_DATAX(FillColor);
			}
		}
		LCD_WR_DATAX(LineColor);
	}
}
/**************************************************************************************
* ��    ��: LCD_ShowChar
* ��    ��: ��ָ����λ����ʾ�ַ�
* ��    ��: x           : x����
*                       y       : y����
*             LineColor : ���ֵ���ɫ
*           FillColor   : ���ֱ�����ɫ
* �� �� ֵ: ��
**************************************************************************************/
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 LineColor,u16 FillColor)
{  							  
  u8 temp,t1,t;
	u8 i = size / 8;
	u8 xi = 0;
	u8 ii = 0;
	
	u8 csize = (size/8+((size%8)?1:0))*(size/2);		//�õ�����һ���ַ���Ӧ������ռ���ֽ���	
	
 	num = num - ' ';//�õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	LCD_SetCursor(x,x,y,y + size);
	for(t = 0;t < csize;t ++)
	{
		if(size == 16)	temp = asc2_1608[num][t];//����1608����
		else if(size == 24)	temp = asc2_2412[num][t];//����2412����
		else return;								//û�е��ֿ�
		
		if(xi == i)
		{
			LCD_WR_DATAX(FillColor);//����һ���������������Ҫ��дһ��GRAM������ʾ�����4���㣬��仰��Ϲ�ճ����ġ�
			xi = 0;ii ++;
			LCD_SetCursor(x + ii,x + ii,y,y + size);
		}
		
		for(t1 = 0;t1 < 8;t1 ++)
		{
			if(temp & (0x80 >> t1))
				LCD_WR_DATAX(LineColor);
			else
				LCD_WR_DATAX(FillColor);
		}
		
		xi ++;	
	}
	LCD_WR_DATAX(FillColor);//����һ���������������Ҫ��дһ��GRAM������ʾ�����4���㣬��仰��Ϲ�ճ����ġ�
}
/**************************************************************************************
* ��    ��: LCD_ShowWord
* ��    ��: ��ָ����λ����ʾ����
* ��    ��: x           : x����
*                       y       : y����
*             LineColor : ���ֵ���ɫ
*           FillColor   : ���ֱ�����ɫ
* �� �� ֵ: ��
**************************************************************************************/
void LCD_ShowWord(u16 x,u16 y,u8 *pHz,u8 size,u16 LineColor,u16 FillColor)
{
	u16 j=0,i=0,k=0,w;
	u16 str,len;
	//---------------------------ͳ���Ƿ����ֿ��г��� 
	if(size == 16)
		len=sizeof(HzLib16)/sizeof(HzLib16[0]);
	else if(size == 24)
		len=sizeof(HzLib24)/sizeof(HzLib24[0]);
	
	for(j = 0;j < len;j++)
	{
		if(size == 16)
		{if((uchar)HzLib16[j].Index[0]==pHz[0]&&(uchar)HzLib16[j].Index[1]==pHz[1])
				break;
		}
		else if(size == 24)
		{if((uchar)HzLib24[j].Index[0]==pHz[0]&&(uchar)HzLib24[j].Index[1]==pHz[1])
				break;
		}
	}
	if(j<len)
	{
		for(i=0;i<size;i++)
		{
			LCD_SetCursor(x+i,x+i,y,y+size);
			for (w=0;w<(size/8);w++)
			{
				if(size == 16)
					str = (uchar)HzLib16[j].Msk[HZ16_w_point*w+i];
				else if(size == 24)
					str = (uchar)HzLib24[j].Msk[HZ24_w_point*w+i]; 
				for(k=0;k<8;k++)
				{
					if(str & (0x80 >> k))
						LCD_WR_DATAX(LineColor);
					else
						LCD_WR_DATAX(FillColor);
				}
			}
			LCD_WR_DATAX(FillColor);//����һ���������������Ҫ��дһ��GRAM������ʾ�����4���㣬��仰��Ϲ�ճ����ġ�
		}
	}    	   	 	  
}
//��ʾ�ַ���
//x,y:�������
//width,height:�����С  
//size:�����С
//*p:�ַ�����ʼ��ַ		  
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u16 LineColor,u16 FillColor)
{
	u16 y1 = y;
 	while(1)
	{
		if(*p == 0)	return;
		
		if(*p > 0x80)           //����
		{
			LCD_ShowWord(x, y1, p, size, LineColor, FillColor);
			x += size;
			if(x>(319-size)){x=0;y1+=size;}
			p += 2;              
		}
		else                        //Ӣ���ַ�
		{
			LCD_ShowChar(x,y1,*p,size,LineColor,FillColor);
			x += size/2;
			if(x>(319-size/2)){x=0;y1+=size;}
			p += 1;              
		}
	}
}

void DrawSingleAscii_big3(u16 x, u16 y, u8 pAscii, u16 LineColor,u16 FillColor)
{
   uchar i, j,k,w,str;
   ulong dizhi;
	 w=h_unm3/8;
	 dizhi=pAscii*w*w_unm3;
	 for (i=0;i<w_unm3;i++)
	 {
			LCD_SetCursor(x+i,x+i,y,y+h_unm3);
			for (k=0;k<w;k++)
			{
				str = AsciiLib3[dizhi+w_unm3*k+i];
				for (j=0;j<8;j++)
				{
					if(str & (0x80>>j))  LCD_WR_DATAX(FillColor);
					else                 LCD_WR_DATAX(LineColor);       
				}
			}
			LCD_WR_DATAX(FillColor);
	 }
}
void DrawSingleAscii_big3_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0)
{
	u16 y1;
	y1=y;
 	while(1)
	{
		if(*pStr == 0)
		{
			return;
		}
		if((*pStr == '0') && (first0 > 0))
		{DrawSingleAscii_big3(x,y,11,LineColor,FillColor);first0 --;}
		else if(*pStr == '.')
			DrawSingleAscii_big3(x,y,10,LineColor,FillColor);
		else
			DrawSingleAscii_big3(x,y,*pStr-0x30,LineColor,FillColor);
		x += w_unm3;
		if(x>(319-w_unm3)){x=0;y1+=h_unm3;}
		pStr += 1;
	}
}
void DrawSingleAscii_big4(u16 x, u16 y, u8 pAscii, u16 LineColor,u16 FillColor)
{
   uchar i, j,k,w,str;
   ulong dizhi;
	 w=h_unm4/8;
	 dizhi=pAscii*w*w_unm4;
	 for (i=0;i<w_unm4;i++)
	 {
			LCD_SetCursor(x+i,x+i,y,y+h_unm4);
			for (k=0;k<w;k++)
			{
				str = AsciiLib4[dizhi+w_unm4*k+i];
				for (j=0;j<8;j++)
				{
					if(str & (0x80>>j))  LCD_WR_DATAX(FillColor);
					else                 LCD_WR_DATAX(LineColor);       
				}
			}
			LCD_WR_DATAX(FillColor);
	 }
}
void DrawSingleAscii_big4_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0)
{
	u16 y1;
	y1=y;
 	while(1)
	{
		if(*pStr == 0)
		{
			return;
		}
		if((*pStr == '0') && (first0 > 0))
		{DrawSingleAscii_big4(x,y,11,LineColor,FillColor);first0 --;}
		else if(*pStr == '.')
			DrawSingleAscii_big4(x,y,10,LineColor,FillColor);
		else
			DrawSingleAscii_big4(x,y,*pStr-0x30,LineColor,FillColor);
		x += w_unm4;
		if(x>(319-w_unm4)){x=0;y1+=h_unm4;}
		pStr += 1;
	}
}
void DrawSingleAscii_big5(u16 x, u16 y, u8 pAscii, u16 LineColor,u16 FillColor)
{
   uchar i, j,k,w,str;
   ulong dizhi;
	 w=h_unm5/8;
	 dizhi=pAscii*w*w_unm5;
	 for (i=0;i<w_unm5;i++)
	 {
			LCD_SetCursor(x+i,x+i,y,y+h_unm5);
			for (k=0;k<w;k++)
			{
				str = AsciiLib5[dizhi+w_unm5*k+i];
				for (j=0;j<8;j++)
				{
					if(str & (0x80>>j))  LCD_WR_DATAX(FillColor);
					else                 LCD_WR_DATAX(LineColor);       
				}
			}
			LCD_WR_DATAX(FillColor);
	 }
}
void DrawSingleAscii_big5_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0)
{
	u16 y1;
	y1=y;
 	while(1)
	{
		if(*pStr == 0)
		{
			return;
		}
		if((*pStr == '0') && (first0 > 0))
		{DrawSingleAscii_big5(x,y,11,LineColor,FillColor);first0 --;}
		else if(*pStr == '.')
			DrawSingleAscii_big5(x,y,10,LineColor,FillColor);
		else
			DrawSingleAscii_big5(x,y,*pStr-0x30,LineColor,FillColor);
		x += w_unm5;
		if(x>(319-w_unm5)){x=0;y1+=h_unm5;}
		pStr += 1;
	}
}
void DrawSingleAscii_big6(u16 x, u16 y, u8 pAscii, u16 LineColor,u16 FillColor)
{
   uchar i, j,k,w,str;
   ulong dizhi;
	 w=h_unm6/8;
	 dizhi=pAscii*w*w_unm6;
	 for (i=0;i<w_unm6;i++)
	 {
			LCD_SetCursor(x+i,x+i,y,y+h_unm6);
			for (k=0;k<w;k++)
			{
				str = AsciiLib6[dizhi+w_unm6*k+i];
				for (j=0;j<8;j++)
				{
					if(str & (0x80>>j))  LCD_WR_DATAX(FillColor);
					else                 LCD_WR_DATAX(LineColor);       
				}
			}
			LCD_WR_DATAX(FillColor);
	 }
}
void DrawSingleAscii_big6_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0)
{
	u16 y1;
	y1=y;
 	while(1)
	{
		if(*pStr == 0)
		{
			return;
		}
		if((*pStr == '0') && (first0 > 0))
		{DrawSingleAscii_big6(x,y,11,LineColor,FillColor);first0 --;}
		else if(*pStr == '.')
			DrawSingleAscii_big6(x,y,10,LineColor,FillColor);
		else
			DrawSingleAscii_big6(x,y,*pStr-0x30,LineColor,FillColor);
		x += w_unm6;
		if(x>(319-w_unm6)){x=0;y1+=h_unm6;}
		pStr += 1;
	}
}
//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 RGB_Code)
{
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);             //�ж��¸���λ�õı�־
	while(a<=b)
	{
		GC9306_DrawPoint(x0-b,y0-a,RGB_Code);
		GC9306_DrawPoint(x0+b,y0-a,RGB_Code);
		GC9306_DrawPoint(x0-a,y0+b,RGB_Code);
		GC9306_DrawPoint(x0-b,y0-a,RGB_Code);
		GC9306_DrawPoint(x0-a,y0-b,RGB_Code);
		GC9306_DrawPoint(x0+b,y0+a,RGB_Code);
		GC9306_DrawPoint(x0+a,y0-b,RGB_Code);
		GC9306_DrawPoint(x0+a,y0+b,RGB_Code);
		GC9306_DrawPoint(x0-b,y0+a,RGB_Code);
		a++;
		//ʹ��Bresenham�㷨��Բ
		if(di<0)
			di += 4*a+6;
		else
		{
			di += 10+4*(a-b);
			b--;
		}
		GC9306_DrawPoint(x0+a,y0+b,RGB_Code);
	}
}
void LCD_DrawFullCircle(u16 Xpos,u16 Ypos,u16 Radius,u16 RGB_Code)
{
	u16 x,y,r=Radius;
	for(y=Ypos - r;y<Ypos +r;y++)
	{
		for(x=Xpos - r;x<Xpos+r;x++)
		{
			if(((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) <= r*r)
			{
				GC9306_DrawPoint(x,y,RGB_Code);
			}
		}
	}
}
void LCD_DrawFullCircle1(u16 Xpos,u16 Ypos,u16 Radius,u16 Rs1,u16 RGB_Code,float bbp)
{
	u16 x,y,r=Radius;
	float y_bbp1 = 1;
	float y_bbp2 = 0;
	if(bbp < 0.5)
	{
		y_bbp1 = bbp*2;
	}
	else if((bbp >= 0.5) && (bbp <= 1))
	{
		y_bbp2 = (bbp - 0.5) * 2;
	}
	for(y=Ypos - r;y<(Ypos-r) + 2*r*y_bbp1;y++)
	{
		for(x=Xpos;x<(Xpos + r);x++)
		{
			if((((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) <= r*r) && (((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) >= (r-Rs1)*(r-Rs1)))
			{
				GC9306_DrawPoint(x,y,RGB_Code);
			}
		}
	}
	for(y=Ypos + r;y>Ypos + r - 2*r*y_bbp2;y--)
	{
		for(x=Xpos;x>(Xpos - r);x--)
		{
			if((((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) <= r*r) && (((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) >= (r-Rs1)*(r-Rs1)))
			{
				GC9306_DrawPoint(x,y,RGB_Code);
			}
		}
	}
}
/*******************************************************************************
* Function Name  : piclib_alpha_blend
* Description    : 
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
u16 piclib_alpha_blend(u16 src,u16 dst,u8 alpha)
{
	u32 src2;
	u32 dst2;
	src2=((src<<16)|src)&0x07E0F81F;
	dst2=((dst<<16)|dst)&0x07E0F81F;
	dst2=((((dst2-src2)*alpha)>>5)+src2)&0x07E0F81F;
	return (dst2>>16)|dst2;  
}
void LCD_DrawFullCircle2(u16 Xpos,u16 Ypos,u16 Radius,u16 Rs1,u16 RGB_Code,float bbp)
{
	u16 x,y,r=Radius;
	float y_bbp1 = 1;
	float y_bbp2 = 0;
	if(bbp < 0.5)
	{
		y_bbp1 = bbp*2;
	}
	else if((bbp >= 0.5) && (bbp <= 1))
	{
		y_bbp2 = (bbp - 0.5) * 2;
	}
	for(y=Ypos - r;y<(Ypos-r) + 2*r*y_bbp1;y++)
	{
		for(x=Xpos;x>(Xpos - r);x--)
		{
			if((((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) <= r*r) && (((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) >= (r-Rs1)*(r-Rs1)))
			{
				GC9306_DrawPoint(x,y,RGB_Code);
			}
		}
	}
	for(y=Ypos + r;y>Ypos + r - 2*r*y_bbp2;y--)
	{
		for(x=Xpos;x<(Xpos + r);x++)
		{
			if((((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) <= r*r) && (((x-Xpos)*(x-Xpos)+(y-Ypos)*(y-Ypos)) >= (r-Rs1)*(r-Rs1)))
			{
				GC9306_DrawPoint(x,y,RGB_Code);
			}
		}
	}
}






















