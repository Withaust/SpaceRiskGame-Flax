#pragma once

#include <Engine/Scripting/Script.h>
#include <Engine/Core/Types/Pair.h>
#include <Engine/Engine/Time.h>

class SleepBlock
{
private:

    // Used to evenly disperse sleep block processing through different frames
    // Usefull for things like level entities that all get enabled on the same frame
    static const float _offsetStep;
    static const float _maxOffset;
    static float _currentStep;

    enum class Type
    {
        Range,
        Singular,
        Disabled
    } _type;

    float _span = 0.0f;
    float _timeout = 0.0f;
    float _timer = 0.0f;
    bool _sleeping = false;

public:

    Action OnActive;
    Action OnSleep;

    // Tps - how many times per second should the sleeping function execute
    // 2.0f - 2 times per second
    // 1.0f - 1 time per second
    // 0.5f - 1 time every 2 seconds
    // 0.25f - 1 time every 4 seconds
    void operator=(float tps);
    // First - for how many seconds should the function sleep
    // Second - for how many seconds should the function run
    // For example, if we want to have a trigger that would enable itself every 3 second, and
    // would be working for 0.5 seconds, params would be SleepBlock = Pair(3.0f, 0.5f);
    void operator=(Pair<float, float> seconds);

    bool Poll(float delta);
};
