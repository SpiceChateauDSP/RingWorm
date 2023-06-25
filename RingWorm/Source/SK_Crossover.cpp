/*
  ==============================================================================

    SK_Crossover.cpp
    Created: 12 Jun 2023 4:36:50pm
    Author:  Tim Leete

  ==============================================================================
*/

#include "SK_Crossover.h"

SK_Crossover::SK_Crossover() {};
SK_Crossover::~SK_Crossover() {};

float SK_Crossover::process_high (double sample, int channel) {
    /* Find Main Outputs */
    double Vo = sample * hp_b0 + hp_x1[channel] * hp_b1 + hp_x2[channel] * hp_b2;
    
    /* Update States */
    double Va = Vo + Vo * hp_bVa + hp_x2[channel] * Rc;

    hp_x1[channel] = TwoRc * (sample - Va) - hp_x1[channel];
    hp_x2[channel] = TwoRc * (Va - Vo) - hp_x2[channel];
    
    return -Vo;
}

float SK_Crossover::process_low (double sample, int channel) {
    /* Find Main Outputs */
    double Vo = sample * lp_b0 + lp_x1[channel] * lp_b1 + lp_x2[channel] * lp_b2;

    /* Update States */
    double Va = Vo + Vo * lp_bVa - lp_x2[channel] * P;

    lp_x1[channel] = TwoRc * (Va - Vo) - lp_x1[channel];
    lp_x2[channel] = TwoRc * Vo - lp_x2[channel];
    
    return Vo;
}

void SK_Crossover::prepare (const int newFs) {
    this -> Fs = newFs;
    Ts = 1/Fs;
    
    Rc = Ts / (2 * C);
    TwoRc = 2/Rc;
    
    updateParameters();
    reset();
}

void SK_Crossover::reset() {
    lp_x1[0] = lp_x1[1] = 0;
    lp_x2[0] = lp_x2[1] = 0;
    hp_x1[0] = hp_x1[1] = 0;
    hp_x2[0] = hp_x2[1] = 0;
}

void SK_Crossover::updateParameters() {
    P = 16500;
    
    /* Intermediate Variables */
    lp_bVa = P/Rc;
    lp_G1 = 2/P + 1/Rc;
    lp_G2 = 1 + lp_bVa - 1/(Rc * lp_G1) - 1/(P * lp_G1);
    
    hp_bVa = Rc/P;
    hp_G1 = 1/P + 2/Rc;
    hp_G2 = 1 + hp_bVa - 1/(P * hp_G1) - 1/(Rc * hp_G1);
    
    /* Coefficients */
    auto t = lp_G1 * lp_G2;
    lp_b0 = 1 / (P * t);
    lp_b1 = 1 / t;
    lp_b2 = P / lp_G2;

    auto u = hp_G1 * hp_G2;
    hp_b0 = 1 / (Rc * u);
    hp_b1 = -1 / u;
    hp_b2 = 1 / u - Rc/hp_G2;
}
