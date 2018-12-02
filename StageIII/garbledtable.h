#ifndef GARBLED_TABLE
#define GARBLED_TABLE

#include "global.h"

// Structure to store data for garbling in each gate
typedef struct {
    uint64_t out_wire_id; // wire c identifier
    
    byte *v[3][2]; // wire a,b,c initialization vectors; need to allocate
    byte *w[3][2]; // wire a,b,c garbled values; need to allocate as well
    byte *e_w[4];  // values in the garbled table
    bool pb[3];    // permutation bits for the wires a,b,c
    bool truth[4]; // the truth table of the gate
} GT;

                            
// A garbled table for use in a primitive gate
class GarbledTable {
    
    private:
        
        void generate_garbled_values(byte *a_iv[2], byte *b_iv[2], byte *c_iv[2], 
                    bool a_pb, bool b_pb, bool c_pb);
        void generate_garbled_table(byte *a_iv[2], byte *b_iv[2], byte *c_iv[2], 
                    bool a_pb, bool b_pb, bool c_pb); 
        byte * convertDec2Hex(int dec);
    public:
        GT table; 
        uint32_t sec_param;
        
        GarbledTable(uint32_t, bool *, uint64_t);
        void init(byte *[2], byte *[2], byte *[2], bool, bool, bool);
        void print();
};

#endif