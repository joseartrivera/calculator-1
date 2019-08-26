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
DEPENDENCY_PROPERTY_INITIALIZATION(OverflowButton, FlyoutMenu);

void OverflowButton::OnApplyTemplate()
{
    if (FlyoutMenu != nullptr)
    {
        FlyoutMenu->Closed += ref new EventHandler<Object ^>(this, &OverflowButton::FlyoutClosed);
    }
}

void OverflowButton::OnToggle()
{
    ToggleButton::OnToggle();

    if (IsChecked)
    {
        FlyoutMenu->ShowAt(this);
    }
}

void OverflowButton::FlyoutClosed(Object ^ sender, Object ^ args)
{
    IsChecked = false;
}
