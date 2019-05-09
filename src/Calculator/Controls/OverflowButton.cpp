// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "OverflowButton.h"

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::UI::ViewManagement;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Media;
using namespace CalculatorApp;
using namespace CalculatorApp::Common;
using namespace CalculatorApp::Controls;

DEPENDENCY_PROPERTY_INITIALIZATION(OverflowButton, Text);
DEPENDENCY_PROPERTY_INITIALIZATION(OverflowButton, Glyph);

inline constexpr auto CHEVRON_CLOSED_GLYPH = L"\uE70D";
inline constexpr auto CHEVRON_OPENED_GLYPH = L"\uE70E";

void OverflowButton::OnApplyTemplate()
{
    m_chevronIcon = dynamic_cast<FontIcon^>(GetTemplateChild("ChevronFontIcon"));
    Flyout->Opened += ref new EventHandler<Object^>(this, &OverflowButton::FlyoutOpened);
    Flyout->Closed += ref new EventHandler<Object^>(this, &OverflowButton::FlyoutClosed);
}

void OverflowButton::FlyoutOpened(Object^ sender, Object^ args)
{
    if (m_chevronIcon == nullptr)
    {
        return;
    }

    //this->Background = safe_cast<Brush^>(Application::Current->Resources->Lookup("AppControlPressedButtonFaceBrush"));
    m_chevronIcon->Glyph = ref new String(CHEVRON_OPENED_GLYPH);
}

void OverflowButton::FlyoutClosed(Object^ sender, Object^ args)
{
    if (m_chevronIcon == nullptr)
    {
        return;
    }
    //this->Background = safe_cast<Brush^>(Application::Current->Resources->Lookup("SystemControlBackgroundTransparentBrush"));
    m_chevronIcon->Glyph = ref new String(CHEVRON_CLOSED_GLYPH);
}
