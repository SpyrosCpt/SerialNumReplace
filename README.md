# SerialNumReplace
Replace the serial number in a hex file

## Concept 
A unique serial number is needed for a product. The microcontroller being used does not have a unique ID and there is no extra space to add a dedicated unique ID IC. 
We could add a random number generator, but there is still a good chance that two devices will have the same serial and no random generator is truly random. 
We could also manually keep track of the serial numbers and compile each time before uploading the firmware. This is okay if we have 10 devices, however this is not suitable for large scale production.

The solution is simple, have an app that:
1. Keeps track of the current serial number 
2. Opens the generated hex file
3. Replaces the serial number in the hex file
4. Calculates a new checksum for the line where the serial was replaced
5. Increments the serial number

But how do we know where the serial is in the hex file? 
Unfortunately this is a manual process the first time. 
You first create a #define SERIAL_NUM and set it to 0xFFF compile and save the hex file,
then set the serial number so something else, I usually use 0xABCD, once again compile and save the file.
Now set the file names in "openFile.c" and run. "openFile.c" compares the hex files and returns a position in 
the file where there is a difference. 
Now open "ChangeHex.c" and change the #define SERIAL_POSITION to your position returned from "openFile.c".
Compile "ChangeHex.c" and set the serial number in "Serials.txt" to the first number you want to start at.
All you need to do now is run the ChangeHex application and upload the file to your MCU. You could automate this further by making a batch file that runs ChangeHex and then programs the MCU via the command line interface.

