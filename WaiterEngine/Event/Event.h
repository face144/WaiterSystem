#pragma once

#include <functional>
#include <mutex>
#include <vector>
#include <utility>


template <typename TArgs, bool bThreadSafe = false>
class FEvent
{
public:
	FEvent() = default;

	void Subscribe(std::function<void(TArgs)> callback);
	void Unsubscribe(std::function<void(TArgs)> callback);

	template<typename... Args>
	void Publish(Args&&... args);

private:
	std::vector<std::function<void(TArgs)>> Callbacks;
	mutable std::mutex SyncMutex;
};


// Subscribe method (adds a new callback to the list)
template <typename TArgs, bool bThreadSafe>
void FEvent<TArgs, bThreadSafe>::Subscribe(std::function<void(TArgs)> callback)
{
	if constexpr (bThreadSafe)
	{
		std::lock_guard<std::mutex> Lock(SyncMutex);  // Safer lock with automatic unlocking
	}
    
	Callbacks.push_back(callback);
}

template <typename TArgs, bool bThreadSafe>
void FEvent<TArgs, bThreadSafe>::Unsubscribe(std::function<void(TArgs)> callback)
{
	if constexpr (bThreadSafe)
	{
		std::lock_guard<std::mutex> Lock(SyncMutex);  // Safer lock with automatic unlocking
	}
    
	// Find and remove the callback
    auto it = std::remove(Callbacks.begin(), Callbacks.end(), callback);
    if (it != Callbacks.end())
    {
    	Callbacks.erase(it, Callbacks.end());
    }
}

// Publish method (calls all the subscribed callbacks)
template <typename TArgs, bool bThreadSafe>
template <typename... Args>
void FEvent<TArgs, bThreadSafe>::Publish(Args&&... args)
{
	if constexpr (bThreadSafe)
	{
		std::lock_guard<std::mutex> Lock(SyncMutex);  // Safer lock
	}

	for (const auto& Callback : Callbacks)
	{
		// Forward the arguments to the callback
		Callback(std::forward<Args>(args)...);
	}
}
