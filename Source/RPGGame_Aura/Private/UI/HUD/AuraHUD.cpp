// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/AuraUserWidget.h"

#include "UI/WidgetController/OverlayAuraWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
UOverlayAuraWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	//ȷ��ֻ����һ��OverlayAuraWidgetController
	if (OverlayAuraWidgetController == nullptr)
	{
		OverlayAuraWidgetController = NewObject<UOverlayAuraWidgetController>(this, OverlayWidgetControllerClass);
		//����������֮ǰ�����Ľṹ�����UAuraController��������ݼ�
		OverlayAuraWidgetController->SetWidgetControllerParams(WCParams);
		//��GAS���Ա仯�¼�������ֻ��UAuraController�д�����δ��������д
		OverlayAuraWidgetController->BindCallbacksToDependencies();

	}
	return OverlayAuraWidgetController;

}


//����������ڴ���UI��Controller������GetOverlayWidgetController�����ݣ���ui��ӵ���Ļ��
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{

	if (!PC || !PS || !ASC || !AS)
	{
		//UE_LOG(LogTemp, Error, TEXT("InitOverlay: One or more input params are null: PC=%s, PS=%s, ASC=%s, AS=%s"),*GetNameSafe(PC), *GetNameSafe(PS), *GetNameSafe(ASC), *GetNameSafe(AS));
		return;
	}

	/*
	��Ķ���ʧ����Դ�������д����� BeginPlay �м��豾����������ĺͱ�����ϵͳһ�����ڣ����ڡ�Play As Client����PIE ��ͻ��ˣ����ڷ�����������ʱ����Щָ�����Ϊ null���Ӷ����� check ���±���������ԭ��

BeginPlay �ڷǱ��� PlayerController����������˻�Զ�̿ͻ��˵�δӵ�п��������ϱ����ã�GetLocalPlayer() ���� nullptr��

�ڿͻ��˳�ʼ�������Enhanced Input ��ϵͳ��δ��ע�����ã�ֱ�ӵ��û�ʧ�ܡ�

ʹ�� check(...) ǿ�ƶ��Ի���ָ��Ϊ null ʱ���������������ǰ�ȫ���ˡ�
	*/
	if (!OverlayWidgetClass || !OverlayWidgetControllerClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("OverlayWidgetClass or ControllerClass not set"));
		return;
	}

	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);
	if (!Widget)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create OverlayWidget"));
		return;
	}

	OverlayWidget = Cast<UAuraUserWidget>(Widget);
	if (!OverlayWidget)
	{
		UE_LOG(LogTemp, Error, TEXT("Cast to UAuraUserWidget failed"));
		return;
	}

	FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
	UOverlayAuraWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParams);
	if (!WidgetController)
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetController is null"));
		return;
	}

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	Widget->AddToViewport();

}

