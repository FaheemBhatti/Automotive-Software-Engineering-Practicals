#include "init.h"


int deltaSpeed = 5, speed = 0,up=1,rec_speed=0;
/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();
	//configure the timer 0 and set payload to 1000ms.
	PIT_ConfigureTimer(0, 200 );

	LED0 = 0;


    /* forever */
	for (;;)
	{
		/********************************************************************
		 *                           Main Loop                              *
		 *                    _____  ___  ___   ___                         *
		 *                   |_   _|/ _ \|   \ / _ \                        *
		 *                     | | | (_) | |) | (_) |                       *
		 *                     |_|  \___/|___/ \___/                        *
		 *                                                                  *
		 *                  Write down your logic here.                     *
		 ********************************************************************/
	}

}

/********************************************************************
 *                      Interrupt Functions                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                Interrupts can be handled below.                  *
 ********************************************************************/  
 
 void PITCHANNEL0() {

	 CAN_0.BUF[0].DATA.B[0] = speed;
	 CAN_0.BUF[0].DATA.B[1] = speed>>8;
	 CAN_0.BUF[0].CS.B.CODE = 0b1100;

	 //Toggeling the LED0.
	 LED0 = ~LED0; 
	 if (speed == 300 && up == 1) {
		 up = 0;
	 }

	 if (speed == 0 && up == 0) {
		 up = 1;
	 }

	 if (up==1) {
		 speed = speed + deltaSpeed;
	 }
	 else {
		 speed = speed - deltaSpeed;
	 }
	 

	/* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1;
}

void PITCHANNEL1() {
    /* clear flag */
    PIT.CH[1].TFLG.B.TIF = 1;
}

/* ... ISRs ... */

/********************************************************************
 *                   Interrupt Vector Table                         *
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
        case 59:
            /* Timer Interrupt */
            PITCHANNEL0();
            break;
        case 60:
            /* Timer Interrupt */
            PITCHANNEL1();
            break;
		case 61:
			break;
		case 62:
			break;
		case 65:
			break;
		case 66:
			break;
		case 67:
			break;
		case 68:
			//FLEXCAN_BUF_00_03

			CAN_0.IFRL.B.BUF00I = 1;
			CAN_0.IFRL.B.BUF01I = 1;
			CAN_0.IFRL.B.BUF02I = 1;
			CAN_0.IFRL.B.BUF03I = 1;

			break;
		case 69:
			if (CAN_0.IFRL.B.BUF05I == 1 && CAN_0.RXFIFO.ID.B.STD_ID == 0x001) {
            	/* start the timer now */
	            PIT_StartTimer(0);
				rec_speed = CAN_0.RXFIFO.DATA.B[0] & CAN_0.RXFIFO.DATA.B[1];
				CAN_0.BUF[5].CS.B.CODE = 0b0100;
				LED1 = ~LED1; 
			}

			CAN_0.IFRL.B.BUF04I = 1;
			CAN_0.IFRL.B.BUF05I = 1;
			CAN_0.IFRL.B.BUF06I = 1;
			CAN_0.IFRL.B.BUF07I = 1;
			break;
		case 70:
			CAN_0.IFRL.B.BUF08I = 1;
			CAN_0.IFRL.B.BUF09I = 1;
			CAN_0.IFRL.B.BUF10I = 1;
			CAN_0.IFRL.B.BUF11I = 1;
			break;
		case 71:
			CAN_0.IFRL.B.BUF12I = 1;
			CAN_0.IFRL.B.BUF13I = 1;
			CAN_0.IFRL.B.BUF14I = 1;
			CAN_0.IFRL.B.BUF15I = 1;
			break;
		case 72:
			break;
		case 127:
			break;
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}