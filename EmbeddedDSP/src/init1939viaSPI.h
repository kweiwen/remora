#pragma once

static void SetupSPI1939(unsigned int SPI_Flag);
static void DisableSPI1939(void);
void Configure1939Register (unsigned char rwaddr,unsigned char regaddr,unsigned char regdata, unsigned int spiselect);
static unsigned char Get1939Register (unsigned char regaddr, unsigned int spiselect);
void init1939viaSPI(void);
static void Delay(int i);
