Micro Bit - Single Wire Communication Protocol

The Project Overview:

The project is based around sending a morse code message between 2 MicroBits. It creates a message buffer, allowing a user to create the message string before initiating a handshake with another MicroBit. Once a connection is established, the message string is parsed, using a map to retrieve a bit string for each character other than space (this is added seperately). The but string is then parsed and for each 0 or 1 that the bit string character could be a seperate timer wait value is executed. This timer is used to control how lonh Pin 1 on the sending MicroBit is high/low for.
On the receiving MicroBit, Pin 1 is checked for how long it remains in a high/low state. Depending on which case this time relates to i.e. dot,dash,space etc. a string is created which will be translated once it has recieved an end of character notice. This string is decrypted and added to another string containing the full message. Once an end of transmission notice is given, the final translated string is scrolled across the MicroBit LEDs.

requires yotta, built in linux


How to build:

1. Open terminal
2. Clone the gitLab repository
3. Change directory into the cloned folder
4. Set the build target by entering "yt target bbc-microbit-classic-gcc"
5. The build the project by entering "yt build"
6. This will result in new folders being created
7. Plug in the MicroBit
8. To copy the cpp file to the MicroBit, enter "cp build/bbc-microbit-classic-gcc/source/iot-example-combined.hex /media/student/MICROBIT"


How to use:

1. With 2 Batteries connect 1 to each MicroBit
2. Use 2 crocodile clips, connecting 1 to each of the Pin1 connections on the MicroBits and the other to each GND pin
3. On the MicroBit you wish to send the message from use morse code or the provided map in the code to contruct the message; A buttonA press is a dot/0 and long hold is a dash/1. Once all dots and dashes for a character have been enetered, press buttonB, this notifies the MicroBit to expect a new character. To enter a space, end an empty character (don't enter dots/dashes between 2 buttonB presses).
4. Once the message is finished, hold buttonB. If another MicroBit is connected, a LED in the centre of the grid will light up else an edge LED will. This will then start transmission.
5. On the receiving MicroBit, the message will translate and scroll across the LED grid.
6. To send any more messages, press the reset button on each MicroBit
