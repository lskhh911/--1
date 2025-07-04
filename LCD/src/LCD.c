#include "LCD.h"
#include "FONT.h" 


u16 POINT_COLOR = 0xFFFF;	//������ɫ
u16 BACK_COLOR = 0x0000;        //����ɫ

u8 _LCD_Curve_Buffer[LCD_CURVE_BUFFER_LEN];//��������


u8 y_buf[480];        //LCD_Curve_Show




/********************************************************************
* ���� : LCD_SetCursor
* ���� : ����λ��
* ���� : Xpos  Ypos
* ��� : �� 
***********************************************************************/
void LCD_SetCursor(u16 Xpos, u16 Ypos)
{
	LCD_WRITE_CMD(0x002A);  //Set column start address
	LCD_WRITE_DATA(Xpos >> 8);
	LCD_WRITE_DATA(0x00FF & Xpos);
	Xpos += 7;
	LCD_WRITE_DATA(Xpos >> 8);
	LCD_WRITE_DATA(0x00FF & Xpos);
	LCD_WRITE_CMD(0x002B);  //Set page start address
	LCD_WRITE_DATA(Ypos >> 8);
	LCD_WRITE_DATA(0x00FF & Ypos);
	Ypos += 15;
	LCD_WRITE_DATA(Ypos >> 8);
	LCD_WRITE_DATA(0x00FF & Ypos);
        LCD_WRITE_CMD(0x002C);
}
/********************************************************************
* ���� : LCD_Init9481
* ���� : Һ����ʼ��
* ���� : ��
* ��� : ��
***********************************************************************/
void LCD_Init9481(void)
{
	TFT_RST = 0;
	delay_100ms();
	TFT_RST = 1;

	LCD_WRITE_CMD(0x0011);
	delay_100ms();
	LCD_WRITE_CMD(0x00D0);
	LCD_WRITE_DATA(0x0007);
	LCD_WRITE_DATA(0x0041);
	LCD_WRITE_DATA(0x0018);

	LCD_WRITE_CMD(0x00D1);

	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x000a);
	LCD_WRITE_DATA(0x0010);

	LCD_WRITE_CMD(0x00D2);
	LCD_WRITE_DATA(0x0001);
	LCD_WRITE_DATA(0x0011);

	LCD_WRITE_CMD(0x00C0);
	LCD_WRITE_DATA(0x0010);
	LCD_WRITE_DATA(0x003B);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0002);
	LCD_WRITE_DATA(0x0011);

	LCD_WRITE_CMD(0x00C1);
	LCD_WRITE_DATA(0x0010);
	LCD_WRITE_DATA(0x0013);
	LCD_WRITE_DATA(0x0088);

	LCD_WRITE_CMD(0x00C5);
	LCD_WRITE_DATA(0x0002);

	LCD_WRITE_CMD(0x00C8);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0037);
	LCD_WRITE_DATA(0x0025);
	LCD_WRITE_DATA(0x0006);
	LCD_WRITE_DATA(0x0004);
	LCD_WRITE_DATA(0x001e);
	LCD_WRITE_DATA(0x0026);
	LCD_WRITE_DATA(0x0042);
	LCD_WRITE_DATA(0x0077);
	LCD_WRITE_DATA(0x0044);
	LCD_WRITE_DATA(0x000f);
	LCD_WRITE_DATA(0x0012);

	LCD_WRITE_CMD(0x00F3);
	LCD_WRITE_DATA(0x0040);
	LCD_WRITE_DATA(0x000A);

	LCD_WRITE_CMD(0x00F6);
	LCD_WRITE_DATA(0x0080);

	LCD_WRITE_CMD(0x00F7);
	LCD_WRITE_DATA(0x0080);

	LCD_WRITE_CMD(0x0036); //��ʾ��������
	LCD_WRITE_DATA(0x0029);

	LCD_WRITE_CMD(0x003a);
	LCD_WRITE_DATA(0x0055);

	LCD_WRITE_CMD(0x0020);
	//	 LCD_WRITE_CMD(0x0021);

	LCD_WRITE_CMD(0x002A);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0001);
	LCD_WRITE_DATA(0x003F);

	LCD_WRITE_CMD(0x002B);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0001);
	LCD_WRITE_DATA(0x00df);

	LCD_WRITE_CMD(0x00C1);
	LCD_WRITE_DATA(0x0000);
	LCD_WRITE_DATA(0x0010);
	LCD_WRITE_DATA(0x0022);

	delay_100ms();
	LCD_WRITE_CMD(0x0029);
	delay_100ms();
	LCD_WRITE_CMD(0x002C);
	
	LCD_Clear1(BLACK);
}



