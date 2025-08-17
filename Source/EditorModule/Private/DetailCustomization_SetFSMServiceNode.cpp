// Fill out your copyright notice in the Description page of Project Settings.


#include "DetailCustomization_SetFSMServiceNode.h"
#include "DetailCategoryBuilder.h"
#include "DetailWidgetRow.h"
#include "DetailLayoutBuilder.h"
#include "Widgets/Input/SComboBox.h"
#include "Monster/BTS_SetFSMState.h"
#include "Widgets/Input/SEditableTextBox.h"


void DetailCustomization_SetFSMServiceNode::CustomizeDetails(IDetailLayoutBuilder& DetailBuilder)
{
	TArray<TWeakObjectPtr<UObject>> selectedObjects;
	DetailBuilder.GetObjectsBeingCustomized(selectedObjects);
	
	if (selectedObjects.Num() > 0)
	{
		m_ServiceNode = Cast<UBTS_SetFSMState>(selectedObjects[0].Get());
	}
	
	if (m_ServiceNode.IsValid())
	{
		IDetailCategoryBuilder& enumCategory = DetailBuilder.EditCategory("EnumStates", FText::FromString(TEXT("Setting EnumStates")));
		
		enumCategory.AddCustomRow(FText::FromString("EnumClassName"))
		   .NameContent()
		   [
			   SNew(STextBlock).Text(FText::FromString("EnumClassName"))
		   ]
		   .ValueContent()
		   [
			   SNew(SEditableTextBox)
			   .Text(this, &DetailCustomization_SetFSMServiceNode::GetEnumClassName)
			   .OnTextCommitted(this, &DetailCustomization_SetFSMServiceNode::OnEnumClassNameCommitted)
		   ];

		const UEnum* enumType = FindObject<UEnum>(ANY_PACKAGE, *m_ServiceNode->GetEnumClassName().ToString(), true); // 마지막으로 디스크에 저장한 EnumClassName을 Find.
		if (enumType != nullptr)
		{
			UpdateComboBox(enumType);
		
			enumCategory.AddCustomRow(FText::FromString("FSMStates"))
			   .NameContent()
			   [
				   SNew(STextBlock).Text(FText::FromString("FSMStatesList"))
			   ]
			   .ValueContent()
			   [
					SAssignNew(m_ComboBox, SComboBox<TSharedPtr<FString>>)
				   .OptionsSource(&m_FSMStates)
				   .OnGenerateWidget(this, &DetailCustomization_SetFSMServiceNode::MakeComboBox) // 어레이원소개수만큼 호출.
				   .OnSelectionChanged(this, &DetailCustomization_SetFSMServiceNode::OnSelectFSMState) // 각 원소 선택시 호출.
				   .InitiallySelectedItem(m_FSMStates[m_ServiceNode->GetFSMStateIndex()])
				   [
						SNew(STextBlock)
						.Text(this, &DetailCustomization_SetFSMServiceNode::GetSelectedFSMState)
				   ]
			   ];
		}
	}
}

void DetailCustomization_SetFSMServiceNode::OnEnumClassNameCommitted(const FText& NewText, ETextCommit::Type CommitInfo)
{
	UE_LOG(LogTemp, Warning, TEXT("DetailCustomization_SetFSMServiceNode :: OnEnumClassNameCommitted"));
	
	if (m_ServiceNode.IsValid())
	{
		const UEnum* enumType = FindObject<UEnum>(ANY_PACKAGE, *NewText.ToString(), true);

		UpdateComboBox(enumType);

		m_ServiceNode->SetEnumClassName(enumType != nullptr ? NewText.ToString() : "");
		m_ServiceNode->SetFSMStateName(enumType != nullptr ? *m_FSMStates[0] : "");
		m_ServiceNode->SetFSMStateIndex(0);

		if (m_ComboBox.IsValid())
		{
			m_ComboBox->RefreshOptions();
		}
	}
}

TSharedRef<SWidget> DetailCustomization_SetFSMServiceNode::MakeComboBox(TSharedPtr<FString> option)
{
	// 태스크노드를 클릭 후, 콤보박스를 눌렀을 때 원소의 개수만큼 호출.
	// 처음 누른 이후에는 그다음부턴 눌러도 호출안됨. 이미 만들어졌으니까. 다시 호출하려면 다른 노드를 클릭 후, 다시 태스크노드를 클릭한다음 콤보박스를 누르면 된다.
	
	return SNew(STextBlock).Text(FText::FromString(*option));
}

void DetailCustomization_SetFSMServiceNode::UpdateComboBox(const UEnum* enumType)
{
	m_FSMStates.Empty();
	
	if (enumType != nullptr)
	{
		if (enumType->NumEnums() == 0) // EnumClass 정의는 되어있는데, 원소를 선언 안해놨을경우
		{
			m_FSMStates.Add(MakeShared<FString>("Empty!!"));
		}
		else 
		{
			for (int32 i = 0; i < enumType->NumEnums() - 1; i++)
			{
				m_FSMStates.Add(MakeShared<FString>(enumType->GetNameStringByIndex(i)));
			}
		}
	}
	else 
	{
		m_FSMStates.Add(MakeShared<FString>("EnumClass Is Not Valid"));
	}
}

void DetailCustomization_SetFSMServiceNode::OnSelectFSMState(TSharedPtr<FString> newValue, ESelectInfo::Type) const
{
	// 콤보박스의 원소들을 선택했을 때 호출. 현재 선택되어져있는것을 선택했을 경우엔 호출안된다.
	
	if (newValue.IsValid())
	{
		const uint8 enumIndex = static_cast<uint8>(m_FSMStates.IndexOfByKey(newValue)); //내부적으로 iterator 반복문으로 찾음.
		m_ServiceNode->SetFSMStateIndex(enumIndex);
		m_ServiceNode->SetFSMStateName(*newValue);
	}
}


