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
	raw.c_lflag &= ~(ECHO); // ECHO causes typed keys to be printed in terminal. Turn off
	

	// Set the modified attributes of terminal
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main() {
	enableRawMode();

	char c;
	while (read(STDIN_FILENO, &c, 1) == 1 && c != 'q');
	return 0;
}
