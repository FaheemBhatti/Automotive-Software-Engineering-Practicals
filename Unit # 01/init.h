// header file for all registers
#include "jdp.h"
// header files for module configurations
#include "siu.h"
#include "me.h"
#include "pit.h"
#include "adc.h"
#include "interrupt.h"
// header file for defined out- and inputs of the
// Reva motherboard and the SPC560P50L5 daughter board
#include "spc560p_io.h"

// function declarations
void main(void);
void Init(void);

// interrupt functions
void PITCHANNEL0(void);
void PITCHANNEL1(void);
