#include "UICore.h"

UICore::UICore(const SpawnParams& params)
    : IUIState(params)
{
    _tickUpdate = true;
}

void UICore::PrintMessage(LogType type, const StringView& message)
{
    _consoleCounter++;
    if (_consoleCounter >= 86)
    {
        _root->RemoveChild(_root->GetFirstChild());
        _consoleCounter = 86;
    }

    String targetMessage = message.ToString();

    targetMessage.Replace(TEXT("\r\n"), TEXT("\n"));

    Array<String> Split;
    targetMessage.Split(TEXT('\n'), Split);

    for (const auto& entry : Split)
    {
        auto newEntry = _entry->Clone();
        auto textAnsi = entry.ToStringAnsi();
        newEntry->SetInnerRML(Rml::String(textAnsi.GetText(), textAnsi.Length()));
        switch (type)
        {
        case LogType::Info:
            newEntry->SetProperty("color", "white");
            break;
        case LogType::Warning:
            newEntry->SetProperty("color", "yellow");
            break;
        case LogType::Error:
        case LogType::Fatal:
            newEntry->SetProperty("color", "red");
            break;
        default:
            newEntry->SetProperty("color", "white");
            break;
        }
        _root->AppendChild(MoveTemp(newEntry));
    }
}

void UICore::OnInfo(const StringView& message)
{
    if (message.StartsWith(TEXT("[NetworkReplicator] Spawn object")) ||
        message.StartsWith(TEXT("[NetworkReplicator] Add new object")) ||
        message.StartsWith(TEXT("[NetworkReplicator] Remap object")))
    {
        return;
    }

    _infoCounter++;
    if (_infoCounter > 999)
    {
        _infoText->SetInnerRML("999+");
    }
    else
    {
        _infoText->SetInnerRML(std::to_string(_infoCounter));
    }
    PrintMessage(LogType::Info, message);
}

void UICore::OnWarning(const StringView& message)
{
    _warnCounter++;
    if (_warnCounter > 999)
    {
        _warnText->SetInnerRML("999+");
    }
    else
    {
        _warnText->SetInnerRML(std::to_string(_warnCounter));
    }
    PrintMessage(LogType::Warning, message);
}

void UICore::OnError(const StringView& message)
{
    _errorCounter++;
    if (_errorCounter > 999)
    {
        _errorText->SetInnerRML("999+");
    }
    else
    {
        _errorText->SetInnerRML(std::to_string(_errorCounter));
    }
    PrintMessage(LogType::Error, message);
}

void UICore::OnInitialize()
{
    Logger::Instance->OnInfo.Bind<UICore, &UICore::OnInfo>(this);
    Logger::Instance->OnWarning.Bind<UICore, &UICore::OnWarning>(this);
    Logger::Instance->OnError.Bind<UICore, &UICore::OnError>(this);
    Logger::Instance->OnFatal.Bind<UICore, &UICore::OnError>(this);

    auto RmlDocument = GetDocument()->GetDocument();
    _window = RmlDocument->GetElementById("console_window");
    _window->SetProperty("opacity", "0");
    _textBox = RmlDocument->GetElementById("console_input");
    _root = RmlDocument->GetElementById("console_content");
    auto originalElement = _root->GetFirstChild();
    _entry = originalElement->Clone();
    _root->RemoveChild(originalElement);

    _infoText = RmlDocument->GetElementById("info_text");
    _infoText->SetInnerRML("0");
    _warnText = RmlDocument->GetElementById("warn_text");
    _warnText->SetInnerRML("0");
    _errorText = RmlDocument->GetElementById("error_text");
    _errorText->SetInnerRML("0");

    IUIState::OnInitialize();
}

void UICore::OnDeinitialize()
{
    _entry.reset();

    Logger::Instance->OnInfo.Unbind<UICore, &UICore::OnInfo>(this);
    Logger::Instance->OnWarning.Unbind<UICore, &UICore::OnWarning>(this);
    Logger::Instance->OnError.Unbind<UICore, &UICore::OnError>(this);
    Logger::Instance->OnFatal.Unbind<UICore, &UICore::OnError>(this);

    IUIState::OnDeinitialize();
}

void UICore::OnUpdate()
{
    if (!_consoleShown)
    {
        _root->ScrollTo(Rml::Vector2f(0.0f, 999999999.0f), Rml::ScrollBehavior::Instant);
    }

    if (Input::GetKeyDown(KeyboardKeys::BackQuote))
    {
        if (_consoleShown)
        {
            _window->SetProperty("opacity", "0");
            _textBox->Blur();
            _textBox->SetAttribute("value", "");
            _consoleShown = false;
            if (!_wasVisible)
            {
                Screen::SetCursorLock(CursorLockMode::Locked);
                Screen::SetCursorVisible(false);
            }
        }
        else
        {
            _window->SetProperty("opacity", "1");
            _consoleShown = true;
            _textBox->Focus();
            _wasVisible = Screen::GetCursorVisible();
            Screen::SetCursorLock(CursorLockMode::None);
            Screen::SetCursorVisible(true);
        }
    }
}

String UICore::GetBackState()
{
    return TEXT("!");
}
