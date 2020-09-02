#include "pch.h"
#include "MainPage.h"
#include "MainPage.g.cpp"

struct __declspec(uuid("5b0d3235-4dba-4d44-865e-8f1d0e4fd04d")) __declspec(novtable) IMemoryBufferByteAccess : ::IUnknown
{
    virtual HRESULT __stdcall GetBuffer(uint8_t** value, uint32_t* capacity) = 0;
};


using namespace winrt;
using namespace Windows::UI::Xaml;
using namespace winrt::Windows::Foundation;
using namespace winrt::Windows::UI::Xaml::Controls;
using namespace winrt::Windows::Media;
using namespace winrt::Windows::Media::Audio;
using namespace winrt::Windows::Media::Capture;
using namespace winrt::Windows::Media::MediaProperties;
using namespace winrt::Windows::Media::Render;

namespace winrt::AudioGraphBug::implementation
{
    MainPage::MainPage()
    {
        InitializeComponent();
    }

    fire_and_forget MainPage::myComboBox_SelectionChanged(IInspectable const& sender, SelectionChangedEventArgs const& e)
    {
        
        if (myComboBox().SelectedIndex() == 0)
            m_deviceInputNode.AddOutgoingConnection(m_frameOutputNode);
        else
        {
            Windows::Storage::Pickers::FileOpenPicker picker{};
            picker.SuggestedStartLocation(Windows::Storage::Pickers::PickerLocationId::MusicLibrary);
            picker.FileTypeFilter().Append(L".mp3");
            picker.FileTypeFilter().Append(L".wav");

            m_file = co_await picker.PickSingleFileAsync();

            CreateAudioFileInputNodeResult fileInputNodeResult = co_await m_audioGraph.CreateFileInputNodeAsync(m_file);
            WINRT_ASSERT(fileInputNodeResult.Status() == AudioFileNodeCreationStatus::Success);
            m_fileInputNode = fileInputNodeResult.FileInputNode();

            if (myComboBox().SelectedIndex() == 1)
                m_fileInputNode.AddOutgoingConnection(m_frameOutputNode);
            else if (myComboBox().SelectedIndex() == 2)
            {
                m_fileInputNode.AddOutgoingConnection(m_frameOutputNode);
                m_fileInputNode.AddOutgoingConnection(m_deviceOutputNode);
            }
        }
            
        

        m_audioGraph.QuantumStarted([=](auto sender, auto args)
            {
                AudioFrame frame = m_frameOutputNode.GetFrame();
                AudioBuffer buffer = frame.LockBuffer(AudioBufferAccessMode::Read);
                IMemoryBufferReference reference = buffer.CreateReference();
                uint8_t* value{};
                uint32_t value_size{};
                check_hresult(reference.as<IMemoryBufferByteAccess>()->GetBuffer(&value, &value_size));

                constexpr unsigned start = 100;
                static unsigned count = 0;
                if (count < start)
                {
                    count++;
                }
                else if (count == start)
                {
                    count++;
                    m_timer.start_timer();
                }
                else if (count > start)
                {
                    //m_timer.timeAverage();
                    m_timer.timeCumulative(value_size / sizeof(float));
                    //m_timer.timeCumulativeReseting(value_size / sizeof(float));
                    _ASSERT(value_size / sizeof(float) == 441);
                }
            });

        m_audioGraph.Start();
    }

    fire_and_forget MainPage::myComboBox_Loaded(IInspectable const& sender, RoutedEventArgs const& e)
    {
        AudioGraphSettings settings{ AudioRenderCategory::Other };
        AudioEncodingProperties encoding{};
        encoding.BitsPerSample(32);
        encoding.ChannelCount(1);
        encoding.SampleRate(44100);
        encoding.Bitrate(encoding.BitsPerSample() * encoding.ChannelCount() * encoding.SampleRate());
        encoding.Subtype(L"Float");

        settings.EncodingProperties(encoding);
        CreateAudioGraphResult result = co_await AudioGraph::CreateAsync(settings);
        WINRT_ASSERT(result.Status() == AudioGraphCreationStatus::Success);
        m_audioGraph = result.Graph();

        CreateAudioDeviceInputNodeResult deviceInputNodeResult =
            co_await m_audioGraph.CreateDeviceInputNodeAsync(MediaCategory::Other, m_audioGraph.EncodingProperties());
        WINRT_ASSERT(deviceInputNodeResult.Status() == AudioDeviceNodeCreationStatus::Success);
        m_deviceInputNode = deviceInputNodeResult.DeviceInputNode();
        
        m_frameOutputNode = m_audioGraph.CreateFrameOutputNode();


        CreateAudioDeviceOutputNodeResult deviceOutputNodeResult = co_await m_audioGraph.CreateDeviceOutputNodeAsync();
        WINRT_ASSERT(deviceOutputNodeResult.Status() == AudioDeviceNodeCreationStatus::Success);
        m_deviceOutputNode = deviceOutputNodeResult.DeviceOutputNode();
    }
}

