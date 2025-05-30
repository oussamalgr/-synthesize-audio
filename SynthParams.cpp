#include "SynthParams.h"


SynthPOD& SynthPOD::operator=(const SharedSynthParameters& other) {
    Guard guard(other.mtx);
    SynthPOD::operator=(static_cast<const SynthPOD&>(other));
    return *this;
}
