#pragma once

#include <MacroUtils.h>

#define LOGF(fmt, ...)			UE_LOG(LogTemp, Log, TEXT("[%s:%d] " fmt), *FString(__FILENAME__), __LINE__, __VA_ARGS__)
#define LOG(msg)				LOGF("%s", msg)
#define LOGF_WARNING(fmt, ...)	UE_LOG(LogTemp, Warning, TEXT("[%s:%d] " fmt), *FString(__FILENAME__), __LINE__, __VA_ARGS__)
#define LOG_WARNING(msg)		LOGF_WARNING("%s", msg)
#define LOGF_ERROR(fmt, ...)	UE_LOG(LogTemp, Error, TEXT("[%s:%d] " fmt), *FString(__FILENAME__), __LINE__, __VA_ARGS__)
#define LOG_ERROR(msg)			LOGF_ERROR("%s", msg)
#define LOG_VECTOR(v)			LOGF("%.2f, %.2f, %.2f", v.X, v.Y, v.Z)
#define LOG_VECTOR2D(v)			LOGF("%.2f, %.2f", v.X, v.Y)
#define LOG_QUAT(q)				LOGF("%.2f, %.2f, %.2f, %.2f", q.X, q.Y, q.Z, q.W)
#define LOG_PLANE(p)			LOGF("%.2f, %.2f, %.2f, %.2f", p.X, p.Y, p.Z, p.W)

#define LOG_SCREEN(time, color, msg)\
       if(GEngine)\
	   {\
		   GEngine->AddOnScreenDebugMessage(-1, time, color, msg);\
	   }

#define LOGF_SCREEN(time, color, fmt, ...) LOG_SCREEN(time, color, FString::Printf(TEXT(fmt), __VA_ARGS__))

#define JOY_EXITCONDITION(cond, msg) \
{ \
	if ((cond)) \
	{ \
		LOGF_ERROR(TEXT("[%s] %s"), *FString(__FUNCTION__), msg); \
		return; \
	} \
}

#define JOY_EXITCONDITIONF(cond, fmt, ...) \
{ \
	if ((cond)) \
	{ \
		LOGF_ERROR(TEXT("[%s] %s"), *FString(__FUNCTION__), *FString::Printf(fmt, __VA_ARGS__)); \
		return; \
	} \
}

#define JOY_EXITCONDITION_RET(cond, ret, msg) \
{ \
	if ((cond)) \
	{ \
		LOGF_ERROR(TEXT("[%s] %s"), *FString(__FUNCTION__), msg); \
		return ret; \
	} \
}

#define JOY_EXITCONDITIONF_RET(cond, ret, fmt, ...) \
{ \
	if ((cond)) \
	{ \
		LOGF_ERROR(TEXT("[%s] %s"), *FString(__FUNCTION__), *FString::Printf(fmt, __VA_ARGS__)); \
		return ret; \
	} \
}