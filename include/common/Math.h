#pragma once
class Math
{
public:
	template<typename T>
	static T Lerp(T a, T b, T f)
	{
		return a + f * (b - a);
	}
};

