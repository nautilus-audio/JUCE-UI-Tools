/*
  ==============================================================================

    Knob.h
    Created: 21 Mar 2022 4:59:08pm
    Author:  Alex Mitchell

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

using namespace juce;

class FilmStripKnob : public Slider
{
public:
    FilmStripKnob(Image image, const bool stripIsHorizontal)
    : Slider(),
    isHorizontal_(stripIsHorizontal),
    filmStrip(image)
    {
        
    }
    
    virtual ~FilmStripKnob(){
        
    }
    
    void init(Image image, const bool stripIsHorizontal){
        setTextBoxStyle(NoTextBox, 0, 0, 0);
        
        
        if (stripIsHorizontal) {
            frameHeight = filmStrip.getHeight();
            numFrames_ = filmStrip.getWidth() / frameHeight;
    //            frameHeight = filmStrip.getHeight();
            frameWidth = filmStrip.getWidth() / numFrames_;
        }
        else {
            frameWidth = image.getWidth();
            numFrames_ = image.getHeight() / frameWidth;
            frameHeight = image.getHeight() / numFrames_;
    //            frameWidth = filmStrip.getWidth();
        }
        
    }
    
    void paint(Graphics& g)
    {
        const float sliderPos = (float) valueToProportionOfLength(getValue());
        
        int value = sliderPos * (numFrames_ - 1);
                
        if (isHorizontal_) {
            g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                                    value * frameWidth, 0, frameWidth, frameHeight);
        }
        else {
            g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                        0, value * frameHeight, frameWidth, frameHeight);
        }
        
        if (isMouseOverOrDragging())
        {
        }
    }
    
    int getFrameWidth() const  { return frameWidth; }
    int getFrameHeight() const { return frameHeight; }
    
private:
    int numFrames_;
    const bool isHorizontal_;
    Image filmStrip;
    int frameWidth, frameHeight;
};
