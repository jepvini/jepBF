// bf interpreter written in C with questionables design choices by jep

#include <stdio.h>
#include <stdlib.h>

unsigned int SIZE = 16;
char ARR[15] = {0}; // main array
char *p = ARR;      // array pointer

// debug section
int DEBUG = 0; // debug var
int DEBUG_PRINT_SIZE = 16;

void evaluate(FILE *, char); // don't delete this
FILE *loop(FILE *);

int main(int argc, char *argv[]) {

  FILE *src; // file pointer

  if (argc == 2) {
    // Running in normal mode
  } else if (argc == 3) {
    puts("running in debug mode");
    DEBUG = 1;
  } else {
    puts("Pass the file name as argument\nexiting...");
    exit(0);
  }

  src = fopen(argv[1], "r");
  if (src == NULL) {
    puts("Error opening the file\nexiting...");
    exit(0);
  }

  char c; // why not call it c
  while (1) {
    c = fgetc(src);
    if (c == EOF)
      break;
    evaluate(src, c); // src is needed for the loop part
  }

  // puts("\n");

  fclose(src);

  return 0;
}

void evaluate(FILE *src, char c) {
  switch (c) {
  case '>':
    if (p - ARR > SIZE) {
      printf("\npos %ld\n", p - ARR);
      puts("Position of pointer out of range");
      exit(0);
    };
    p += 1;
    break;
  case '<':
    if (p - ARR < 0) {
      puts("Position of pointer below zero");
      exit(0);
    };
    p -= 1;
    break;
  case '+':
    *p += 1;
    break;
  case '-':
    *p -= 1;
    break;
  case '.':
    putchar(*p);
    if (DEBUG == 1) {
      putchar(' ');
      printf("%i\n", *p);
      getchar();
    }
    break;
  case ',':
    *p = getchar();
    break;
  case '[':
    loop(src); // another function is needed to make it recursive
    break;
  default:
    break; // all other characters are ignored
  }

  if (DEBUG == 1) {
    printf("int %i, in %c, pos %ld\n", *p, c, ftell(src));
    printf("\npos %ld\n", p - ARR);
    for (int i = 0; i < DEBUG_PRINT_SIZE; i++) {
      printf("%+3d ", ARR[i]);
    }
    putchar('\n');
    for (int i = 0; i < p - ARR; i++) {
      putchar(' ');
      putchar(' ');
      putchar(' ');
      putchar(' ');
      // I don't want new lines
    }
    putchar(' ');
    putchar('^');
    getchar();
  }
}

FILE *loop(FILE *src) {
  char c;
  long pos = ftell(src); // get [ position
  while (1) {
    c = fgetc(src);
    if (c == ']') {
      if (*p != 0) {
        fseek(src, pos, SEEK_SET); // goto [ position
      } else {
        break;
      }
    }
    evaluate(src, c);
  }

  return src;
}
