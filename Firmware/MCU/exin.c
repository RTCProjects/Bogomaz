#include "exin.h"

sbit P7_P4 = P7^4;
sbit P7_P6 = P7^6;

sbit P2_P14 = P2^14;
sbit P2_P15 = P2^15;


//инициализация входов
void InitializationEXIN(void)
{
	DP2 = 0x0000;
	DP7 = 0x0000;
	EXICON = 0xAAAA;
	EXISEL1 = 0x5500;
	/*
	
	CCM2 = 0x1113;
	CCM3 = 0x1111;
	
	CCM7 = 0x0003;
	
*/
	CC8IC = 0x0077;
	CC9IC = 0x0077;
	CC10IC = 0x0077;
	CC11IC = 0x0077;
	CC12IC = 0x0077;
	CC13IC = 0x0077;
	CC14IC = 0x0077;
	CC15IC = 0x0077;
	
	CC28IC = 0x0077;
	CC29IC = 0x0077;
	CC30IC = 0x0077;
	CC31IC = 0x0077;
}




//прерывания первой группы
/*void group1_1_in(void) interrupt CC8IC_VEC	//P2.8
{

	printf("1_1 P2.8\n");
}

void group1_2_in(void) interrupt CC9IC_VEC	//P2.9
{
		uCountersArray[1]++;
}

void group1_3_in(void) interrupt CC10IC_VEC	//P2.10
{
	printf("1_3 P2.10\n");
}

void group1_4_in(void) interrupt CC11IC_VEC	//P2.11
{
	printf("1_4 P2.11\n");
}

void group1_5_in(void) interrupt CC12IC_VEC	//P2.12
{
	printf("1_5 P2.12\n");
}

void group1_6_in(void) interrupt CC13IC_VEC	//P2.13
{
	printf("1_6 P2.13\n");
}

//прерывания второй группы
void group2_1_in(void) interrupt CC14IC_VEC	//P2.14
{
	printf("2_1 P 2.14\n");
}

void group2_2_in(void) interrupt CC15IC_VEC	//P2.15
{
	
		if(P2_P15 == 1)
		{
			printf("2_2 P2.15\n");
			return;
		}
		if(P7_P4 == 1)
		{
			printf("2_3 P7.4\n");
			return;
		}
		if(P7_P6 == 1)
		{
			printf("2_6 P7.6\n");
			return;
		}

}

void group2_3_in(void) interrupt CC28IC_VEC	//P7.4
{
	printf("2_3 P7.4\n");
}

void group2_4_in(void) interrupt CC29IC_VEC	//P7.5
{
	printf("2_4 P7.5\n");
}

void group2_5_in(void) interrupt CC30IC_VEC	//P7.6
{
	printf("2_5 P7.6\n");
}

void group2_6_in(void) interrupt CC31IC_VEC	//P7.7
{
	printf("2_6 P7.7\n");
}*/