#ifndef _CAN_H
#define _CAN_H

#include "main.h"

void CAN_Init(void);
void CAN_LoadData(uint8 moNubmer,uint8 *canData);
void CAN_SendMessage(uint8 moNubmer);
#endif
