#include "WeaponCommand.h"
#include "WeaponStyle.h"

#include "LevelEditor.h"
#include "WeaponAssetEditor.h"

FWeaponCommand::FWeaponCommand()
	: TCommands("Toolbar_Button", FText::FromString(""), NAME_None, FEditorStyle::GetStyleSetName())
{
	Command = MakeShareable(new FUICommandList());
}

FWeaponCommand::~FWeaponCommand()
{
	if (Command.IsValid())
		Command.Reset();

	if (Extender.IsValid())
		Extender.Reset();
}

void FWeaponCommand::Startup()
{
	FWeaponCommand::RegisterCommands();

	Extender = MakeShareable(new FExtender());

	FToolBarExtensionDelegate toolbar = FToolBarExtensionDelegate::CreateRaw(this, &FWeaponCommand::AddToolBar);
	Extender->AddToolBarExtension("Settings", EExtensionHook::After, Command, toolbar);

	FLevelEditorModule& levelEditor = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");
	levelEditor.GetToolBarExtensibilityManager()->AddExtender(Extender);
}

void FWeaponCommand::RegisterCommands()
{
#define LOCTEXT_NAMESPACE ""
	UI_COMMAND(Id, "Weapon", "", EUserInterfaceActionType::Button, FInputChord());
#undef LOCTEXT_NAMESPACE

	FExecuteAction action;
	action.BindRaw(this, &FWeaponCommand::OnClicked);

	Command->MapAction(Id, action, FCanExecuteAction());
}

void FWeaponCommand::AddToolBar(FToolBarBuilder& InBuilder)
{
	InBuilder.AddSeparator();
	InBuilder.AddToolBarButton(Id, NAME_None, FText::FromString("Weapon"), FText::FromString("Weapon Asset Editor"), FWeaponStyle::Get()->ToolBar_Icon, NAME_None);
}

void FWeaponCommand::OnClicked()
{
	FWeaponAssetEditor::OpenWindow();
}
