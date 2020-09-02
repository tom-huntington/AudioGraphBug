#pragma once

#include "MainPage.g.h"
#include <Timer.h>

namespace winrt::AudioGraphBug::implementation
{
    struct MainPage : MainPageT<MainPage>
    {
        MainPage();
        fire_and_forget myComboBox_SelectionChanged(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::Controls::SelectionChangedEventArgs const& e);
        fire_and_forget myComboBox_Loaded(winrt::Windows::Foundation::IInspectable const& sender, winrt::Windows::UI::Xaml::RoutedEventArgs const& e);
        
    private:
        winrt::Windows::Media::Audio::AudioFileInputNode m_fileInputNode{ nullptr };
        winrt::Windows::Media::Audio::AudioDeviceOutputNode m_deviceOutputNode{ nullptr };
        winrt::Windows::Media::Audio::AudioFrameOutputNode m_frameOutputNode{ nullptr };
        winrt::Windows::Media::Audio::AudioDeviceInputNode m_deviceInputNode{ nullptr };

        winrt::Windows::Media::Audio::AudioGraph m_audioGraph{ nullptr };

        timer m_timer;
        Windows::Storage::StorageFile m_file{ nullptr };
    };
}

namespace winrt::AudioGraphBug::factory_implementation
{
    struct MainPage : MainPageT<MainPage, implementation::MainPage>
    {
    };
}
