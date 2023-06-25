/*
  ==============================================================================

    LFO.h
    Created: 24 Jun 2023 12:43:23pm
    Author:  Tim Leete

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class LFO
{
public:
    LFO() {};
    ~LFO() {};
    
    void prepare (const float newFs) {
        if (Fs != newFs) {
            Fs = newFs;
            Ts = 1/Fs;
            
            phi = pi * 2 * rate * Ts;
        }
    }
    
    void reset () {
        angle[0] = angle[1] = 0;
    }
    
    double getCurrentLFOPosition (const int channel) {
        double lfoPosition;
        switch (shape) {
            case 1:
                lfoPosition = processSine (angle[channel]);
                break;
                
            default:
                lfoPosition = processSine (angle[channel]);
                break;
        }
        
        // Update Angle
        angle[channel] += phi;
        if (angle[channel] > (pi * 2))
            angle[channel] -= (pi * 2);
        
        return lfoPosition;
    }
    
    double processSine (const double angle) {
        return depth * math.SinCubic (angle) + depth;
    }
    
    double processSquare (const double angle) {
        return depth * math.SinCubic (angle) + depth;
    }
    
    void setRate (const double newRate){
        if (rate != newRate) {
            rate = newRate;
            
            phi = pi * 2 * rate * Ts;
        }
    }
    
    void setDepth (const double newDepth) {
        if (depth != newDepth)
            depth = newDepth;
    }
    
private:
    MathApprox math; // SC_Utilities
    
    enum LFO_Shape {
        Sine,
        Square,
        Triangle,
        Saw,
        Random
    };
    
    /* Digital Variables */
    float Fs = 48000;
    double Ts = 1/Fs;
    
    /* Control Parameters */
    double rate = 0;
    double depth = 0;
    LFO_Shape shape = Sine;
    
    /* LFO Variables */
    double angle[2] = {0.f}, phi = 0.f;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(LFO);
};
