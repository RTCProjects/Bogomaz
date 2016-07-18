#include "main.h"
#include "can.h"
#include "flash.h"
#include "exin.h"
#include "settings.h"
#include "asc0.h"

sbit P3_P1 = P3^1;

sbit P9_P0 = P9^0;
sbit P9_P1 = P9^1;

sbit P9_P4 = P9^4;
sbit P9_P5 = P9^5;

sbit P9_P7 = P9^7;

/*
T0 		- таймер 1 сек.
Т7/Т8 - таймера ШИМ генерации
Т1		- таймер опроса P7 входов
*/
uint16	uCountersArray[12];



void main(void)
{

	int i = 0;
	int *mainP = 0;
	
	mainP = (int*)malloc(sizeof(int));
	
  init_asc0(8000000, 9600);

	SETTINGS_Init();


	DP9 = 0x00B3;
	P9 = 0x0000;
	
	
	
	T01CON = 0x0004;

	T0IC = 0x006A;
	
	T78CON = 0x0000;
	T7IC = 0x006B;
	T8IC = 0x006C;
	
	CC16IC = 0x0075;
	CC20IC = 0x0076;

	
	//P9.0
	CC16 = 0xFC88;	
	CCM4 |= 0x0006;	//подключаем CC16O в режиме сравнения к таймеру 7 генерация прерывания по сравнению

	//P9.4
	CC20 = 0xFC88;
	CCM5 |= 0x000E;	//подключаем CC20О в режиме сравнения к таймеру 8 генерация прерывания по сравнению

	InitializationEXIN();
	
	T0R = 1;
	T7R = 1;
	T8R = 1;
	
	IEN = 1;
	
	CAN_Init();
	

	
	

	/*
		for(i = 0;i<10;i+=2)
		{
			FLASH_WriteWord(0x030000 + i,i);
			FLASH_FSRControl();
			
			printf("%d OK\n",i);
		}
	*/
	IEN = 1;
	

		while(1)
		{
		
		}
}
void _timer0(void) interrupt 0x20
{
	//P3_P1 =! P3_P1;
		/*	
	if(P3_P1 == 1)
	{
		CCM4 = 0;
	}
	else
	{
		CCM4 = 0x00F7;
	}*/

	P9_P7 =! P9_P7;
	T0 = 0;	
	
}
void _timer7(void) interrupt 0x3D	
{
	
	T7 = 0xFC26;

	P9_P0 = 0;	//выключаем верхний ключ
	P9_P1 = 1;	//включаем нижний ключ
	

}
void cc16_io(void) interrupt 0x30
{
	
	
	P9_P1 = 0;	//выключаем нижний ключ
	P9_P0 = 1;	//включаем верхний ключ
	


	
}
void cc20_io(void) interrupt 0x34
{
	P9_P5 = 0;
	P9_P4 = 1;
}

void _timer8(void) interrupt 0x3E	
{

	
	T8 = 0xFC26;
	
	P9_P4 = 0;
	P9_P5 = 1;
}

void delay(unsigned long z){
  unsigned long x;

  for (x = 0; x < z; x++);
}