#ifndef CIRCUIT
#define CIRCUIT
    
#include "global.h"
#include "garbledtable.h" 

// An input/output wire has an id and a bit value
typedef struct {
    uint64_t id;
    bool value;
    
    GarbledTable *gt;  // only used to store the garbled values in wire c's storage<<<<<<< 
    byte *garbled_value; // garbled value in wire (during evaluation) <---
} IO;
    
// Types of primitive gates
enum GateType { AND, OR, XOR };

// A primitive Gate has multiple components
typedef struct {
    uint64_t id;                    // id of gate
    GateType type;                  // type of gate (AND, OR, XOR)
    uint64_t in1, in2;              // id of the two input wires
    
    bool value;                     // output boolean value of gate
    bool evaluated;                 // has the gate been evaluated (value available)? 
    
    GarbledTable *gt;               // garbled table of gate 
    byte *garbled_value;            // output garbled value of gate <-----
} Gate;

// The Circuit class
class Circuit {
    private:
        std::vector<Gate> gates;    // the gates in the circuit
        std::vector<IO> input_map, output_map; // the input/output wires of the circuit
                
        
        bool get_Gate_Value(uint64_t id);
        Gate findGate(uint64_t id);
        bool evaluate_boolean_gate(uint64_t);
        byte* process_gate_garbled_value(uint64_t id);
        //byte* process_gate_garbled_value(Gate g);
        
    public:    
        Circuit();
        void add_gate(Gate);
        void add_input(IO);
        void add_output(IO);
        void set_boolean_inputs(bool *);
        void get_boolean_outputs(bool *);
        uint64_t get_numeric_output();

        void evaluate_boolean_circuit();
        
        void garble(uint32_t); 
        
        // <----
        void evaluate_garbled_circuit();
        void set_garbled_inputs(byte **);
        byte *get_garbled_input(uint64_t, bool);
        uint64_t get_numeric_output_from_garble();
        // <----
        
        void print();
};

#endif