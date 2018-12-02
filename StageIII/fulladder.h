#ifndef FULL_ADDER
#define FULL_ADDER

#include "global.h"
#include "primitivegate.h"
#include "circuit.h"

// Full Adder (made of two XORs, two ANDs and one OR gate)
class FullAdder {
    PrimitiveGate xor_a, xor_b;
    PrimitiveGate and_a, and_b;
    PrimitiveGate or_a;
             
    public:
        FullAdder();
        void generate_circuit(uint32_t *, Wire *, Wire *, Circuit *);
};

#endif