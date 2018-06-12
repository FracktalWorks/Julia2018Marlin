#ifndef SAVE_PROGRESS_UTIL_H
#define SAVE_PROGRESS_UTIL_H

	// conversion utils
		
	char *ftostr31(const float &x);

	// Convert float to string with 123.4 format
	char *ftostr31ns(const float &x);


	// Convert float to string with 12345.678 format
	char *ftostr53(const float &x);
	
	// Convert float to string with 123.456 format
	char *ftostr33(const float &x);
#endif