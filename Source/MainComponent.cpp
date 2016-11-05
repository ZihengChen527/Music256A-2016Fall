// Music 256a / CS 476a | fall 2016
// CCRMA, Stanford University
// Homework 1
//
// Author: Ziheng Chen (zihengcATccrmaDOTstanfordDOTedu)
// Description: A waveform player that can gradually generate square wave, triangle wave, and 
//              sawtooth wave, by using additive synthesis.

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Sine.h"
#include "math.h"

class MainContentComponent :
    public AudioAppComponent,
    private Slider::Listener,
    private ToggleButton::Listener
{
public:
    MainContentComponent() : squareGain (0.0), triangleGain (0.0), sawtoothGain (0.0), 
                             squarePartial (1), trianglePartial (1), sawtoothPartial (1),
                             onOff (0), samplingRate(0.0)
    {
        // configuring square wave fundamental frequency slider and adding it to the main window
        addAndMakeVisible (squareFrequencySlider);
        squareFrequencySlider.setRange (50.0, 5000.0);
        squareFrequencySlider.setSkewFactorFromMidPoint (500.0);
        squareFrequencySlider.setValue(1000);
        squareFrequencySlider.addListener (this);
        
        // configuring square wave fundamental frequency label box and adding it to the main window
        addAndMakeVisible(squareFrequencyLabel);
        squareFrequencyLabel.setText ("Square Wave Fundamental Frequency", dontSendNotification);
        squareFrequencyLabel.attachToComponent (&squareFrequencySlider, false);
        
        // configuring triangle wave fundamental frequency slider and adding it to the main window
        addAndMakeVisible (triangleFrequencySlider);
        triangleFrequencySlider.setRange (50.0, 5000.0);
        triangleFrequencySlider.setSkewFactorFromMidPoint (500.0);
        triangleFrequencySlider.setValue(1000);
        triangleFrequencySlider.addListener (this);
        
        // configuring triangle wave fundamental frequency label box and adding it to the main window
        addAndMakeVisible(triangleFrequencyLabel);
        triangleFrequencyLabel.setText ("Triangle Wave Fundamental Frequency", dontSendNotification);
        triangleFrequencyLabel.attachToComponent (&triangleFrequencySlider, false);
        
        // configuring sawtooth wave fundamental frequency slider and adding it to the main window
        addAndMakeVisible (sawtoothFrequencySlider);
        sawtoothFrequencySlider.setRange (50.0, 5000.0);
        sawtoothFrequencySlider.setSkewFactorFromMidPoint (500.0);
        sawtoothFrequencySlider.setValue(1000);
        sawtoothFrequencySlider.addListener (this);
        
        // configuring sawtooth wave fundamental frequency label box and adding it to the main window
        addAndMakeVisible(sawtoothFrequencyLabel);
        sawtoothFrequencyLabel.setText ("Sawtooth Wave Fundamental Frequency", dontSendNotification);
        sawtoothFrequencyLabel.attachToComponent (&sawtoothFrequencySlider, false);

        // configuring square wave gain slider and adding it to the main window
        addAndMakeVisible (squareGainSlider);
        squareGainSlider.setRange (0.0, 1.0);
        squareGainSlider.setValue(0.5);
        squareGainSlider.addListener (this);
        
        
        // configuring square wave gain label and adding it to the main window
        addAndMakeVisible(squareGainLabel);
        squareGainLabel.setText ("Square Wave Gain", dontSendNotification);
        squareGainLabel.attachToComponent (&squareGainSlider, false);
        
        // configuring triangle wave gain slider and adding it to the main window
        addAndMakeVisible (triangleGainSlider);
        triangleGainSlider.setRange (0.0, 1.0);
        triangleGainSlider.setValue(0.5);
        triangleGainSlider.addListener (this);
        
        
        // configuring triangle wave gain label and adding it to the main window
        addAndMakeVisible(triangleGainLabel);
        triangleGainLabel.setText ("Triangle Wave Gain", dontSendNotification);
        triangleGainLabel.attachToComponent (&triangleGainSlider, false);
        
        // configuring sawtooth wave gain slider and adding it to the main window
        addAndMakeVisible (sawtoothGainSlider);
        sawtoothGainSlider.setRange (0.0, 1.0);
        sawtoothGainSlider.setValue(0.5);
        sawtoothGainSlider.addListener (this);
        
        
        // configuring sawtooth wave gain label and adding it to the main window
        addAndMakeVisible(sawtoothGainLabel);
        sawtoothGainLabel.setText ("Sawtooth Wave Gain", dontSendNotification);
        sawtoothGainLabel.attachToComponent (&sawtoothGainSlider, false);
        
        
        // configuring square wave partial slider and adding it to the main window
        addAndMakeVisible (squarePartialSlider);
        squarePartialSlider.setRange (1, maxPartialNum, 1);
        squarePartialSlider.setValue(1);
        squarePartialSlider.addListener (this);
        
        // configuring square wave partial label and adding it to the main window
        addAndMakeVisible(squarePartialLabel);
        squarePartialLabel.setText ("Square Wave Partial Number", dontSendNotification);
        squarePartialLabel.attachToComponent (&squarePartialSlider, false);
        
        // configuring triangle wave partial slider and adding it to the main window
        addAndMakeVisible (trianglePartialSlider);
        trianglePartialSlider.setRange (1, maxPartialNum, 1);
        trianglePartialSlider.setValue(1);
        trianglePartialSlider.addListener (this);
        
        // configuring triangle wave partial label and adding it to the main window
        addAndMakeVisible(trianglePartialLabel);
        trianglePartialLabel.setText ("Triangle Wave Partial Number", dontSendNotification);
        trianglePartialLabel.attachToComponent (&trianglePartialSlider, false);
        
        // configuring sawtooth wave partial slider and adding it to the main window
        addAndMakeVisible (sawtoothPartialSlider);
        sawtoothPartialSlider.setRange (1, maxPartialNum, 1);
        sawtoothPartialSlider.setValue(1);
        sawtoothPartialSlider.addListener (this);
        
        // configuring sawtooth wave partial label and adding it to the main window
        addAndMakeVisible(sawtoothPartialLabel);
        sawtoothPartialLabel.setText ("Sawtooth Wave Partial Number", dontSendNotification);
        sawtoothPartialLabel.attachToComponent (&sawtoothPartialSlider, false);
        
        // configuring on/off button and adding it to the main window
        addAndMakeVisible(onOffButton);
        onOffButton.addListener(this);
        
        // configuring on/off label and adding it to the main window
        addAndMakeVisible(onOffLabel);
        onOffLabel.setText ("On/Off", dontSendNotification);
        onOffLabel.attachToComponent (&onOffButton, true);
        
        setSize (1200, 400);
        nChans = 1;
        setAudioChannels (0, nChans); // no inputs, one output
    }
    
    ~MainContentComponent()
    {
        shutdownAudio();
    }
    
    void resized() override
    {
        // placing the UI elements in the main window
        const int sliderLeft = 50;
        const int sliderTop = 50;
        const int heightInterval = (getHeight() - 2*sliderTop - 20*4) / 3;  // height interval between two slider box
        
        squareFrequencySlider.setBounds (sliderLeft, sliderTop, getWidth()/3 - 2*sliderLeft, 20);
        squareGainSlider.setBounds (sliderLeft, sliderTop + 20 + heightInterval, getWidth()/3 - 2*sliderLeft, 20);
        squarePartialSlider.setBounds (sliderLeft, sliderTop + 40 + 2*heightInterval, getWidth()/3 - 2*sliderLeft, 20);
        
        triangleFrequencySlider.setBounds (getWidth()/3 + sliderLeft, sliderTop, getWidth()/3 - 2*sliderLeft, 20);
        triangleGainSlider.setBounds (getWidth()/3 + sliderLeft, sliderTop + 20 + heightInterval, getWidth()/3 - 2*sliderLeft, 20);
        trianglePartialSlider.setBounds (getWidth()/3 + sliderLeft, sliderTop + 40 + 2*heightInterval, getWidth()/3 - 2*sliderLeft, 20);
        
        sawtoothFrequencySlider.setBounds (getWidth()/3*2 + sliderLeft, sliderTop, getWidth()/3 - 2*sliderLeft, 20);
        sawtoothGainSlider.setBounds (getWidth()/3*2 + sliderLeft, sliderTop + 20 + heightInterval, getWidth()/3 - 2*sliderLeft, 20);
        sawtoothPartialSlider.setBounds (getWidth()/3*2 + sliderLeft, sliderTop + 40 + 2*heightInterval, getWidth()/3 - 2*sliderLeft, 20);
        
        onOffButton.setBounds (getWidth()/2, getHeight() - sliderTop, 20, 20);
    }
    
    void sliderValueChanged (Slider* slider) override
    { 
        if (samplingRate > 0.0){
            
            // if frequency slider changes, then also change the partials, for square wave, the frequency ratio is 1:3:5:7...
            if (slider == &squareFrequencySlider) {   
                for (int i = 0; i < maxPartialNum; i++) {
                    sineForSquare[i].setFrequency(squareFrequencySlider.getValue() * (2*i+1));
                }
            }
            // get the value of gain slider
            else if (slider == &squareGainSlider) {   
                squareGain = squareGainSlider.getValue();
            }
            // get the value of partial number slider
            else if (slider == &squarePartialSlider) {
                squarePartial = squarePartialSlider.getValue();
            }
            // if frequency slider changes, then also change the partials, for triangle wave, the frequency ratio is 1:3:5:7...
            else if (slider == &triangleFrequencySlider) {
                for (int i = 0; i < maxPartialNum; i++) {
                    sineForTriangle[i].setFrequency(triangleFrequencySlider.getValue() * (2*i+1));
                }
            }
            // get the value of gain slider
            else if (slider == &triangleGainSlider) {
                triangleGain = triangleGainSlider.getValue();
            }
            // get the value of partial number slider
            else if (slider == &trianglePartialSlider) {
                trianglePartial = trianglePartialSlider.getValue();
            }
            // if frequency slider changes, then also change the partials, for sawtooth wave, the frequency ratio is 1:2:3:4...
            else if (slider == &sawtoothFrequencySlider) {
                for (int i = 0; i < maxPartialNum; i++) {
                    sineForSawtooth[i].setFrequency(sawtoothFrequencySlider.getValue() * (i+1));
                }
            }
            // get the value of gain slider
            else if (slider == &sawtoothGainSlider) {
                sawtoothGain = sawtoothGainSlider.getValue();
            }
            // get the value of partial number slider
            else if (slider == &sawtoothPartialSlider) {
                sawtoothPartial = sawtoothPartialSlider.getValue();
            }            
        }
    }
    
    void buttonClicked (Button* button) override
    {
        // turns audio on or off
        if(button == &onOffButton && onOffButton.getToggleState()){
            onOff = 1;
        }
        else{
            onOff = 0;
        }
    }
    
    void prepareToPlay (int /*samplesPerBlockExpected*/, double sampleRate) override
    {
        samplingRate = sampleRate;
        
        for (int i = 0; i < maxPartialNum; i++) {
            sineForSquare[i].setSamplingRate(sampleRate);
            sineForTriangle[i].setSamplingRate(sampleRate);
            sineForSawtooth[i].setSamplingRate(sampleRate);
        }
    }
    
    void releaseResources() override
    {
    }
    
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // getting the audio output buffer to be filled
        float* const buffer = bufferToFill.buffer->getWritePointer (0, bufferToFill.startSample);
        
        // computing one block
        for (int sample = 0; sample < bufferToFill.numSamples; ++sample)
        {
            float squareSum = 0, triangleSum = 0, sawtoothSum = 0;
            if(onOff == 1) {
                // compute each sample using additive synthesis, scale to make sure the amplitude is below 1
                for (int i = 0; i < squarePartial; i++) {     
                    squareSum += sineForSquare[i].tick() * 4/PI * 1/(2*i+1);
                }
                for (int i = 0; i < trianglePartial; i++) {
                    triangleSum += sineForTriangle[i].tick() * 8/(PI*PI) * 1/pow(2*i+1, 2) * pow(-1, i);
                }
                for (int i = 0; i < sawtoothPartial; i++) {
                    sawtoothSum += sineForSawtooth[i].tick() * 2/PI * 1/(i+1);
                }
                
                // add the three waves together to compose the output
                buffer[sample] = squareSum * squareGain + triangleSum * triangleGain + sawtoothSum * sawtoothGain;
            }
            else buffer[sample] = 0.0;
        }
    }
    
    
private:
    // UI Elements
    Slider squareFrequencySlider, triangleFrequencySlider, sawtoothFrequencySlider;
    Slider squareGainSlider, triangleGainSlider, sawtoothGainSlider;
    Slider squarePartialSlider, trianglePartialSlider, sawtoothPartialSlider;
    ToggleButton onOffButton;
    
    Label squareFrequencyLabel, triangleFrequencyLabel, sawtoothFrequencyLabel;
    Label squareGainLabel, triangleGainLabel, sawtoothGainLabel;
    Label squarePartialLabel, trianglePartialLabel, sawtoothPartialLabel;
    Label onOffLabel;
    
    const static int maxPartialNum = 16;
    
    Sine sineForSquare[maxPartialNum];    // the sine waves which composed a square wave
    Sine sineForTriangle[maxPartialNum];  // the sine waves which composed a triangle wave
    Sine sineForSawtooth[maxPartialNum];  // the sine waves which composed a sawtooth wave
    
    // Global Variables
    float squareGain, triangleGain, sawtoothGain;   // Gain for each wave
    int squarePartial, trianglePartial, sawtoothPartial;   // Partial Number for each wave
    int onOff, samplingRate, nChans;
    
    const double PI = 3.1415926535897931;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};

Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
