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



2025.10.21

在蓝图中新建BP_TestEffectActor和GE_TestEffectActor测试了modifier中各个选项的作用，在MagnitudeCalculationType==AttrubuteBased情况下，添加各种属性对目标属性的作用及计算方法

添加PrimaryAttributes，包括Strength Intelligence Resilience

添加SecondAttributes，包括Armor ArmorPenetration BlockChance CriticalHitChance  CriticalHitDamage  CriticalHitResistance  HealthRegeneration  ManaRegeneration  MaxHealth  MaxMana

 这些属性将后续参与Modifier的运算，用于结算各种游戏效果



2025.10.22

给Second Attributes添加Modifier，计算各个属性，添加关系

添加Character等级，添加level接口，并在Character和Enemy中重写获取等级的方法

添加ModMagCalculate，用于自定义Modifier，添加一些计算方法





2025.10.23

继续添加ModMagCalculate,增加MaxHealth和MaxMana的计算

添加DefaultVitalAttributes，用GameplayEffect初始化，初始血量和蓝量

添加各种AttributeMenu的各种组件，添加打开和关闭菜单的逻辑



2025.10.25

添加GameplayTags和AssetManager类，管理Primary和secondary属性



2025.10.26

添加属性相关的AttributeInfo的DataAsset

添加AttributeMenu相关组件和控制器

添加blueprintFunctionLibrary，添加获取控制器方法

在蓝图中，给attributemenu添加对应的控制器





2025.10.27

给每个属性行添加gameplaytags

添加属性info的广播到attributemenuwidgetcontroller

使用函数指针对广播的流程进行包装

添加属性值变化广播

2025.10.31

添加第二属性到菜单

添加gameplay相关内容



2025.11.1

添加Ability相关的输入tag

制作包含1，2，3，4和鼠标左右按键的增强输入组件

添加InputConfig的DataAsset

绑定相关输入事件



2025.11.2

添加角色点击移动功能

添加角色单击时的自动寻路功能

添加弹丸ability

添加弹丸到法杖并成功发射，但存在bug，只能发射第一次



2025.11.3

添加AbilityTask：

​	添加鼠标点击事件

​	添加projectile Montage

添加点击目标数据，projectile可以朝着目标方向发射

添加shift输入，projectile可朝着任意方向发射

TODO：角色朝向改变







2025.11.4

通过Motion Wraping组件实现角色攻击时的朝向改变

增加子弹发射时的粒子效果和声音特效

修改客户端子弹不显示的bug
