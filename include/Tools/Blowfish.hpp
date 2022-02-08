// blowfish.h     interface file for blowfish.cpp
// _THE BLOWFISH ENCRYPTION ALGORITHM_
// by Bruce Schneier
// Revised code--3/20/94
// Converted to C++ class 5/96, Jim Conger
#pragma once
#include "types.h"

#define MAXKEYu8S 	56		// 448 bits max
#define NPASS       16		// SBox passes

class BlowFish
{
private:
    u32 	*PArray;
    u32		(*SBoxes)[256];
    void 	Blowfish_encipher(u32 *xl, u32 *xr);
    void 	Blowfish_decipher(u32 *xl, u32 *xr);

public:
        BlowFish();
        ~BlowFish();
    void 	Initialize(const u8 key[], int keyu8s);
    u32		GetOutputLength(u32 lInputLong);
    u32		Encode(u8 *pInput, u8 *pOutput, u32 lSize);
    void	Decode(u8 *pInput, u8 *pOutput, u32 lSize);

} ;

// choose a u8 order for your hardware
#define ORDER_DCBA	// chosing Intel in this case

#ifdef ORDER_DCBA  	// DCBA - little endian - intel
    union aword {
      u32 raw;
      u8  byte[4];
      struct {
        unsigned int byte3:8;
        unsigned int byte2:8;
        unsigned int byte1:8;
        unsigned int byte0:8;
      } w;
    };
#endif

#ifdef ORDER_ABCD  	// ABCD - big endian - motorola
    union aword {
      u32 u32;
      u8  byte[4];
      struct {
        unsigned int byte0:8;
        unsigned int byte1:8;
        unsigned int byte2:8;
        unsigned int byte3:8;
      } w;
    };
#endif

#ifdef ORDER_BADC  	// BADC - vax
    union aword {
      u32 u32;
      u8  byte[4];
      struct {
        unsigned int byte1:8;
        unsigned int byte0:8;
        unsigned int byte3:8;
        unsigned int byte2:8;
      } w;
};
#endif
