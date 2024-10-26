// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "IDetailCustomization.h"
#include "Monster/BTS_SetFSMState.h"
#include "CoreMinimal.h"


class EDITORMODULE_API DetailCustomization_SetFSMServiceNode : public IDetailCustomization
{
 
public:
	static TSharedRef<IDetailCustomization> MakeInstance() { return MakeShareable(new DetailCustomization_SetFSMServiceNode); }
	virtual void CustomizeDetails(IDetailLayoutBuilder& DetailBuilder) override;
	
	void OnEnumClassNameCommitted(const FText& NewText, ETextCommit::Type CommitInfo);
	TSharedRef<SWidget> MakeComboBox(TSharedPtr<FString> option);
	void UpdateComboBox(const UEnum* enumType);
	void OnSelectFSMState(TSharedPtr<FString> newValue, ESelectInfo::Type) const;
	
	FText GetSelectedFSMState() const
	{
		return m_ServiceNode.IsValid() ? m_ServiceNode->GetFSMStateName() : FText::GetEmpty();
	}
	
	FText GetEnumClassName() const
	{ 
		return m_ServiceNode.IsValid() ? m_ServiceNode->GetEnumClassName() : FText::GetEmpty();
	}


private:
	TSharedPtr<SComboBox<TSharedPtr<FString>>> m_ComboBox;
	TWeakObjectPtr<UBTS_SetFSMState> m_ServiceNode;
	TArray<TSharedPtr<FString>> m_FSMStates;
};
