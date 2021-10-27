#include "serial_flash.h"

int command = 0;

int status_buff[2];

int write_buff[3];

int read_buff[2];

void wait_till_busy()
{

	int st=Read_Status();
	
	while((st&BIT_7)!=0)
	{
			st=Read_Status();
			
	}
}



int Read_Status()
{
	
	command=RDSR<<24;
	
	bitrev(command);
	
	
	*pSPIDMAC=0x00000080;
	
	*pSPICTL=TXFLSH|RXFLSH;
	
	*pSPIFLG=0xFF01;
	
	*pIISPI=(int)status_buff;
	
	*pIMSPI=1;
		
	*pCSPI=2;
	
	*pSPICTL=SPIEN|SPIMS|WL8|SENDZ|CLKPL|CPHASE|TIMOD2;
	
	*pTXSPI=command;

	*pSPIFLG=0xFE01;
	
	*pSPIDMAC=0x00000007;
	
	while((*pSPIDMAC&SPIDMAS)!=0);
	
	return status_buff[1];
}


int Read_Flash(int address)
{
	
	int temp;
	
	command=READ<<24;
	
	command|=address;
	
	bitrev(command);
	
	
	*pSPIDMAC=0x00000080;
	
	*pSPICTL=TXFLSH|RXFLSH;
	
	*pSPIFLG=0xFF01;
	
	*pIISPI=(int)read_buff;
	
	*pIMSPI=1;
		
	*pCSPI=2;
	
	*pSPICTL=SPIEN|SPIMS|WL32|SENDZ|CLKPL|CPHASE|TIMOD2;
	
	*pTXSPI=command;

	*pSPIFLG=0xFE01;
	
	*pSPIDMAC=0x00000007;
	
	while((*pSPIDMAC&SPIDMAS)!=0);
	
	*pSPIDMAC=0x00000080;
	
	*pSPICTL=TXFLSH|RXFLSH;
	
	temp=read_buff[1]&0x000000FF;
	
	return temp;
	
}

void Enable_Write()
{
		
	command=WREN<<24;
	
	bitrev(command);
	
	*pSPICTL=TXFLSH|RXFLSH;
	
	*pSPIFLG=0xFF01;
	
	*pSPICTL=SPIEN|SPIMS|WL8|SENDZ|CLKPL|CPHASE|TIMOD1;

	*pSPIFLG=0xFE01;	
	
	*pTXSPI=command;
	
	while((*pSPISTAT&TXS)!=0);
	
	while((*pSPISTAT&SPIF)==0);
	
	*pSPICTL=TXFLSH|RXFLSH;
	


}

void Sector_Erase(int address)
{
	
	Enable_Write();
	
	command=SECTOR_ERASE<<24;
	
	command|=address;
	
	bitrev(command);
	
	*pSPICTL=TXFLSH|RXFLSH;
	
	*pSPIFLG=0xFF01;
	
	*pSPICTL=SPIEN|SPIMS|WL32|SENDZ|CLKPL|CPHASE|TIMOD1;

	*pSPIFLG=0xFE01;	
	
	*pTXSPI=command;
	
	while((*pSPISTAT&TXS)!=0);
	
	while((*pSPISTAT&SPIF)==0);
	
	
	*pSPICTL=TXFLSH|RXFLSH;
	
	wait_till_busy();
	
}


void Write_Flash(int address, int data)
{
	
	int t;
	
	Enable_Write();
	
	command=PROGRAM<<24;
	
	command|=address;//(page*256);
	
	bitrev(command);
	
	write_buff[0]=command;
	
	write_buff[1]=data|0xFFFFFF00;
	
	write_buff[2]=0xFFFFFFFF;
	
	*pSPICTL=TXFLSH|RXFLSH;
	
	*pSPIFLG=0xFF01;
	
	*pSPICTL=SPIEN|SPIMS|WL32|SENDZ|CLKPL|CPHASE|TIMOD1;

	*pSPIFLG=0xFE01;	
	
	for(t=0;t<3;t++)
	{
		*pTXSPI=write_buff[t];
	
	   
		while((*pSPISTAT&TXS)!=0);
	
		while((*pSPISTAT&SPIF)==0);
		
	}

	*pSPIFLG=0xFF01;	
	
	*pSPICTL=TXFLSH|RXFLSH;
	
	wait_till_busy();
}	

	
void initSPI()
{
	*pSPIFLG=0xFF01;
	*pSPIBAUD=0x64;
}
