#ifndef PRIMITIVE_GATE
#define PRIMITIVE_GATE

#include "global.h"
#include "circuit.h"

// A primitive gate takes two bits as input and outputs a bit
class PrimitiveGate {    
    
    GateType gate_type;    
    std::string gate_name;
    
    public:        
        void set_gate_type(GateType);
        void generate_circuit(uint32_t *, Wire [2], Wire *, Circuit *);
};

#endif