#include "SleepBlock.h"

SleepBlock::SleepBlock(float tps)
{
    _timeout = 1.0f / tps;
    _timer = _timeout + 1.0f;
    _type = Type::Singular;
}

SleepBlock::SleepBlock(float sleepSeconds, float activeSeconds)
{
    _span = activeSeconds;
    _timeout = sleepSeconds;
    _type = Type::Range;
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
