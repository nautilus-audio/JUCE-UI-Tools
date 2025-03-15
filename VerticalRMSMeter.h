/*
  ==============================================================================

    VerticalRMSMeter.h
    Created: 6 Oct 2022 2:01:54pm
    Author:  Alex Mitchell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

namespace Gui
{
    class VerticalRMSMeter : public Component, public Timer
    {
    public:
        VerticalRMSMeter(std::function<float()>&& valueFunction,
                         std::function<float()>&& meterValueFunction)
            : valueSupplier(std::move(valueFunction)), //"move" allows us to continuously update
                                                       // our meters with Rms level and hold
                                                       // level values per channel
              meterHoldValueSupplier(std::move(meterValueFunction))
        {
            // Start Callback timer
            startTimerHz(60);
            setAlpha(.7f);
        }

        void paint(Graphics& g) override
        {
            auto bounds = getLocalBounds().toFloat().reduced(3.f);

            uint8 tempColor = 255;
            Colour transparent(tempColor, tempColor, tempColor, 0.f);

            // Draw meter background
            g.setColour(transparent);
            g.fillRect(bounds);

            // Draw RMS Level Meter
            //            g.setGradientFill(gradient);
            g.setColour(Colours::red);
            const auto scaledY = jmap(valueSupplier(), -60.f, 0.f, 0.f, static_cast<float>(getHeight()));
            g.fillRect(bounds.removeFromBottom(scaledY));

            // Draw Meter Hold Bar into Meter
            int meterHoldBarHeight = getHeight() / 100;
            const auto scaledHold = jmap(meterHoldValueSupplier(), -60.f, 0.f, 0.f, static_cast<float>(getHeight()));

            Rectangle<float> newBounds = bounds;
            newBounds.setHeight(meterHoldBarHeight);
            newBounds.setY(getHeight() - (scaledHold + meterHoldBarHeight));

            g.fillRect(newBounds);
        }

        void SetColorBars(Graphics& g, Colour color) {}

        void resized() override
        {
            const auto bounds = getLocalBounds().toFloat();
            gradient = ColourGradient{Colours::green, bounds.getBottomLeft(), Colours::red, bounds.getTopLeft(), true};
            gradient.addColour(.7f, Colours::yellow);
        }

        void paintOverChildren(::juce::Graphics& g) override {}

        void timerCallback() override { repaint(); }

    private:
        std::function<float()> valueSupplier;
        std::function<float()> meterHoldValueSupplier;
        ColourGradient gradient{};
        Array<juce::Colour> colors{Colours::red, Colours::yellow, Colours::green};
    };
} // namespace Gui
