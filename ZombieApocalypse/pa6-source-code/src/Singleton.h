// Defines a templated Singleton class

#pragma once

// You must place this macro inside the class definition of your derived Singleton
#define DECLARE_SINGLETON(SingletonClass) friend class Singleton<SingletonClass>;
#include <memory>
template <class T>
class Singleton
{
private:
	static std::unique_ptr<T> _instance;
protected:
	Singleton() = default;
public:
	static T& Get()
	{
		if (_instance)
		{
			return *_instance;
		}
	
		_instance = std::make_unique<T>();
		return *_instance;
	}
};

template <class T>
std::unique_ptr<T> Singleton<T>::_instance{}; // NOLINT
