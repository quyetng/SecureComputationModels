#ifndef GLOBAL
#define GLOBAL

#include <iostream>
#include <cstring>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <chrono>
#include <cmath>
#include <iomanip>
#include <sstream>

#define ID_ASSIGN 1

#define WIRE_A 0                // input 1 of a gate
#define WIRE_B 1                // input 2 of a gate
#define WIRE_C 2                // output of a gate

#define WIRE_ID_SIZE_BITS 64    // number of bits to represent id

// Typedefs            
typedef uint64_t Wire;
typedef unsigned char byte;

   
// Constants
const char hexmap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};


// Helper functions                        
std::string hex_string(byte *, int);
uint64_t compose_booleans(bool *, int);

#endif