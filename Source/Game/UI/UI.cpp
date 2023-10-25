#include "UI.h"
#undef RadiansToDegrees
#undef DegreesToRadians
#undef NormaliseAngle
#include <ThirdParty/RmlUi/Core.h>

UIMPL_SINGLETON(UI)

UI::UI(const SpawnParams& params)
    : ISystem(params)
{
    _tickUpdate = true;
}

UIViewports* UI::GetTextures()
{
    return _textures;
}

void UI::OnInitialize()
{
    _textures = New<UIViewports>();
    RmlUiPlugin::OnLoadTexture.Bind<UIViewports, &UIViewports::OnLoadTexture>(_textures);
    RmlUiPlugin::OnReleaseTexture.Bind<UIViewports, &UIViewports::OnReleaseTexture>(_textures);

    Array<IUIState*> result;
    EngineHelper::FindScripts<IUIState>(GetActor(), result);

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
        auto Document = state.Value->GetDocument();
        auto Context = Document->GetContext();
        Context->SetDensityIndependentPixelRatio(1.0f);
        state.Value->OnInitialize();
        state.Value->GetActor()->SetIsActive(false);
        Array<Script*> scripts;
        EngineHelper::FindScripts(state.Value->GetActor(), scripts);
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

    if (_textures)
    {
        RmlUiPlugin::OnLoadTexture.Unbind();
        RmlUiPlugin::OnReleaseTexture.Unbind();
        _textures->ClearViewports();
        Delete(_textures);
    }
    UDEINIT_SINGLETON();
}

void UI::OnUpdate()
{
    if (Input::GetKeyUp(KeyboardKeys::Escape))
    {
        GoBack();
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
        EngineHelper::FindScripts(_currentState->GetActor(), scripts);
        for (const auto& script : scripts)
        {
            script->SetEnabled(false);
        }
        auto document = _currentState->GetDocument();
        document->Defocus();
        document->Hide();
    }

    IUIState* target = _states[state];
    target->GetActor()->SetIsActive(true);
    Array<Script*> scripts;
    EngineHelper::FindScripts(target->GetActor(), scripts);
    for (const auto& script : scripts)
    {
        script->SetEnabled(true);
    }
    _currentState = target;
    auto document = _currentState->GetDocument();
    document->Show();
    document->Focus();
}

void UI::GoBack()
{
    if (!_currentState || _currentState->HasCustomBack())
    {
        return;
    }

    GoForward(_currentState->GetBackState());
}

void UI::RefreshCache()
{
    Rml::Factory::ClearStyleSheetCache();
    Rml::Factory::ClearTemplateCache();
}
