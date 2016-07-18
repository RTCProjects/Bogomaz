#ifndef _SETTINGS_H
#define _SETTINGS_H

#include "main.h"

#define FACTORS_COUNT	12

typedef struct
{
	float 	fFactor[FACTORS_COUNT];
	float 	fDeadTime;
	uint16	uLimitMax;
	uint16	uLimitMin;
}tSettings;

extern tSettings	MainSettings;

void	SETTINGS_Init(void);
void	SETTINGS_Default(void);
void 	SETTINGS_Save(void);
uint8  SETTINGS_Load(void);

#endif
