#include "global.h"
    
// Returns the hex representation of a byte array
std::string hex_string(byte *data, int len) {
  std::string s(len * 2, ' ');
  
  for (int i = 0; i < len; ++i) {
    s[2 * i]     = hexmap[(data[i] & 0xF0) >> 4];
    s[2 * i + 1] = hexmap[data[i] & 0x0F];
  }
  return s;
}

// Returns the decimal equivalent of a boolean array (LSB at index 0)
uint64_t compose_booleans(bool *y, int len) {
    uint64_t res = 0;
    
    for (int i=0; i<len; i++)
        res = (y[i] << i) | res;
        
    return res;
}
