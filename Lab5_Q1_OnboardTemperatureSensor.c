#include <msp430.h>
#define CALADC_15V_30C  *((unsigned int *)0x1A1A)                 // Temperature Sensor Calibration-30 C //6682
                                                                  // See device datasheet for TLV table memory mapping //6684
#define CALADC_15V_85C  *((unsigned int *)0x1A1C)                 // Temperature Sensor Calibration-High Temperature (85 for Industrial, 105 for Extended)



volatile long temp, tempRaw;
volatile float IntDegF;
volatile float IntDegC;


int count;

void ConfigureAdc_temp();

void main(void)
{
    WDTCTL = WDTPW + WDTHOLD; // Stop watchdog timer
    _delay_cycles(5);         // Wait for ADC Ref to settle
    ConfigureAdc_temp();

    while(1){


           temp = ADCMEM0;                    // read the converted data into a variable

           // The temperature (Temp, C)=
           IntDegC = (temp-CALADC_15V_30C)*(85-30)/(CALADC_15V_85C-CALADC_15V_30C)+30;

                       // Temperature in Fahrenheit
                       // Tf = (9/5)*Tc | 32
           IntDegF = 9*IntDegC/5+32;

    }
}


// Configure ADC Temperature
void ConfigureAdc_temp(){


        ADCCTL0 |= ADCSHT_8 | ADCON;                                  // ADC ON,temperature sample period>30us
        ADCCTL1 |= ADCSHP| ADCCONSEQ_2;                                            // s/w trig, single ch/conv, MODOSC
        ADCCTL2 &= ~ADCRES;                                           // clear ADCRES in ADCCTL
        ADCCTL2 |= ADCRES_1;                                          // 12-bit conversion results
        ADCMCTL0 |= ADCSREF_1 | ADCINCH_12;                           // ADC input ch A12 => temp sense
        ADCIE |=ADCIE0;
}
