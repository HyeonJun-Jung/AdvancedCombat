// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/Inventory/Widget_Inventory.h"
#include "Widget/Inventory/Widget_InvContentList.h"
#include "Widget/Inventory/Widget_Inventory_CategorySlot.h"
#include "Components/ScrollBox.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UWidget_Inventory::NativeConstruct()
{
	Super::NativeConstruct();

	CategorySlots.Empty();
	TArray<UWidget*> childrens = ScrollBox_Category->GetAllChildren();
	for (int i = 0; i < childrens.Num(); i++)
	{
		UWidget_Inventory_CategorySlot* CategorySlot =
			Cast<UWidget_Inventory_CategorySlot>(childrens[i]);
		if (CategorySlot)
		{
			CategorySlots.Add(CategorySlot);
		}
	}

	Button_BeforeCategory->OnClicked.Clear();
	Button_BeforeCategory->OnClicked.AddDynamic(this, &UWidget_Inventory::ShowBeforeCategory);
	Button_NextCategory->OnClicked.Clear();
	Button_NextCategory->OnClicked.AddDynamic(this, &UWidget_Inventory::ShowNextCategory);

	FTimerHandle timerHandle;
	GetWorld()->GetTimerManager().SetTimer(timerHandle, [&]() {
		UpdateContents(EItemCategory::Weapon);
		}, 0.25, false);
}

void UWidget_Inventory::UpdateContents(EItemCategory Category)
{
	switch (Category)
	{
	case EItemCategory::None:
		break;
	case EItemCategory::Weapon:
		Text_CategoryName->SetText(FText::FromString(FString::Printf(TEXT("무기"))));
		break;
	case EItemCategory::Rune:
		Text_CategoryName->SetText(FText::FromString(FString::Printf(TEXT("룬"))));
		break;
	case EItemCategory::FragmentOfGod:
		Text_CategoryName->SetText(FText::FromString(FString::Printf(TEXT("신의 파편"))));
		break;
	case EItemCategory::UseableItem:
		Text_CategoryName->SetText(FText::FromString(FString::Printf(TEXT("사용 아이템"))));
		break;
	case EItemCategory::Magic:
		Text_CategoryName->SetText(FText::FromString(FString::Printf(TEXT("마법"))));
		break;
	case EItemCategory::Upgrades:
		Text_CategoryName->SetText(FText::FromString(FString::Printf(TEXT("강화 전용 아이템"))));
		break;
	case EItemCategory::Hunting:
		Text_CategoryName->SetText(FText::FromString(FString::Printf(TEXT("기타"))));
		break;
	default:
		break;
	}

	for (int i = 0; i < CategorySlots.Num(); i++)
	{
		if (CategorySlots[i]->Category == Category)
		{
			CategorySlots[i]->Hover();
		}
		else
			CategorySlots[i]->UnHover();
	}

	ContentList->UpdateContents(Category);
}

void UWidget_Inventory::ShowBeforeCategory()
{
	if (CurrentCategoryIdx - 1 >= 0)
	{
		CurrentCategoryIdx--;

		ScrollBox_Category->ScrollWidgetIntoView(CategorySlots[CurrentCategoryIdx]);
		UpdateContents(CategorySlots[CurrentCategoryIdx]->Category);
	}
}

void UWidget_Inventory::ShowNextCategory()
{
	if (CurrentCategoryIdx + 1 < CategorySlots.Num())
	{
		CurrentCategoryIdx++;

		ScrollBox_Category->ScrollWidgetIntoView(CategorySlots[CurrentCategoryIdx]);
		UpdateContents(CategorySlots[CurrentCategoryIdx]->Category);
	}
}

