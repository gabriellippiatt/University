 //----------------------------------------------------------------------
//EASTRISING TECHNOLOGY CO,.LTD.//
// Module    : 1.8 INCH TFT LCD  128RGB*160 DOTS
// Lanuage   : C51 Code
// Create    : JAVEN LIU
// Date      : 2016-05-11
// Drive IC  : LCD:ILI9163c 
// INTERFACE : LCD 4 WRIE SPI   
// MCU 		 : STC12LE5C60S2
// VDD		 : 5V OR 3.3 V
// MCU VDD   : 3.3 V
//----------------------------------------------------------------------

#include <stc12c5a.h>
#include <stdio.h>
#include <intrins.h>
#include<9163TP.h>
#include<word.h>
//#include<TP.h>
//=============================================================
//DEFINE SD CARD IO
sbit SD_CLK = P1^7;
sbit SD_DI  = P1^5;
sbit SD_DO  = P1^6;
sbit SD_CS  = P1^4;
//===========================================================

//===========================================================
//DEFINE 512 BYTE BUFFER
unsigned char xdata DATA[512];

//===========================================================
//WRITE ONE BYTE TO SD CARD   SPI MODE
void SdWrite(unsigned char n)
{

unsigned char i;

for(i=8;i;i--)
{
SD_CLK=0;
SD_DI=(n&0x80);
n<<=1;
SD_CLK=1;
}
SD_DI=1; 
} 
//===========================================================
//READ ONE BYTE FROM SD CARD   SPI MODE
unsigned char SdRead()
{
unsigned char n,i;
for(i=8;i;i--)
{
SD_CLK=0;
SD_CLK=1;
n<<=1;
if(SD_DO) n|=1;

}
return n;
}
//============================================================
//CHECK SD CARD RDSPONSE 

unsigned char SdResponse()
{
unsigned char i=0,response;

while(i<=8)
{
response = SdRead();
if(response==0x00)
break;
if(response==0x01)
break;
i++;
}
return response;
} 
//================================================================
//SEND COMMAND TO SD CARD
void SdCommand(unsigned char command, unsigned long argument, unsigned char CRC)
{

SdWrite(command|0x40);
SdWrite(((unsigned char *)&argument)[0]);
SdWrite(((unsigned char *)&argument)[1]);
SdWrite(((unsigned char *)&argument)[2]);
SdWrite(((unsigned char *)&argument)[3]);
SdWrite(CRC);
}
//================================================================
//RESET SD CARD
unsigned char SdInit(void)
{
int delay=0, trials=0;
unsigned char i;
unsigned char response=0x01;

SD_CS=1;
for(i=0;i<=9;i++)
SdWrite(0xff);
SD_CS=0;

//Send Command 0 to put MMC in SPI mode
SdCommand(0x00,0,0x95);


response=SdResponse();

if(response!=0x01)
{
return 0;
} 

while(response==0x01)
{
SD_CS=1;
SdWrite(0xff);
SD_CS=0;
SdCommand(0x01,0x00ffc000,0xff);
response=SdResponse();
} 

SD_CS=1;
SdWrite(0xff);
return 1; 
}
//================================================================
//WRITE DATA TO SD CARD ,One time can't more than 512 bytes
unsigned char SdWriteBlock(unsigned char *Block, unsigned long address,int len)
{
unsigned int count;
unsigned char dataResp;
//Block size is 512 bytes exactly
//First Lower SS

SD_CS=0;
//Then send write command
SdCommand(0x18,address,0xff);

if(SdResponse()==00)
{
SdWrite(0xff);
SdWrite(0xff);
SdWrite(0xff);
//command was a success - now send data
//start with DATA TOKEN = 0xFE
SdWrite(0xfe);
//now send data
for(count=0;count<len;count++) SdWrite(*Block++);

for(;count<512;count++) SdWrite(0);
//data block sent - now send checksum
SdWrite(0xff); //两字节CRC校验, 为0XFFFF 表示不考虑CRC
SdWrite(0xff);
//Now read in the DATA RESPONSE token
dataResp=SdRead();
//Following the DATA RESPONSE token
//are a number of BUSY bytes
//a zero byte indicates the MMC is busy

while(SdRead()==0);

dataResp=dataResp&0x0f; //mask the high byte of the DATA RESPONSE token
SD_CS=1;
SdWrite(0xff);
if(dataResp==0x0b)
{
//printf("DATA WAS NOT ACCEPTED BY CARD -- CRC ERROR\n");
return 0;
}
if(dataResp==0x05)
return 1;

//printf("Invalid data Response token.\n");
return 0;
}
//printf("Command 0x18 (Write) was not received by the MMC.\n");
return 0;
}

