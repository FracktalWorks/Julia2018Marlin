#include "Marlin.h"
#include "save_progress_util.h"

char *ftostr31(const float &x)
	{
		char conv[8] = { 0 };
	  int xx=x*10;
	  conv[0]=(xx>=0)?'+':'-';
	  xx=abs(xx);
	  conv[1]=(xx/1000)%10+'0';
	  conv[2]=(xx/100)%10+'0';
	  conv[3]=(xx/10)%10+'0';
	  conv[4]='.';
	  conv[5]=(xx)%10+'0';
	  conv[6]=0;
	  return conv;
	}

	// Convert float to string with 123.4 format
	char *ftostr31ns(const float &x)
	{
		char conv[8] = { 0 };
	  int xx=x*10;
	  //conv[0]=(xx>=0)?'+':'-';
	  xx=abs(xx);
	  conv[0]=(xx/1000)%10+'0';
	  conv[1]=(xx/100)%10+'0';
	  conv[2]=(xx/10)%10+'0';
	  conv[3]='.';
	  conv[4]=(xx)%10+'0';
	  conv[5]=0;
	  return conv;
	}


	// Convert float to string with 12345.678 format
	char *ftostr53(const float &x)
	{
		char conv[8] = { 0 };
	  long xx=x*1000;
	  if (xx >= 0)
		conv[0]=(xx/10000000)%10+'0';
	  else
		conv[0]='-';
	  xx = abs(xx);
	  conv[1]=(xx/1000000)%10+'0';
	  conv[2]=(xx/100000)%10+'0';
	  conv[3]=(xx/10000)%10+'0';
	  conv[4]=(xx/1000)%10+'0';
	  conv[5] = '.';
	  conv[6] = (xx / 100) % 10 + '0';
	  conv[7] = (xx / 10) % 10 + '0';
	  conv[8] = (xx) % 10 + '0';
	  conv[9] = 0;
	  return conv;
	}
	
	// Convert float to string with 123.456 format
	char *ftostr33(const float &x)
	{
		char conv[8] = { 0 };
	  long xx=x*1000;
	  if (xx >= 0)
		conv[0]=(xx/100000)%10+'0';
	  else
		conv[0]='-';
	  xx = abs(xx);
	  conv[1]=(xx/10000)%10+'0';
	  conv[2]=(xx/1000)%10+'0';
	  conv[3] = '.';
	  conv[4] = (xx / 100) % 10 + '0';
	  conv[5] = (xx / 10) % 10 + '0';
	  conv[6] = (xx) % 10 + '0';
	  conv[7] = 0;
	  return conv;
	}