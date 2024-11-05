# 基于Game Ability System开发的RPG类的游戏

## GAS

### Attribute属性记录

:m: 这个数值只是基于GAS公式进行简单套用，具体数值的应用应当基于策划方案

#### Primary Attribute

`Strength`：力量（增加物理伤害）

`Intelligence`:智慧（增加魔伤）

`Resilience`：恢复力（提高防护和破防）

`Vigor`：能量（增加生命力）

#### Secondary Attribute

10个 Secondary Attributes

- `Armor`：抗性，减少受到的伤害

  Related Resilience

  数值计算公式：Armor = (Resilience + 1) * 0.5 + 2

- `Armor Penetration`：破甲值

  Related Resilience 

  数值计算公式：Armor Penetration = (Resilience + 0.5) * 0.2 + 1.5

- `Block Chance` ： 格挡率

  Related Armor

  数值计算公式：Block Chance =  ((Armor * 0.2) + 5）%

- `Critical Hit Chance` ： 暴击率

  Related Armor Penetration

  数值计算公式：Critical Hit Chance = (Armor Penetration) * 0.35

- `Critical Hit Damage` ： 暴击伤害

  Related Armor Penetration

  数值计算公式：Critical Hit Damage : (Armor Penetration) * 0.6 + 10

- `Critical Hit Resistance` ： 防暴击伤害

  Related Armor

  数值计算公式：Critical Hit Resistance: (Armor) * 0.5 + 5

- `Health Regeneration` ： 生命恢复

  Related Vigor

  数值计算公式：Health Regeneration : (Vigor + 1 ） * 0.15 + 3

- `Mana Regeneration` ： 法力恢复

  Related Intelligece

  数值计算公式：Mana Regeneration : (Intelligece + 2 ） * 0.10 + 3

- `Max Health` ： 最大生命值

  Related Vigor

  数值计算公式：Max Health : (Vigor + 5 ） * 1.2

- `Max Mana` ： 最大法力值

  Related Intelligece

  数值计算公式：Max Mana: (Intelligece+ 5 ） * 1.2

![image-20241024124413560](https://jiyidatabase.oss-cn-shanghai.aliyuncs.com/typora/202410241244153.png)

### 实现思考

等级未必要设置为Attribute，Attribute有时候更像是一种复杂的关联属性

补充学习：

1. Draw As Border的作用和含义
2. Overlay当中Auto和Fill
3. Wrap Box

## BUG点记录

### NewObject的上下文关联

```c++
AttributeMenuWidgetController = NewObject<UAttributeMenuWidgetController>(GetWorld(),AttributeMenuWidgetControllerClass);
```

在我们创建对象或者需要上下文关联、存在生命周期的物体时，记得传入一个UObject* Outer

:m: 主要是用于引用或者参考

## 开发疑惑点

### 回调函数传参不同会出BUG

```C++
void AAuraPlayerController::AbilityInputTagHeld(FGameplayTag GameplayTag)
{
	GEngine->AddOnScreenDebugMessage(3,3.f,FColor::Green,*GameplayTag.ToString());
}

AuraInputComponent->BindAbilityInputActions(AuraInputConfig,this,&ThisClass::AbilityInputTagPressed,&ThisClass::AbilityInputTagReleased,&ThisClass::AbilityInputTagHeld);
```

但是如果参数使用 const + 引用的方式会报错

```C++
void AAuraPlayerController::AbilityInputTagHeld(const FGameplayTag& GameplayTag)
{
	GEngine->AddOnScreenDebugMessage(3,3.f,FColor::Green,*GameplayTag.ToString());
}
```

:m: 猜测是对回调函数类型进行一定的限制，需要再研究下源代码深入了解
