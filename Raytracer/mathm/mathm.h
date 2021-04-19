#pragma once
class mathm
{
public:
	static void Clamp(int& value, int hi, int lo);
	static void Clamp(float& value, float hi, float lo);

	static void Swap(int& firstValue, int& secondValue);
	static void Swap(float& firstValue, float& secondValue);

	static bool CompFloat(float a, float b);
};

