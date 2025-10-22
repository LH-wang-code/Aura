// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability/MMC/MMC_MaxHealth.h"
#include "Ability/AuraAttributeSet.h"
#include "GameplayEffect.h"
#include "GameplayEffectAttributeCaptureDefinition.h"
UMMC_MaxHealth::UMMC_MaxHealth()
{
	VigorDef.AttributeToCapture = UAuraAttributeSet::GetVigorAttribute();

	VigorDef.AttributeSource = EGameplayEffectAttributeCaptureSource::Target;
	VigorDef.bSnapshot = false;

	RelevantAttributesToCapture.Add(VigorDef);

}

float UMMC_MaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	const FGameplayTagContainer* SourcaeTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = SourcaeTags;
	EvaluateParameters.TargetTags = TargetTags;
	return 0;
}
