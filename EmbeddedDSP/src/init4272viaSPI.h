#pragma once

void SetupSPICS4272(unsigned int SPI_Flag);
void DisableSPICS4272(void);
void ConfigureCS4272Register (unsigned char rwaddr,unsigned char regaddr,unsigned char regdata, unsigned int spiselect);
unsigned char GetCS4272Register (unsigned char regaddr, unsigned int spiselect);
void initCS4272(void);
static void Delay(int i);
