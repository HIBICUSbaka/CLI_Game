#pragma once
#include "GameAppStructs.h"

void SetManagedCurrScene(SCENENODE* sceneNode);

SCENENODE* GetManagedCurrScene();

void InitCurrScene();

void UpdateCurrScene();

void SwitchSceneTo(SCENENODE* sceneNode);
