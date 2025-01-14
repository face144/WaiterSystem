#include "DefaultOutputDevice.h"

FDefaultOutputDevice& FDefaultOutputDevice::Get()
{
	static FDefaultOutputDevice Inst;
	return Inst;
}

FDefaultOutputDevice& FDefaultOutputDevice::operator<<(const std::string& Message)
{
	Write(std::string(Message), EVerbosity::Log);
	return *this;
}

FDefaultOutputDevice& FDefaultOutputDevice::operator<<(const char* Message)
{
	Write(std::string(Message), EVerbosity::Log);
	return *this;
}

void FDefaultOutputDevice::Write(const char* Message, EVerbosity Verbosity)
{
	Write(std::string(Message), EVerbosity::Log);
}

void FDefaultOutputDevice::Write(const std::string& Message, EVerbosity Verbosity)
{
	std::chrono::time_point<std::chrono::system_clock> Now = std::chrono::system_clock::now();
		
	std::ostringstream FormattedMessage;
	std::string ColorCode;
	FormattedMessage << "[" << Now << "] ";
	switch (Verbosity)
	{
	case EVerbosity::Log:
		{
			ColorCode = "\033[37m";
			FormattedMessage << "Log: ";
		}
		break;
		
	case EVerbosity::Warning:
		{
			ColorCode = "\033[33m";
			FormattedMessage << "Warning: ";
		}
		break;
		
	case EVerbosity::Error:
		{
			ColorCode = "\033[31m";
			FormattedMessage << "Error: ";
		}
		break;
	}

	FormattedMessage << Message;
	AddOutput(ColorCode + FormattedMessage.str());
}

void FDefaultOutputDevice::AddOutput(const std::string& String)
{
	std::scoped_lock Lock(WriteMutex);
	
	Output << String + "\n";
	std::cout << String << "\n" << std::flush;
}
