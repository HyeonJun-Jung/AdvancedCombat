// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Equipment/Widget_EquipSelectWindow.h"
#include "Widget/Equipment/Widget_EquipSelect_ContentList.h"
#include "Components/TextBlock.h"
#include "Components/Overlay.h"
#include "Components/WidgetSwitcher.h"

void UWidget_EquipSelectWindow::NativeConstruct()
{
	Super::NativeConstruct();
}

void UWidget_EquipSelectWindow::UpdateContents(EItemCategory Category)
{
	switch (Category)
	{
	case EItemCategory::None:
	{
		break;
	}
	case EItemCategory::Weapon:
	{
		WidgetSwitcher->SetActiveWidget(Overlay_Weapon);
		Text_Category->SetText(FText::FromString(FString("Weapon")));
		Text_SelectedItem->SetText(FText::FromString(FString::Printf(TEXT("선택된 장비"))));
		break;
	}
	case EItemCategory::Rune:
	{
		WidgetSwitcher->SetActiveWidget(Overlay_Rune);
		Text_Category->SetText(FText::FromString(FString("Rune")));
		Text_SelectedItem->SetText(FText::FromString(FString::Printf(TEXT("선택된 룬"))));
		break;
	}
	case EItemCategory::FragmentOfGod:
	{
		WidgetSwitcher->SetActiveWidget(Overlay_Fragment);
		Text_Category->SetText(FText::FromString(FString("Fragment")));
		Text_SelectedItem->SetText(FText::FromString(FString::Printf(TEXT("선택된 신의 파편"))));
		break;
	}
	case EItemCategory::UseableItem:
	{
		WidgetSwitcher->SetActiveWidget(Overlay_Item);
		Text_Category->SetText(FText::FromString(FString("Usable Item")));
		Text_SelectedItem->SetText(FText::FromString(FString::Printf(TEXT("선택된 아이템"))));
		break;
	}
	case EItemCategory::Magic:
	{
		WidgetSwitcher->SetActiveWidget(Overlay_Magic);
		Text_Category->SetText(FText::FromString(FString("Magic")));
		Text_SelectedItem->SetText(FText::FromString(FString::Printf(TEXT("선택된 마법"))));
		break;
	}
	default:
		break;
	}

	ContentList->UpdateContents(Category);
}

