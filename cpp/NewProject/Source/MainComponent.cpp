#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent()
{
    setSize (500, 500);
    setAudioChannels (0, 2);
    initSlider(&levelSlider, "Level", new juce::Range<double>(0, 0.1), 0.001);
    initSlider(&frequencySlider, "Freq", new juce::Range<double>(20, 1000), 1);

    frequencySlider.onValueChange = [this]
    {
        if (currentSampleRate > 0.0)
            updateAngleDelta();
    };
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    shutdownAudio();
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    currentSampleRate = sampleRate;
    updateAngleDelta();
}

void MainComponent::getNextAudioBlock (const juce::AudioSourceChannelInfo& bufferToFill)
{
    auto level = (float) (levelSlider.getValue());
    auto* leftBuffer = bufferToFill.buffer->getWritePointer(0, bufferToFill.startSample);
    auto* rightBuffer = bufferToFill.buffer->getWritePointer(1, bufferToFill.startSample);

    for (auto sample = 0; sample < bufferToFill.numSamples; ++sample)
    {
        auto currentSample = (float)std::sin(currentAngle);
        currentAngle += angleDelta;
        leftBuffer[sample] = currentSample * level;
        rightBuffer[sample] = currentSample * level;
    }
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    // You can add your drawing code here!
}

void MainComponent::resized()
{
    auto margin = 15;
    levelSlider.setBounds(margin, margin, getWidth() - margin * 2, margin * 2);
    frequencySlider.setBounds(margin, margin * 3, getHeight() - margin * 2, margin * 2);
}

void MainComponent::updateAngleDelta()
{
    auto cyclesPerSample = frequencySlider.getValue() / currentSampleRate;  
    angleDelta = cyclesPerSample * 2.0 * juce::MathConstants<double>::pi;   
}

void MainComponent::initSlider(juce::Slider* slider, juce::String label, juce::Range<double>* range, double interval)
{
    // these define the parameters of our slider object
    slider->setSliderStyle(juce::Slider::LinearHorizontal);
    slider->setRange(*range, interval);
    slider->setTextBoxStyle(juce::Slider::TextBoxAbove, false, 90, 0);
    slider->setPopupDisplayEnabled(true, false, this);
    slider->setTextValueSuffix(label);
    slider->setValue((slider->getMaximum() - slider->getMinimum()) * 0.5);

    // this function adds the slider to the editor
    addAndMakeVisible(slider);
}


