// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#pragma once

#include "CalcViewModel/Common/Utils.h"

namespace CalculatorApp
{
    namespace Controls
    {
        public ref class OverflowButton sealed : Windows::UI::Xaml::Controls::Button
        {
        public:
            OverflowButton()
            {
            }

            DEPENDENCY_PROPERTY_OWNER(OverflowButton);

            DEPENDENCY_PROPERTY(Platform::String^, Text);
            DEPENDENCY_PROPERTY(Platform::String^, Glyph);

        protected:
            virtual void OnApplyTemplate() override;

        private:
            void FlyoutOpened(Platform::Object^ sender, Platform::Object^ args);
            void FlyoutClosed(Platform::Object^ sender, Platform::Object^ args);

            Windows::UI::Xaml::Controls::FontIcon^ m_chevronIcon;
        };
    }
}
