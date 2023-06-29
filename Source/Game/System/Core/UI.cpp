#include "UI.h"

UIMPL_SINGLETON(UI)

UI::UI(const SpawnParams& params)
    : ISystem(params)
{
}

void UI::FindStates(Actor* target, Array<IUIState*>& result)
{
    IUIState* state = target->GetScript<IUIState>();

    if (state)
    {
        result.Push(state);
    }

    for (int32 i = 0; i < target->Children.Count(); i++)
    {
        FindStates(target->Children[i], result);
    }
}

void UI::FindScripts(Actor* target, Array<Script*>& result)
{
    Array<Script*> scripts = target->Scripts;

    if (scripts.Count() != 0)
    {
        result.Add(scripts);
    }

    for (int32 i = 0; i < target->Children.Count(); i++)
    {
        FindScripts(target->Children[i], result);
    }
}

void UI::OnInitialize()
{
    Array<IUIState*> result;
    FindStates(GetActor(), result);

    for (const auto& state : result)
    {
        if (state->GetType().Fullname == "Game.UI")
        {
            continue;
        }
        _states[String(state->GetType().Fullname)] = state;
    }

    for (auto state : _states)
    {
        state.Value->OnInitialize();
        state.Value->GetActor()->SetIsActive(false);
        Array<Script*> scripts;
        FindScripts(state.Value->GetActor(), scripts);
        for (const auto& script : scripts)
        {
            script->SetEnabled(false);
        }
    }
}

void UI::OnDeinitialize()
{
    for (const auto& state : _states)
    {
        state.Value->OnDeinitialize();
    }
}

void UI::GoForward(String state)
{
    if (!_states.ContainsKey(state))
    {
        return;
    }
    if (_currentState)
    {
        _currentState->GetActor()->SetIsActive(false);
        Array<Script*> scripts;
        UI::Instance->FindScripts(_currentState->GetActor(), scripts);
        for (const auto& script : scripts)
        {
            script->SetEnabled(false);
        }
    }

    IUIState* target = _states[state];
    target->GetActor()->SetIsActive(true);
    Array<Script*> scripts;
    UI::Instance->FindScripts(target->GetActor(), scripts);
    for (const auto& script : scripts)
    {
        script->SetEnabled(true);
    }
    _currentState = target;
}

void UI::GoBack()
{
    if (!_currentState)
    {
        return;
    }

    GoForward(_currentState->GetBackState());
}
