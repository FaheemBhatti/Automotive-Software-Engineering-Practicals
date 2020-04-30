#include "init.h"


void showData(int value);

/********************************************************************
 *                              main                                *
 ********************************************************************/
void main(void)
{
    /* board initialization */
    Init();

SIU.GPDO[9].R = 1;
SIU.GPDO[42].R = 1;
SIU.GPDO[13].R = 1;
SIU.GPDO[12].R = 1;
SIU.GPDO[62].R = 1;
SIU.GPDO[61].R = 1;
SIU.GPDO[59].R = 1;
SIU.GPDO[11].R = 1;


PIT_ConfigureTimer( 1, 70 );
    /* forever */
    for(;;)
    {
    
/********************************************************************
 *                           Main Loop                              *
 *                    ____
___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 *                  Write down your logic here.                     *
 ********************************************************************/  
    
    
   // showData(ADC_0.CDR[2].B.CDATA);  // Light Sensor
    
    
     if(SIU.GPDI[1].R == 0){
        showData(ADC_0.CDR[2].B.CDATA);
    }
    else{
    SIU.GPDO[9].R = 1;
    SIU.GPDO[42].R = 1;
    SIU.GPDO[13].R = 1;
    SIU.GPDO[12].R = 1;
    SIU.GPDO[62].R = 1;
    SIU.GPDO[61].R = 1;
    SIU.GPDO[59].R = 1;
    SIU.GPDO[11].R = 1;
    }
    
    
    
    
    
    
    
  //showData(ADC_0.CDR[4].B.CDATA); // potentiometer
    
    if(SIU.GPDI[0].R == 0){
        SIU.GPDO[61].R = 0;
    }
    else{
    SIU.GPDO[61].R = 1;
    }



   // if(SIU.GPDI[1].R == 0){
    //    SIU.GPDO[59].R = 0;
    //}
    //else{
    //SIU.GPDO[59].R = 1;
    //}







  if(SIU.GPDI[0].R == 0){
        PIT_StopTimer( 1 );
    }
    else{
   SIU.GPDO[61].R = 1;
    PIT_StartTimer( 1 );
    
    //}


    }
}

void showData(int value)
{
/********************************************************************
 *                    _____  ___  ___   ___                         *
 *                   |_   _|/ _ \|   \ / _ \                        *
 *                     | | | (_) | |) | (_) |                       *
 *                     |_|  \___/|___/ \___/                        *
 *                                                                  *
 ********************************************************************/  

if(value <= 256)
  {   
    /* LED0 - on, LED1 - off, LED2 - off, LED3 - off */
  SIU.GPDO[9].R = 1;
  SIU.GPDO[42].R = 0;
  SIU.GPDO[13].R = 0;
  SIU.GPDO[12].R = 0;
  SIU.GPDO[62].R = 0;
  
  }
  else if(value > 256 && value <= 512)
  {  
    /* LED0 - on, LED1 - on, LED2 - off, LED3 - off */
    SIU.GPDO[9].R = 1;
  SIU.GPDO[42].R = 1;
  SIU.GPDO[13].R = 0;
  SIU.GPDO[12].R = 0;
  SIU.GPDO[62].R = 0;
  
  }
  else if(value > 512 && value <= 768)
  {  
    /* LED0 - on, LED1 - on, LED2 - on, LED3 - off */
    SIU.GPDO[9].R = 1;
  SIU.GPDO[42].R = 1;
  SIU.GPDO[13].R = 1;
  SIU.GPDO[12].R = 0;
  SIU.GPDO[62].R = 0;
  
  }    
else if(value > 768 && value <= 850 )
 {  
  SIU.GPDO[9].R = 1;
  SIU.GPDO[42].R = 1;
  SIU.GPDO[13].R = 1;
  SIU.GPDO[12].R = 1;
  SIU.GPDO[62].R = 0;
 
 } 

  else
  {  
    /* LED0 - on, LED1 - on, LED2 - on, LED3 - on */
  SIU.GPDO[9].R = 1;
  SIU.GPDO[42].R = 1;
  SIU.GPDO[13].R = 1;
  SIU.GPDO[12].R = 1;
  SIU.GPDO[62].R = 1;
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
    /* clear flag */
    PIT.CH[0].TFLG.B.TIF = 1;
}

void PITCHANNEL1() {
    /* clear flag */
    PIT.CH[1].TFLG.B.TIF = 1;
SIU.GPDO[11].R = ~SIU.GPDO[11].R;
}

/********************************************************************
 *                   Interrupt Vector Table                         *
 *                                                                  *
 *                 Don't touch anything below!                      *
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
        default:
            break;
    }
    /* End of Interrupt Request */
    INTC.EOIR.R = 0x00000000;
}
