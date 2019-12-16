#ifndef __GC9306_H
#define __GC9306_H	 
#include "sys.h"


//LCD��Ҫ������
typedef struct  
{										    
	u16 width;			//LCD ���
	u16 height;			//LCD �߶�
	u8  dir;			//���������������ƣ�0��������1��������	
	u16	wramcmd;		//��ʼдgramָ��
	u16 setxcmd;		//����x����ָ��
	u16 setycmd;		//����y����ָ��	 
}_lcd_dev; 	  

//LCD����
extern _lcd_dev lcddev;	//����LCD��Ҫ����


#define WORD_COLOR  BLACK//������ɫ
#define WORD_COLOR1  WHITE//������ɫ
#define BACK_COLOR  WHITE//����ɫ 
#define BACK_COLOR1  BLACK//����ɫ 


//==========================LCD DEFINE
#define RED      0xf800
#define GREEN    0x07e0
#define BLUE     0x001f
#define BLUE0    0x07ff
#define RED1     0xfbe0
#define WHITE    0xffff
#define BLACK    0x0000	
#define YELLOW   0xFff0

#define BROWN 			 	0XBC40 	//��ɫ
#define BRRED 			 	0XFC07 	//�غ�ɫ
#define GRAY  			 	0X8430 	//��ɫ
#define DARKBLUE      0X01CF 	//����ɫ
#define LIGHTBLUE     0X7D7C 	//ǳ��ɫ  
#define GRAYBLUE      0X5458 	//����ɫ

#define	Fense					0xe9ce	//��ɫ
#define Chase			  	0xd5b1	//��ɫ
#define Hese		      0xa285	//��ɫ
#define Tianlanse		 	0x05ff	//����ɫ
#define Jialanse		  0x435c	//�ʼ���
#define Bohese			  0xf5ff	//����ɫ
#define Yinbaise			0xc618	//����ɫ 
#define Qianhuise		 	0x8410	//ǳ��ɫ
#define Chengse			  0xfbea	//��ɫ

#define	LCD_LED PAout(0) //LCD����

#define LCDDATA_ODR_Addr(data)   GPIOC->ODR=(GPIOC -> ODR & 0xff00)|(data & 0x00ff)
#define DATAIN   GPIO_ReadInputData(GPIOC)



#define	LCD_CS_SET  GPIOA->BSRR=1<<1     //Ƭѡ�˿�
#define	LCD_RS_SET	GPIOD->BSRR=1<<0     //����/����   
#define	LCD_WR_SET	GPIOC->BSRR=1<<15    //д����
#define	LCD_RD_SET	GPIOC->BSRR=1<<14    //������
#define	LCD_RST_SET	GPIOD->BSRR=1<<1     //��λ
#define	LCD_TE_SET	GPIOB->BSRR=1<<13    //TE
								    
#define	LCD_CS_CLR  GPIOA->BRR=1<<1      //Ƭѡ�˿�
#define	LCD_RS_CLR	GPIOD->BRR=1<<0      //����/����	   
#define	LCD_WR_CLR	GPIOC->BRR=1<<15     //д����	
#define	LCD_RD_CLR	GPIOC->BRR=1<<14     //������	
#define	LCD_RST_CLR	GPIOD->BRR=1<<1      //��λ
#define	LCD_TE_CLR	GPIOB->BRR=1<<13     //TE



#define	HZ24_w_point  24 //24���������С���
#define	HZ24_h_point  24 //24���������С�߶�
#define	ZF24_w_point  12 //24��ĸ�����С���
#define	ZF24_h_point  24 //24��ĸ�����С�߶�

#define	HZ16_w_point  16 //16���������С���
#define	HZ16_h_point  16 //16���������С�߶�
#define	ZF16_w_point  8  //16��ĸ�����С���
#define	ZF16_h_point  16 //16��ĸ�����С�߶�



#define  w_unm3  42
#define	 h_unm3  72
#define  w_unm4  28
#define	 h_unm4  48
#define  w_unm5  23
#define	 h_unm5  40
#define  w_unm6  18
#define	 h_unm6  32




void LCD_INIT_GC9306(void);
void LCD_Clear1(u16 color);
void LCD_Clear(u16 color);
void LCD_Fill(u16 x,u16 y,u16 w,u16 h,u16 color);
void GC9306_DrawPoint(u16 Xpos,u16 Ypos,u16 RGB_Code);
//u16 LCD_ReadPoint(u16 x,u16 y);
void LCD_DrawLine(u16 sx, u16 sy, u16 ex, u16 ey,u16 cala);
void Draw_pic(u16 x, u16 y, u16 w, u16 h,const u8 *pHz, u16 LineColor,u16 FillColor);
void Draw_pic1(u16 x, u16 y, u16 w, u16 h,const u8 *pHz, u16 LineColor,u16 FillColor);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u16 LineColor,u16 FillColor);
void LCD_ShowWord(u16 x,u16 y,u8 *pHz,u8 size,u16 LineColor,u16 FillColor);
void LCD_ShowString(u16 x,u16 y,u8 size,u8 *p,u16 LineColor,u16 FillColor);
void DrawSingleAscii_big3_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0);
void DrawSingleAscii_big4_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0);
void DrawSingleAscii_big5_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0);
void DrawSingleAscii_big5(u16 y, u16 x, u8 pAscii, u16 LineColor,u16 FillColor);
void DrawSingleAscii_big6_s(u16 x, u16 y, u8 *pStr, u16 LineColor,u16 FillColor,u8 first0);
void DrawSingleAscii_big6(u16 y, u16 x, u8 pAscii, u16 LineColor,u16 FillColor);
void lcd_show_bj_bmp(u16 x,u16 y,u16 w,u16 h);
void LCD_Draw_Circle(u16 x0,u16 y0,u8 r,u16 RGB_Code);
void LCD_DrawFullCircle(u16 Xpos,u16 Ypos,u16 Radius,u16 RGB_Code);
void LCD_DrawFullCircle1(u16 Xpos,u16 Ypos,u16 Radius,u16 Rs1,u16 RGB_Code,float bbp);
void LCD_DrawFullCircle2(u16 Xpos,u16 Ypos,u16 Radius,u16 Rs1,u16 RGB_Code,float bbp);
u16 piclib_alpha_blend(u16 src,u16 dst,u8 alpha);


#endif

























