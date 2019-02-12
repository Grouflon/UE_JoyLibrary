#pragma once

#include <MacroUtils.h>
#include <Log.h>

#include <MessageDialog.h>

#ifdef UE_EDITOR
	#define JOY_ASSERT_ENABLED 1
#else
	#define JOY_ASSERT_ENABLED 0
#endif

#if JOY_ASSERT_ENABLED

	static TArray<FName> s_bypassedAsserts;

	#if PLATFORM_PS4
		#define JOY_ASSERT(condition)\
		{\
			if (FGenericPlatformMisc::IsDebuggerPresent())\
			{\
				__debugbreak();\
			}\
			else\
			{\
				check(condition);\
			}\
		}

		#define JOY_ASSERT_MSGF(condition, fmt, ...)\
		{\
			if (FGenericPlatformMisc::IsDebuggerPresent())\
			{\
				__debugbreak();\
			}\
			else\
			{\
				checkf(condition, (fmt), __VA_ARGS__); \
			}\
		}

	#else
		#define JOY_ASSERT(condition)\
		{\
			if (!(condition))\
			{\
				FName __key = FName(*FString::Printf(TEXT("%s%d"), TEXT(__FILE__), __LINE__));\
				if (s_bypassedAsserts.Find(__key) == INDEX_NONE)\
				{\
					EAppReturnType::Type __ret = FMessageDialog::Open(\
						EAppMsgType::YesNoCancel,\
						FText::FromString(FString::Printf(TEXT("[%s:%d -> %s]\n\n[Assertion failed] => " #condition "\n\nYes\t\t\t-> Crash and generate dump\nNo\t\t\t\t-> Pass Assert\nCancel\t-> Deactivate Assert"), *FString(__FILENAME__), __LINE__, *FString(__FUNCTION__)))\
					); \
					switch (__ret)\
					{\
					case EAppReturnType::Yes:\
					{\
						if (FGenericPlatformMisc::IsDebuggerPresent())\
						{\
							__debugbreak();\
						}\
						else\
						{\
							check(condition);\
						}\
					}\
					break;\
					case EAppReturnType::Cancel:\
					{\
						s_bypassedAsserts.Add(__key);\
					}\
					break;\
					default: break; \
					}\
				}\
			}\
		}

		#define JOY_ASSERT_MSGF(condition, fmt, ...)\
		{\
			if (!(condition))\
			{\
				FName __key = FName(*FString::Printf(TEXT("%s%d"), TEXT(__FILE__), __LINE__)); \
				if (s_bypassedAsserts.Find(__key) == INDEX_NONE)\
				{\
					EAppReturnType::Type __ret = FMessageDialog::Open(\
						EAppMsgType::YesNoCancel, \
						FText::FromString(FString::Printf(fmt TEXT("\n\n[%s:%d -> %s]\n\n[Assertion failed] -> " #condition "\n\nYes\t\t\t-> Crash and generate dump\nNo\t\t\t\t-> Pass Assert\nCancel\t-> Deactivate Assert"), __VA_ARGS__, *FString(__FILENAME__), __LINE__, *FString(__FUNCTION__)))\
					); \
					switch (__ret)\
					{\
					case EAppReturnType::Yes:\
					{\
						if (FGenericPlatformMisc::IsDebuggerPresent())\
						{\
							__debugbreak(); \
						}\
						else\
						{\
							checkf(condition, (fmt), __VA_ARGS__); \
						}\
					}\
						break; \
					case EAppReturnType::Cancel:\
						{\
							s_bypassedAsserts.Add(__key); \
						}\
							break; \
					default: break; \
					}\
				}\
			}\
		}

	#endif
#else
	#define JOY_ASSERT(condition)
	#define JOY_ASSERT_MSGF(condition, fmt, ...)
#endif

#define JOY_ASSERT_MSG(condition, msg) JOY_ASSERT_MSGF(condition, TEXT("%s"), msg)
