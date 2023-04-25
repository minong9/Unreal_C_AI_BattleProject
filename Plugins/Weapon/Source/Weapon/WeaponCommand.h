#pragma once

#include "CoreMinimal.h"
#include "FrameWork/Commands/Commands.h" //싱글톤, 스태틱 이용하기 위해 사용하는 방법


class WEAPON_API FWeaponCommand
	: public TCommands<FWeaponCommand>
{
public:
	FWeaponCommand();
	~FWeaponCommand();

	void Startup();

public:
	void RegisterCommands() override;

private:
	TSharedPtr<FExtender> Extender;

	TSharedPtr<FUICommandList> Command;
	TSharedPtr<FUICommandInfo> Id;

private:
	void AddToolBar(FToolBarBuilder& InBuilder);
	void OnClicked();
};
