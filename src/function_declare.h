/**terminal terminal.cpp die.cpp**/
void disableRawMode();
void enableRawMode();
void die(const char*);
int getWindowSize(int&, int&);

/**input key_input.cpp**/
void editorProcessKeyPress();
int editorReadKey();

/**output output_screen.cpp**/
void editorRefreshScreen();
void clearTerminal();
void editorScroll();
void editorSetStatusMessage(const char *, ...);

/**buffer buffer.h**/
void bufAppend(struct buffer *, char *, int );
void bufFree(struct buffer *);

/**file I/O file_io.cpp**/
void editorOpen(char *);
void editorUpdateRow(struct edtRow *);
void editorInsertRow(int, char *, int);
void editorSaveFile();

/**editor operations edit_text.cpp**/
void editorInsertChar(char);
void editorDelChar();
void editorInsertNewLine();

/**editor prompt editor_prompt.cpp**/
char * editorPrompt(char *);

/**editor search editor_search.cpp**/
void editorSearch();