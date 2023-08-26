#include "UIInteractor.h"

UIInteractor::UIInteractor(const SpawnParams& params)
    : IUIUnit(params)
{
    _tickUpdate = true;
}

void UIInteractor::OnInitialize()
{
    PlayerInteractor::OnShowMenu.Bind<UIInteractor, &UIInteractor::OnShowMenu>(this);
    PlayerInteractor::OnHideMenu.Bind<UIInteractor, &UIInteractor::OnHideMenu>(this);
    auto RmlDocument = GetDocument()->GetDocument();
    _menu = RmlDocument->GetElementById("interact_menu");
    _menu->SetProperty("opacity", "0");
    auto originalElement = _menu->GetFirstChild();
    _entry = originalElement->Clone();
    _menu->RemoveChild(originalElement);
}

void UIInteractor::OnDeinitialize()
{
    PlayerInteractor::OnShowMenu.Unbind<UIInteractor, &UIInteractor::OnShowMenu>(this);
    PlayerInteractor::OnHideMenu.Unbind<UIInteractor, &UIInteractor::OnHideMenu>(this);
}

void UIInteractor::OnUpdate()
{
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

    if (Input::GetKeyDown(KeyboardKeys::F))
    {
        PlayerManager::Instance->GetOurPlayer()->GetComponent<PlayerInteractor>()->ExecuteOption(_options[_counter]);
    }
}

void UIInteractor::OnShowMenu(IInteractable* interactible)
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

void UIInteractor::OnHideMenu(IInteractable* interactible)
{
    _isVisible = false;

    while (_menu->GetLastChild() != nullptr)
    {
        _menu->RemoveChild(_menu->GetLastChild());
    }

    _menu->SetProperty("opacity", "0");
}
