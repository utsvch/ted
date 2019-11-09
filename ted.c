#include <ctype.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

struct termios orig_termios; // Original Terminal attributes

void disableRawMode() {
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enableRawMode() {
	
	// Get the current attributes of terminal
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	// Modify the current terminal attributes
	struct termios raw = orig_termios;
	
	/**
	 *  Turn off ECHO. Causes every typed key to be printed in terminal. 
	 *  Turn off ICANON, which disables reading line-by-line and reads input byte-by-byte
	 *  Turn off Ctrl-C & Ctrl-Z signals (ISIG)
	 *  Disable IXON flag - Ctrl-S and Ctrl-Q. Ctrl-S stops data transmission to terminal until you press Ctrl-Q.
	 *  ISTRIP causes the 8th bit of each input byte to be stripped, meaning it will set it to 0.
	 * */

	// Input Flags
	raw.c_iflag &= ~(
			BRKINT	|
			ICRNL	|
			INPCK	|
			ISTRIP	|
			IXON   
			);
	
	// Output Flags
	raw.c_oflag &= ~(
			OPOST
			);
	
	// C Flags
	// CS8 sets the character size(CS) to 8 bits per byte. Usually set in modern terminals.
	raw.c_cflag |= (CS8);

	// Local Flags - a dumping ground for other state
	raw.c_lflag &= ~(
			ECHO	|
			ICANON  |
			IEXTEN	|
			ISIG	
			); 	

	// Set the modified attributes of terminal
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enableRawMode();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if (iscntrl(c)) { // check if c is a control char
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
	}

	return 0;
}
