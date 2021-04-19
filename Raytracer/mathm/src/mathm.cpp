// MathM.cpp : Defines the functions for the static library.
//

#include "pch.h"

// cmath only used for abs, line 51
#include <cmath>

// clamps a value by a min and a max
void mathm::Clamp(int& value, int hi, int lo)
{
    if (value > hi)
    {
        value = hi;
    }
    else if (value < lo)
    {
        value = lo;
    }
}

void mathm::Clamp(float& value, float hi, float lo)
{
    if (value > hi)
    {
        value = hi;
    }
    else if (value < lo)
    {
        value = lo;
    }
}

// swaps two values
void mathm::Swap(int& firstValue, int& secondValue)
{
    const int temp = secondValue;
    secondValue = firstValue;
    firstValue = temp;
}

void mathm::Swap(float& firstValue, float& secondValue)
{
    const float temp = secondValue;
    secondValue = firstValue;
    firstValue = temp;
}

// compares two floats and accounts for floating point
// inaccuracies
bool mathm::CompFloat(float a, float b)
{
    return abs(a - b) < 1e-5f;
	
}

// squares a float
float mathm::Squared(float a)
{
    return a * a;
}

// calculates the machine epsilon, provided to me by Justin Kujawa
float mathm::GetMachineEpsilon(float start)
{
    float previous;
	while((1 + start) != 1)
	{
        previous = start;
        start /= 2;
	}
    return start;
}
