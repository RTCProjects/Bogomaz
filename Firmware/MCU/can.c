#include "can.h"
#include "settings.h"


void CAN_Interrupt0() interrupt 0x54	//722 прерывание(передача)
{
	
}

void CAN_Interrupt1() interrupt 0x55	//222 прерывание(прием)
{
	uint8 command		= (uint8)(CAN_Message_16[3].MODATALL & 0x00FF);
	uint8 index			= (uint8)((CAN_Message_16[3].MODATALL >> 8) & 0x00FF);
	uint8 subindex	= (uint8)((CAN_Message_16[3].MODATALH >> 8) & 0x00FF);
		
	
	
	uint16 	*pFactor = 0;
	uint16 	hexValueL = 0;
	uint16	hexValueH = 0;
	uint8		canData[8];
	
	switch(command)
	{
		case 0x04:	//задание параметра
		{
			switch(index)
			{
				case 0x93: //поправочный коэфициент
				{
					hexValueL = (CAN_Message_16[3].MODATAHL);
					hexValueH = (CAN_Message_16[3].MODATAHH);
					
					pFactor =  (uint16*)&MainSettings.fFactor[subindex];
					
					*(pFactor) = hexValueL;
					*(pFactor + 1) = hexValueH;
					
					canData[0] = command;
					canData[1] = index;
					canData[2] = 0x00;
					canData[3] = subindex;
					canData[4] =  (uint8)(hexValueL & 0x00FF);
					canData[5] =  (uint8)((hexValueL >> 8) & 0x00FF);
					canData[6] =  (uint8)(hexValueH & 0x00FF);
					canData[7] =  (uint8)((hexValueH >> 8) & 0x00FF);
					
					CAN_LoadData(0,canData);
					CAN_SendMessage(0);
					
				}break;
				
				case 0xE0:	//запись во FLASH
				{
					hexValueL = (CAN_Message_16[3].MODATAHL);
					hexValueH = (CAN_Message_16[3].MODATAHH);
					
					if(subindex == 0x00){
						SETTINGS_Default();
						SETTINGS_Save();
					}
					if(subindex == 0xFF){
						SETTINGS_Save();
					}
					
					canData[0] = command;
					canData[1] = index;
					canData[2] = 0x00;
					canData[3] = subindex;
					canData[4] =  (uint8)(hexValueL & 0x00FF);
					canData[5] =  (uint8)((hexValueL >> 8) & 0x00FF);
					canData[6] =  (uint8)(hexValueH & 0x00FF);
					canData[7] =  (uint8)((hexValueH >> 8) & 0x00FF);
					
					CAN_LoadData(0,canData);
					CAN_SendMessage(0);
				}break;
			}
		}break;
		
		case 0x05:	//получение параметра
		{
			switch(index)
			{
				case 0x93:	//поправочный коэфициент
				{
					pFactor =  (uint16*)&MainSettings.fFactor[subindex];
					hexValueL = *(pFactor);
					hexValueH = *(pFactor + 1);

					canData[0] = command;
					canData[1] = index;
					canData[2] = 0x00;
					canData[3] = subindex;
					canData[4] =  (uint8)(hexValueL & 0x00FF);
					canData[5] =  (uint8)((hexValueL >> 8) & 0x00FF);
					canData[6] =  (uint8)(hexValueH & 0x00FF);
					canData[7] =  (uint8)((hexValueH >> 8) & 0x00FF);
					
					CAN_LoadData(0,canData);
					CAN_SendMessage(0);
					
				}break;
			}
		}break;
	}
	

}

void CAN_Interrupt2() interrupt 0x56	//180 прерывание(прием)
{
	
}