void LCD_Init9488A(void)
{
        TFT_RST = 1;
	delay_100ms();
        TFT_RST = 0;
	delay_100ms();
	TFT_RST = 1;
        delay_100ms();
	LCD_WRITE_CMD(0x11);
	delay_100ms();
        
	LCD_WRITE_CMD(0xd0);
	LCD_WRITE_DATA(0x07);
	LCD_WRITE_DATA(0x47);//45 47 47
	LCD_WRITE_DATA(0x19);//1c 19 1b
	LCD_WRITE_CMD(0xd1);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x36); //2a/2F  36/3B  3a/3C
	LCD_WRITE_DATA(0x1f);//1a       1f     1f
	LCD_WRITE_CMD(0xd2);
	LCD_WRITE_DATA(0x01);
	LCD_WRITE_DATA(0x11);
	LCD_WRITE_CMD(0xE4);
	LCD_WRITE_DATA(0xa0);
	LCD_WRITE_CMD(0xf3);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x2a);
	LCD_WRITE_CMD(0xc0);
	LCD_WRITE_DATA(0x10);
	LCD_WRITE_DATA(0x3b);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x02);
	LCD_WRITE_DATA(0x11);
	LCD_WRITE_CMD(0xc5);
	LCD_WRITE_DATA(0x03);
	LCD_WRITE_CMD(0xc8);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x35);
	LCD_WRITE_DATA(0x23);
	LCD_WRITE_DATA(0x07);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x04);
	LCD_WRITE_DATA(0x45);
	LCD_WRITE_DATA(0x53);
	LCD_WRITE_DATA(0x77);
	LCD_WRITE_DATA(0x70);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x04);
//*************GAMMA SETTING ***************

        LCD_WRITE_CMD(0x0036);          //��ʾ��������
        LCD_WRITE_DATA(0x0029);       //ILI9488A
	LCD_WRITE_CMD(0x003a);
	LCD_WRITE_DATA(0x0055);
	LCD_WRITE_CMD(0x0020);
	LCD_WRITE_CMD(0x2a);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x01);
	LCD_WRITE_DATA(0xDF);

	LCD_WRITE_CMD(0x2b);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x01);
	LCD_WRITE_DATA(0x3F);
	delay_100ms();
	LCD_WRITE_CMD(0x29);
	LCD_WRITE_CMD(0x2c);
        delay_100ms();
        LCD_Clear1(BLACK);
}
void LCD_Init9488(void)
{
        TFT_RST = 1;
	delay_100ms();
        TFT_RST = 0;
	delay_100ms();
	TFT_RST = 1;
        delay_100ms();
	LCD_WRITE_CMD(0x11);
	delay_100ms();
        
	LCD_WRITE_CMD(0xd0);
	LCD_WRITE_DATA(0x07);
	LCD_WRITE_DATA(0x47);//45 47 47
	LCD_WRITE_DATA(0x19);//1c 19 1b
	LCD_WRITE_CMD(0xd1);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x36); //2a/2F  36/3B  3a/3C
	LCD_WRITE_DATA(0x1f);//1a       1f     1f
	LCD_WRITE_CMD(0xd2);
	LCD_WRITE_DATA(0x01);
	LCD_WRITE_DATA(0x11);
	LCD_WRITE_CMD(0xE4);
	LCD_WRITE_DATA(0xa0);
	LCD_WRITE_CMD(0xf3);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x2a);
	LCD_WRITE_CMD(0xc0);
	LCD_WRITE_DATA(0x10);
	LCD_WRITE_DATA(0x3b);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x02);
	LCD_WRITE_DATA(0x11);
	LCD_WRITE_CMD(0xc5);
	LCD_WRITE_DATA(0x03);
	LCD_WRITE_CMD(0xc8);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x35);
	LCD_WRITE_DATA(0x23);
	LCD_WRITE_DATA(0x07);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x04);
	LCD_WRITE_DATA(0x45);
	LCD_WRITE_DATA(0x53);
	LCD_WRITE_DATA(0x77);
	LCD_WRITE_DATA(0x70);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x04);
