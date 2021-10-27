/*********************************************************************************

 Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

 This software is proprietary and confidential.  By using this software you agree
 to the terms of the associated Analog Devices License Agreement.

 *********************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////
// NAME:     init1939viaSPI.c (Sample-based Talkthrough)
//
// DATE:     02/13/09
//
// PURPOSE:  Programs the control registers on the AD1939 via SPI accesses.
//
// USAGE:    This file contains the subroutines for accessing the AD1939 control
//           registers via SPI, and sets up the AD1939s for TDM serial communication.
//
////////////////////////////////////////////////////////////////////////////////////////

#include <builtins.h>  				/* Get definitions of compiler builtin functions */
#include <platform_include.h>		/* System and IOP register bit and address definitions. */

#include "ADDS_21479_EzKit.h"
#include "init1939viaSPI.h"
#include "ad1939.h"
asm("#include <def21479.h>");


// NOTE: To select ADC/DAC Sample Rate of either 48 kHz, 96 kHz or 192 kHz, choose one
//       of the following 3 macros to set the sample rate (only one at a time).

struct
{
    unsigned char mode_ctrl1[2];
    unsigned char dac_ctrl[2];
    unsigned char dac_vol_mix[2];
    unsigned char cha_volctrl[2];
    unsigned char chb_volctrl[2];
    unsigned char adc_ctrl[2];
    unsigned char mode_ctrl2[2];
    unsigned char chip_id[2];
} CodecMap;


unsigned char ConfigParam1939 [] =
{
		0x10,CodecMapmode_ctrl20 ,CodecMapmode_ctrl21 ,
		0x10,CodecMapmode_ctrl10,CodecMapmode_ctrl11,
		0x10,CodecMapdac_ctrl0,CodecMapdac_ctrl1 ,
		0x10,CodecMapdac_vol_mix0,CodecMapdac_vol_mix1,
		0x10,CodecMapcha_volctrl0,CodecMapcha_volctrl1,
		0x10,CodecMapchb_volctrl0, CodecMapchb_volctrl1,
		0x10,CodecMapadc_ctrl0,CodecMapadc_ctrl1,
		0x10,CodecMapmode_ctrl20,0x02,
};


unsigned char AD1938_Regs_Read[sizeof(ConfigParam1939) / 3];

static void SetupSPI1939(unsigned int SPI_Flag)
{
/* Configure the SPI Control registers */
/* First clear a few registers */
    *pSPICTL = TXFLSH|RXFLSH;
    *pSPIFLG = 0;

/* Setup the baud rate to 1MHz */
    //*pSPIBAUD = 100;
    *pSPIBAUD = 25;

/* Setup the SPI Flag register using the Flag specified in the call */
    *pSPIFLG = (CLEAR_DSXEN_BITS|SPI_Flag);

/* Now setup the SPI Control register */
    *pSPICTL = (SPIEN | SPIMS | WL8 | MSBF | TIMOD1 | CPHASE | CLKPL | SMLS|GM);
   
  
}

/* Disable the SPI Port */
static void DisableSPI1939(void)
{
    *pSPICTL = TXFLSH|RXFLSH;
}

/*  Send a word to the AD1939 via SPI
 *  Call with the AD1939 address, register address, register data,
 *  and SPI flag to use
 */
void Configure1939Register (unsigned char rwaddr,unsigned char regaddr,unsigned char regdata, unsigned int spiselect)
{
    int i;
    unsigned char val[3];

    SELECT_SPI_SLAVE(spiselect);

      val[0]=WR(rwaddr);
      val[1]=regaddr;
      val[2]=regdata;

    for(i=0;i<3;i++)
    {

        *pTXSPI = val[i] ;
        Delay(136);
///* Wait for the SPI to indicate that it has finished. */
        while ((*pSPISTAT & TXS))
        {NOP();}
    }


///* Wait for the SPI to indicate that it has finished. */
    while (!(*pSPISTAT & SPIFE))
    	{NOP();}
    Delay(10);
    DESELECT_SPI_SLAVE(spiselect);
}

/* Receive a register setting from the AD1939
 * Call with the register address, and SPI flag to use; returns register data
 */
static unsigned char Get1939Register (unsigned char regaddr, unsigned int spiselect)
{
    int i;
    unsigned char val[3];
    unsigned int tmp;
    
    SELECT_SPI_SLAVE(spiselect);

    val[0]=RD(AD1939_ADDR);
    val[1]=regaddr;
    val[2]=0;
    
    
    for(i=0;i<3;i++)
    {
        *pTXSPI = val[i] ;
        
       
/* Wait for the SPI to indicate that it has finished. */
       while ((*pSPISTAT & TXS))
        {NOP();}
       
        
    }

/* Wait for the SPI to indicate that it has finished. */
    
    while (!(*pSPISTAT & SPIF))
    {NOP();}
    while (!(*pSPISTAT & SPIFE))
    {NOP();}
    
    Delay(10);
    
    tmp = *pRXSPI;
    
    DESELECT_SPI_SLAVE(spiselect);

    return tmp;
}

/* Set up all AD1939 registers via SPI */
void init1939viaSPI(void)
{
    int configSize = sizeof(ConfigParam1939);
    int i = 0;
    unsigned char tmpA[sizeof(ConfigParam1939) / 3];
    SetupSPI1939(AD1939_CS);

    /* Write register settings*/
    for(i = 0; i < configSize; i+=3)
    {
        Configure1939Register(ConfigParam1939[i], ConfigParam1939[i+1], ConfigParam1939[i+2], AD1939_CS);
        Delay(5);
    }


    DisableSPI1939();
}

/* Delay loop */
static void Delay(int i)
{
    int j;
    for(j=i ; j > 0; --j)
    	NOP();
}
