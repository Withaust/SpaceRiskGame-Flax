#include "SleepBlock.h"

const float SleepBlock::_offsetStep = 0.01f;
const float SleepBlock::_maxOffset = 0.3f;
float SleepBlock::_currentStep = 0.0f;

void SleepBlock::operator=(float tps)
{
    _timeout = 1.0f / tps;
    _timer = _timeout + 1.0f;
    _type = Type::Singular;
    _currentStep += _offsetStep;
    if (_currentStep >= _maxOffset)
    {
        _currentStep = 0.0f;
    }
    _timer += _currentStep;
}

void SleepBlock::operator=(Pair<float, float> seconds)
{
    _span = seconds.Second;
    _timeout = seconds.First;
    _type = Type::Range;
    _currentStep += _offsetStep;
    if (_currentStep >= _maxOffset)
    {
        _currentStep = 0.0f;
    }
    _timer += _currentStep;
}

bool SleepBlock::Poll(float delta)
{
    // Acting as sleep with range
    if (_type == Type::Range)
    {
        _timer += delta;
        if (_sleeping)
        {
            if (_timer >= _timeout)
            {
                _timer = 0.0f;
                OnActive();
                _sleeping = false;
                return true;
            }
        }
        else
        {
            if (_timer >= _span)
            {
                _timer = 0.0f;
                OnSleep();
                _sleeping = true;
            }
        }
        return false;
    }
    // Acting as a singular sleep
    else
    {
        _timer += delta;
        if (_timer >= _timeout)
        {
            _timer = 0.0f;
            return true;
        }
        return false;
    }
}
