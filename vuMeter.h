/*
  ==============================================================================

    vuMeter.h
    Created: 15 Apr 2022 9:33:01pm
    Author:  Alexx Mitchell

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>
#include "DistortionProcessor.h"

using namespace juce;

class FilmStripMeter: public Component
{
public:
    FilmStripMeter(Image image, const bool stripIsHorizontal)
    : Component(),
    isHorizontal_(stripIsHorizontal),
    filmStrip(image)
    {
        
    }
    
    virtual ~FilmStripMeter(){
        
    }
    
    void init(Image image, const bool stripIsHorizontal, Image refImage, int numFrames, std::string labelText){
        Label label;
        Font font;
        
        font.setTypefaceName("Courier");
        label.setFont(font);
        label.setJustificationType(Justification::horizontallyCentred);
        label.attachToComponent (this, false);
        label.setText(labelText, dontSendNotification);
        
        addAndMakeVisible(label);
        
        numFrames_ = numFrames;
        frameWidth_ = refImage.getWidth();
        frameHeight_ = refImage.getHeight();
        
    }
    
    void paint(Graphics& g)
    {
//        const float sliderPos = (float) valueToProportionOfLength(getValue());
        
        int value = currentValue * (numFrames_ - 1);
//        int value = 127;
        
        if (value>127)
            value = 127;
                
        if (isHorizontal_) {
            g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                        value * frameWidth_, 0, frameWidth_, frameHeight_);
        }
        else {
            g.drawImage(filmStrip, 0, 0, getWidth(), getHeight(),
                        0, value * frameHeight_, frameWidth_, frameHeight_);
        }
        
        if (isMouseOverOrDragging())
        {
        }
    }
    
    void setMeterValue(float newValue){
        // to dbu
        
        currentValue = newValue;
        repaint();
    }
    
//    void repaint(){
//
//    }
    
//    int getFrameValue(int dbValue){
//        int result;
//        
//        
//    }
    
    int getFrameWidth() const  { return frameWidth_; }
    int getFrameHeight() const { return frameHeight_; }
    
    float currentValue= 86.f ;
    
private:
    int numFrames_;
    const bool isHorizontal_;
    Image filmStrip;
    int frameWidth_, frameHeight_;
    
//    DistortionProcessor& proc;
};

