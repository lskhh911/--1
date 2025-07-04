#ifndef LCD_H_
#define LCD_H_
#include "stm32f4xx.h"

#define True 1
#define False 0
#define not !
#define or ||
#define and &&

#define TFT_CMD       (*((volatile unsigned short *) 0x60060000)) //TFT命令寄存器片选地址
#define TFT_DATA      (*((volatile unsigned short *) 0x60060002)) //TFT数据寄存器片选地址
#define TFT_RST       (*((volatile unsigned short *) 0x60060004)) //TFT复位寄存器地址
#define TFTLED        (*((volatile unsigned short *) 0x60060008)) //TFT背光寄存器地址




#define LCD_WRITE_CMD(Command)  (TFT_CMD= Command )
#define LCD_WRITE_DATA(Data)    (TFT_DATA= Data)





//LCD重要参数集

extern u16 POINT_COLOR; //默认红色
extern u16 BACK_COLOR; //背景颜色.默认为白色

//画笔颜色
#define WHITE         	 0x7FDF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0X781F
#define GRED 	         0X7FE0
#define GBLUE		 0X07FF
#define RED           	 0x7800
#define MAGENTA       	 0x781F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0x7FE0
#define BROWN 	         0X3C40 //棕色
#define BRRED 	         0X7C07 //棕红色
#define GRAY  		 0X0430 //灰色

//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色  
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X041F //浅绿色
#define LIGHTGRAY        0XEF5B //浅灰色(PANNEL)
#define LGRAY 	         0X4618 //浅灰色(PANNEL),窗体背景色
#define LGRAYBLUE        0X1651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)
/*液晶缓冲长度*/
#define LCD_CURVE_BUFFER_LEN 400




/*设置单点像素*/
#define _LCD_Set_Pixel(x,y,color)	\
{	\
	LCD_WRITE_CMD(0x002A);\
	LCD_WRITE_DATA(x >> 8);	\
	LCD_WRITE_DATA(0x00FF & x);	\
	LCD_WRITE_DATA(x >> 8);	\
	LCD_WRITE_DATA(0x00FF & x);	\
	LCD_WRITE_CMD(0x002B);  	\
	LCD_WRITE_DATA(y >> 8);	\
	LCD_WRITE_DATA(0x00FF & y);	\
	LCD_WRITE_DATA(y >> 8);	\
	LCD_WRITE_DATA(0x00FF & y);	\
	LCD_WRITE_CMD(0x002C);;\
	LCD_WRITE_DATA(color);\
}


#define START_X0 5              //矩形框X轴起点
#define START_Y0 5              //矩形框Y轴起点
#define LCD_CURVE_WIDTH 320      //坐标宽度
#define WIDTH_PART_SIZE  40      //一个间隔宽度
#define LCD_CURVE_HEIGHT 256     //曲线高度
#define HEIGHT_PART_SIZE 50      //一个间隔宽度


/********************************************************************
* 名称 : LCD_SetCursor
* 功能 : 设置位置
* 输入 : Xpos  Ypos
* 输出 : 无 
***********************************************************************/
void  LCD_SetCursor(u16 Xpos, u16 Ypos);
/********************************************************************
* 名称 : LCD_Init9481
* 功能 : 液晶初始化
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LCD_Init9481(void);
/********************************************************************
* 名称 : LCD_Init9488A
* 功能 : 液晶初始化
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LCD_Init9488A(void);
/********************************************************************
* 名称 : LCD_Init9488
* 功能 : 液晶初始化
* 输入 : 无
* 输出 : 无
***********************************************************************/
void LCD_Init9488(void);
/********************************************************************
* 名称 : LCD_ShowChar
* 功能 : 显示1608字符 
* 输入 :  x y 字符(采用asc3_1608[][] 字库，num=0x20表示首字符
* 输出 : 无 
***********************************************************************/
void LCD_ShowChar(u16 x, u16 y, u8 num,u16 color);


/********************************************************************
* 名称 : LCD_ShowString
* 功能 : 显示1608字符串
* 输入 :  x y  大小 字符串 颜色
* 输出 : 无 
***********************************************************************/
void LCD_ShowString(u16 x, u16 y,  char *p,u16 color);


