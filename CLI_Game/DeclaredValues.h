#pragma once

#include "DefinedValues.h"
#include "Structs.h"

#define DELTATIME 16

int GetDeclaredDeltaTime();

void SetDeclaredDeltaTime(int value);

HANDLE GetPrintHandle();

void SetPrintHandle(HANDLE handle);

void SwapPrintChain();

char* GetOutputBufferToUpdate();

char* GetOutputBufferToPrint();

#define UIOBJSIZE 10000

UIOBJECT* GetUIObj();

int* GetUIOIndex();

void SetUIOIndex(int value);

void AddUIOIndex();

UIOBJECT* GetTopUIO();

void SetTopUIO(UIOBJECT* uiObj);
