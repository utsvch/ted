#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <stdlib.h>

/** defines **/
#define CTRL_KEY(k) ((k) & 0x1f)

struct termios orig_termios; // Original Terminal attributes

void die(const char* s) {
	perror(s);
	exit(1);
}

void disableRawMode() {
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1 )
		die("tcsetattr");
}

void enableRawMode() {
	
	// Get the current attributes of terminal
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1) die("tcgetattr");
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

	/**
	 * VMIN and VTIME are indexes into c_cc field which stands for "control characters", an array of bytes that
	 * control various terminal settings.
	 * VMIN - sets the minimum num of bytes of input needed before read() can return.
	 * VTIME - sets the maximum amount of time to wait before read() returns. It is in tenths of a second, 
	 * 	   so we set it to 1/10 of a second or 100 milliseconds.
	 * */
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;

	// Set the modified attributes of terminal
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) die("tcsetattr");
}

char editorReadKey() {
	int nread;
	char c;
	
	while ((nread = read(STDIN_FILENO, &c, 1)) != 1) {
		if (nread == -1 && errno != EAGAIN) die("read");
	}
	return c;
}

// input
void editorProcessKeypress() {
	char c = editorReadKey();

	switch(c) {
		case CTRL_KEY('q'):
			exit(0);
			break;
	}
}

// output
void editorRefreshScreen() {
	write(STDOUT_FILENO, "\x1b[2J", 4);
}

int main() {
	enableRawMode();

	/**
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q') {
		if (iscntrl(c)) { // check if c is a control char
			printf("%d\r\n", c);
		} else {
			printf("%d ('%c')\r\n", c, c);
		}
	}
	*/

	while (1) {
		editorRefreshScreen();
		editorProcessKeypress();
	}

	return 0;
}
