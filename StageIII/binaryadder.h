#ifndef BINARY_ADDER
#define BINARY_ADDER

#include "global.h"
#include "fulladder.h"
#include "circuit.h"

// Binary Adder
class BinaryAdder {
    FullAdder *fa; // array of Full Adders
    
    public:
        uint32_t bitlen; // number of bits in each input
        
        BinaryAdder(uint32_t);
        void generate_circuit(uint32_t *, Wire *, Wire *, Circuit *);
};

#endif