#pragma once

#include "Core/Minimal.h"
#include <mutex>
#include <sstream>
#include <string>
#include <chrono>
#include <iostream>


enum class EVerbosity : uint8
{
	Log,
	Warning,
	Error,
};

class FDefaultOutputDevice
{
public:
	WAITERENGINE_API static FDefaultOutputDevice& Get();
	
	WAITERENGINE_API FDefaultOutputDevice() = default;
	WAITERENGINE_API FDefaultOutputDevice(FDefaultOutputDevice& Other) = delete;
	WAITERENGINE_API FDefaultOutputDevice operator=(FDefaultOutputDevice& Other) = delete;

	WAITERENGINE_API FDefaultOutputDevice& operator<<(const std::string& Message);
	WAITERENGINE_API FDefaultOutputDevice& operator<<(const char* Message);

	WAITERENGINE_API void Write(const char* Message, EVerbosity Verbosity);

	template <typename T>
	void Write(T* CallingObject, const std::string& Message, EVerbosity Verbosity);
	
	WAITERENGINE_API void Write(const std::string& Message, EVerbosity Verbosity);
	
	
private:
	std::ostringstream Output;
	std::mutex WriteMutex;

	void AddOutput(const std::string& String);
};


template <typename T>
void FDefaultOutputDevice::Write(T* CallingObject, const std::string& Message, EVerbosity Verbosity)
{
	std::ostringstream NewMessage;
	NewMessage << typeid(T).name()  << ": " << Message;
		
	Write(NewMessage.str(), Verbosity);
}
