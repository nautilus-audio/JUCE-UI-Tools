/*
  ==============================================================================

    VerticalColorBlockMeter.h
    Created: 27 Oct 2022 3:46:05am
    Author:  Alex Mitchell

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "Constants.h"

namespace Gui
{
    class VerticalColorBlockMeter : public Component, public Timer
    {
    public:
        VerticalColorBlockMeter(std::function<float()>&& valueFunction, std::function<float()>&& meterValueFunction,
                                bool is_K20_Meter)
            : valueSupplier(std::move(valueFunction)), //"move" allows us to continuously update
                                                       // our meters with Rms level and hold
                                                       // level values per channel
              meterHoldValueSupplier(std::move(meterValueFunction))
        {
            // Start Callback timer
            startTimerHz(60);
            is_K20 = is_K20_Meter;
            setAlpha(.7f);

            //            K-12, -12 dBFS
            //            K-20, -20 dBFS
        }

        void paint(Graphics& g) override
        {
            bounds = getLocalBounds().toFloat().reduced(3.f);

            uint8 tempColor = 255;
            Colour transparent(tempColor, tempColor, tempColor, 0.f);

            // Draw meter background
            g.setColour(transparent);
            g.fillRect(bounds);

            float meterRange;

            if (is_K20) // Calculated by dividing db range by full range of meter (ie 4 dB/ 48db for yellow section
                        // range)
            {
                redBarFraction = Constants::K20::redBarFraction;    // 20 db headroom
                yellowBarFraction = Constants::K20::yellowBarFraction; // 4 db
                greenBarFraction = Constants::K20::greenBarFraction;                // 24 db
                meterRange = -48.f;
            }
            else
            {
                redBarFraction = Constants::K12::redBarFraction;    // 12 dB headroom
                yellowBarFraction = Constants::K12::yellowBarFraction; // 4 db
                greenBarFraction = Constants::K12::greenBarFraction;  // 24 db
                meterRange = -40.f;
            }

            // Draw RMS Level Meter
            colorBarArea = bounds;
            int height = getHeight();
            const auto scaledY = jmap(valueSupplier(), meterRange, 0.f, 0.f, static_cast<float>(getHeight()));
            const auto scaledHold =
                jmap(meterHoldValueSupplier(), meterRange, 0.f, 0.f, static_cast<float>(getHeight()));

            // Get Meter Hold Bar Size And Position
            int meterHoldBarHeight = getHeight() / 100;
            Rectangle<float> newBounds = bounds;
            newBounds.setHeight(meterHoldBarHeight);
            newBounds.setY(getHeight() - (scaledHold + meterHoldBarHeight));

            if (scaledY < (height * greenBarFraction))
            {
                g.setColour(Colours::green);
                g.fillRect(bounds.removeFromBottom(scaledY));
            }
            else if ((scaledY > (height * greenBarFraction)) && (scaledY < (height * (1 - redBarFraction))))
            {

                g.setColour(Colours::yellow);
                g.fillRect(bounds.removeFromBottom(scaledY));
                g.setColour(Colours::green);
                colorBarArea.setY(height * (1 - greenBarFraction));
                colorBarArea.setHeight(height * greenBarFraction);
                g.fillRect(colorBarArea);
            }
            else if (scaledY > (height * (1 - redBarFraction)))
            {
                g.setColour(Colours::red);
                g.fillRect(bounds.removeFromBottom(scaledY));

                g.setColour(Colours::yellow);
                colorBarArea.setY(height * (1 - (yellowBarFraction + greenBarFraction)));
                colorBarArea.setHeight(height * yellowBarFraction);
                g.fillRect(colorBarArea);

                g.setColour(Colours::green);
                colorBarArea.setY(height * (1 - greenBarFraction));
                colorBarArea.setHeight(height * greenBarFraction);
                g.fillRect(colorBarArea);
            }

            // Draw Meter Hold
            g.setColour(Colours::red);
            g.fillRect(newBounds);
        }

        void SetColorBars(Graphics& g, Colour color) {}

        void resized() override
        {
            const auto bounds = getLocalBounds().toFloat();
            gradient = ColourGradient{Colours::green, bounds.getBottomLeft(), Colours::red, bounds.getTopLeft(), true};
            gradient.addColour(.7f, Colours::yellow);
            colorBarArea = bounds;
        }

        void paintOverChildren(::juce::Graphics& g) override {}

        void timerCallback() override { repaint(); }

    private:
        std::function<float()> valueSupplier;
        std::function<float()> meterHoldValueSupplier;
        ColourGradient gradient{};
        Array<juce::Colour> colors{Colours::red, Colours::yellow, Colours::green};
        Rectangle<float> bounds, colorBarArea;
        float redBarFraction, yellowBarFraction, greenBarFraction = 0.f;
        bool is_K20;
    };
} // namespace Gui
