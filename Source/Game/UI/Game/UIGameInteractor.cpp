#include "UIGameInteractor.h"

UIGameInteractor::UIGameInteractor(const SpawnParams& params)
    : IUIUnit(params)
{
    _tickUpdate = true;
}

void UIGameInteractor::OnInitialize()
{
    PlayerInteractor::OnShowMenu.Bind<UIGameInteractor, &UIGameInteractor::OnShowMenu>(this);
    PlayerInteractor::OnHideMenu.Bind<UIGameInteractor, &UIGameInteractor::OnHideMenu>(this);
    PlayerInteractor::OnShowProgress.Bind<UIGameInteractor, &UIGameInteractor::OnShowProgress>(this);
    PlayerInteractor::OnUpdateProgress.Bind<UIGameInteractor, &UIGameInteractor::OnUpdateProgress>(this);
    PlayerInteractor::OnHideProgress.Bind<UIGameInteractor, &UIGameInteractor::OnHideProgress>(this);
    auto RmlDocument = GetDocument()->GetDocument();
    _menu = RmlDocument->GetElementById("interact_menu");
    _menu->SetProperty("opacity", "0");
    _progress = RmlDocument->GetElementById("interaction");
    _progress->SetProperty("opacity", "0");
    auto originalElement = _menu->GetFirstChild();
    _entry = originalElement->Clone();
    _menu->RemoveChild(originalElement);
}

void UIGameInteractor::OnDeinitialize()
{
    PlayerInteractor::OnShowMenu.Unbind<UIGameInteractor, &UIGameInteractor::OnShowMenu>(this);
    PlayerInteractor::OnHideMenu.Unbind<UIGameInteractor, &UIGameInteractor::OnHideMenu>(this);
    PlayerInteractor::OnShowProgress.Unbind<UIGameInteractor, &UIGameInteractor::OnShowProgress>(this);
    PlayerInteractor::OnUpdateProgress.Unbind<UIGameInteractor, &UIGameInteractor::OnUpdateProgress>(this);
    PlayerInteractor::OnHideProgress.Unbind<UIGameInteractor, &UIGameInteractor::OnHideProgress>(this);
    _entry.reset();
}

void UIGameInteractor::OnUpdate()
{
    if (Input::GetKeyDown(KeyboardKeys::F))
    {
        PlayerManager::Instance->GetOurPlayer()->GetComponent<PlayerInteractor>()->StartInteract(_options[_counter]);
    }
    if (Input::GetKeyUp(KeyboardKeys::F))
    {
        PlayerManager::Instance->GetOurPlayer()->GetComponent<PlayerInteractor>()->StopInteract();
    }

    if (!_isVisible)
    {
        return;
    }

    float delta = Input::GetMouseScrollDelta();

    if (!delta == 0.0f)
    {
        _menu->GetChild(_counter)->SetProperty("color", "white");

        if (delta > 0.0f)
        {
            _counter--;
            if (_counter < 0)
            {
                _counter = _options.Count() - 1;
            }
        }
        else
        {
            _counter++;
            if (_counter > _options.Count() - 1)
            {
                _counter = 0;
            }
        }

        _menu->GetChild(_counter)->SetProperty("color", "lime");
    }
}

void UIGameInteractor::OnShowMenu(IInteractable* interactible)
{
    _isVisible = true;
    _menu->SetProperty("opacity", "1");

    _options = interactible->GetOptions();

    for (const auto& option : _options)
    {
        auto newEntry = _entry->Clone();
        String text = ScriptingEnum::ToString(option.Type);
        auto textAnsi = text.ToStringAnsi();
        newEntry->SetInnerRML(Rml::String(textAnsi.GetText(), textAnsi.Length()));
        if (!option.Enabled)
        {
            newEntry->SetProperty("background-color", "#ffffff20");
        }
        _menu->AppendChild(MoveTemp(newEntry));
    }
}

void UIGameInteractor::OnHideMenu()
{
    _isVisible = false;

    while (_menu->GetLastChild() != nullptr)
    {
        _menu->RemoveChild(_menu->GetLastChild());
    }

    _menu->SetProperty("opacity", "0");
}

void UIGameInteractor::OnShowProgress(float timer)
{
    _progress->SetProperty("opacity", "1");
    _progress->SetAttribute("max", timer);
    _progress->SetAttribute("value", "0.0");
}

void UIGameInteractor::OnUpdateProgress(float value)
{
    _progress->SetAttribute("value", value);
}

void UIGameInteractor::OnHideProgress()
{
    _progress->SetProperty("opacity", "0");
}