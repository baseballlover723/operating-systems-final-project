Morgan Cook, Josh Green and Philip Ross: Team 1-F

Instructions for building our operating system:
  -First, navigate to the folder m1. Then run ./compileOS.sh

Instructions for testing and using our operating system:
  -As of now, our OS simply prints hello world.  To use this feature, navigate to the folder m1 then run ./compileOS.sh

Any bugs or deficiencies:
  -Our code does not currently have any bugs or deficiencies.

Additional features:
  -We have not yet added any extra features.

### MILESTONE 3

What we did: In this milestone, we implemented the file system for out operating system, created
3 new services for interrupt 21 (readFile, executeProgram, and terminate), and added a shell with
two implemented commands.

How to verify: To run our operating system, type "./compileOS.sh" into the terminal. This will 
compile all necessary files, construct the floppya.img, load certain files into the img, and run
bochs. On execution, the OS will print out "SHELL>" and wait for input. recognized commands are
"type [filename]" and "execute [filename]" To verify the readFile interrupt, enter "type messag"
into the shell. This will read the message.txt file and output its contents. To verify the 
executeCommand interrupt, enter "execute tstpr2" into the shell. This will execute the tstpr2 file,
and you will see a message displayed to the console. To verify the shell, simply type any incorrect
command, and the shell will handle that appropriately.

Lastly, to clean the files generated by compileOS.sh, just execute "./cleanOS.sh" in the terminal.


### MILESTONE 4

What we did: In this milestone, we implemented the ability to write to a sector, delete a file, write a file, copy a file, list all of the files in our system and create a text file from the shell.

How to verify: To run our operating system, type "./compileOS.sh" into the terminal.  This will compile all necessary files, construct the floppya.img, load certain files into the img, and run bochs.  On execution, the OS will print out "SHELL>" and wait for input.  Our system now recognizes all of the commands from previous milestone as well as the following commands:

	delete FILENAME (where FILENAME is the name of the file you want to delete)
	copy FILENAME1 FILENAME2 (where FILENAME1 is the name of the file you want to copy and FILENAME2 is the name of the file you want to write the copied data to)
	dir (this command takes no arguments and prints out the names of all of the files in the system)
	create FILENAME (where FILENAME is the name of the file you wish to create.  This command will prompt you to enter the data line by line that you want to write to your new file.)

Known bugs and deficiencies: Our system currently has no known bugs or deficiencies.

Additional features: When we print out the names of all of the files in our system, we also print out the size of the file.  Also, if you try to run a command on a file that is not in the system, the shell will print out "FNF" which stands for "file not found."