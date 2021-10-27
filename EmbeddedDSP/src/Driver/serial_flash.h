#pragma once
#include <def21479.h>
#include <Cdef21479.h>
#include <sru21479.h>
#include <stdio.h>

#define READ 0x00000003

#define SECTOR_ERASE 0xD8

#define CHIP_ERASE 0xD7

#define PROGRAM 0x02

#define WREN 0x06

#define WRSR 0x01

#define RDSR 0x05


void initSPI(void);

void Sector_Erase(int address);

void Enable_Write(void);

void Write_Flash(int address, int data);

int Read_Flash(int address);

int Read_Status(void);

void wait_till_busy(void);

extern "C"
{
	extern void bitrev(int x);
}
