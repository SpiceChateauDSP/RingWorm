/*
  ==============================================================================

    SK_Crossover.h
    Created: 12 Jun 2023 4:36:50pm
    Author:  Tim Leete

  ==============================================================================
*/

#pragma once
#include <JuceHeader.h>

class SK_Crossover
{
public:
    SK_Crossover();
    ~SK_Crossover();
    
    float process_high (double sample, int channel);
    float process_low (double sample, int channel);
    
    void prepare (const int newFs);
    void reset();
    
    void updateParameters();
    
private:
    
    /* Time Variables */
    float Fs;
    double Ts;
    
    /* Components */
    const double C = 10e-9;
    double Rc, TwoRc;
    double P;
    
    /* States */
    double lp_x1[2] = {0}, lp_x2[2] = {0};
    double hp_x1[2] = {0}, hp_x2[2] = {0};
    
    /* Intermediate Variables */
    double lp_G1, lp_G2, hp_G1, hp_G2;
    
    /* Coefficients */
    double lp_b0, lp_b1, lp_b2, lp_bVa;
    double hp_b0, hp_b1, hp_b2, hp_bVa;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SK_Crossover);
};
