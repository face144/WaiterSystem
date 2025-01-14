#pragma once

#include <functional>
#include <vector>


template<typename ...TArgs>
class WAITERCORE_API TEvent
{
public:
	TEvent() = default;
	~TEvent() = default;

	void Broadcast(TArgs... Args)
	{
		for (auto& Callback : Callbacks)
		{
			Callback(std::forward<TArgs>(Args));
		}
	}
	void Subscribe(const std::function<void(...)>& InCallback)
	{
		Callbacks.push_back(InCallback);
	}

private:
	std::vector<std::function<void(...)>> Callbacks;
};
