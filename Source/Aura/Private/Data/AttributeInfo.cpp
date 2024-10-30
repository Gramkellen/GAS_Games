// Copyright kellendeng


#include "Data/AttributeInfo.h"

FAuraAttributeInfo UAttributeInfo::FindAttributeInfoByTag(const FGameplayTag& AttributeTag, bool bLogIfNotFound) const
{
	for(const FAuraAttributeInfo& Info : AttributeInformation)
	{
		if(Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if(bLogIfNotFound)
	{
		// 加 * 其实是因为 %s 期待的是一个 const char * 类型的 C字符串
		UE_LOG(LogTemp,Error,TEXT("[%s] Can't Find In [%s]"),*AttributeTag.ToString(),*GetNameSafe(this));
	}

	return FAuraAttributeInfo();
}
