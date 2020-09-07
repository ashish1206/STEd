#include<stdlib.h>
#include<string.h>
#define BUFFER_INIT {NULL, 0}
struct buffer{
	char *str;
	int len;
};

void bufAppend(struct buffer *b, char *s, int len){
	char *new_buf = (char *)realloc((void *)b->str, b->len + len);
	if(new_buf == NULL)return;
	memcpy(&new_buf[b->len], s, len);
	b->str = new_buf;
	b->len += len;
}

void bufFree(struct buffer *b){
	free(b->str);
}