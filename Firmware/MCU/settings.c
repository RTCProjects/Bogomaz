#include "settings.h"
#include "flash.h"

tSettings	MainSettings;

void	SETTINGS_Init()
{

	if(SETTINGS_Load())
		printf("Load OK\n");
	else
	{
		SETTINGS_Default();
		SETTINGS_Save();
		printf("Settngs default\n");
	}
}

void	SETTINGS_Default()
{
	int i = 0;
	
	for(i = 0;i<FACTORS_COUNT;i++){
		MainSettings.fFactor[i] = 1.5f;
	}
	
	MainSettings.fDeadTime = 0.004f;
	MainSettings.uLimitMin = 10;
	MainSettings.uLimitMax = 1480;
	
}
void 	SETTINGS_Save()
{
	int i = 0,addr_counter = 0;
	
	 uint16  *pSettings = (tSettings*)&MainSettings;
	 uint16 sizeSettings = sizeof(MainSettings);
	
	 uint16	chkSum = 0;
	
	 for(i = 0;i<sizeSettings >> 1;i++)
			chkSum += *(pSettings + i);
	
	FLASH_EraseSector(0x22000);
	FLASH_FSRControl();
	
	FLASH_WriteWord(0x22000,chkSum);
	FLASH_FSRControl();
	
	for(i = 0;i<sizeSettings;i+=2)
	{
		FLASH_WriteWord(0x22002 + i,*(pSettings + addr_counter));
		FLASH_FSRControl();
		

		addr_counter++;
	}
}
uint8 SETTINGS_Load()
{
	int i = 0;
	unsigned int far *addressChkSum = (unsigned int far *) 0x022000;
	unsigned int far *addressData = (unsigned int far *) 0x022002;
	
	uint16	*pSettings = (tSettings*)&MainSettings;
	uint16	dataSize = sizeof(MainSettings);
	uint16  chkSum = 0;
	
	for(i = 0;i<dataSize >> 1;i++){
		chkSum += *(addressData + i);

	}

	if(chkSum != *addressChkSum)
		return 0;
	
	
	for(i = 0;i<dataSize >> 1;i++){
		*(pSettings + i) = *(addressData + i);
		
	}
	return 1;
}
