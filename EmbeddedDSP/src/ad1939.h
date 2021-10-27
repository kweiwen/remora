/*********************************************************************************

 Copyright(c) 2012 Analog Devices, Inc. All Rights Reserved.

 This software is proprietary and confidential.  By using this software you agree
 to the terms of the associated Analog Devices License Agreement.

 *********************************************************************************/

/*
 * NAME:     ad1939.h
 * PURPOSE:  Header file with AD1939 Register definitions
 */
#ifndef _ad1939_H_
#define _ad1939_H_

#ifdef _LANGUAGE_C
typedef struct{
    unsigned char rwaddr;
    unsigned char regaddr;
    unsigned char regdata;
} ad1939_reg;
#endif

/*-------------------------------------------------------------------------------*/

/* AD1939 Register address definitions*/
#define CLKCTRL0    (0)       /* PLL and Clock Control register 0*/
#define CLKCTRL1    (1)       /* PLL and Clock Control register 1*/
#define DACCTRL0    (2)       /* DAC control register 0    (R/W)*/
#define DACCTRL1    (3)       /* DAC control register 1    (R/W)*/
#define DACCTRL2    (4)       /* DAC control register 2    (R/W)*/
#define DACMUTE     (5)       /* DAC Individual Channel Mutes*/
#define DACVOL_L1   (6)       /* DAC volume - left 1       (R/W)*/
#define DACVOL_R1   (7)       /* DAC volume - right 1      (R/W)*/
#define DACVOL_L2   (8)       /* DAC volume - left 2       (R/W)*/
#define DACVOL_R2   (9)       /* DAC volume - right 2      (R/W)*/
#define DACVOL_L3   (10)      /* DAC volume - left 3       (R/W)*/
#define DACVOL_R3   (11)      /* DAC volume - right 3      (R/W)*/
#define DACVOL_L4   (12)      /* DAC volume - left 4       (R/W)*/
#define DACVOL_R4   (13)      /* DAC volume - right 4      (R/W)*/
#define ADCCTRL0    (14)      /* ADC control 0             (R/W)*/
#define ADCCTRL1    (15)      /* ADC control 1             (R/W)*/
#define ADCCTRL2    (16)      /* ADC control 2             (R/W)*/



/* CS4272 Register address definitions*/

#define CodecMapmode_ctrl10     (1)
#define CodecMapdac_ctrl0       (2)
#define CodecMapdac_vol_mix0    (3)
#define CodecMapcha_volctrl0    (4)
#define CodecMapchb_volctrl0    (5)
#define CodecMapadc_ctrl0       (6)
#define CodecMapmode_ctrl20     (7)
#define CodecMapchip_id0        (8)

/*-------------------------------------------------------------------------------*/
/* CS4272 Register*/


#define CodecMapmode_ctrl11     (0x29)
#define CodecMapdac_ctrl1       (0x0C)
#define CodecMapdac_vol_mix1    (0x0F)
#define CodecMapcha_volctrl1    (0x00)
#define CodecMapchb_volctrl1    (0x00)
#define CodecMapadc_ctrl1       (0x10)
#define CodecMapmode_ctrl21     (0x03)
#define CodecMapchip_id1        (0x00)



/*-------------------------------------------------------------------------------*/
/* Global register bit definitions*/
#define RD(address)     ((address<<1)|1)
#define WR(address)     (address<<1)       /* Write to register*/

#define AD1939_ADDR (4)    /* Global Address of any ad1939*/

#define CS4272_ADDR (10)   /* Global Address of any CS4272*/

/*-------------------------------------------------------------------------------*/
/* PLL and Clock Control 0 Register*/
#define DIS_ADC_DAC         (0x00)
#define ENA_ADC_DAC         (0x80)

#define PLL_IN_MCLK         (0x00)
#define PLL_IN_DLRCLK       (0x20)
#define PLL_IN_ALRCLK       (0x40)

#define MCLK_OUT_XTAL       (0x00)
#define MCLK_OUT_256FS      (0x08)
#define MCLK_OUT_512FS      (0x10)
#define MCLK_OUT_OFF        (0x18)

#define INPUT256            (0x00)
#define INPUT384            (0x02)
#define INPUT512            (0x04)
#define INPUT768            (0x06)

#define PLL_PWR_UP          (0x00)
#define PLL_PWR_DWN         (0x01)

/*-------------------------------------------------------------------------------*/
/* PLL and Clock Control 1 Register*/
#define AD1938_PLL_LOCK     (0x08)

#define DIS_VREF            (0x00)
#define ENA_VREF            (0x40)

#define ADC_CLK_PLL         (0x00)
#define ADC_CLK_MCLK        (0x20)

#define DAC_CLK_PLL         (0x00)
#define DAC_CLK_MCLK        (0x01)

/*-------------------------------------------------------------------------------*/
/* DAC Control Register 0*/
#define DAC_FMT_I2S         (0x00)
#define DAC_FMT_TDM         (0x40)
#define DAC_FMT_AUX         (0x80)
#define DAC_FMT_DUALTDM     (0xc0)


#define DAC_BCLK_DLY_1      (0x00)
#define DAC_BCLK_DLY_0      (0x08)
#define DAC_BCLK_DLY_8      (0x01)
#define DAC_BCLK_DLY_12     (0x18)
#define DAC_BCLK_DLY_16     (0x20)

