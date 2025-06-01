#include "SynthParams.h"


/**
 * Assignment operator for SynthPOD to copy from SharedSynthParameters.
 * Locks the mutex of the SharedSynthParameters source to ensure thread safety.
 */
SynthPOD& SynthPOD::operator=(const SharedSynthParameters& other) {
    Guard guard(other.mtx);
    SynthPOD::operator=(static_cast<const SynthPOD&>(other));
    return *this;
}
