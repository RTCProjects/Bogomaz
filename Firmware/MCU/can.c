#include "can.h"

void CAN_Init()
{
	DP4 &= 0x00CF;					// P4.4 P4.5 настраиваем на ввод(вход CAN узла 0 и узла 1)
  ALTSEL0P4 &= 0x00DF;			    // Выключить альтернативную функцию P4.5(для P4.4 ничего не делаем)
  DP4 |= 0x00C0;					// P4.6 P4.7 настраиваем на передачу(Выход CAN узла 0 и узла 1)
  ALTSEL0P4 |= 0x00C0;			    // Включить альтернативные функции P4.6 и P4.7

	CAN_IC0 = 0x005C;
	CAN_IC1 = 0x0060;	
	CAN_IC2 = 0x0074;	
	
	NCR0  = 0x0041;		//разрешаем вносить изменения в конфигурацию
	NPCR0 = 0x0000;		//Выбрали P4.5 для приема в CAN узел 0
	NBTR0 = 0x2341;		//скорость узла 500kb/sec при тактировании 16Мгц
	NCR0  = 0x0000;		//Запрет после реконфигурации
	
	CAN_Message_16[0].MOCTRH = 0x0E08;
	CAN_Message_16[0].MOCTRL = 0x0000;
	CAN_Message_16[0].MOIPRL = 0x0000;	/*выбор линии прерываний для передачи - 0*/
  CAN_Message_16[0].MOFCRH = 0x0800; // DLC = 8, запретить прерывание на передачу 
	
	CAN_Message_16[1].MOCTRH = 0x0080;
  CAN_Message_16[1].MOCTRL = 0x0000;
  CAN_Message_16[1].MOIPRL = 0x0000;	// выбор линии прерываний по приему - 0
	CAN_Message_16[1].MOFCRH = 0x0801;	//разрешить прерываение на прием
	
	CAN_Message_16[2].MOCTRH = 0x0080;
  CAN_Message_16[2].MOCTRL = 0x0000;
  CAN_Message_16[2].MOIPRL = 0x0002;	// выбор линии прерываний по приему - 2
	CAN_Message_16[2].MOFCRH = 0x0801;	//разрешить прерываение на прием

	CAN_Message_16[3].MOCTRH = 0x0E08;
  CAN_Message_16[3].MOCTRL = 0x0000;
  CAN_Message_16[3].MOIPRL = 0x0010;	// выбор линии прерываний для передачи - 1
	CAN_Message_16[3].MOFCRH = 0x0802;	// разрешить прерывание на передачу
	
	CAN_Message_16[4].MOCTRH = 0x0E08;
  CAN_Message_16[4].MOCTRL = 0x0000;
  CAN_Message_16[4].MOIPRL = 0x0000;	// выбор линии прерываний для передачи 
	CAN_Message_16[4].MOFCRH = 0x0800;	// DLC = 8, запрет прерываний 
	
	delay(10);
	
	CAN_Message_16[0].MOARH = 0x8000;	 // идентификатор 000h
  CAN_Message_16[0].MOAMRH = 0x3FFF; //	маска идентификатора	
		
	CAN_Message_16[1].MOARH = 0x8400;	 // идентификатор 100h
  CAN_Message_16[1].MOAMRH = 0x3FFF; //	маска идентификатора		
	
	CAN_Message_16[2].MOARH = 0x8404;	 // идентификатор 101h
  CAN_Message_16[2].MOAMRH = 0x3FFF; //	маска идентификатора		
	
	CAN_Message_16[3].MOARH = 0x8804;	 // идентификатор 201h
  CAN_Message_16[3].MOAMRH = 0x3FFF; //	маска идентификатора	
	
	CAN_Message_16[4].MOARH = 0x8004;	 // идентификатор 001h
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
void CAN_SendMessage(char	*buffer)
{
	CAN_Message_16[0].MODATALL = buffer[0];
	CAN_Message_16[0].MODATALH = buffer[1];
	CAN_Message_16[0].MODATAHL = buffer[2];
	CAN_Message_16[0].MODATAHH = buffer[3];
	
	CAN_Message_16[0].MOCTRH = 0x0100;	
}
