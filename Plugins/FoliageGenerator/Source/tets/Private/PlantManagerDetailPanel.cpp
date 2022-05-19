// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantManagerDetailPanel.h"
#include "DetailLayoutBuilder.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "PlantBunchManager.h"
#include "tets.h"

// Add default functionality here for any IPlantManagerDetailPanel functions that are not pure virtual.

TSharedRef<IDetailCustomization> FPlantManagerDetailPanel::MakeInstance()
{
	return MakeShareable(new FPlantManagerDetailPanel);
}

void FPlantManagerDetailPanel::CustomizeDetails(IDetailLayoutBuilder& DetailLayout)
{
	IDetailCategoryBuilder& CustCategory = DetailLayout.EditCategory(FName("CustCategory"), LOCTEXT("CustCategory", "Custom Category"), ECategoryPriority::Important);
	TArray<TWeakObjectPtr<UObject>> customizedObjects;
	DetailLayout.GetObjectsBeingCustomized(customizedObjects);
	//为此目录添加一行记录。第一个参数用于过滤搜索的文本，第二个参数是是否为高级项。
	CustCategory.AddCustomRow(LOCTEXT("CustRow", "CustRow"), false).NameContent()	//NameContent是属性名
		[
			SNew(STextBlock).Text(LOCTEXT("Generate Button", "Generate Button"))
		].ValueContent()
		[	//ValueContent是值的内容。如果不想区分Key和Value分割的话，可以直接使用WholeRowContent,下面有。。
			SNew(SVerticalBox) + SVerticalBox::Slot().HAlign(HAlign_Fill).AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("Generate Button", "Generate Button"))
				.OnClicked_Lambda([customizedObjects]() {
					for (auto o : customizedObjects) {
						if (o.IsValid() && o->IsA(APlantBunchManager::StaticClass())) {
						Cast<APlantBunchManager>(o)->Generate();
						}
					}
					return FReply::Handled();
				})
			] 
		];

}
