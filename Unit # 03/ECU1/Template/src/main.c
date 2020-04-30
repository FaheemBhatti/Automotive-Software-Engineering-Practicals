#include "init.h"
/********************************************************************
*                          global variables                         *
********************************************************************/

int second_int = 0;

int drive_wheels_speed, non_driven_wheels_speed;
int light_control_is_alive = 1;
unsigned char gear = 2;

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();

    /* turn off leds */
    LED0 = 1;
    LED1 = 1;
    LED2 = 1;
    LED3 = 1;
    LED4 = 1;
    LED5 = 1;
    LED6 = 1;
    LED7 = 1; 

	/* channel 0 and 1 of PIT timer are configured to interrupt every 100ms and 200ms respectively */
	PIT_ConfigureTimer(0, 100);
	PIT_ConfigureTimer(1, 200);
	
	/* start PIT timer for channel 0 and 1 */
	PIT_StartTimer(0);
	PIT_StartTimer(1); 

    /* forever */
    for(;;)
    {
		// shift up
		if(BT5 == 0) {
			if(gear < 128) {
				gear = gear << 1;
				while(BT5 == 0);
			}
		}
		
		//shift down
		if(BT6 == 0) {
			if(gear > 1) {
				gear = gear >> 1;
				while(BT6 == 0);
			}
		}

    }
}

/********************************************************************
 *                      Interrupt Functions                         *
 ********************************************************************/  

void PITCHANNEL0(void)
{
	/* send (0.1)RPM value */
	CAN_0.BUF[0].DATA.B[0] = ADC_0.CDR[4].B.CDATA;
	CAN_0.BUF[0].DATA.B[1] =  ADC_0.CDR[4].B.CDATA >> 8;		
	CAN_0.BUF[0].CS.B.CODE = 12;
	
    PIT.CH[0].TFLG.B.TIF = 1;
}

void PITCHANNEL1(void)
{
	// send gear data
    CAN_0.BUF[1].DATA.B[0] = gear;		
	CAN_0.BUF[1].CS.B.CODE = 12;
	
	// send alive signal
	CAN_0.BUF[3].CS.B.CODE = 12;
	
	// send error signal
	if(light_control_is_alive == 0) {
		CAN_0.BUF[4].CS.B.CODE = 12;
		LED7 = ~LED7;
	} else {
		light_control_is_alive = 0;
		LED7 = 1;
	}
	
	if(second_int == 1) {
		// send ASR warning
		int difference;
		if(drive_wheels_speed > non_driven_wheels_speed)
			difference = drive_wheels_speed - non_driven_wheels_speed;
		else
			difference = non_driven_wheels_speed - drive_wheels_speed;
		if(difference > 15)
			CAN_0.BUF[2].DATA.B[0] = 0x00;
		else
			CAN_0.BUF[2].DATA.B[0] = 0x01;
		CAN_0.BUF[2].CS.B.CODE = 12;
		second_int = 0;
	} else
		second_int = 1;
	
    PIT.CH[1].TFLG.B.TIF = 1;
}

void CANMB0003(void)
{
/* No modifications needed here */
/* Receive interrupts are being cleared here */
    CAN_0.IFRL.B.BUF00I = 1;
    CAN_0.IFRL.B.BUF01I = 1;
    CAN_0.IFRL.B.BUF02I = 1;
    CAN_0.IFRL.B.BUF03I = 1;
}

void CANMB0407(void)
{
	if(CAN_0.IFRL.B.BUF05I) {
		switch(CAN_0.RXFIFO.ID.B.STD_ID) {
			case 0x302:
				non_driven_wheels_speed = (CAN_0.RXFIFO.DATA.B[1] << 8) | CAN_0.RXFIFO.DATA.B[0];
				LED0 = ~LED0;
				break;
			case 0x502:
				drive_wheels_speed = (CAN_0.RXFIFO.DATA.B[1] << 8) | CAN_0.RXFIFO.DATA.B[0];
				LED1 = ~LED1;
				break;
			case 0x303:
				light_control_is_alive = 1;
				LED2 = ~LED2;
				break;
		}
	}  
   
    /* clear flags as last step here! */
    /* don't change anything below! */
    CAN_0.IFRL.B.BUF04I = 1;
    CAN_0.IFRL.B.BUF05I = 1;
    CAN_0.IFRL.B.BUF06I = 1;
    CAN_0.IFRL.B.BUF07I = 1;
}

void CANMB0811(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */

    CAN_0.IFRL.B.BUF08I = 1;
    CAN_0.IFRL.B.BUF09I = 1;
    CAN_0.IFRL.B.BUF10I = 1;
    CAN_0.IFRL.B.BUF11I = 1;
}

void CANMB1215(void)
{
/* No modifications needed here */
/* transmit interrupts are being cleared here */
    CAN_0.IFRL.B.BUF12I = 1;
    CAN_0.IFRL.B.BUF13I = 1;
    CAN_0.IFRL.B.BUF14I = 1;
    CAN_0.IFRL.B.BUF15I = 1;
}



/********************************************************************
 *                   Interrupt Vector Table                         *
 ********************************************************************/
#pragma interrupt Ext_Isr
#pragma section IrqSect RX address=0x040
#pragma use_section IrqSect Ext_Isr

void Ext_Isr() {
    switch(INTC.IACKR.B.INTVEC)
    {
        case 59:
            PITCHANNEL0();
            break;
        case 60:
            PITCHANNEL1();
        case 68:
            CANMB0003();
            break;
        case 69:
            CANMB0407();
            break;
        case 70:
            CANMB0811();
            break;
        case 71:
            CANMB1215();
            break;        
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
