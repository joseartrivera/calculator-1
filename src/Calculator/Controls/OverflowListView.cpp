// Copyright (c) Microsoft Corporation. All rights reserved.
// Licensed under the MIT License.

#include "pch.h"
#include "OverflowListView.h"

using namespace CalculatorApp;
using namespace CalculatorApp::Controls;

using namespace Platform;
using namespace std;
using namespace Windows::Devices::Input;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Controls;

OverflowListView::OverflowListView()
{
}

void OverflowListView::OnApplyTemplate()
{
    m_scrollViewer = safe_cast<ScrollViewer^>(GetTemplateChild("ScrollViewer"));
    m_scrollLeft = safe_cast<Button^>(GetTemplateChild("ScrollLeft"));
    m_scrollRight = safe_cast<Button^>(GetTemplateChild("ScrollRight"));
    m_border = safe_cast<Border^>(GetTemplateChild("Border"));
    m_content = safe_cast<ItemsPresenter^>(GetTemplateChild("Content"));

    m_scrollLeft->Click += ref new RoutedEventHandler(this, &OverflowListView::OnScrollClick);
    m_scrollRight->Click += ref new RoutedEventHandler(this, &OverflowListView::OnScrollClick);

    m_border->PointerEntered += ref new PointerEventHandler(this, &OverflowListView::OnPointerEntered);
    m_border->PointerExited += ref new PointerEventHandler(this, &OverflowListView::OnPointerExited);

    ListView::OnApplyTemplate();
}

void OverflowListView::OnScrollClick(_In_ Object^ sender, _In_ RoutedEventArgs^)
{
    auto clicked = safe_cast<Button^>(sender);
    if (clicked == m_scrollLeft)
    {
        ScrollLeft();
    }
    else
    {
        ScrollRight();
    }
}

void OverflowListView::OnPointerEntered(_In_ Object^ sender, _In_ PointerRoutedEventArgs^ e)
{
    if (e->Pointer->PointerDeviceType == PointerDeviceType::Mouse)
    {
        UpdateScrollButtons();
    }
}

void OverflowListView::OnPointerExited(_In_ Object^ sender, _In_ PointerRoutedEventArgs^)
{
    m_scrollLeft->Visibility = ::Visibility::Collapsed;
    m_scrollRight->Visibility = ::Visibility::Collapsed;
}

void OverflowListView::ScrollLeft()
{
        double offset = m_scrollViewer->HorizontalOffset - (scrollRatio * m_scrollViewer->ViewportWidth);
        m_scrollViewer->UpdateLayout();
        m_scrollViewer->ChangeView(offset, nullptr, nullptr);
        m_scrollViewer->UpdateLayout();
        UpdateScrollButtons();
}

void OverflowListView::ScrollRight()
{
        double offset = m_scrollViewer->HorizontalOffset + (scrollRatio * m_scrollViewer->ViewportWidth);
        m_scrollViewer->UpdateLayout();
        m_scrollViewer->ChangeView(offset, nullptr, nullptr);
        m_scrollViewer->UpdateLayout();
        UpdateScrollButtons();
}


void OverflowListView::UpdateScrollButtons()
{
    // When the width is smaller than the container, don't show any
    if (m_content->ActualWidth <= m_scrollViewer->ActualWidth)
    {
        ShowHideScrollButtons(::Visibility::Collapsed, ::Visibility::Collapsed);
    }
    // We have more number on both side. Show both arrows
    else if ((m_scrollViewer->HorizontalOffset > 0) && (m_scrollViewer->HorizontalOffset < (m_scrollViewer->ExtentWidth - m_scrollViewer->ViewportWidth)))
    {
        ShowHideScrollButtons(::Visibility::Visible, ::Visibility::Visible);
    }
    // Width is larger than the container and left most part of the number is shown. Should be able to scroll left.
    else if (m_scrollViewer->HorizontalOffset == 0)
    {
        ShowHideScrollButtons(::Visibility::Collapsed, ::Visibility::Visible);
    }
    else // Width is larger than the container and right most part of the number is shown. Should be able to scroll left.
    {
        ShowHideScrollButtons(::Visibility::Visible, ::Visibility::Collapsed);
    }
}

void OverflowListView::ShowHideScrollButtons(::Visibility vLeft, ::Visibility vRight)
{
    if (m_scrollLeft != nullptr && m_scrollRight != nullptr)
    {
        m_scrollLeft->Visibility = vLeft;
        m_scrollRight->Visibility = vRight;
    }
}
