#include <stdio.h>
#include <stdbool.h>

bool isUpper(int c) 
{
	return c >= 'A' && c <= 'Z';
}

bool isLower(int c)
{
	return c >= 'a' && c <= 'z';
}

bool isWhitespace(int c)
{
	return c == ' ';
}

bool isDot(int c)
{
	return c == '.';
}

int main(int argc, char* argv[]) {
	if (argc != 3)
		return 1;

	FILE* fr = fopen(argv[1], "r");
	if (!fr)
		return 2;

	FILE* fw = fopen(argv[2], "w");
	if (!fw)
		return 2;

	typedef enum {
		SENTENCE,
		DOT,
		SPACE_AFTER_DOT
	} State;

	State state = SPACE_AFTER_DOT;

	int c;
	int last_c = 0;
	size_t counter = 0;
	while ((c = fgetc(fr)) != EOF) {
		switch (state) {
			case SENTENCE:
				if (isUpper(c)) {
					fputc(c - 'A' + 'a', fw); // (1) převedeme na malá písmena
				}
				else if (isLower(c)) {
					fputc(c, fw); // (2) zapíšeme
				}
				else if (isWhitespace(c)) { // (2) ubírání více mezer za sebou
					if(!isWhitespace(last_c)) {
						fputc(' ', fw);
					}
				}
				else if (isDot(c)) {
					fputc(c, fw);
					state = DOT; // (3) konec věty
					counter++; 
				}
				break;
			case DOT:
				if (isWhitespace(c)) {
					fputc(' ', fw); // (4) pošleme na výstup korektní mezeru
					state = SPACE_AFTER_DOT;
				}
				else if (isUpper(c) || isLower(c) || isDot(c)) {
					// Smyčka na uzlu DOT
				}
				break;
			case SPACE_AFTER_DOT:
				if (isWhitespace(c) || isDot(c)) {
					// Smyčka na uzlu SPACE_AFTER_DOT
				}
				else if (isUpper(c)) {
					fputc(c, fw); 
					state = SENTENCE; // (5) začátek věty
				}
				else if (isLower(c)) {
					fputc(c - 'a' + 'A', fw); // (6) převedeme na velké písmeno na začátku věty
					state = SENTENCE;
				}
				break;
			default:
				perror("Unknown value");
				break;	
		}
		last_c = c;
	}

	fclose(fr);
	fclose(fw);

	printf("Pocet vet: %zu\n", counter);

	return 0;
}