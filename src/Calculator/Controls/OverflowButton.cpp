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
DEPENDENCY_PROPERTY_INITIALIZATION(OverflowButton, GlyphFontSize);
DEPENDENCY_PROPERTY_INITIALIZATION(OverflowButton, ChevronFontSize);

void OverflowButton::OnApplyTemplate()
{
    if (Flyout != nullptr)
    {
        m_flyoutOpenBackgroundBrush = dynamic_cast<Brush^>(Application::Current->Resources->Lookup("AppControlPressedButtonFaceBrush"));
        m_flyoutClosedBackgroundBrush = dynamic_cast<Brush^>(Application::Current->Resources->Lookup("SystemControlBackgroundTransparentBrush"));

        Flyout->Opened += ref new EventHandler<Object^>(this, &OverflowButton::FlyoutOpened);
        Flyout->Closed += ref new EventHandler<Object^>(this, &OverflowButton::FlyoutClosed);
    }
}

void OverflowButton::FlyoutOpened(Object^ sender, Object^ args)
{
    if (m_flyoutOpenBackgroundBrush != nullptr)
    {
        this->Background = m_flyoutOpenBackgroundBrush;
    }
}

void OverflowButton::FlyoutClosed(Object^ sender, Object^ args)
{
    if (m_flyoutClosedBackgroundBrush != nullptr)
    {
        this->Background = m_flyoutClosedBackgroundBrush;
    }
}
