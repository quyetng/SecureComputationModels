#include "global.h"
#include "binaryadder.h"
#include "fulladder.h"
#include "circuit.h"

// Constructor initializes the full adder array that make this b-bit adder
BinaryAdder::BinaryAdder(uint32_t b) {
    fa = new FullAdder[b];
    bitlen = b;
}

// Generate a binary adder and add to circuit cir; the function should
//   recursively call generate_circuit of lower level gates/circuits
//   Input wire ids are passed in in (size = 2*bitlen+1)
//   Output wire ids (sum and carry) are returned in out (size = bitlen+1)
//      start_id contains smallest id to assign for output wires
void BinaryAdder::generate_circuit(uint32_t *start_id, Wire *in, Wire *out, Circuit *cir) {
    // TODO
    Wire input[3];
    Wire output[2];

	for(int i = 0; i < bitlen; i++)
    {
        input[0] = in[i]; // x
        input[1] = in[bitlen + i]; //y 

        if(i == 0)
        {
            // cary bit
            input[2] = in[2*bitlen];
        }
        else
        {
            input[2] = output[1]; // cary bit     
        }
       
      
        fa[i].generate_circuit(start_id, input, output, cir);
        out[i] = output[0]; // bit sum

    }
    out[bitlen] = output[1];
}

