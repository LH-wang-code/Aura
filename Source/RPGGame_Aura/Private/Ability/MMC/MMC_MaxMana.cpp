// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/MMC/MMC_MaxMana.h"
#include "Ability/AuraAttributeSet.h"
#include "Interaction/CombatInterface.h"
#include "GameplayEffect.h"
#include "GameplayEffectAttributeCaptureDefinition.h"
UMMC_MaxMana::UMMC_MaxMana()
{
	IntelligenceDef.AttributeToCapture = UAuraAttributeSet::GetIntelligenceAttribute();
	IntelligenceDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDef.bSnapshot = false;
	RelevantAttributesToCapture.Add(IntelligenceDef);
}

float UMMC_MaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourceTag = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTag = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourceTag;
	EvaluateParameters.TargetTags = TargetTag;

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDef, Spec, EvaluateParameters, Intelligence);

	Intelligence = FMath::Max<float>(0.f, Intelligence);
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(Spec.GetContext().GetSourceObject());

	const int32 PlayerLevel = CombatInterface->GetPlayerLevel();
	return 50 + Intelligence * 2 + 10 * PlayerLevel;
}
