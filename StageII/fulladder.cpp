#include "global.h"
#include "fulladder.h"
#include "primitivegate.h"
#include "circuit.h"

using namespace std;

// set type
// Constructor initializes the gate types that make up this adder
FullAdder::FullAdder() {
    xor_a.set_gate_type(XOR);
    xor_b.set_gate_type(XOR);
    and_a.set_gate_type(AND);
    and_b.set_gate_type(AND);
    or_a.set_gate_type(OR);
}


// Create a full adder
// cal output
// Generate a full adder and add to circuit cir; the function should
//   recursively call generate_circuit of lower level gates/circuits
//   Input wire ids are passed in in (size = 3)
//   Output wire ids (sum and carry) are returned in out (size = 2)
//      start_id contains smallest id to assign for output wires
void FullAdder::generate_circuit(uint32_t *start_id, Wire *in, Wire *out, Circuit *cir) { 
    Wire w[2];
    Wire w_ret1, w_ret2, w_ret3; // result of xor_a, and_a, and_b
    
    w[0] = in[0]; // X
    w[1] = in[1]; // Y
    xor_a.generate_circuit(start_id, w, &w_ret1, cir); // do first xor to get output w_ret1
    and_a.generate_circuit(start_id, w, &w_ret2, cir); 
    
    w[0] = in[2]; // carry bit
    w[1] = w_ret1; // output from xor_a
    and_b.generate_circuit(start_id, w, &w_ret3, cir); 
    xor_b.generate_circuit(start_id, w, &out[0], cir); 
    
    w[0] = w_ret2; 
    w[1] = w_ret3;
    or_a.generate_circuit(start_id, w,&out[1], cir); // do or to get carry bit
}




