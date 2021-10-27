#include <cdef21479.h>
#include <def21479.h>

#include "cs4272.h"
#include "init4272viaSPI.h"

/* Setup the SPI parameters here in a buffer first */
unsigned char ConfigParamCS4272[] =
{
		(CS4272_ADDR), MODE_CONTROL_2, 	CPEN | PDN 							/* 0x03 */,
		(CS4272_ADDR), MODE_CONTROL_1,	DAC_DIF0 | MS_Mode | Ratio1 		/* 0x29 */,
		(CS4272_ADDR), DAC_CONTROL, 	RMP_DN | RMP_UP 					/* 0x0C */,
		(CS4272_ADDR), DAC_VOLUME, 		ATAPI0 | ATAPI1 | ATAPI2 | ATAPI3 	/* 0x0F */,
		(CS4272_ADDR), DAC_VOLUME_A, 	VOL_VOLUME_A(0),
		(CS4272_ADDR), DAC_VOLUME_B, 	VOL_VOLUME_B(0),
		(CS4272_ADDR), ADC_CONTROL, 	ADC_DIF 							/* 0x10 */,
		(CS4272_ADDR), MODE_CONTROL_2, 	CPEN 								/* 0x02 */,
};

unsigned char CS4272_Regs_Read[sizeof(ConfigParamCS4272) / 3];

/*
 * Set up the SPI port to access the CS4272
 * Call with SPI flag to use
 */
void SetupSPICS4272(unsigned int SPI_Flag)
{
    /* Configure the SPI Control registers */
	/* First clear a few registers */
	*pSPICTL = (TXFLSH | RXFLSH) ;
    *pSPIFLG = 0;
    /* Setup the baud rate to 1MHz *pSPIBAUD = 100; */
    *pSPIBAUD = 25;
    /* Setup the SPI Flag register using the Flag specified in the call */
    *pSPIFLG = (CLEAR_DSXEN_BITS|SPI_Flag);
    /* Now setup the SPI Control register */
    *pSPICTL = (SPIEN | SPIMS | WL8 | MSBF | TIMOD1 | CPHASE | CLKPL | SMLS | GM);
}

/* Disable the SPI Port */
void DisableSPICS4272(void)
{
    *pSPICTL = (TXFLSH | RXFLSH);
}

/*  Send a word to the CS4272 via SPI
 *  Call with the CS4272 address, register address, register data,
 *  and SPI flag to use
 */
void ConfigureCS4272Register (unsigned char rwaddr,unsigned char regaddr,unsigned char regdata, unsigned int spiselect)
{
    int i,j;
    unsigned char val[3];

    SELECT_SPI_SLAVE(spiselect);

    val[0]=WR(rwaddr);
    val[1]=regaddr;
    val[2]=regdata;

    for(i=0;i<3;i++)
    {
        j=0;
        *pTXSPI = val[i];
        Delay(136);
        /* Wait for the SPI to indicate that it has finished. */
        while ((*pSPISTAT & TXS))
        {j++;}
    }

    j=0;
    /* Wait for the SPI to indicate that it has finished. */
    while (!(*pSPISTAT & SPIFE))
    {j++;}

    Delay(10);

    DESELECT_SPI_SLAVE(spiselect);
}


/* Receive a register setting from the CS4272
 * Call with the register address, and SPI flag to use; returns register data
 */
unsigned char GetCS4272Register (unsigned char regaddr, unsigned int spiselect)
{
    int i,j;
    unsigned char val[3];
    unsigned int tmp;

    SELECT_SPI_SLAVE(spiselect);

    val[0]=RD(CS4272_ADDR);
    val[1]=regaddr;
    val[2]=0;

    for(i=0;i<3;i++)
    {
    	j=0;
    	*pTXSPI = val[i] ;

    	/* Wait for the SPI to indicate that it has finished. */
    	while ((*pSPISTAT & TXS))
    	{j++;}
    }
    j=0;
    /* Wait for the SPI to indicate that it has finished. */
    while (!(*pSPISTAT & SPIF))
    {j++;}
    while (!(*pSPISTAT & SPIFE))
    {j++;}

    Delay(10);

    tmp = *pRXSPI;

    DESELECT_SPI_SLAVE(spiselect);

    return tmp;
}

/* Set up all CS4272 registers via SPI */
void initCS4272(void)
{
	int configSize = sizeof(ConfigParamCS4272);
    int i,j=0;

    /* Set up CS4272 */
    SetupSPICS4272(CS4272_CS);

    /* Write register settings */
    for(i = 0; i < configSize-6; i+=3)
    {
        ConfigureCS4272Register(ConfigParamCS4272[i], ConfigParamCS4272[i+1], ConfigParamCS4272[i+2], CS4272_CS);
        Delay(272);

        /* Read back register settings for debugging */
        CS4272_Regs_Read[j++] = GetCS4272Register(ConfigParamCS4272[i+1], CS4272_CS);
        Delay(272);
    }

    /* Write register settings */
    for(i = configSize-6; i < configSize; i+=3)
    {
        ConfigureCS4272Register(ConfigParamCS4272[i], ConfigParamCS4272[i+1], ConfigParamCS4272[i+2], CS4272_CS);
        Delay(272);

        /* Read back register settings for debugging */
        CS4272_Regs_Read[j++] = GetCS4272Register(ConfigParamCS4272[i+1], CS4272_CS);
        Delay(272);
    }
    DisableSPICS4272();
}

#if 0
void enableADCHpf(bool enable)
{
	unsigned char dither16State;
	/* if we are going to use bits lower than the 16 MSB, we cannot dither to 16 bits */
    if (bridgeADCGainFactor == 0)
    {
        dither16State = ADC_CONTROL_DITHER16;
    }
    else
    {
        dither16State = 0;
    }
    if (enable)
    {
    	ConfigureCS4272Register(CS4272_ADDR, ADC_CONTROL, dither16State, CS4272_CS);
    }
    else
    {
    	ConfigureCS4272Register(CS4272_ADDR,ADC_CONTROL, ADC_CONTROL_HPF_DISABLE_B | ADC_CONTROL_HPF_DISABLE_A | dither16State, CS4272_CS);
    }
}
#endif

/* Delay loop */
static void Delay(int i)
{
    int j;
    for(j=i ; j > 0; --j)
    	asm("nop;");
}