//*************GAMMA SETTING ***************

        LCD_WRITE_CMD(0x0036);          //��ʾ��������
        LCD_WRITE_DATA(0x00A9);         //ILI9488
	LCD_WRITE_CMD(0x003a);
	LCD_WRITE_DATA(0x0055);
	LCD_WRITE_CMD(0x0020);
	LCD_WRITE_CMD(0x2a);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x01);
	LCD_WRITE_DATA(0xDF);

	LCD_WRITE_CMD(0x2b);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x00);
	LCD_WRITE_DATA(0x01);
	LCD_WRITE_DATA(0x3F);
        
 
	delay_100ms();
	LCD_WRITE_CMD(0x29);
	LCD_WRITE_CMD(0x2c);
        delay_100ms();
        LCD_Clear1(BLACK);
}
/********************************************************************
* ���� : LCD_ShowChar
* ���� : ��ʾ1608�ַ� 
* ���� :  x y �ַ�(����asc3_1608[][] �ֿ⣬num=0x20��ʾ���ַ�
* ��� : �� 
***********************************************************************/
void LCD_ShowChar(u16 x, u16 y, u8 num,u16 color)
{
	u8 a0 = 1;
	u8 temp, t1, t;
	u16 color_2 = POINT_COLOR;
	num = num - ' ';
	LCD_SetCursor(x, y);		                //���ù��λ��

	for (t = 15; t > 0; t--)
	{
		temp = asc3_1608[num][t];		//����1608����
         
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				POINT_COLOR = color;
			else
				POINT_COLOR = BACK_COLOR;
                        
			LCD_WRITE_DATA(POINT_COLOR);
			temp <<= 1;
		}
		if (((a0 == 1) & (t == 1)) == 1)
		{
			t++;
			a0 = 0;
		}
	}
	POINT_COLOR = color_2;
}

/********************************************************************
* ���� : LCD_ShowString
* ���� : ��ʾ1608�ַ���
* ���� :  x y  ��С �ַ��� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_ShowString(u16 x, u16 y,  char *p,u16 color)
{
	while ((*p <= '~') && (*p >= ' '))		 //�ж��ǲ��ǷǷ��ַ�!
	{
		LCD_ShowChar(x, y, *p, color);
		x += 8;
		p++;
	}
}



/********************************************************************
* ���� : LCD_ShowCharBig
* ���� : ��ʾ3216�ַ�
* ���� :  x y  ��С �ַ�λ��asc_3216�����е�λ�� ��ɫ
* ��� : �� 
*  ע�⣺numΪASCII��
***********************************************************************/
void LCD_ShowCharBig(u16 x,u16 y,u8 num,u16 color)
{
	int temp, t1, t;
	u16 color_2 = POINT_COLOR;
	LCD_SetCursor(x, y+16);		                //���ù��λ��
	                   
	for (t = 15; t >= 0; t--)
	{
	temp = asc_3216[num-' '][t];		        //����3216����
	for (t1 = 0; t1 < 8; t1++)
          {
            if (temp & 0x80)
              POINT_COLOR = color;
            else
              POINT_COLOR = BACK_COLOR;
              LCD_WRITE_DATA(POINT_COLOR);
              temp <<= 1;
          }
	}
        LCD_SetCursor(x, y);		                //���ù��λ��
        
	for (t = 31; t >= 16; t--)
	{
		temp = asc_3216[num-' '][t];		//����3216����
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
                          POINT_COLOR = color;
			else
                          POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_DATA(POINT_COLOR);
			temp <<= 1;
		}
	}
        LCD_SetCursor(x+8, y+16);                       //���ù��λ��
       
	for (t = 47; t >= 32; t--)
	{
		temp = asc_3216[num-' '][t];		//����3216����
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
                          POINT_COLOR = color;
			else
                          POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_DATA(POINT_COLOR);
			temp <<= 1;
		}
	}
        LCD_SetCursor(x+8, y);			        //���ù��λ��
       
	for (t = 63; t >= 48; t--)
	{
		temp = asc_3216[num-' '][t];		//����3216����
		for (t1 = 0; t1 < 8; t1++)
		{
			if (temp & 0x80)
				POINT_COLOR = color;
			else
				POINT_COLOR = BACK_COLOR;
			LCD_WRITE_DATA(POINT_COLOR);
			temp <<= 1;
		}
	}
	POINT_COLOR = color_2;
}


