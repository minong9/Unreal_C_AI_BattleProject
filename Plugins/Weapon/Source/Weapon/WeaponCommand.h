#pragma once

#include "CoreMinimal.h"
#include "FrameWork/Commands/Commands.h" //�̱���, ����ƽ �̿��ϱ� ���� ����ϴ� ���


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
