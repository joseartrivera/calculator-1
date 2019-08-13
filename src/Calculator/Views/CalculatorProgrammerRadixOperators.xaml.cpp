// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

//
// CalculatorProgrammerRadixOperators.xaml.cpp
// Implementation of the CalculatorProgrammerRadixOperators class
//

#include "pch.h"
#include "CalculatorProgrammerRadixOperators.xaml.h"
#include "Controls/CalculatorButton.h"
#include "Converters/BooleanToVisibilityConverter.h"
#include "Views/NumberPad.xaml.h"

using namespace std;
using namespace CalculatorApp;
using namespace CalculatorApp::ViewModel;
using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace CalculatorApp::Common;
using namespace Windows::UI::Xaml::Media;

CalculatorProgrammerRadixOperators::CalculatorProgrammerRadixOperators()
    : m_isErrorVisualState(false)
{
    InitializeComponent();

    auto booleanToVisibilityNegationConverter = ref new Converters::BooleanToVisibilityNegationConverter;
    SetVisibilityBinding(ProgRadixOps, L"IsBinaryBitFlippingEnabled", booleanToVisibilityNegationConverter);
}

void CalculatorProgrammerRadixOperators::OnLoaded(Object ^, RoutedEventArgs ^)
{
    m_progModeRadixChangeToken = Model->ProgModeRadixChange +=
        ref new ProgModeRadixChangeHandler(this, &CalculatorProgrammerRadixOperators::ProgModeRadixChange);
}
void CalculatorProgrammerRadixOperators::OnUnloaded(Object ^, RoutedEventArgs ^)
{
    Model->ProgModeRadixChange -= m_progModeRadixChangeToken;
}

void CalculatorProgrammerRadixOperators::bitwiseFlyout_Toggle(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e)
{
    bool isShiftChecked = static_cast<ToggleButton ^>(sender)->IsChecked->Value;
    if (isShiftChecked)
    {
        //BitwiseFunctions->Visibility = ::Visibility::Collapsed;
        //InverseBitwiseFunctions->Visibility = ::Visibility::Visible;
    }
    else
    {
        //BitwiseFunctions->Visibility = ::Visibility::Visible;
       // InverseBitwiseFunctions->Visibility = ::Visibility::Collapsed;
    }
}

void CalculatorProgrammerRadixOperators::flyoutButton_Clicked(_In_ Platform::Object^ /*sender*/, _In_ Windows::UI::Xaml::RoutedEventArgs^ /*e*/)
{
    this->bitwiseFlyout->Hide();
}

void CalculatorProgrammerRadixOperators::checkDefaultBitShift()
{
    this->arithmeticShiftButton->IsChecked = true;
}

void CalculatorProgrammerRadixOperators::bitshiftFlyout_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    // Load deferred load buttons
    if (rolButton == nullptr)
    {
        FindName("RolButton");
        FindName("RorButton");
        FindName("rolCarryButton");
        FindName("rorCarryButton");
        FindName("lshLogicalButton");
        FindName("rshLogicalButton");
    }

    // Since arithmeticShiftButton defaults to IsChecked = true, this event an fire before we can load the deferred loaded controls. If that is the case, just return and do nothing.
    if (rolButton == nullptr)
    {
        return;
    }

    collapseBitshiftButtons();

    auto radioButton = static_cast<RadioButton^>(sender);

    if (radioButton == arithmeticShiftButton)
    {
        lshButton->Visibility = ::Visibility::Visible;
        rshButton->Visibility = ::Visibility::Visible;
        lshButton->IsEnabled = true;
        rshButton->IsEnabled = true;
    }
    else if (radioButton == logicalShiftButton)
    {
        lshLogicalButton->Visibility = ::Visibility::Visible;
        rshLogicalButton->Visibility = ::Visibility::Visible;
        lshLogicalButton->IsEnabled = true;
        rshLogicalButton->IsEnabled = true;
    }
    else if (radioButton == rotateCircularButton)
    {
        RolButton->Visibility = ::Visibility::Visible;
        RorButton->Visibility = ::Visibility::Visible;
        rolButton->IsEnabled = true;
        rorButton->IsEnabled = true;
    }
    else if (radioButton == rotateCarryShiftButton)
    {
        rolCarryButton->Visibility = ::Visibility::Visible;
        rorCarryButton->Visibility = ::Visibility::Visible;
        rolCarryButton->IsEnabled = true;
        rorCarryButton->IsEnabled = true;
    }

    this->BitShiftFlyout->Hide();
}

void CalculatorProgrammerRadixOperators::collapseBitshiftButtons()
{
    RolButton->Visibility = ::Visibility::Collapsed;
    RorButton->Visibility = ::Visibility::Collapsed;
    rolCarryButton->Visibility = ::Visibility::Collapsed;
    rorCarryButton->Visibility = ::Visibility::Collapsed;
    lshButton->Visibility = ::Visibility::Collapsed;
    rshButton->Visibility = ::Visibility::Collapsed;
    lshLogicalButton->Visibility = ::Visibility::Collapsed;
    rshLogicalButton->Visibility = ::Visibility::Collapsed;

    // We need to set the collapsed buttons to disabled so that the KeyboardShortcutManager can skip the keybinds for the disabled buttons
    rolButton->IsEnabled = false;
    rorButton->IsEnabled = false;
    rolCarryButton->IsEnabled = false;
    rorCarryButton->IsEnabled = false;
    lshButton->IsEnabled = false;
    rshButton->IsEnabled = false;
    lshLogicalButton->IsEnabled = false;
    rshLogicalButton->IsEnabled = false;
}

void CalculatorProgrammerRadixOperators::SetVisibilityBinding(FrameworkElement ^ element, String ^ path, IValueConverter ^ converter)
{
    Binding ^ commandBinding = ref new Binding();
    commandBinding->Path = ref new PropertyPath(path);
    commandBinding->Converter = converter;
    element->SetBinding(VisibilityProperty, commandBinding);
}

void CalculatorProgrammerRadixOperators::ProgModeRadixChange()
{
    NumberPad->ProgModeRadixChange();
}

bool CalculatorProgrammerRadixOperators::IsErrorVisualState::get()
{
    return m_isErrorVisualState;
}

void CalculatorProgrammerRadixOperators::IsErrorVisualState::set(bool value)
{
    if (m_isErrorVisualState != value)
    {
        m_isErrorVisualState = value;
        String ^ newState = m_isErrorVisualState ? L"ErrorLayout" : L"NoErrorLayout";
        VisualStateManager::GoToState(this, newState, false);
        NumberPad->IsErrorVisualState = m_isErrorVisualState;
    }
}

String ^ CalculatorProgrammerRadixOperators::ParenthesisCountToString(unsigned int count)
{
    return (count == 0) ? ref new String() : ref new String(to_wstring(count).data());
}

void CalculatorProgrammerRadixOperators::CalculatorProgrammerRadixOperators::OpenParenthesisButton_GotFocus(Object ^ sender, RoutedEventArgs ^ e)
{
    Model->SetOpenParenthesisCountNarratorAnnouncement();
}
