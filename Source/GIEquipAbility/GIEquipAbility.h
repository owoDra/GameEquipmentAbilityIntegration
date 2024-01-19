// Copyright (C) 2024 owoDra

#pragma once

#include "Modules/ModuleManager.h"


/**
 *  Modules for the main features of the Game Equipment: Ability Integration plugin
 */
class FGIEquipAbilityModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

};
