#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Engine/Time.h>

class SleepBlock
{
private:
    enum class Type
    {
        Range,
        Singular
    } _Type;

    float _Span = 0.0f;
    float _Timeout = 0.0f;
    float _Timer = 0.0f;
    bool _Sleeping = false;
public:

    Action OnActive;
    Action OnSleep;

    SleepBlock() = delete;
    // Tps - how many times per second should the sleeping function execute
    // 2.0f - 2 times per second
    // 1.0f - 1 time per second
    // 0.5f - 1 time every 2 seconds
    // 0.25f - 1 time every 4 seconds
    SleepBlock(float Tps);
    // SleepSeconds - for how many seconds should the function sleep
    // ActiveSeconds - for how many seconds should the function run
    // For example, if we want to have a trigger that would enable itself every 3 second, and
    // would be working for 0.5 seconds, params would be SleepBlock(3.0f, 0.5f)
    SleepBlock(float SleepSeconds, float ActiveSeconds);

    bool Poll(float Delta);
};
