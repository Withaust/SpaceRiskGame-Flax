#include "SleepBlock.h"

SleepBlock::SleepBlock(float Tps)
{
    _Timeout = 1.0f / Tps;
    _Timer = _Timeout + 1.0f;
    _Type = Type::Singular;
}

SleepBlock::SleepBlock(float SleepSeconds, float ActiveSeconds)
{
    _Span = ActiveSeconds;
    _Timeout = SleepSeconds;
    _Type = Type::Range;
}

bool SleepBlock::Poll(float Delta)
{
    // Acting as sleep with range
    if (_Type == Type::Range)
    {
        _Timer += Delta;
        if (_Sleeping)
        {
            if (_Timer >= _Timeout)
            {
                _Timer = 0.0f;
                OnActive();
                _Sleeping = false;
                return true;
            }
        }
        else
        {
            if (_Timer >= _Span)
            {
                _Timer = 0.0f;
                OnSleep();
                _Sleeping = true;
            }
        }
        return false;
    }
    // Acting as a singular sleep
    else
    {
        _Timer += Delta;
        if (_Timer >= _Timeout)
        {
            _Timer = 0.0f;
            return true;
        }
        return false;
    }
}