/********************************************************************
* 名称 : LCD_ShowCharBig
* 功能 : 显示3216字符
* 输入 :  x y  大小 字符位于asc_3216数组中的位置 颜色
* 输出 : 无 
*  注意：num为ASCII码
***********************************************************************/
void LCD_ShowCharBig(u16 x,u16 y,u8 num,u16 color);


/********************************************************************
* 名称 :  LCD_ShowStringBig
* 功能 : 显示3216字符串
* 输入 :  x y  大小 数字 颜色
* 输出 : 无 
***********************************************************************/
void LCD_ShowStringBig(u16 x, u16 y,char *p,u16 color);

/********************************************************************
* 名称 : LCD_ShowNumBig
* 功能 : 显示3216数字
* 输入 :  x y  数字 颜色
* 输出 : 无 
***********************************************************************/
void LCD_ShowNumBig(u16 x,u16 y,u32 num,u16 color);

/********************************************************************
* 名称 : LCD_ShowNumBig_L
* 功能 : 右对齐 显示3216数字
* 输入 :  x开始 x结束位置  y  大小 数字 颜色
* 输出 : 无 
***********************************************************************/
void LCD_ShowNumBig_L(u16 x,u16 x_end,u16 y,u32 num,u16 color);

/********************************************************************
* 名称 : LCD_ShowChinese
* 功能 : 显示1616中文字符
* 输入 :  x y  大小 中文位于asc4_1616数组中的位置 颜色
* 输出 : 无 
***********************************************************************/

void LCD_ShowChinese(u16 x, u16 y, u8 num,u16 color);

/********************************************************************
* 名称 : LCD_ShowChineseBig
* 功能 : 显示3232汉字
* 输入 :  x y  大小 字符位于asc_3232数组中的位置 颜色
* 输出 : 无 
***********************************************************************/
void LCD_ShowChineseBig(u16 x,u16 y,u8 num,u16 color);

/********************************************************************
* 名称 : LCD_ShowChineseStringBig
* 功能 : 显示3232汉字串
* 输入 :  x y  大小 字符位于asc_3232数组中的位置 颜色
* 输出 : 无 
***********************************************************************/
void LCD_ShowChineseStringBig(u16 x,u16 y,u8 num,u8 len,u16 color);

/********************************************************************
* 名称 : LCD_Clear1
* 功能 : 清空画面
* 输入 : 颜色
* 输出 : 无 
***********************************************************************/
void LCD_Clear1(u16 color);

/********************************************************************
* 名称 : _LCD_Set_Curve_Pixel
* 功能 : 设置曲线像素点 (可以避开矩形坐标框刻度线）
* 输入 : 曲线 x,y,color
* 输出 : 无 
***********************************************************************/
void  _LCD_Set_Curve_Pixel(u16 x,u16 y,u16 color);
/********************************************************************
* 名称 : _LCD_Show_Curve_Compensation
* 功能 : 插值补偿 
* 输入 : 曲线 x1,y1,x2,y2,color
* 输出 : 无 
***********************************************************************/
void _LCD_Show_Curve_Compensation(u16 x1, u16 y1, u16 x2, u16 y2,u16 color);



/********************************************************************
* 名称 : LCD_Curve_Show
* 功能 : 曲线显示
* 输入 : 曲线  y轴
* 输出 : 无 
***********************************************************************/
void LCD_Curve_Show(u16 y_buf[480],u16 x, u16 y2,u16 color);


/********************************************************************
* 名称 : LCD_Show_RectBox
* 功能 : 画带刻度矩形框
* 输入 : 曲线  开始位置x 开始位置y 颜色
* 输出 : 无 
***********************************************************************/
void LCD_Show_RectBox(u16 start_x, u16 start_y);

/********************************************************************
* 名称 : LCD_Show_Rect
* 功能 : 画矩形
* 输入 : x , y ， lenth，width， 颜色
* 输出 : 无 
***********************************************************************/
void LCD_Show_Rect(u16 x,u16 y,u16 lenth,u16 width,u16 point_color);
/********************************************************************
* 名称 : LCD_Clear_Rect
* 功能 : 清除矩形
* 输入 : x , y ， lenth，width， 颜色
* 输出 : 无 
***********************************************************************/
void LCD_Clear_Rect(u16 x,u16 y,u16 lenth,u16 width,u16 point_color);
/********************************************************************
* 名称 : delay_10ms
* 功能 : 软件延时
* 输入 : 无
* 输出 : 无 
***********************************************************************/
void delay_100ms(void);


#endif

