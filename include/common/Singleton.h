#pragma once

//Template class for creating single - instance global classes.
template <typename T>
class Singleton
{
private:
	//Explicit private copy constructor. This is a forbidden operation.
	Singleton(const Singleton<T>&);
	//Private operator= . This is a forbidden operation. 
	Singleton& operator=(const Singleton<T>&);

protected:
	static T* singleton;

public:
	Singleton()
	{
		assert(!singleton);
		
		singleton = static_cast<T*>(this);
	}

	~Singleton()
	{
		assert(singleton);
		singleton = 0;
	}

	static T& GetSingleton()
	{
		assert(singleton);
		return *singleton;
	}

	static T* GetSingletonPtr()
	{
		return singleton;
	}
};

