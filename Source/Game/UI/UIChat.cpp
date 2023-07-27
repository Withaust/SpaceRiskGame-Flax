#include "UIChat.h"

UIChat::UIChat(const SpawnParams& params)
    : IUIUnit(params)
{
    _tickUpdate = true;
}

bool UIChat::IsFocused() 
{ 
    return _isFocused; 
}

void UIChat::OnInitialize()
{
    Chat::Instance->OnChatMessage.Bind<UIChat, &UIChat::OnChatMessage>(this);
    auto RmlDocument = GetDocument()->GetDocument();
    _window = RmlDocument->GetElementById("window");
    _window->SetProperty("opacity", "0");
    _textBox = RmlDocument->GetElementById("text");
    _textBox->SetProperty("display", "none");
    _root = RmlDocument->GetElementById("content");
    auto originalElement = _root->GetFirstChild();
    _entry = originalElement->Clone();
    _root->RemoveChild(originalElement);
}

void UIChat::OnDeinitialize()
{

}

void UIChat::OnUpdate()
{
    if (!_isFocused)
    {
        return;
    }

    if (!Input::GetKeyDown(KeyboardKeys::Escape) && !Input::GetKeyDown(KeyboardKeys::Return))
    {
        return;
    }
    Rml::Variant* attribute = _textBox->GetAttribute("value");
    if (attribute == nullptr || attribute->GetType() != Rml::Variant::Type::STRING)
    {
        Unfocus();
        return;
    }
    Rml::String text = _textBox->GetAttribute("value")->Get<Rml::String>();
    String realText(text.data(), (int32)text.length());
    if (realText == TEXT(""))
    {
        Unfocus();
        return;
    }
    Chat::Instance->SendMessage(realText);
    Unfocus();
}

void UIChat::OnChatMessage(uint32 Sender, String Message)
{
    if (_counter > 64)
    {
        _root->RemoveChild(_root->GetFirstChild());
    }
    auto newEntry = _entry->Clone();
    String text = PlayerManager::Instance->GetPlayerInfo(Sender)->Name + TEXT(": ") + Message;
    auto textAnsi = text.ToStringAnsi();
    newEntry->SetInnerRML(Rml::String(textAnsi.GetText(), textAnsi.Length()));
    _root->AppendChild(MoveTemp(newEntry));
    _counter++;
    _window->SetProperty("opacity", "1");
    _window->Animate("opacity", Rml::Property(0.0f, Rml::Unit::NUMBER), 2.0f, {}, false, false, 1.0f);
}

void UIChat::Focus()
{
    Screen::SetCursorLock(CursorLockMode::None);
    Screen::SetCursorVisible(true);
    _isFocused = true;
    _window->Animate("opacity", Rml::Property(1.0f, Rml::Unit::NUMBER), 0.01f, {}, false, false);
    _textBox->SetProperty("display", "block");
    _textBox->Focus();
    PlayerManager::Instance->GetOurPlayer()->GetComponent<PlayerMovement>()->CanMove = false;
}

void UIChat::Unfocus(bool AnimateWindow)
{
    _textBox->Blur();
    _textBox->SetAttribute("value", "");
    _textBox->SetProperty("display", "none");
    PlayerManager::Instance->GetOurPlayer()->GetComponent<PlayerMovement>()->CanMove = true;
    _isFocused = false;
    _window->Animate("opacity", Rml::Property(0.0f, Rml::Unit::NUMBER), 2.0f, {}, false, false, 1.0f);
    Screen::SetCursorLock(CursorLockMode::Locked);
    Screen::SetCursorVisible(false);
}
