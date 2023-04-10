#include "SleepBlock.h"

SleepBlock::SleepBlock(float Tps)
{
    _timeout = 1.0f / Tps;
    _timer = _timeout + 1.0f;
    _type = Type::Singular;
}

SleepBlock::SleepBlock(float SleepSeconds, float ActiveSeconds)
{
    _span = ActiveSeconds;
    _timeout = SleepSeconds;
    _type = Type::Range;
}

bool SleepBlock::Poll(float Delta)
{
    // Acting as sleep with range
    if (_type == Type::Range)
    {
        _timer += Delta;
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
        _timer += Delta;
        if (_timer >= _timeout)
        {
            _timer = 0.0f;
            return true;
        }
        return false;
    }
}
