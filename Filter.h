//
// Created by oussa on 10-04-25.
//

#ifndef FILTER_H
#define FILTER_H


class Filter {
private:
    float cutoff;
    float resonance;
    double a0, a1, a2, b1, b2;
    float x1L, x2L, y1L, y2L;
    float x1R, x2R, y1R, y2R;
    void calculateInternParams();


public:
    Filter();


    void setCutoff(float frequency);

    void setResonance(float value);

    void applyToBuffer(float *buffer, unsigned long framesPerBuffer);
};


#endif //FILTER_H
