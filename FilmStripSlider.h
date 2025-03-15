/*
  ==============================================================================

    Slider.h
    Created: 28 Mar 2022 8:29:43pm
    Author:  Alex Mitchell

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

namespace Gui
{
    class FilmStripSlider : public Slider
    {
    public:
        FilmStripSlider(Image image, const bool stripIsHorizontal)
            : Slider(), isHorizontal_(stripIsHorizontal), filmStrip(image)
        {
        }

        virtual ~FilmStripSlider() {}

        void init(Image image, const bool stripIsHorizontal, Image refImage, int numFrames, std::string labelText)
        {
            setTextBoxStyle(NoTextBox, 0, 0, 0);
            Label label;
            Font font;

            font.setTypefaceName("Courier");
            label.setFont(font);
            label.setJustificationType(Justification::horizontallyCentred);
            addAndMakeVisible(label);
            label.setText(labelText, dontSendNotification);
            label.attachToComponent(this, true);

            this->setName(labelText);

            numFrames_ = numFrames;
            frameWidth_ = refImage.getWidth();
            frameHeight_ = refImage.getHeight();
        }

        void paint(Graphics& g)
        {
            const float sliderPos = (float)valueToProportionOfLength(getValue());

            int value = sliderPos * (numFrames_ - 1);

            if (isHorizontal_)
            {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(), value * frameWidth_, 0, frameWidth_,
                            frameHeight_);
            }
            else
            {
                g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(), 0, value * frameHeight_, frameWidth_,
                            frameHeight_);
            }

            if (isMouseOverOrDragging())
            {
            }
        }

        int getFrameWidth() const { return frameWidth_; }
        int getFrameHeight() const { return frameHeight_; }

    private:
        int numFrames_;
        const bool isHorizontal_;
        Image filmStrip;
        int frameWidth_, frameHeight_;
    };
} // namespace Gui
