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
	 *  Turn off ECHO. Causes typed keys to be printed in terminal. 
	 *  Turn off Ctrl-C & Ctrl-Z signals (ISIG)
	 *  Disable Ctrl-S and Ctrl-Q. Ctrl-S stops data transmission to terminal until you press Ctrl-Q.
	 *
	 * */

	// Input Flags
	raw.c_iflag &= ~(
			ICRNL	|
			IXON   
			);
	
	// Output Flags
	raw.c_oflag &= ~(
			OPOST
			);

	// Local Flags
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
