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

using namespace CalculatorApp;

using namespace CalculatorApp::ViewModel;
using namespace Platform;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace CalculatorApp::Common;
using namespace Windows::UI::Xaml::Media;

CalculatorProgrammerRadixOperators::CalculatorProgrammerRadixOperators() :
    m_isErrorVisualState(false)
{
    InitializeComponent();

    auto booleanToVisibilityNegationConverter = ref new Converters::BooleanToVisibilityNegationConverter;
    SetVisibilityBinding(ProgRadixOps, L"IsBinaryBitFlippingEnabled", booleanToVisibilityNegationConverter);
}

void CalculatorProgrammerRadixOperators::OnLoaded(Object^, RoutedEventArgs^)
{
    m_progModeRadixChangeToken = Model->ProgModeRadixChange += ref new ProgModeRadixChangeHandler(this, &CalculatorProgrammerRadixOperators::ProgModeRadixChange);
    m_propertyChangedToken = Model->PropertyChanged += ref new PropertyChangedEventHandler(this, &CalculatorProgrammerRadixOperators::OnViewModelPropertyChanged);
}
void CalculatorProgrammerRadixOperators::OnUnloaded(Object^, RoutedEventArgs^)
{
    Model->ProgModeRadixChange -= m_progModeRadixChangeToken;
    Model->PropertyChanged -=  m_propertyChangedToken;
}

void CalculatorProgrammerRadixOperators::bitwiseFlyout_Toggle(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    bool isShiftChecked = static_cast<ToggleButton^>(sender)->IsChecked->Value;
    if (isShiftChecked)
    {
        BitwiseFunctions->Visibility = ::Visibility::Collapsed;
        InverseBitwiseFunctions->Visibility = ::Visibility::Visible;
    }
    else
    {
        BitwiseFunctions->Visibility = ::Visibility::Visible;
        InverseBitwiseFunctions->Visibility = ::Visibility::Collapsed;
    }
}

void CalculatorProgrammerRadixOperators::bitshiftFlyout_Checked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    if (rolButton == nullptr)
    {
        FindName("rolButton");
        FindName("rorButton");
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

    RadioButton^ radioButton = static_cast<RadioButton^>(sender);

    rolButton->Visibility = ::Visibility::Collapsed;
    rorButton->Visibility = ::Visibility::Collapsed;
    rolCarryButton->Visibility = ::Visibility::Collapsed;
    rorCarryButton->Visibility = ::Visibility::Collapsed;
    lshButton->Visibility = ::Visibility::Collapsed;
    rshButton->Visibility = ::Visibility::Collapsed;
    lshLogicalButton->Visibility = ::Visibility::Collapsed;
    rshLogicalButton->Visibility = ::Visibility::Collapsed;

    if (radioButton == arithmeticShiftButton)
    {
        lshButton->Visibility = ::Visibility::Visible;
        rshButton->Visibility = ::Visibility::Visible;
    }
    else if (radioButton == logicalShiftButton)
    {
        lshLogicalButton->Visibility = ::Visibility::Visible;
        rshLogicalButton->Visibility = ::Visibility::Visible;
    }
    else if (radioButton == rotateCircularButton)
    {
        rolButton->Visibility = ::Visibility::Visible;
        rorButton->Visibility = ::Visibility::Visible;
    }
    else if (radioButton == rotateCarryShiftButton)
    {
        rolCarryButton->Visibility = ::Visibility::Visible;
        rorCarryButton->Visibility = ::Visibility::Visible;
    }
}

void CalculatorProgrammerRadixOperators::Shift_Clicked(Platform::Object^ sender, Windows::UI::Xaml::RoutedEventArgs^ e)
{
    bool isShiftChecked = static_cast<ToggleButton^>(sender)->IsChecked->Value;
    auto scvm = safe_cast<StandardCalculatorViewModel^>(this->DataContext);
    scvm->IsShiftProgrammerChecked = isShiftChecked;

    if (rolButton == nullptr)
    {
        FindName("rolButton");
        FindName("rorButton");
    }

    if (isShiftChecked)
    {
        rolButton->Visibility = ::Visibility::Visible;
        rorButton->Visibility = ::Visibility::Visible;
        lshButton->Visibility = ::Visibility::Collapsed;
        rshButton->Visibility = ::Visibility::Collapsed;
    }
    else
    {
        rolButton->Visibility = ::Visibility::Collapsed;
        rorButton->Visibility = ::Visibility::Collapsed;
        lshButton->Visibility = ::Visibility::Visible;
        rshButton->Visibility = ::Visibility::Visible;
    }
}

void CalculatorProgrammerRadixOperators::SetVisibilityBinding(FrameworkElement^ element, String^ path, IValueConverter^ converter)
{
    Binding^ commandBinding = ref new Binding();
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
        String^ newState = m_isErrorVisualState ? L"ErrorLayout" : L"NoErrorLayout";
        VisualStateManager::GoToState(this, newState, false);
        NumberPad->IsErrorVisualState = m_isErrorVisualState;
    }
}

void CalculatorProgrammerRadixOperators::OnViewModelPropertyChanged(Object^ sender, PropertyChangedEventArgs^ e)
{
    if (e->PropertyName == StandardCalculatorViewModel::OpenParenthesisCountPropertyName && closeParenthesisButton->FocusState != ::FocusState::Unfocused)
    {
        Model->SetOpenParenthesisCountNarratorAnnouncement();
    }
}
