#pragma once

#include <CoreMinimal.h>

JOYLIBRARY_API UEnum* GetEnumType(const FString& _enumName);

template<typename TEnum>
JOYLIBRARY_API extern FORCEINLINE FString EnumToName(const FString& EnumName, TEnum Value)
{
	const UEnum* enumPtr = GetEnumType(EnumName);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetNameByValue((int64)Value).ToString();
}

template<typename TEnum>
JOYLIBRARY_API extern FORCEINLINE FString EnumToDisplayName(const FString& EnumName, TEnum Value)
{
	const UEnum* enumPtr = GetEnumType(EnumName);
	if (!enumPtr)
	{
		return FString("Invalid");
	}
	return enumPtr->GetDisplayNameTextByValue((int64)Value).ToString();
}

template <typename EnumType>
JOYLIBRARY_API extern FORCEINLINE EnumType NameToEnum(const FString& EnumName, const FString& String)
{
	UEnum* Enum = GetEnumType(EnumName);
	if (!Enum)
	{
		return EnumType(0);
	}
	return (EnumType)Enum->GetIndexByName(FName(*String));
}
