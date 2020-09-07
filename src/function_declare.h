/**terminal terminal.cpp die.cpp**/
void disableRawMode();
void enableRawMode();
void die(const char*);
int getWindowSize(int&, int&);

/**input key_input.cpp**/
void editorProcessKeyPress();
char editorReadKey();

/**output output_screen.cpp**/
void editorRefreshScreen();
void clearTerminal();

/**buffer buffer.cpp**/
void bufAppend(struct buffer *, char *, int );
void bufFree(struct buffer *);