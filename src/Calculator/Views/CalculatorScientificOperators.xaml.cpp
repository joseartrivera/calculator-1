// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

//
// CalculatorScientificOperators.xaml.cpp
// Implementation of the CalculatorScientificOperators class
//

#include "pch.h"
#include "CalculatorScientificOperators.xaml.h"
#include "CalcViewModel/Common/KeyboardShortcutManager.h"
#include "Controls/CalculatorButton.h"
#include "CalcViewModel/StandardCalculatorViewModel.h"

using namespace CalculatorApp;
using namespace CalculatorApp::Common;
using namespace CalculatorApp::ViewModel;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;

DEPENDENCY_PROPERTY_INITIALIZATION(CalculatorScientificOperators, IsErrorVisualState);
DEPENDENCY_PROPERTY_INITIALIZATION(CalculatorScientificOperators, IsWideLayout);

CalculatorScientificOperators::CalculatorScientificOperators()
{
    InitializeComponent();

    expButton->SetValue(Common::KeyboardShortcutManager::VirtualKeyProperty, Common::MyVirtualKey::E);
    Common::KeyboardShortcutManager::ShiftButtonChecked(false);
}

void CalculatorScientificOperators::OnLoaded(Object^, RoutedEventArgs^)
{
    m_propertyChangedToken = Model->PropertyChanged += ref new PropertyChangedEventHandler(this, &CalculatorScientificOperators::OnViewModelPropertyChanged);
}
void CalculatorScientificOperators::OnUnloaded(Object^, RoutedEventArgs^)
{
    Model->PropertyChanged -= m_propertyChangedToken;
}

void CalculatorScientificOperators::ShortLayout_Completed(_In_ Platform::Object^ /*sender*/, _In_ Platform::Object^ /*e*/)
{
    IsWideLayout = false;
    SetOperatorRowVisibility();
    Common::KeyboardShortcutManager::ShiftButtonChecked(Model->IsShiftChecked);
}

void CalculatorScientificOperators::WideLayout_Completed(_In_ Platform::Object^ /*sender*/, _In_ Platform::Object^ /*e*/)
{
    IsWideLayout = true;
    SetOperatorRowVisibility();
    Common::KeyboardShortcutManager::ShiftButtonChecked(Model->IsShiftChecked);
}

void CalculatorScientificOperators::OnIsErrorVisualStatePropertyChanged(bool /*oldValue*/, bool newValue)
{
    String^ newState = newValue ? L"ErrorLayout" : L"NoErrorLayout";
    VisualStateManager::GoToState(this, newState, false);
    NumberPad->IsErrorVisualState = newValue;
}

void CalculatorScientificOperators::shiftButton_Check(_In_ Platform::Object^ /*sender*/, _In_ Windows::UI::Xaml::RoutedEventArgs^ /*e*/)
{
    bool isChecked = shiftButton->IsChecked->Value;
    Model->IsShiftChecked = isChecked;
    SetOperatorRowVisibility();
}

void CalculatorScientificOperators::trigFlyoutShift_Toggle(_In_ Platform::Object^ /*sender*/, _In_ Windows::UI::Xaml::RoutedEventArgs^ /*e*/)
{
    SetTrigRowVisibility();
    Common::KeyboardShortcutManager::ShiftButtonChecked(trigShiftButton->IsEnabled && trigShiftButton->IsChecked->Value);
}

void CalculatorScientificOperators::trigFlyoutHyp_Toggle(_In_ Platform::Object^ /*sender*/, _In_ Windows::UI::Xaml::RoutedEventArgs^ /*e*/)
{
    SetTrigRowVisibility();
}

void CalculatorScientificOperators::flyoutButton_Clicked(_In_ Platform::Object^ /*sender*/, _In_ Windows::UI::Xaml::RoutedEventArgs^ /*e*/)
{
    this->Trigflyout->Hide();
}

void CalculatorScientificOperators::shiftButton_IsEnabledChanged(_In_ Platform::Object^ /*sender*/, _In_ Windows::UI::Xaml::DependencyPropertyChangedEventArgs^ /*e*/)
{
    SetOperatorRowVisibility();
}

void CalculatorScientificOperators::SetTrigRowVisibility()
{
    bool isShiftChecked = trigShiftButton->IsChecked->Value;
    bool isHypeChecked = hypButton->IsChecked->Value;

    InverseHyperbolicTrigFunctions->Visibility = ::Visibility::Collapsed;
    InverseTrigFunctions->Visibility = ::Visibility::Collapsed;
    HyperbolicTrigFunctions->Visibility = ::Visibility::Collapsed;
    TrigFunctions->Visibility = ::Visibility::Collapsed;

    if (isShiftChecked && isHypeChecked)
    {
        InverseHyperbolicTrigFunctions->Visibility = ::Visibility::Visible;
    }
    else if (isShiftChecked && !isHypeChecked)
    {
        InverseTrigFunctions->Visibility = ::Visibility::Visible;
    }
    else if (!isShiftChecked && isHypeChecked)
    {
        HyperbolicTrigFunctions->Visibility = ::Visibility::Visible;
    }
    else
    {
        TrigFunctions->Visibility = ::Visibility::Visible;
    }
}

void CalculatorScientificOperators::SetOperatorRowVisibility()
{
    ::Visibility rowVis, invRowVis;
    if (shiftButton->IsChecked->Value)
    {
        rowVis = ::Visibility::Collapsed;
        invRowVis = ::Visibility::Visible;
    }
    else
    {
        rowVis = ::Visibility::Visible;
        invRowVis = ::Visibility::Collapsed;
    }

    Row1->Visibility = rowVis;
    InvRow1->Visibility = invRowVis;
}

void CalculatorScientificOperators::OnViewModelPropertyChanged(Object^ sender, PropertyChangedEventArgs^ e)
{
    if (e->PropertyName == StandardCalculatorViewModel::OpenParenthesisCountPropertyName && closeParenthesisButton->FocusState != ::FocusState::Unfocused)
    {
        Model->SetOpenParenthesisCountNarratorAnnouncement();
    }
}