/********************************************************************
* ���� :  LCD_ShowStringBig
* ���� : ��ʾ3216�ַ���
* ���� :  x y  ��С ���� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_ShowStringBig(u16 x, u16 y,char *p,u16 color)
{
  while ((*p <= '~') && (*p >= ' '))		 //�ж��ǲ��ǷǷ��ַ�!
  {
          LCD_ShowCharBig(x, y, *p,color);
          x += 16;
          p++;
  }
}


/********************************************************************
* ���� : LCD_ShowNumBig
* ���� : ��ʾ3216����
* ���� :  x y  ���� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_ShowNumBig(u16 x,u16 y,u32 num,u16 color)
{
    char temp[20],len = 0,len_max,i;
    while(num)
    {
      temp[len] = num%10;
      num /= 10;
      len++;
      
    }
    if (len)
    {
      len_max = len;
      len--;
    }
    else
    {
      len_max = 1;
      temp[0] = 0;
    }
    
    for(i=0;i<len_max;i++)
    {
      LCD_ShowCharBig(x+i*16,y,temp[len]+'0',color);
      len--;
    }
}
/********************************************************************
* ���� : LCD_ShowNumBig_L
* ���� : �Ҷ��� ��ʾ3216����
* ���� :  x��ʼ x����λ��  y  ��С ���� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_ShowNumBig_L(u16 x,u16 x_end,u16 y,u32 num,u16 color)
{
    int temp[20],len = 0,len_max,i;
    while(num)
    {
      temp[len] = num%10+'0';
      num /= 10;
      len++;
     
    }
    len_max = len;
    len--;
    if(len_max == 0)
    {
      len = 0;
      temp[len] = '0';
      len_max = 1;
    }
    if(len_max*16 > (x-x_end))
    {
      return ;//���ȹ��� ����
    }
    
    else if(len_max)
    {
      x-= len_max*16;
      for(i=x_end;i<=x;i+=16)
      {
        LCD_ShowCharBig(i,y,' ',color);
      }
      for(i=0;i<len_max;i++)
      {
        LCD_ShowCharBig(x+i*16,y,temp[len],color);
        len--;
      }
    }
}

/********************************************************************
* ���� : LCD_ShowChinese
* ���� : ��ʾ1616�����ַ�
* ���� :  x y  ��С ����λ��asc4_1616�����е�λ�� ��ɫ
* ��� : �� 
***********************************************************************/

