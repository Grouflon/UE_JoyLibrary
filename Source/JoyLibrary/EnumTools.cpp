#include "EnumTools.h"
#include <UObjectGlobals.h>
#include <Package.h>
#include <Class.h>

UEnum* GetEnumType(const FString& _enumName)
{
	return FindObject<UEnum>(ANY_PACKAGE, *_enumName, true);
}
