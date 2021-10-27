#pragma once

/*-------------------------------------------------------------------------------*/
/* Global register bit definitions*/
#define RD(address)	((address<<1)|1)   /* Read from register */
#define WR(address)	(address<<1)       /* Write to register */

#define CS4272_ADDR (0x10)   /* Global Address of any CS4272 */

#define CLEAR_DSXEN_BITS 0xF00
#define CS4272_CS DS0EN
#define SELECT_SPI_SLAVE(select) (*pSPIFLG &= ~(spiselect<<8))
#define DESELECT_SPI_SLAVE(select) (*pSPIFLG |= (spiselect<<8))

/*-------------------------------------------------------------------------------*/
/* CS4272 Register address definitions*/
#define MODE_CONTROL_1	(1)
#define DAC_CONTROL     (2)
#define DAC_VOLUME     	(3)
#define DAC_VOLUME_A    (4)
#define DAC_VOLUME_B    (5)
#define ADC_CONTROL     (6)
#define MODE_CONTROL_2  (7)
#define CHIP_ID        	(8)

/* Address 01h MODE_CONTROL_1 */
#define DAC_DIF0 	(BIT_0)
#define DAC_DIF1 	(BIT_1)
#define DAC_DIF2 	(BIT_2)
#define MS_Mode		(BIT_3)
#define Ratio0 		(BIT_4)
#define Ratio1 		(BIT_5)
#define M0 			(BIT_6)
#define M1 			(BIT_7)

/* Address 02h DAC_CONTROL */
#define INV_B		(BIT_0)
#define INV_A 		(BIT_1)
#define RMP_DN 		(BIT_2)
#define RMP_UP		(BIT_3)
#define DEM0 		(BIT_4)
#define DEM1 		(BIT_5)
#define FILT_SEL 	(BIT_6)
#define AMUTE 		(BIT_7)

/* Address 03h DAC_VOLUME */
#define ATAPI0		(BIT_0)
#define ATAPI1 		(BIT_1)
#define ATAPI2 		(BIT_2)
#define ATAPI3		(BIT_3)
#define ZERO_CROSS 	(BIT_4)
#define VOL_CHB_CHA (BIT_5)
#define FILT_SEL 	(BIT_6)

/* Address 04h DAC_VOLUME_A */
#define VOL_VOLUME_A(x)	(((x) & 0x7F) << 0)
#define MUTE_A 			(BIT_7)

/* Address 05h DAC_VOLUME_B */
#define VOL_VOLUME_B(x)	(((x) & 0x7F) << 0)
#define MUTE_B 			(BIT_7)

/* Address 06h ADC_CONTROL */
#define HPFDisableB	(BIT_0)
#define HPFDisableA (BIT_1)
#define MUTEB 		(BIT_2)
#define MUTEA		(BIT_3)
#define ADC_DIF	 	(BIT_4)
#define DITHER16 	(BIT_5)

/* Address 07h MODE_CONTROL_2 */
#define PDN 		(BIT_0)
#define CPEN 		(BIT_1)
#define FREEZE		(BIT_2)
#define MUTE_TRACK	(BIT_3)
#define LOOP 		(BIT_4)
