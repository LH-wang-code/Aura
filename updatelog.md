2025.10.16



修改bug，涉及到AuraCharacter.cpp,AuraController.cpp,AuraHUD.cpp文件，修改了一些关于check导致关于以player as listener/client启动的崩溃问题，但是出现了新的问题（也不一定算问题，是跟教程不一样的情况）在服务端的UI不显示

在AuraEffectActor中添加新的游戏效果Infinite，蓝图更新BP_FireArea和GE_FIreArea



2025.10.17

修改Infinite游戏效果，修复离开FireArea扣血效果不移除的问题（详情见下）

添加InstantGameplayEffect、DurationGameplayEffect、InfiniteGameplayEffect的效果增加和移除策略，涉及到AuraEffectorActor类

增加蓝量和血量的限制区间，在AttributeSet中重写PreAttributeChange

添加context上下文信息，在AttributeSet中重写PostGameplayEffectExecute，调用SetEffectProperties方法

在蓝图中增加红蓝量的等级效果，使用CurveTable控制，修改了在AuraEffectActor中EffectSpecHandle的参数（设置为等级）





2025.10.18

增加GameplayTags相关内容

增加MessageWidgetData，传递道具信息用于制作UI播报

修改角色属性广播形式，使用AddLambda代替回调函数广播



2025.10.20



增加红蓝属性条的滞后效果，并修改一些bug，修改属性修改方式，修复一些bug



