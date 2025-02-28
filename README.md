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

### 待修复Bug

#### 服务器客户端模式时，火球消失了

https://blog.csdn.net/qq_30100043/article/details/138033334

因为火球创建还没有Spawn就EndAbility，导致没有发射

**解决方案：**在蓝图当中Delay一下

 **探索问题：？？？ 可是很奇怪，服务器会调用吗？？？**

:m: 服务器会调用

#### 客户端的敌人的血条不显示和更新

**属性没有同步过来，真的有点奇怪**

:m: 2025年2/26日记录 ： 花费一天，未解决且找不到原因
找到原因：MaxHealth没有发生同步

#### 双客户端模式，FireBolt刚出去就爆炸了

Overlap的触发问题，明明没有Overlap，但是火球和另一个Character显示Overlap了（Debug发现的）

因此将Character对这个火球（FireBolt)的Overlap给取消掉

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

### 网络同步问题

在网络同步时需要注意网络传递和同步的问题，不能客户端已经完成了，数据还没有到达服务端进行同步

#### 能力预测同步的流程

```C++
void ServerSetReplicatedTargetData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey, const FGameplayAbilityTargetDataHandle& ReplicatedTargetDataHandle, FGameplayTag ApplicationTag, FPredictionKey CurrentPredictionKey);
```

- AbilityOriginalPredictionKey是能力激活时就会获得的标志键
- 那么每次通过RPC进行网络同步时，会更新CurrentPredictionkey，这样可以记录正在进行同步的能力和数据
- 同时如果客户端的预测被服务器拒绝了，可以根据CUrrentPredictionKey找到对应的bility进行回滚

##### 客户端激活能力

- 当玩家在客户端激活某个能力时，客户端会生成一个 `AbilityOriginalPredictionKey`，这是能力生命周期中的初始预测键。
- 客户端开始进行预测，假设能力会立即生效，并可能开始显示一些效果（如动画、特效等）。

##### 客户端发送请求到服务器

- 客户端向服务器发送能力的请求时，除了发送能力的目标数据（`TargetData`）外，还会附带 `AbilityOriginalPredictionKey`。这个键标识了这次能力请求的 **源始**，即它属于哪个能力激活过程。

##### 服务器接收并验证请求

- 服务器会接收到客户端发送的请求，并验证该请求的合法性。服务器还会使用 `AbilityOriginalPredictionKey` 来确认这次请求是否有效，并且是否是客户端首次发起的能力请求。
- 如果服务器允许该能力的执行，它会处理该能力的目标数据，生成新的 `CurrentPredictionKey`，并将该信息返回给客户端。

##### 客户端更新并同步

- 客户端收到服务器的响应后，可能会更新能力的状态，并根据服务器的反馈调整其预测结果。
- 在这个过程中，**`CurrentPredictionKey`** 可能会更新。每当能力的状态变化时（例如，提交新的目标数据、调整效果等），`CurrentPredictionKey` 会跟随这些变化而改变，确保每个阶段的执行都是唯一且正确的。

##### 回滚或修正

如果客户端的预测结果与服务器的最终决策不一致，客户端需要根据 `CurrentPredictionKey` 来执行回滚操作，将能力的状态恢复到服务器确认的正确状态。

### SourceObject问题

对于GAS应用GameplayEffect，需要设置SourceObject，否则对于一些Attributes的更新，可能会出现数值的问题

因为我并不知道这个GameplayEffect的来源是什么，怎么进行更新

```C++
// PrimaryAttributes初始化
FGameplayEffectContextHandle PrimaryAttributeContextHandle =  ASC->MakeEffectContext();
PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);  // 不添加SourceActor会导致缺少Effect来源
const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(ClassDefaultInfo.PrimaryAttributes, Level, PrimaryAttributeContextHandle);
ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());
```

