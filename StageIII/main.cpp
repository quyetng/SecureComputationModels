#include "global.h"
#include "binaryadder.h"
#include "circuit.h"

using namespace std;

#define BITLEN 3            // number of bits of an input
//#define BITLEN 20            // number of bits of an input
#define INS 2*BITLEN + 1    // number of input bits
#define OUTS BITLEN+1       // number of output bits

#define DEBUG 


int main() {
    srand(time(NULL));
    
    Circuit cir;  // the circuit
      
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
    ba.generate_circuit(&global_start_id,in,out,&cir);
    
    // third, the output wires
    for (int i=0; i<OUTS; i++) {
        m.id = out[i];
        m.value = false;
        cir.add_output(m);
    }
    
    cir.print();
    
    // garble circuit
    cout << "Garbling circuit\n";
    cir.garble(SECURITY_PARAM); // 80-bit security parameter

    bool v[INS]; //  boolean input values          
    
    // <----
    byte **g_v = new byte *[INS]; // garbled input values
    for (int i=0; i<INS; i++) g_v[i] = new byte[1+SECURITY_PARAM/2];
    // <----
        
    // create some test input
    // first BITLEN bits signify first input (LSB first)
    // next BITLEN bits signify second input (LSB first)
    for (int j=0; j<BITLEN; j++) {
        v[j] = rand() % 2;
        v[j+BITLEN] = rand() % 2;
        
        // the garbled input values
        // <--------
        // ideally g_v[j] will be provided by constructor, and
        // g_v[j+BITLEN] will have to obtained using an oblivious transfer
        g_v[j] = cir.get_garbled_input(j,v[j]);
        g_v[j+BITLEN] = cir.get_garbled_input(j+BITLEN,v[j+BITLEN]);
        // <---------
    }
    v[2*BITLEN] = 0; // lastly carry bit (no carry into circuit)     
    g_v[2*BITLEN] = cir.get_garbled_input(2*BITLEN,0); // <---
    
    cout << "Setting garbled inputs\n";
    //cir.set_boolean_inputs(v);
    cir.set_garbled_inputs(g_v);  // <-----------
        
    cout << "Running garbled circuit\n";
    //cir.evaluate_boolean_circuit();
    cir.evaluate_garbled_circuit(); // <--------
    
    uint64_t result;
    //result = cir.get_numeric_output();
    //<------------
    result = cir.get_numeric_output_from_garble(); // convert garbled output to boolean output
    //<--------------
    
    // print input and output
    cout << "Input 1:  " << compose_booleans(v,BITLEN) << "\n";
    cout << "Input 2:  " << compose_booleans(v+BITLEN,BITLEN) << "\n";
    //cout << "Boolean Circuit Output: " << result << "\n";
    cout << "Garbled Circuit Output: " << result << "\n"; // <--------
    
}



