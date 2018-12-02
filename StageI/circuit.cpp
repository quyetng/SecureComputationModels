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