#define DAC_SR_48K          (0x00)
#define DAC_SR_96K          (0x02)
#define DAC_SR_192K         (0x04)

#define DAC_PWR_UP          (0x00)
#define DAC_PWR_DWN         (0x01)

/*-------------------------------------------------------------------------------*/
/* DAC Control Register 1 */
#define DAC_BCLK_POL_NORM       (0x00)
#define DAC_BCLK_POL_INV        (0x80)

#define DAC_BCLK_SRC_PIN        (0x00)
#define DAC_BCLK_SRC_INTERNAL   (0x40)

#define DAC_BCLK_SLAVE          (0x00)
#define DAC_BCLK_MASTER         (0x20)

#define DAC_LRCLK_SLAVE         (0x00)
#define DAC_LRCLK_MASTER        (0x10)

#define DAC_LRCLK_POL_NORM      (0x00)
#define DAC_LRCLK_POL_INV       (0x08)

#define DAC_CHANNELS_2          (0x00)
#define DAC_CHANNELS_4          (0x02)
#define DAC_CHANNELS_8          (0x04)
#define DAC_CHANNELS_16         (0x06)

#define DAC_LATCH_MID           (0x00)
#define DAC_LATCH_END           (0x01)

/*-------------------------------------------------------------------------------*/
/* DAC Control Register 2 */
#define DAC_OUT_POL_NORM    (0x00)
#define DAC_OUT_POL_INV     (0x20)

#define DAC_WIDTH_24        (0x00)
#define DAC_WIDTH_20        (0x08)
#define DAC_WIDTH_16        (0x18)

#define DAC_DEEMPH_FLAT     (0x00)
#define DAC_DEEMPH_48K      (0x02)
#define DAC_DEEMPH_44_1K    (0x04)
#define DAC_DEEMPH_32K      (0x06)

#define DAC_UNMUTE_ALL      (0x00)
#define DAC_MUTE_ALL        (0x01)

/*-------------------------------------------------------------------------------*/
/* DAC Individual Channel Mute Register*/
#define DACMUTE_R4    (0x80)
#define DACMUTE_L4    (0x40)
#define DACMUTE_R3    (0x20)
#define DACMUTE_L3    (0x10)
#define DACMUTE_R2    (0x08)
#define DACMUTE_L2    (0x04)
#define DACMUTE_R1    (0x02)
#define DACMUTE_L1    (0x01)

/*-------------------------------------------------------------------------------*/
/*DAC Volume Control Register - 8-bit granularity (256 levels)*/
#define DACVOL_MIN      (0xFF)
#define DACVOL_LOW      (0xC0)
#define DACVOL_MED      (0x80)
#define DACVOL_HI       (0x40)
#define DACVOL_MAX      (0x00)
#define DACVOL_MASK     (0xFF)  /* Volume in dB*/


/*-------------------------------------------------------------------------------*/
/*  ADC Control 0 Register*/
#define ADC_SR_48K          (0x00)
#define ADC_SR_96K          (0x40)
#define ADC_SR_192K         (0x80)

#define ADC_R2_UNMUTE       (0x00)
#define ADC_R2_MUTE         (0x20)

#define ADC_L2_UNMUTE       (0x00)
#define ADC_L2_MUTE         (0x10)

#define ADC_R1_UNMUTE       (0x00)
#define ADC_R1_MUTE         (0x08)

#define ADC_L1_UNMUTE       (0x00)
#define ADC_L1_MUTE         (0x04)

#define ADC_HP_FILT_OFF     (0x00)
#define ADC_HP_FILT_ON      (0x02)

#define ADC_PWR_UP          (0x00)
#define ADC_PWN_DWN         (0x01)

/*-------------------------------------------------------------------------------*/
/*  ADC Control 1 Register*/
#define ADC_LATCH_MID       (0x00)
#define ADC_LATCH_END       (0x80)

#define ADC_FMT_I2S         (0x00)
#define ADC_FMT_TDM         (0x20)
#define ADC_FMT_AUX         (0x40)

#define ADC_BCLK_DLY_1      (0x00)
#define ADC_BCLK_DLY_0      (0x04)
#define ADC_BCLK_DLY_8      (0x08)
#define ADC_BCLK_DLY_12     (0x0c)
#define ADC_BCLK_DLY_16     (0x10)

#define ADC_WIDTH_24        (0x00)
#define ADC_WIDTH_20        (0x01)
#define ADC_WIDTH_16        (0x03)





/*-------------------------------------------------------------------------------*/
/*  ADC Control 2 Register*/
#define ADC_BCLK_SRC_PIN        (0x00)
#define ADC_BCLK_SRC_INTERNAL   (0x80)

#define ADC_BCLK_SLAVE          (0x00)
#define ADC_BCLK_MASTER         (0x40)

#define ADC_CHANNELS_2          (0x00)
#define ADC_CHANNELS_4          (0x10)
#define ADC_CHANNELS_8          (0x20)
#define ADC_CHANNELS_16         (0x30)

#define ADC_LRCLK_SLAVE         (0x00)
#define ADC_LRCLK_MASTER        (0x08)

#define ADC_LRCLK_POL_NORM      (0x00)
#define ADC_LRCLK_POL_INV       (0x04)

#define ADC_BCLK_POL_NORM       (0x00)
#define ADC_BCLK_POL_INV        (0x02)

#define ADC_LRCLK_FMT_50_50     (0x00)
#define ADC_LRCLK_FMT_PULSE     (0x01)


#endif
