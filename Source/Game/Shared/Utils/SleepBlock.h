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
    } _type;

    float _span = 0.0f;
    float _timeout = 0.0f;
    float _timer = 0.0f;
    bool _sleeping = false;

public:

    Action OnActive;
    Action OnSleep;

    SleepBlock() = delete;
    // Tps - how many times per second should the sleeping function execute
    // 2.0f - 2 times per second
    // 1.0f - 1 time per second
    // 0.5f - 1 time every 2 seconds
    // 0.25f - 1 time every 4 seconds
    SleepBlock(float tps);
    // SleepSeconds - for how many seconds should the function sleep
    // ActiveSeconds - for how many seconds should the function run
    // For example, if we want to have a trigger that would enable itself every 3 second, and
    // would be working for 0.5 seconds, params would be SleepBlock(3.0f, 0.5f)
    SleepBlock(float sleepSeconds, float activeSeconds);

    bool Poll(float delta);
};
