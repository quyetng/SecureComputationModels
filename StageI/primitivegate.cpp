#include "global.h"
#include "primitivegate.h"
#include "circuit.h"

using namespace std;

// Sets the type of a primitive gate (AND, OR, XOR)
void PrimitiveGate::set_gate_type (GateType type) {
    gate_type = type;
    
    switch (type) {
        case AND: gate_name = "AND"; break;
        case OR:  gate_name = " OR"; break;
        case XOR: gate_name = "XOR"; break;
    }
}

// Generate a Gate and add to circuit cir
//   Input wire ids are passed in in (size = 2)
//   Output wire id is returned in out (size = 1)
//      start_id contains id to use for output wire (should be
//      incremented after use)
void PrimitiveGate::generate_circuit(uint32_t *start_id, Wire in[2], Wire *out, Circuit *cir) {
    Gate g;  // create a new gate
    g.id = *start_id; *start_id = *start_id+1; // assign id
    g.type = gate_type; // set gate
    g.in1 = in[0]; // set 2 input
    g.in2 = in[1]; 
    g.evaluated = false; // not evaluate 
    g.value = 0; // this is output value
    
    cir->add_gate(g); // add this gate to circuit
    
    *out = g.id; 
}