#include <iostream>
#include <cassert>
#include <iomanip>

#include "global.h"
#include "circuit.h"


Circuit::Circuit() {
}

// Add a primitive Gate to the circuit
void Circuit::add_gate(Gate g) {
    gates.push_back(g); // push to vector
}

// Add an input wire to the circuit
void Circuit::add_input(IO m) {
    input_map.push_back(m);
}

// Add an output wire to the circuit
void Circuit::add_output(IO m) {
    output_map.push_back(m);
}

// Assign bit values to the input wires
void Circuit::set_boolean_inputs(bool *vals) {

    for (int i=0; i<input_map.size(); i++) {
        input_map.at(i).value = vals[i];
    }

}

// Obtain output wire values in boolean array vals
void Circuit::get_boolean_outputs(bool *vals) {
    for (int i=0; i<output_map.size(); i++) {
        vals[i] = output_map.at(i).value;
    }
}

// Obtain the output of the circuit as decimal number
uint64_t Circuit::get_numeric_output() {
    uint64_t res = 0;
    bool val;
    
    for (int i=0; i<output_map.size(); i++) {
        val = output_map.at(i).value;
  
        res = ((uint64_t)val << i) | res;
    }
    
    return res;
}

// Print the circuit (input wire ids, gates with input/output wire ids, and output
//  wire ids)
void Circuit::print() {
    
    std::cout << "\nInputs\n";
    std::cout << "-----------------------------------\n";
    for (int i=0; i<input_map.size(); i++) {
        std::cout << input_map.at(i).id << "\n";
    }
    
    std::cout << "\nCircuit\n";
    std::cout << "-----------------------------------\n";
    Gate g;
    for (int i=0; i<gates.size(); i++) {
        g = gates.at(i);
        switch (g.type) {
            case AND: std::cout << "AND."; break;
            case OR: std::cout << " OR."; break;
            case XOR: std::cout << "XOR."; break;
            default: std::cout << "UKN."; break;
        }
        
        std::cout << g.id << "\t" << g.in1 << "\t" << g.in2 << "\n";
    }
    
    std::cout << "\nOutputs\n";
    std::cout << "-----------------------------------\n";
    for (int i=0; i<output_map.size(); i++) {
        std::cout << output_map.at(i).id << "\n";
    }
}

// Evaluate the circuit after input wires have been assigned values
//  values for each output wire needs to be computed

void Circuit::evaluate_boolean_circuit() {
    // TODO
    bool value;
    
    for(int i = 0; i < output_map.size(); i++) {
        value = get_Gate_Value(output_map.at(i).id);
        output_map[i].value = value;
    }

}

Gate Circuit::findGate(uint64_t id)
{
    Gate g;
    for(int j = 0; j < gates.size(); j++){
        if(gates.at(j).id == id){
            g = gates.at(j);
            break;
        }
    }
    return g;
}

bool Circuit::get_Gate_Value(uint64_t id)
{
    bool result;
    if(id <= input_map.back().id)
    {
        return input_map.at(id).value;
    }
    Gate g = findGate(id);
    if(!g.evaluated)
    {
        
        switch (g.type) 
        {
            case AND: 
                result = get_Gate_Value(g.in1) & get_Gate_Value(g.in2);
                break;
            case OR:  
                result = get_Gate_Value(g.in1) | get_Gate_Value(g.in2);
                break;
            case XOR: 
                result = get_Gate_Value(g.in1) ^ get_Gate_Value(g.in2);
                break;
        }
    }
    else
    {
        result =  g.value;
    }
   
    return result;
}

// Create garbled values and tables
//     k is the security parameter in bits
//     IV will be k/8 bytes in size

// Create garbled values and tables
//     k is the security parameter in bits
//     IV will be k/8 bytes in size

// Create garbled values and tables
//     k is the security parameter in bits
//     IV will be k/8 bytes in size

void Circuit::garble(uint32_t k) {
    assert(k % 8 == 0); // security parameter must be a multiple of 8
    
    // create the random values for each wire
    uint32_t wire_count = input_map.size() + gates.size();
    byte *iv[wire_count][2];  // initialization vector for 0 and 1 on each wire
    bool pb[wire_count];      // permutation bit for each wire
    
    for (int wire=0; wire<wire_count; wire++) {
        for (int bit=0; bit<2; bit++) {
            iv[wire][bit] = new byte[k/8];
            // sample some random values for use as IV and PB
            for (int i=0; i<k/8; i++) {
                iv[wire][bit][i] = myrand() % 0xFF;
                pb[wire] = myrand() % 2;
            }
        }
    }
           
    // create the garbled values of input IO wires
    IO *m;    
    for (int i=0; i<input_map.size(); i++) {
        m = &(input_map.at(i));
        
        // wires do not have a garbled table; only gates do
        // we will only use WIRE_C's space to store the garbled values
        m->gt = new GarbledTable(k, NULL, m->id);
        m->gt->init(NULL, NULL, iv[m->id], false, false, pb[m->id]);        
    }
    
     
    // create garbled values and table in each gate
    Gate *g;
    bool *tt;   // truth table
    bool AND_GATE[4] = {0,0,0,1}; // AND gate truth table
    bool OR_GATE[4] = {0,1,1,1};  // OR gate truth table
    bool XOR_GATE[4] = {0,1,1,0}; // XOR gate truth table

    
    for (int i=0; i<gates.size(); i++) {
        g = &(gates.at(i));
        switch (g->type) { // select the truth table based on gate type
            case AND: tt = AND_GATE; break;
            case  OR: tt = OR_GATE; break;
            case XOR: tt = XOR_GATE; break;
        }
        
        g->gt = new GarbledTable(k, tt, g->id);
        g->gt->init(iv[g->in1], iv[g->in2], iv[g->id], pb[g->in1], pb[g->in2], pb[g->id]);
        // Added
        g->gt->print();
    }
    
    // clean up
    for (int wire=0; wire<wire_count; wire++) {
        for (int bit=0; bit<2; bit++) {
            delete iv[wire][bit];
        }
    }
}