void LCD_ShowChinese(u16 x, u16 y, u8 num,u16 color)
{
	u8 temp, t1;
        int t;
	LCD_SetCursor(x, y);		//���ù��λ��
	//LCD_WriteRAM_Prepare();	        //��ʼд��GRAM
	for (t = 15; t > 0; t--)
	{
          temp = asc4_1616[num][t];		 //����1616����
          for (t1 = 0; t1 < 8; t1++)
          { 
             if (temp & 0x80)
               POINT_COLOR = color;
             else
               POINT_COLOR = BACK_COLOR;
               LCD_WRITE_DATA(POINT_COLOR);
               temp <<= 1;
          }
	}
        LCD_SetCursor(x+8, y);
       // LCD_WriteRAM_Prepare();	//��ʼд��GRAM
        for (t = 15; t > 0; t--)
	{
		temp = asc4_1616[num][t+16];		 //����1616����
		for (t1 = 0; t1 < 8; t1++)
		{
                  if (temp & 0x80)
                    POINT_COLOR = color;
		  else
                    POINT_COLOR = BACK_COLOR;
                    LCD_WRITE_DATA(POINT_COLOR);
                  temp <<= 1;
		}
	}
}


/********************************************************************
* ���� : LCD_ShowChineseBig
* ���� : ��ʾ3232����
* ���� :  x y  ��С �ַ�λ��asc_3232�����е�λ�� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_ShowChineseBig(u16 x,u16 y,u8 num,u16 color)
{
	int temp, t1,t2,t3;
	u16 color_2 = POINT_COLOR;
        for(t3 = 0;t3<128;t3+=64)
        {
          LCD_SetCursor(x+t3/4, y+16);		//���ù��λ��
         
          for (t2 = 15+t3; t2 >= t3; t2--)
          {
                  temp = asc_3232[num][t2];		//����3232����
                  for (t1 = 0; t1 < 8; t1++)
                  {
                          if (temp & 0x80)
                                  POINT_COLOR = color;
                          else
                                  POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_DATA(POINT_COLOR);
                          temp <<= 1;
                  }
          }
          LCD_SetCursor(x+t3/4, y);		//���ù��λ��
         
          for (t2 = 31+t3; t2 >= 16+t3; t2--)
          {
                  temp = asc_3232[num][t2];		//����3232����
                  for (t1 = 0; t1 < 8; t1++)
                  {
                          if (temp & 0x80)
                                  POINT_COLOR = color;
                          else
                                  POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_DATA(POINT_COLOR);
                          temp <<= 1;
                  }
          }
          LCD_SetCursor(x+8+t3/4, y+16);//���ù��λ��
         
          for (t2 = 47+t3; t2 >= 32+t3; t2--)
          {
                  temp = asc_3232[num][t2];		//����3232����
                  for (t1 = 0; t1 < 8; t1++)
                  {
                          if (temp & 0x80)
                                  POINT_COLOR = color;
                          else
                                  POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_DATA(POINT_COLOR);
                          temp <<= 1;
                  }
          }
          LCD_SetCursor(x+8+t3/4, y);			//���ù��λ��
          
          for (t2 = 63+t3; t2 >= 48+t3; t2--)
          {
                  temp = asc_3232[num][t2];		//����3232����
                  for (t1 = 0; t1 < 8; t1++)
                  {
                          if (temp & 0x80)
                                  POINT_COLOR = color;
                          else
                                  POINT_COLOR = BACK_COLOR;
                          LCD_WRITE_DATA(POINT_COLOR);
                          temp <<= 1;
                  }
          }
        }
	POINT_COLOR = color_2;
}
/********************************************************************
* ���� : LCD_ShowChineseStringBig
* ���� : ��ʾ3232���ִ�
* ���� :  x y  ��С �ַ�λ��asc_3232�����е�λ�� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_ShowChineseStringBig(u16 x,u16 y,u8 num,u8 len,u16 color)
{
  for(int i=0;i<len;i++)
  {
    LCD_ShowChineseBig(x+32*i,y,num+i,color);
  }
}





/********************************************************************
* ���� : LCD_Clear1
* ���� : ��ջ���
* ���� : ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_Clear1(u16 color)
{
	u32 i;
	LCD_WRITE_CMD(0x002A);		 //Set column address
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(1);
	LCD_WRITE_DATA(223);
	LCD_WRITE_CMD(0x002B);		 //Set page address
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(0);
	LCD_WRITE_DATA(1);
	LCD_WRITE_DATA(63);
	LCD_WRITE_CMD(0x002C); //write memory start
	for (i = 0; i < 0x25800; i++)
	LCD_WRITE_DATA(color);
}








/********************************************************************
* ���� : _LCD_Set_Curve_Pixel
* ���� : �����������ص� (���Աܿ����������̶��ߣ�
* ���� : ���� x,y,color
* ��� : �� 
***********************************************************************/
void  _LCD_Set_Curve_Pixel(u16 x,u16 y,u16 color)
{
	u16 x1= x-START_X0;
	u16 y1= y-START_Y0;
        u8 dispsign;
        dispsign=(y1%HEIGHT_PART_SIZE==0 and (x1<6 or x1>LCD_CURVE_WIDTH-WIDTH_PART_SIZE or x1%WIDTH_PART_SIZE==0)) or
		(x1%WIDTH_PART_SIZE==0 and (y1<6 or y1>LCD_CURVE_HEIGHT-HEIGHT_PART_SIZE or y1%HEIGHT_PART_SIZE==0));
	if(dispsign==0)
	{
            _LCD_Set_Pixel(x,y,color);
	}
}
/********************************************************************
* ���� : _LCD_Show_Curve_Compensation
* ���� : ��ֵ���� 
* ���� : ���� x1,y1,x2,y2,color
* ��� : �� 
***********************************************************************/
void _LCD_Show_Curve_Compensation(u16 x1, u16 y1, u16 x2, u16 y2,u16 color)
{
    int i = 0;	
    int y_different = (int) y2 - y1;
    int y_now = y1;
    int count = y_different ;
            
    if (count > 0)
    {
        for (i = 0; i < (count + 1) / 2; i++)
        {
            y_now = y1 + i;
            _LCD_Set_Curve_Pixel(x1, y_now, color);
        }
        x1++;
        for (i = count / 2; i <= count; i++)
        {
            y_now =  y1 + i;
            _LCD_Set_Curve_Pixel(x1, y_now, color);
        }
    }
    else if (count < 0)
    {
        for (i = 0; i > (count - 1) / 2; i--)
        {
            y_now =y1 + i;
            _LCD_Set_Curve_Pixel(x1, y_now, color);
        }
        x1++;
        for (i = count / 2; i >= count; i--)
        {
            y_now =  y1 + i;
            _LCD_Set_Curve_Pixel(x1, y_now, color);
        }
    }
    else
    {
        y_now =  y1;
        _LCD_Set_Curve_Pixel(x1, y_now, color);
        x1++;
    }
 

}