//=======================================================================
//Read data from SD card ,One time can't more than 512 bytes
unsigned char SdReadBlock(unsigned char *Block, unsigned long address,int len)
{
unsigned int count;
//Block size is 512 bytes exactly
//First Lower SS

 //printf("MMC_read_block\n");

SD_CS=0;
//Then send write command
SdCommand(0x11,address,0xff);

if(SdResponse()==00)
{
//command was a success - now send data
//start with DATA TOKEN = 0xFE
while(SdRead()!=0xfe);

for(count=0;count<len;count++) *Block++=SdRead(); 

for(;count<512;count++) SdRead();

//data block sent - now send checksum
SdRead();
SdRead();
//Now read in the DATA RESPONSE token
SD_CS=1;
SdRead();
return 1;
}
 //printf("Command 0x11 (Read) was not received by the MMC.\n");
return 0;
}

//============================================================
//Main 	
main()
{
 unsigned char  num; 		  
 unsigned long j;  
  unsigned int i=0;
  unsigned long AddTemp;//SD card first address . Use Win HXE software check physical addresses.  SD CARD is different, the address is different yet.

 	P1=0xff;
 	P2=0xff;
 	P3=0xff;
 	P0=0xff;
	BL_ON=0;
	ILI9163_Initial();//TFT Initial
	BL_ON=1;
	while(1)
		{  	AddTemp=270336;//SD card first address . Use Win HXE software check physical addresses.  SD CARD is different, the address is different yet.
		
			LCD_FULL(0xf800);
			 while(KEY);
			 delayms(300);
			 while(KEY);	
			 		 	    
			LCD_FULL(0x07e0);
			 while(KEY);
			 delayms(300);
			 while(KEY);	 	
			LCD_FULL(0x001f);
			 while(KEY);
			 delayms(300);
			 while(KEY);
			LCD_FULL(0x07ff);
			 while(KEY);
			 delayms(300);
			 while(KEY);
			LCD_FULL(0xffe0);
			 while(KEY);
			 delayms(300);
			 while(KEY); 
			LCD_FULL(0xf81f);
			 while(KEY);
			 delayms(300);
			 while(KEY);
		
			ClearScreen(0x00);	
			 while(KEY);
			 delayms(300);
			 while(KEY);
		
			ClearScreen(0xffff);
			 while(KEY);
			 delayms(300);
			 while(KEY);
		
			DispRGBGray();
			 while(KEY);
			 delayms(300);
			 while(KEY);


			show_picture();        //pictuer in mcu
			 while(KEY);
			 delayms(300);
			 while(KEY);	
		
		//	TPTEST();

			SdInit();         //SD Initial
			num=5;		//picture number
		 while(num--)
		 {
		  	    LCD_SetPos(0,127,0,159);//160x128 
		 for(j=0;j<80;j++)   //A picture contains 80 x512 bytes data
		    {
		    SdReadBlock(DATA,AddTemp+(j*512),512);//One time read 512 byte to buffer 
		    for(i=0;i<256;i++)                    //Write buffer data to display.one dots tow byte .512/2=256 dots
			   {   
		   	   Write_Data(DATA[2*i],DATA[2*i+1]);	 
	
		       }
		     } 
		     AddTemp = AddTemp+((j+0)*512);        // 20 ：This value is not data area, SD Card is different, this value, there may be different. Use Win HXE software check the value.
		     while(KEY);                         //Press the STOP button,display next picture
		
			 
		 } 
		}


}


