#include "global.h"
#include "binaryadder.h"
#include "circuit.h"

using namespace std;

#define BITLEN 20            // number of bits of an input
#define INS 2*BITLEN+1       // number of input bits
#define OUTS BITLEN+1       // number of output bits

int main() {
    srand(time(NULL));
    
    Circuit cir;  // the circuit
      
    FullAdder fa; // a full adder
    BinaryAdder ba(BITLEN); // a binary adder
      
    Wire in[INS];       // input wires
    Wire out[OUTS];     // output wires

    IO m;
    uint32_t global_start_id;
    
    // assign ids and build circuit
    // first, input wires
    for (int id = 0; id<INS; id++){
        in[id] = id;
        m.id = id;
        m.value = false;
        cir.add_input(m);
    }
    
    
    // second, the gates
    global_start_id = INS;
    //fa.generate_circuit(&global_start_id,in,out,&cir);
    ba.generate_circuit(&global_start_id,in,out,&cir );
    
    // third, the output wires
    for (int i=0; i<OUTS; i++) {
        m.id = out[i];
        m.value = false;
        cir.add_output(m);
    }
    
    cir.print();

    bool v[INS]; //  input values          
        
    // create some test input
    // first BITLEN bits signify first input (LSB first)
    // next BITLEN bits signify second input (LSB first)
    for (int j=0; j<BITLEN; j++) {
        v[j] = rand() % 2;
        
        v[j+BITLEN] = rand() % 2;
    }
    v[2*BITLEN] = rand() % 2; // lastly carry bit   

    cout << "Setting inputs\n";
    cir.set_boolean_inputs(v);
        
    cout << "Running circuit\n";
    cir.evaluate_boolean_circuit();
    
    uint64_t result;
    result = cir.get_numeric_output();
    
    // print input and output
    cout << "Input 1:  " << compose_booleans(v,BITLEN) << "\n";
    cout << "Input 2:  " << compose_booleans(v+BITLEN,BITLEN) << "\n";
    cout << "Carry  :  " << compose_booleans(v+2*BITLEN,1) << "\n";
    cout << "Boolean Circuit Output: " << result << "\n";
    
}



