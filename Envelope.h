#ifndef ENVELOPE_H
#define ENVELOPE_H

class Envelope {
private:

    float currentValue;
    float targetValue;
    float attack;
    float release;
    bool note_pressed;

public:
    Envelope();
    
    void setAttack(float seconds);
    void setRelease(float seconds);
    void noteOn();
    void noteOff();
    float process();
    void applyToBuffer(float* buffer, unsigned long framesPerBuffer);
};

#endif // ENVELOPE_H