/********************************************************************
* ���� : LCD_Curve_Show
* ���� : ������ʾ
* ���� : ����  y��
* ��� : �� 
***********************************************************************/
void LCD_Curve_Show(u16 y_buf[480],u16 x, u16 y2,u16 color)
{
	
    u16 len =LCD_CURVE_WIDTH;
    u16 x1 = (x-1 + len) % len;
    u16 y1 = y_buf[x1];
    u16 y3 =y_buf[x+1];
    u16 TFTx=x+START_X0;
    u16 TFTy=y_buf[x]+START_Y0;
    if (y2==0)  y2 += 1;
    if(y2 >= 256)   y2 = 255;
    if (x == 0)
    {
        _LCD_Set_Curve_Pixel(TFTx,TFTy, BACK_COLOR);
        _LCD_Show_Curve_Compensation(TFTx,TFTy,TFTx+1, y3+START_Y0, BACK_COLOR);
        y_buf[x] = y2;
         _LCD_Set_Curve_Pixel(TFTx, y2+START_Y0, color);
    }
    else
    {
        
        _LCD_Set_Curve_Pixel(TFTx+ 1, y3+START_Y0, BACK_COLOR);                         //�����������
        _LCD_Show_Curve_Compensation(TFTx, TFTy, TFTx+1, y3+START_Y0, BACK_COLOR);  
       
        y_buf[x] = y2;            //���������
        _LCD_Set_Curve_Pixel(TFTx, y2+START_Y0, color);//д����������
        
        _LCD_Show_Curve_Compensation(TFTx-1, y1+START_Y0,TFTx, y2+START_Y0, color);
    }
		
}



