// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "CalcViewModel/Common/Utils.h"

namespace CalculatorApp
{
    namespace Controls
    {
        public ref class OverflowListView sealed : public Windows::UI::Xaml::Controls::ListView
        {
        public:
            OverflowListView();

        protected:
            virtual void OnApplyTemplate() override;

        private:
            void OnScrollClick(_In_ Platform::Object^ sender, _In_ Windows::UI::Xaml::RoutedEventArgs^ e);
            void OnPointerEntered(_In_ Platform::Object^ sender, _In_ Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
            void OnPointerExited(_In_ Platform::Object^ sender, _In_ Windows::UI::Xaml::Input::PointerRoutedEventArgs^ e);
            void ShowHideScrollButtons(Windows::UI::Xaml::Visibility vLeft, Windows::UI::Xaml::Visibility vRight);
            void OnTokensUpdatedPropertyChanged(bool oldValue, bool newValue);
            void UpdateScrollButtons();

            void UpdateVisualState();
            void UpdateExpressionState();
            void UpdateAllState();
            void ScrollLeft();
            void ScrollRight();

            double scrollRatio = 1.0;
            bool m_scrollingLeft;
            bool m_scrollingRight;
            Windows::UI::Xaml::Controls::ItemsPresenter^ m_content;
            Windows::UI::Xaml::Controls::Border^ m_border;
            Windows::UI::Xaml::Controls::ScrollViewer^ m_scrollViewer;
            Windows::UI::Xaml::Controls::Button^ m_scrollLeft;
            Windows::UI::Xaml::Controls::Button^ m_scrollRight;

            Windows::Foundation::EventRegistrationToken m_scrollLeftClickEventToken;
            Windows::Foundation::EventRegistrationToken m_scrollRightClickEventToken;
            Windows::Foundation::EventRegistrationToken m_pointerEnteredEventToken;
            Windows::Foundation::EventRegistrationToken m_pointerExitedEventToken;
        };
    }
}
