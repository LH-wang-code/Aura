// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/AuraHUD.h"
#include "Blueprint/UserWidget.h"
#include "UI/Widgets/AuraUserWidget.h"

#include "UI/WidgetController/OverlayAuraWidgetController.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
UOverlayAuraWidgetController* AAuraHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	//确保只创建一次OverlayAuraWidgetController
	if (OverlayAuraWidgetController == nullptr)
	{
		OverlayAuraWidgetController = NewObject<UOverlayAuraWidgetController>(this, OverlayWidgetControllerClass);
		//这里是我们之前创建的结构体包含UAuraController的相关数据集
		OverlayAuraWidgetController->SetWidgetControllerParams(WCParams);
		//绑定GAS属性变化事件，我们只在UAuraController中创建还未在子类重写
		OverlayAuraWidgetController->BindCallbacksToDependencies();

	}
	return OverlayAuraWidgetController;

}


//这个方法用于创建UI和Controller，调用GetOverlayWidgetController绑定数据，将ui添加到屏幕上
void AAuraHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{

	if (!PC || !PS || !ASC || !AS)
	{
		//UE_LOG(LogTemp, Error, TEXT("InitOverlay: One or more input params are null: PC=%s, PS=%s, ASC=%s, AS=%s"),*GetNameSafe(PC), *GetNameSafe(PS), *GetNameSafe(ASC), *GetNameSafe(AS));
		return;
	}

	/*
	你的断言失败来源于这两行代码在 BeginPlay 中假设本地玩家上下文和本地子系统一定存在，但在“Play As Client”（PIE 多客户端）或在服务器上运行时，这些指针可能为 null，从而触发 check 导致崩溃。常见原因：

BeginPlay 在非本地 PlayerController（如服务器端或远程客户端的未拥有控制器）上被调用，GetLocalPlayer() 返回 nullptr。

在客户端初始化流程里，Enhanced Input 子系统还未被注册或可用，直接调用会失败。

使用 check(...) 强制断言会在指针为 null 时立即崩溃，而不是安全回退。
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

