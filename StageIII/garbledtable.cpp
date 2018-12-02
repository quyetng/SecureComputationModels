#include "global.h"
#include "garbledtable.h"
#include <string> 
#include <cstring> 
#include <openssl/sha.h>


// Constructor
//  Input: 
//      k = security parameter k in bits; this is size of an IV
//      tt = truth table of gate (NULL if used on an IO wire)
//      id = 64-bit id of wire (wire c if used on a gate)
GarbledTable::GarbledTable(uint32_t k, bool *tt, uint64_t id) {
    sec_param = k; // bits
    
    assert(k % 8 == 0); // security parameter is multiple of 8
    assert(WIRE_ID_SIZE_BITS % 8 == 0); // wire_id takes whole number of bytes
    
    // assign output wire id
    table.out_wire_id = id;
    
    // assign a truth table
    if (tt != NULL)
        memcpy(table.truth,tt,4*sizeof(bool));
};

// Print garbled values and table for a gate
void GarbledTable::print() {
    
    std::cout << "-------------------------------------------\n";
    std::cout << " TABLE ID: " << table.out_wire_id << "\n";
    std::cout << "-------------------------------------------\n";
    
    // print wire garbled values
    for (int i=0; i<3; i++) {
        switch (i) {
            case 0: std::cout << "Garbled input A\n"; break;
            case 1: std::cout << "Garbled input B\n"; break;
            case 2: std::cout << "Garbled output C\n"; break;
        }
                
        for (int j=0; j<2; j++) {
            std::cout << "\t" << j << ": " << hex_string(table.w[i][j],1+sec_param / 8) << "\n";
        }
    }
    
    std::cout << "Garbled Table\n";
    // print garbled table
    for (int i=0; i<2; i++) {
        for (int j=0; j<2; j++) {
            int idx = ((i << 1) | j) & 0x3;
            std::cout << "\t(" << i << "," << j << "):  " << hex_string(table.e_w[idx],HASH_SIZE_BYTES) << "\n";
        }
    }
}

// Create the garbled values and table
// Input:
//      a_iv, b_iv, c_iv: IVs for 0 and 1 in wires a, b and c
//      a_pb, b_pb, c_pb: PB for wires a, b and c
//
// When calling this function on an input IO wire, only c_iv and
// and c_pb needs to be provided; others can be NULL/false. In
// this case only wire c's garbled values have to be computed. See
// project description for more on what needs to happen in this function.
//
// Useful macros from global.h:
//      WIRE_ID_SIZE_BITS = number of bits to represent id
//      HASH_SIZE_BYTES   = number of bytes in hash function output

void GarbledTable::init(byte *a_iv[2], byte *b_iv[2], byte *c_iv[2], 
                    bool a_pb, bool b_pb, bool c_pb) {
                        
    // TODO

    table.pb[WIRE_C] = c_pb;
    if(!a_iv && !b_iv && !a_pb && !b_pb)
    {
        generate_garbled_values(a_iv, b_iv, c_iv,  a_pb, b_pb, c_pb);
    }
    else
    {
        generate_garbled_values(a_iv, b_iv, c_iv,  a_pb, b_pb, c_pb);
        generate_garbled_table(a_iv, b_iv, c_iv,  a_pb, b_pb, c_pb);
    }
    
    
}


