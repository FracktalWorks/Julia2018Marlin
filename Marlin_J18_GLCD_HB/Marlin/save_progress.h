#ifndef SAVE_PROGRESS_H
#define SAVE_PROGRESS_H

	#if ENABLED(SAVE_PROGRESS) && ENABLED(SDSUPPORT)
		#include "save_progress_util.h"
		
		// save recovery file
			void saveRecoveryFile(CardReader card, Temperature thermalManager, float *current_position) {
				
				char cmd[30];
				char* c;
				
				int bedTarget = thermalManager.degTargetBed();
				int hotendTarget = thermalManager.degTargetHotend(0); 
				
				card.pauseSDPrint(); //Pauses the print by setting sdprinting = false
				
				uint32_t pos = card.getFilePos(); //gets sd pos, the current position in SD card
				card.closefile(); //also sets saving = false

				card.openFile("RESR.GCO", false);  //open a file to write, also sets saving = true

				sprintf_P(cmd, PSTR("M117 Restarting %s"), card.longFilename);
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G28"));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G90"));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("M190 S%s"), ftostr31ns(bedTarget));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("M109 S%s"), ftostr31ns(hotendTarget));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G1 Z%s"), ftostr33((current_position[Z_AXIS] + 5)));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G92 E0"));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G92 F200 E5"));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G92 E0"));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G92 E%s"), ftostr53(current_position[E_AXIS]));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G1 F1200 X%s"), ftostr33(current_position[X_AXIS]));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G1 Y%s"), ftostr33(current_position[Y_AXIS]));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("G1 Z%s"), ftostr33(current_position[Z_AXIS]));
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("M23 %s"), card.longFilename);  //opens a file for reading from the SD card
				for(c = &cmd[4]; *c; c++)
					*c = tolower(*c);
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("M26 S%lu"), pos); //check if it works
				card.write_command(cmd);

				sprintf_P(cmd, PSTR("M24"));
				card.write_command(cmd);
				card.closefile(); //sets saving = false and closes the file.
				stepper.synchronize();
				clear_command_queue();
				quickstop_stepper();
				print_job_timer.stop();
				thermalManager.disable_all_heaters();
				#if FAN_COUNT > 0
				  for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
				#endif
				
				wait_for_heatup = false;
				
				lcd_setstatusPGM(PSTR("PROGRESS SAVED"), -1);
				card.stopSDPrint();
				
				enqueue_and_echo_commands_P(PSTR("G28"));
			}
		
	#endif
	
#endif