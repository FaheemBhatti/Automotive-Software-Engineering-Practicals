#include "jdp.h" 

void InterruptInit()
{
    /* vector table entry size = 8 bytes (1) 4 bytes (0) */
    INTC.MCR.B.VTES = 0;
    /* software vector mode enable */
    INTC.MCR.B.HVEN = 0;
    /* vector table base adress */
    INTC.IACKR.B.VTBA = 0x12000;
    /* interrupt priority register PIT */
    INTC.PSR[59].B.PRI = 0x5;
    /* interrupt priority register PIT */
    INTC.PSR[60].B.PRI = 0x5;
}