void GarbledTable::generate_garbled_values(byte *a_iv[2], byte *b_iv[2], byte *c_iv[2], 
                    bool a_pb, bool b_pb, bool c_pb)
{
     //Assigning garbled values
    //Wi[0] = IVi[0]||(0 XOR PBi)
    //Wi[1] = IVi[1]||(1 XOR PBi)  
 
    int len = sec_param/8;

    // allocate 
    for(int i = 0; i < 3; i++)
        for(int j = 0; j < 2; j++)
        {
            table.w[i][j] = new byte[len + 1];
        }

    
    byte c0[len + 1];
    byte c1[len + 1];

    byte a0[len + 1];
    byte a1[len + 1];

    byte b0[len + 1];
    byte b1[len + 1];

    byte tmp;


    if(!a_iv && !b_iv && !a_pb && !b_pb)
    {
        memcpy(c0, c_iv[0], sizeof(byte)*len);
        memcpy(c1, c_iv[1], sizeof(byte)*len);

        tmp = 0 ^ c_pb;
        memcpy(c0 + sizeof(byte)*len, &tmp, sizeof(byte));
        tmp = 1 ^ c_pb;
        memcpy(c1 + sizeof(byte)*len, &tmp, sizeof(byte));
        
        table.w[WIRE_C][0] = new byte[len + 1];
        table.w[WIRE_C][1] = new byte[len + 1];
        memcpy(table.w[WIRE_C][0], c0, sizeof(c0));
        memcpy(table.w[WIRE_C][1], c1, sizeof(c0));
        
    }
    else
    {
        
        memcpy(a0, a_iv[0], sizeof(byte)*len);
        memcpy(a1, a_iv[1], sizeof(byte)*len);

        memcpy(b0, b_iv[0], sizeof(byte)*len);
        memcpy(b1, b_iv[1], sizeof(byte)*len);
        
        memcpy(c0, c_iv[0], sizeof(byte)*len);
        memcpy(c1, c_iv[1], sizeof(byte)*len);

        tmp = 0 ^ a_pb;
        memcpy(a0 + sizeof(byte)*len, &tmp, sizeof(byte));
        tmp = 1 ^ a_pb;

        memcpy(a1 + sizeof(byte)*len, &tmp, sizeof(byte));
        
        tmp = 0 ^ b_pb;
        memcpy(b0 + sizeof(byte)*len, &tmp, sizeof(byte));
        tmp = 1 ^ b_pb;
        memcpy(b1 + sizeof(byte)*len, &tmp, sizeof(byte));

        tmp = 0 ^ c_pb;
        memcpy(c0 + sizeof(byte)*len, &tmp, sizeof(byte));
        tmp = 1 ^ c_pb;
        memcpy(c1 + sizeof(byte)*len, &tmp, sizeof(byte));
        
        table.w[WIRE_A][0] = new byte[len + 1];
        table.w[WIRE_A][1] = new byte[len + 1];
        memcpy(table.w[WIRE_A][0], a0, sizeof(a0));
        memcpy(table.w[WIRE_A][1], a1, sizeof(a1));

        table.w[WIRE_B][0] = new byte[len + 1];
        table.w[WIRE_B][1] = new byte[len + 1];
        memcpy(table.w[WIRE_B][0], b0, sizeof(b0));
        memcpy(table.w[WIRE_B][1], b1, sizeof(b0));


        table.w[WIRE_C][0] = new byte[len + 1];
        table.w[WIRE_C][1] = new byte[len + 1];
        memcpy(table.w[WIRE_C][0], c0, sizeof(c0));
        memcpy(table.w[WIRE_C][1], c1, sizeof(c0));
        
    }

}

void GarbledTable::generate_garbled_table(byte *a_iv[2], byte *b_iv[2], byte *c_iv[2], 
                    bool a_pb, bool b_pb, bool c_pb)
{

    // allocate mem
    for(int i = 0; i < 4; i++)
    {
        table.e_w[i] = new byte[HASH_SIZE_BYTES];
    }
    
    byte h[HASH_SIZE_BYTES];
    byte h2[HASH_SIZE_BYTES];

    byte p[1];
    byte c[WIRE_ID_SIZE_BITS/8];
    byte w[HASH_SIZE_BYTES];
    byte result[HASH_SIZE_BYTES];

    memset(c, 0, sizeof(byte)*(WIRE_ID_SIZE_BITS/8));
    memcpy(c, &table.out_wire_id, sizeof(byte)*WIRE_ID_SIZE_BITS/8);

    int a;
    int g_xy;
    int idx;
    // i - x
    // j - y
    for(int i = 0; i < 2; i++)
    {
        for(int j = 0; j < 2; j++)
        {

            p[0] = ((i ^ a_pb) << 1) | (j ^ b_pb);
    
            // Do hash
            SHA_CTX shactx;
            SHA1_Init(&shactx);
            SHA1_Update(&shactx, a_iv[i], sec_param/8); // 10 byte
            SHA1_Update(&shactx, c, WIRE_ID_SIZE_BITS/8);
            SHA1_Update(&shactx, p, 1);
            SHA1_Final(h,&shactx);

            SHA1_Init(&shactx);
            SHA1_Update(&shactx, b_iv[j], sec_param/8);
            SHA1_Update(&shactx, c, WIRE_ID_SIZE_BITS/8);                                       
            SHA1_Update(&shactx, p, 1);
            SHA1_Final(h2,&shactx);
            
            // calculate index to get value of truth table
            idx = (i << 1 | j);
            g_xy = table.truth[idx];
           
            memset(w, 0, HASH_SIZE_BYTES*sizeof(byte));
            memcpy(w, table.w[WIRE_C][g_xy], sizeof(byte)*(sec_param/8 + 1)); 

            // calculate index to save garbled entry
            //a = ((i << 1) | j);
            a = ((i ^ a_pb) << 1) | (j ^ b_pb);
            
            memset(result, 0, HASH_SIZE_BYTES*sizeof(byte));

            for (int k = 0; k < HASH_SIZE_BYTES; k++)
            {
                
                result[k] = (h[k] ^ h2[k]) ^ w[k];
                
            }

            memcpy(table.e_w[a], result, sizeof(result));

        }
    }

}