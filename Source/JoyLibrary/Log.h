#pragma once

#include <MacroUtils.h>

#define LOGF(fmt, ...)			UE_LOG(LogTemp, Log, TEXT("[%s:%d] " fmt), *FString(__FILENAME__), __LINE__, __VA_ARGS__)
#define LOG(msg)				LOGF("%s", msg)
#define LOGF_WARNING(fmt, ...)	UE_LOG(LogTemp, Warning, TEXT("[%s:%d] " fmt), *FString(__FILENAME__), __LINE__, __VA_ARGS__)
#define LOG_WARNING(msg)		LOGF_WARNING("%s", msg)
#define LOGF_ERROR(fmt, ...)	UE_LOG(LogTemp, Error, TEXT("[%s:%d] " fmt), *FString(__FILENAME__), __LINE__, __VA_ARGS__)
#define LOG_ERROR(msg)			LOGF_ERROR("%s", msg)
#define LOG_VECTOR(v)			LOG("%.2f, %.2f, %.2f", v.X, v.Y, v.Z)
#define LOG_VECTOR2D(v)			LOG("%.2f, %.2f", v.X, v.Y)

#define LOGF_SCREEN(time, color, msg, ...)\
       if(GEngine)\
	   {\
		   GEngine->AddOnScreenDebugMessage(-1, time, color, FString::Printf(TEXT(msg), __VA_ARGS__));\
	   }