/********************************************************************
* ���� : LCD_Show_RectBox
* ���� : �����̶Ⱦ��ο�
* ���� : ����  ��ʼλ��x ��ʼλ��y ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_Show_RectBox(u16 start_x, u16 start_y)
{
	u16 i,j;

	u16 End_x = start_x+LCD_CURVE_WIDTH;
	u16 End_y = start_y+LCD_CURVE_HEIGHT;
        
        
        for(i=start_y;i<=End_y;i++)             /*�����ο�*/
        {
        	_LCD_Set_Pixel(start_x-1,i,WHITE);
        	_LCD_Set_Pixel(End_x+1,i,WHITE);
        }
        for(i=start_x;i<=End_x;i++)
        {
        	_LCD_Set_Pixel(i,start_y-1,WHITE);
        	_LCD_Set_Pixel(i,End_y+1,WHITE);
        }
        
        
        for(j=HEIGHT_PART_SIZE;j<LCD_CURVE_HEIGHT;j+=HEIGHT_PART_SIZE)          /*�ھ��ο�����ʾ�̶�*/
	{
		for(i=0;i<6;i++)
		{
			_LCD_Set_Pixel(i+start_x,start_y+j,WHITE);
			_LCD_Set_Pixel(LCD_CURVE_WIDTH-i+start_x,start_y+j,WHITE);
		}
	}
        for(j=HEIGHT_PART_SIZE;j<LCD_CURVE_HEIGHT;j+=HEIGHT_PART_SIZE)
	{
		for(i=WIDTH_PART_SIZE;i<LCD_CURVE_WIDTH;i+=WIDTH_PART_SIZE)
		{
			_LCD_Set_Pixel(i+start_x,start_y+j,WHITE);
		}
	}
	for(i=WIDTH_PART_SIZE;i<LCD_CURVE_WIDTH;i+=WIDTH_PART_SIZE)
	{
		for(j=0;j<6;j++)
		{
			_LCD_Set_Pixel(i+start_x,start_y+j,WHITE);
			_LCD_Set_Pixel(i+start_x,start_y+256-j,WHITE);
		}
	}
        
       
}



/********************************************************************
* ���� : LCD_Show_Rect
* ���� : ������
* ���� : x , y �� lenth��width�� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_Show_Rect(u16 x,u16 y,u16 lenth,u16 width,u16 point_color)
{
	u16 i,j;
	for(i=0;i<lenth;i++)
        {
          _LCD_Set_Pixel(x+i,y+width,point_color);
          _LCD_Set_Pixel(x+i,y,point_color);
	}
        for(j=0;j<width;j++)
        {
          _LCD_Set_Pixel(x+lenth,y+j,point_color);
          _LCD_Set_Pixel(x,y+j,point_color);
	}
}


/********************************************************************
* ���� : LCD_Clear_Rect
* ���� : �������
* ���� : x , y �� lenth��width�� ��ɫ
* ��� : �� 
***********************************************************************/
void LCD_Clear_Rect(u16 x,u16 y,u16 lenth,u16 width,u16 point_color)
{
  u16 i,j;
  for(i=0;i<lenth;i++)
  {
    for(j=0;j<width;j++)
    {
      _LCD_Set_Pixel(x+i,y+j,point_color);
    }
  }
}
/********************************************************************
* ���� : delay_100ms
* ���� : �����ʱ
* ���� : ��
* ��� : �� 
***********************************************************************/
void delay_100ms(void)
{
    volatile u16 i = 0;
    volatile u16 time=100;
    while (time--)
    {
      i = 12000;  //
    while (i--);
    }
}


