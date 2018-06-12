#ifndef POWER_PANIC_H
#define POWER_PANIC_H

	#if ENABLED(POWER_PANIC)
		#define POWER_PANIC_PIN 63 //Also A9 on AUX2

		volatile bool powerPanicActive = false;
		
		void pciSetup(byte pin) //Initialising pin change interrupt
		{
			*digitalPinToPCMSK(pin) |= bit (digitalPinToPCMSKbit(pin)); // enable pin
			PCIFR |= bit (digitalPinToPCICRbit(pin)); // clear any outstanding interrupt
			PCICR |= bit (digitalPinToPCICRbit(pin)); // enable interrupt for the group
		}

		void setup_PowerPanic() {
			pinMode(POWER_PANIC_PIN, INPUT);
			digitalWrite(POWER_PANIC_PIN, HIGH);
			pciSetup(POWER_PANIC_PIN);
		}
		
		

		// save recovery file
		#if ENABLED(SDSUPPORT)
			#include "save_progress_util.h"
			
			void panicSaveRecoveryFile(CardReader card, Temperature thermalManager, float *current_position) {
				if (!card.sdprinting || powerPanicActive) return;
				
				powerPanicActive = true;
				//dissable interrupt so that it doesnt get called twice
				card.pauseSDPrint(); //Pauses the print by setting sdprinting = false
				
				
				int bedTarget = thermalManager.degTargetBed();
				int hotendTarget = thermalManager.degTargetHotend(0);
				thermalManager.disable_all_heaters();
				clear_command_queue(); //cleared ques commands
				  quickstop_stepper(); //clears stepper buffer
				  disable_X();
				  disable_Y();
				  disable_e_steppers();  //dissables steper motor axis'
				  //dissable Z stepper if its not doing anything
				  
				print_job_timer.stop();
				#if FAN_COUNT > 0
				  for (uint8_t i = 0; i < FAN_COUNT; i++) fanSpeeds[i] = 0;
				#endif
				wait_for_heatup = false;
				
				  char cmd[30];
				  char* c;
				  card.pauseSDPrint(); //Pauses the print by setting sdprinting = false
				  uint32_t pos = card.getFilePos(); //gets sd pos, the current position in SD card
				  card.closefile(); //also sets saving = false

				  card.openFile("RESR.GCO",false);  //open a file to write, also sets saving = true

				  sprintf_P(cmd, PSTR("M117 Restarting %s"), card.longFilename); //check if it works
				  card.write_command(cmd);

				  sprintf_P(cmd, PSTR("M109 S150")); //heats nozzle a little so that print doesnt not come off while homing
				  card.write_command(cmd);

				  sprintf_P(cmd, PSTR("G28"));
				  card.write_command(cmd);

				  sprintf_P(cmd, PSTR("G90"));
				  card.write_command(cmd);

				  sprintf_P(cmd, PSTR("M190 S%s"), i8tostr3(bedTarget));
				  card.write_command(cmd);

				  sprintf_P(cmd, PSTR("M109 S%s"), i8tostr3(hotendTarget));
				  card.write_command(cmd);

				  sprintf_P(cmd, PSTR("G1 Z%s"), ftostr33((current_position[Z_AXIS] + 5)));
				  card.write_command(cmd);

				  card.write_command("G92 E0");

				  card.write_command("G1 F200 E5");
				  card.write_command("G92 E0");
				  sprintf_P(cmd, PSTR("G92 E%s"), ftostr53(current_position[E_AXIS]));
				  card.write_command(cmd);

				  //Wipe??

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

				  sprintf_P(cmd, PSTR("M26 S%lu"), pos);
				  card.write_command(cmd);

				  sprintf_P(cmd, PSTR("M24"));
				  card.write_command(cmd);
				  card.closefile(); //sets saving = false and closes the file.

				SERIAL_ECHOLNPGM("File Saved!!");
				card.stopSDPrint();
				enqueue_and_echo_commands_P(PSTR("G28 Z0"));
			}
		#endif
		
		
		// Interrupt
		ISR(PCINT2_vect) {
			if (digitalRead(POWER_PANIC_PIN) == LOW)
				SERIAL_ECHOLNPGM("Power Outage Detected!!");
		}
		

	#endif
	
#endif // POWER_PANIC_H
