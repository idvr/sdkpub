/*********************************************************************
*
* Example program: 
*    DAQsingleBufStopTrig_Eseries.c
*
* Description: 
*    Read a waveform from one analog input channel until an external
*     digital STOP trigger (10 samples taken after STOP trigger), using
*     internal timing (uses low-level NI-DAQ functions, for E-series
*     devices)
*
* Example Category: 
*    AI
*
* Example Task Types: 
*    BUF, 1CH, BURST, INTTIM, ASYNC, STOPTRIG, ESER
*
* List of key parameters: 
*    iStopTrigON, ulPtsAfterStopTrig, iSampTB
*
*    [Since variables are hardcoded, there is no guarantee that this
*     program will work for your setup.  This example is simply
*     presented as a code snippet of how you can use NI-DAQ functions
*     to perform a task.]
*
* List of NI-DAQ Functions used in this example: 
*    DAQ_StopTrigger_Config, NIDAQErrorHandler, Select_Signal,
*     DAQ_Rate, DAQ_Start, DAQ_Check, NIDAQYield, DAQ_VScale,
*     DAQ_Clear, NIDAQPlotWaveform
*
*    [NOTE: For further details on each NI-DAQ function, please refer
*     to the NI-DAQ On-Line Help (NIDAQPC.HLP).]
*
* Pin Connection Information: 
*    Connect your analog signal to AI channel 1. The default analog
*     input mode for the DAQ device will be used. Also, connect a
*     digital pulse (falling edge) to the PFI1/TRIG2 pin, and the
*     ground reference to 'digital ground'.
*
*    [For further I/O connection details, please refer to your hardware
*     User Manual.]
*
*    [For further details on how to run this example, please refer to
*     the NI-DAQ Examples On-Line Help (NIDAQEx.HLP).]
*
*********************************************************************/
/*
 * Includes: 
 */

#include "nidaqex.h"


/*
 * Main: 
 */

void main(void)
{
    /*
     * Local Variable Declarations: 
     */

    i16 iStatus = 0;
    i16 iRetVal = 0;
    i16 iDevice = 1;
    i16 iChan = 1;
    i16 iStopTrigON = 1;
    i16 iStopTrigOFF = 0;
    u32 ulPtsAfterStopTrig = 10;
    i16 iGain = 1;
    u32 ulCount = 100;
    f64 dGainAdjust = 1.0;
    f64 dOffset = 0.0;
    f64 dSampRate = 1000.0;
    i16 iUnits = 0;
    i16 iSampTB = 0;
    u16 uSampInt = 0;
    static i16 piBuffer[100] = {0};
    static f64 pdVoltBuffer[100] = {0.0};
    i16 iDAQstopped = 0;
    u32 ulRetrieved = 0;
    i16 iIgnoreWarning = 0;
    i16 iYieldON = 1;

    /* Setup for external stop trigger with iStopTrig = 1. */

    iStatus = DAQ_StopTrigger_Config(iDevice, iStopTrigON,
     ulPtsAfterStopTrig);

    iRetVal = NIDAQErrorHandler(iStatus, "DAQ_StopTrigger_Config",
     iIgnoreWarning);

    /* Setup for external stop trigger into PFI1 with iStopTrig = 1.
     (This is actually done be default in DAQ_StopTrigger_Config.) */
    

    iStatus = Select_Signal(iDevice, ND_IN_STOP_TRIGGER, ND_PFI_1,
     ND_HIGH_TO_LOW);

    iRetVal = NIDAQErrorHandler(iStatus, "Select_Signal",
     iIgnoreWarning);

    /* Convert sample rate (S/sec) to appropriate timebase and sample
     interval values. */

    iStatus = DAQ_Rate(dSampRate, iUnits, &iSampTB, &uSampInt);

    /* Acquire data in a circular buffer fashion into 'piBuffer' from
     a single channel until a digital stop trigger. You must allow the
     number of samples before the stop trigger (in this case, ulCount -
     ulPtsAfterStopTrig) to be acquired or you may get an
     'earlyTrigError'. */

    iStatus = DAQ_Start(iDevice, iChan, iGain, piBuffer, ulCount,
     iSampTB, uSampInt);

    iRetVal = NIDAQErrorHandler(iStatus, "DAQ_Start", iIgnoreWarning);
    

    printf(" Apply your external stop trigger pulse to PFI1.\n");

    while ((iDAQstopped != 1) && (iStatus == 0)) {


        /* Loop until all acquisition is complete.  HINT: You can be
         doing other foreground tasks during this time. */

        iStatus = DAQ_Check(iDevice, &iDAQstopped, &ulRetrieved);
        

        iRetVal = NIDAQYield(iYieldON);

    }


    iRetVal = NIDAQErrorHandler(iStatus, "DAQ_Check", iIgnoreWarning);
    

    iStatus = DAQ_VScale(iDevice, iChan, iGain, dGainAdjust, dOffset,
     ulCount, piBuffer, pdVoltBuffer);

    iRetVal = NIDAQErrorHandler(iStatus, "DAQ_VScale",
     iIgnoreWarning);

    /* CLEANUP - Don't check for errors on purpose. */

    /* Set triggering mode back to initial state. */

    iStatus = DAQ_StopTrigger_Config(iDevice, iStopTrigOFF,
     ulPtsAfterStopTrig);

    iStatus = DAQ_Clear(iDevice);

    /* Plot acquired data */

    iRetVal = NIDAQPlotWaveform(pdVoltBuffer, ulCount, WFM_DATA_F64);
    

    printf(" The data is available in 'pdVoltBuffer'.\n");


}

/* End of program */
