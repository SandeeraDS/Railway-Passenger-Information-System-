

#include <avr/io.h>
#include <avr/pgmspace.h>
#include "rtc_routines.h"
#include "UART_routines.h"
#include "i2c_routines.h"



//***************************************************************************
//Function to set initial address of the RTC for subsequent reading / writing
//***************************************************************************
unsigned char RTC_setStartAddress(void)
{
   unsigned char errorStatus;
   
   errorStatus = i2c_start();
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC start1 failed.."));
   	 i2c_stop();
	 return 1;
   } 
   
   errorStatus = i2c_sendAddress(DS1307_W);
   
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC sendAddress1 failed.."));
	 i2c_stop();
	 return 1;
   } 
   
   errorStatus = i2c_sendData(0x00);
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC write-2 failed.."));
	 i2c_stop();
	 return 1;
   } 

   i2c_stop();
   return 0;
}

//***********************************************************************
//Function to read RTC registers and store them in buffer rtc_register[]
//***********************************************************************    
unsigned char RTC_read(void)
{

  unsigned char errorStatus, i, data;
  
  errorStatus = i2c_start();
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC start1 failed.."));
   	 i2c_stop();
	 return 1;
   } 
   
   errorStatus = i2c_sendAddress(DS1307_W);
   
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC sendAddress1 failed.."));
	 i2c_stop();
	 return 1;
   } 
   
   errorStatus = i2c_sendData(0x00);
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC write-1 failed.."));
	 i2c_stop();
	 return 1;
   } 

    errorStatus = i2c_repeatStart();
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC repeat start failed.."));
   	 i2c_stop();
	 return 1;
   } 
   
    errorStatus = i2c_sendAddress(DS1307_R);
   
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC sendAddress2 failed.."));
	 i2c_stop();
	 return 1;
   } 
 
    for(i=0;i<7;i++)
   {
      if(i == 6)  	 //no Acknowledge after receiving the last byte
	   	  data = i2c_receiveData_NACK();
	  else
	  	  data = i2c_receiveData_ACK();
		  
   	  if(data == ERROR_CODE)
   	  {
       		//RTC receive failed..
			i2c_stop();
	   		return 1;
   	  }
	  
	  rtc_register[i] = data;
	}
	
	i2c_stop();
	return 0;
}	  



  
		  		     	  
//******************************************************************
//Function to update buffer rtc_register[] for next writing to RTC
//****************************************************************** 
void RTC_updateRegisters(void)
{
  SECONDS = ((time[6] & 0x07) << 4) | (time[7] & 0x0f);
  MINUTES = ((time[3] & 0x07) << 4) | (time[4] & 0x0f);
  HOURS = ((time[0] & 0x03) << 4) | (time[1] & 0x0f);  
  DAY = date[10];
  DATE = ((date[0] & 0x03) << 4) | (date[1] & 0x0f);
  MONTH = ((date[3] & 0x01) << 4) | (date[4] & 0x0f);
  YEAR = ((date[8] & 0x0f) << 4) | (date[9] & 0x0f);
}  


//******************************************************************
//Function to write new time in the RTC 
//******************************************************************   
unsigned char RTC_writeTime(void)
{
  unsigned char errorStatus, i;
  
   errorStatus = i2c_start();
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC start1 failed.."));
   	 i2c_stop();
	 return(1);
   } 
   
   errorStatus = i2c_sendAddress(DS1307_W);
   
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC sendAddress1 failed.."));
	 i2c_stop();
	 return(1);
   } 
   
   errorStatus = i2c_sendData(0x00);
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC write-1 failed.."));
	 i2c_stop();
	 return(1);
   } 

    for(i=0;i<3;i++)
    {
	  errorStatus = i2c_sendData(rtc_register[i]);  
   	  if(errorStatus == 1)
   	  {
       		//transmitString_F(PSTR("RTC write time failed.."));
			i2c_stop();
	   		return(1);
   	  }
    }
	
	i2c_stop();
	return(0);
}


//******************************************************************
//Function to write new date in the RTC
//******************************************************************   
unsigned char RTC_writeDate(void)
{
  unsigned char errorStatus, i;
  
   errorStatus = i2c_start();
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC start1 failed.."));
   	 i2c_stop();
	 return(1);
   } 
   
   errorStatus = i2c_sendAddress(DS1307_W);
   
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC sendAddress1 failed.."));
	 i2c_stop();
	 return(1);
   } 
   
   errorStatus = i2c_sendData(0x03);
   if(errorStatus == 1)
   {
     //transmitString_F(PSTR("RTC write-1 failed.."));
	 i2c_stop();
	 return(1);
   } 

    for(i=3;i<7;i++)
    {
	  errorStatus = i2c_sendData(rtc_register[i]);  
   	  if(errorStatus == 1)
   	  {
       		//transmitString_F(PSTR("RTC write date failed.."));
			i2c_stop();
	   		return(1);
   	  }
    }
	
	i2c_stop();
	return(0);
}
  

//******************************************************************
//Function to get RTC date & time in FAT32 format
//******************************************************************   
unsigned char getDateTime_FAT(void)
{
  
   unsigned char mth, dt, hr, min, sec, error; 
   unsigned int yr;

   error = RTC_read();
   if(error) return 1;

   yr = (YEAR & 0xf0) >> 4;
   yr = (yr * 10)+(YEAR & 0x0f);
   yr = yr+2000;
   yr = yr - 1980;

   dateFAT = yr;

   mth = (MONTH & 0xf0) >> 4;
   mth = (mth * 10)+(MONTH & 0x0f);

   dateFAT = (dateFAT << 4) | mth;

   dt = (DATE & 0xf0) >> 4;
   dt = (dt * 10)+(DATE & 0x0f);

   dateFAT = (dateFAT << 5) | dt;


   hr = (HOURS & 0xf0) >> 4;
   hr = (hr * 10)+(HOURS & 0x0f);

   timeFAT = hr;

   min = (MINUTES & 0xf0) >> 4;
   min = (min * 10)+(MINUTES & 0x0f);

   timeFAT = (timeFAT << 6) | min;

   sec = (SECONDS & 0xf0) >> 4;
   sec = (sec * 10)+(SECONDS & 0x0f);
   sec = sec / 2;    //FAT32 fromat accepts dates with 2sec resolution (e.g. value 5 => 10sec)

   timeFAT = (timeFAT << 5) | sec;

   
   return 0;
}
	  
 
 

 