void CAN_Init()
{
	DP4 &= 0x00CF;					// P4.4 P4.5 настраиваем на ввод(вход CAN узла 0 и узла 1)
  ALTSEL0P4 &= 0x00DF;			    // Выключить альтернативную функцию P4.5(для P4.4 ничего не делаем)
  DP4 |= 0x00C0;					// P4.6 P4.7 настраиваем на передачу(Выход CAN узла 0 и узла 1)
  ALTSEL0P4 |= 0x00C0;			    // Включить альтернативные функции P4.6 и P4.7

	CAN_IC0 = 0x005C;	//722
	CAN_IC1 = 0x0060;	//222
	CAN_IC2 = 0x0074;	//180
	
	NCR0  = 0x0041;		//разрешаем вносить изменения в конфигурацию
	NPCR0 = 0x0000;		//Выбрали P4.5 для приема в CAN узел 0
	NBTR0 = 0x2341;		//скорость узла 500kb/sec при тактировании 16Мгц
	NCR0  = 0x0000;		//Запрет после реконфигурации
	
	/*
		Список областей сообщений
	
		На приём
			1. 222 - параметры(прерывание)
	    2. 180 - прием старт/стоп БДГП(прерывание)
		На передачу
		  1. 422 - параметры
			2. 722 - данные (прерывание)
			3. 280 - передача старт/стоп
	*/
	//422
	CAN_Message_16[0].MOCTRH = 0x0E08;
	CAN_Message_16[0].MOCTRL = 0x0000;
	CAN_Message_16[0].MOIPRL = 0x0000;	/*выбор линии прерываний для передачи - 0*/
  CAN_Message_16[0].MOFCRH = 0x0800; // DLC = 8, запретить прерывание на передачу 
	
	//722
	CAN_Message_16[1].MOCTRH = 0x0E08;
  CAN_Message_16[1].MOCTRL = 0x0000;
  CAN_Message_16[1].MOIPRL = 0x0000;	// выбор линии прерываний для передачи - 0
	CAN_Message_16[1].MOFCRH = 0x0802;	// разрешить прерывание на передачу
	
	//280
	CAN_Message_16[2].MOCTRH = 0x0E08;
	CAN_Message_16[2].MOCTRL = 0x0000;
	CAN_Message_16[2].MOIPRL = 0x0000;	/*выбор линии прерываний для передачи - 0*/
  CAN_Message_16[2].MOFCRH = 0x0800; // DLC = 8, запретить прерывание на передачу 
	
	//222
	CAN_Message_16[3].MOCTRH = 0x0080;
  CAN_Message_16[3].MOCTRL = 0x0000;
  CAN_Message_16[3].MOIPRL = 0x0001;	//выбор линии прерываний по приему - 1
	CAN_Message_16[3].MOFCRH = 0x0801;	//разрешить прерываение на прием
	
	//180
	CAN_Message_16[4].MOCTRH = 0x0080;
  CAN_Message_16[4].MOCTRL = 0x0000;
  CAN_Message_16[4].MOIPRL = 0x0002;	// выбор линии прерываний по приему - 2
	CAN_Message_16[4].MOFCRH = 0x0801;	//разрешить прерываение на прием

	delay(10);
	
	CAN_Message_16[0].MOARH = 0x9088;	 // идентификатор 422h
  CAN_Message_16[0].MOAMRH = 0x3FFF; //	маска идентификатора	
		
	CAN_Message_16[1].MOARH = 0x9C88;	 // идентификатор 722h
  CAN_Message_16[1].MOAMRH = 0x3FFF; //	маска идентификатора		
	
	CAN_Message_16[2].MOARH = 0x8A00;	 // идентификатор 280h
  CAN_Message_16[2].MOAMRH = 0x3FFF; //	маска идентификатора		
	
	CAN_Message_16[3].MOARH = 0x8888;	 // идентификатор 222h
  CAN_Message_16[3].MOAMRH = 0x3FFF; //	маска идентификатора	
	
	CAN_Message_16[4].MOARH = 0x8600;	 // идентификатор 180h
  CAN_Message_16[4].MOAMRH = 0x3FFF; //	маска идентификатора	

	delay(10);
	
	PANCTR_H = 0x0100;				 // message object 0 -> List 1
  PANCTR = 0x0002;				   
   _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
	 
	PANCTR_H = 0x0101;				 //	message object 1 -> List 1
  PANCTR = 0x0002;			    	 
   _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 
	 
	 
	PANCTR_H = 0x0102;				 //	message object 2 -> List 1
  PANCTR = 0x0002;			    	 
   _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 	 
	 
	PANCTR_H = 0x0103;				 //	message object 3 -> List 1
  PANCTR = 0x0002;			    	 
   _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 	 


	PANCTR_H = 0x0104;				 //	message object 4 -> List 1
  PANCTR = 0x0002;			    	 
   _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); 	 


	CAN_Message_16[0].MOCTRH = 0x0020; 
	CAN_Message_16[0].MOCTRL = 0x0000;
	
	CAN_Message_16[1].MOCTRH = 0x0020; 
	CAN_Message_16[1].MOCTRL = 0x0000;
	
	CAN_Message_16[2].MOCTRH = 0x0020; 
	CAN_Message_16[2].MOCTRL = 0x0000;
	
	CAN_Message_16[3].MOCTRH = 0x0020; 
	CAN_Message_16[3].MOCTRL = 0x0000;
	
	CAN_Message_16[4].MOCTRH = 0x0020; 
	CAN_Message_16[4].MOCTRL = 0x0000;	
	
}
void CAN_SendMessage(uint8 moNubmer)
{ 

	CAN_Message_16[moNubmer].MOCTRH = 0x0100;	
}
void CAN_LoadData(uint8 moNubmer,uint8 *canData)
{
	CAN_Message_16[moNubmer].MODATALL = (canData[1] << 8) | canData[0];
	CAN_Message_16[moNubmer].MODATALH = (canData[3] << 8) | canData[2];
	CAN_Message_16[moNubmer].MODATAHL = (canData[5] << 8) | canData[4];
	CAN_Message_16[moNubmer].MODATAHH = (canData[7] << 8) | canData[6];
}


