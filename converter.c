// txt to bf converter, written by jep

#include <stdio.h>

unsigned int FILE_LENGHT = 256;
int BF_FACTOR = 0;
int MAX_LENGHT = 10;
int POS = 0;
int MAX_POS = 16; // must match with the interpreter max size
void O0(FILE *, FILE *);
void O1(FILE *, FILE *);
void O2(FILE *, FILE *);

// print functions
void plus(FILE *out) { fputc('+', out); }
void minus(FILE *out) { fputc('-', out); }
void right(FILE *out) {
  fputc('>', out);
  POS++;
}
void left(FILE *out) {
  fputc('<', out);
  POS--;
}
void open(FILE *out) { fputc('[', out); }
void close(FILE *out) { fputc(']', out); }
void dot(FILE *out) { fputc('.', out); }

int main(int argc, char *argv[]) {

  FILE *src; // file pointer

  if (argc == 2) {
    // BF_FACTOR is the default value

    // this monstrosity handles the BF_FACTOR from argument
    // it is written poorly? yes, it works? yes (hopefully)
  } else if (argc == 3) {
    if (argv[2][0] == '-' && argv[2][1] == 'O') {
      if (argv[2][2] == '0') {
        BF_FACTOR = 0;
      } else if (argv[2][2] == '1') {
        BF_FACTOR = 1;
      } else if (argv[2][2] == '2') {
        BF_FACTOR = 2;
      } else if (argv[2][2] == '3') {
        BF_FACTOR = 3;
      } else {
        puts("Invalid BF factor, should be between 0 and 3\nexiting...");
        return 0;
      }
    } else {
      puts("Invalid argument, try again:)");
      return 0;
    }
  } else {
    puts("Pass the file name as argument\nexiting...");
    return 0;
  }

  src = fopen(argv[1], "r");
  if (src == NULL) {
    puts("Error opening the file\nexiting...");
    return 0;
  }

  // out file name - in file with .bf extension in current dir
  char out_name[FILE_LENGHT - 1];
  int i;
  int j = 0;
  for (i = 0; argv[1][i] != '\0'; i++) {
    if (argv[1][i] == '/')
      j = i;
  }

  if (j != 0)
    j++;

  i = 0;
  for (; argv[1][j] != '.'; j++) {
    out_name[i] = argv[1][j];
    i++;
  }
  out_name[i] = '.';
  out_name[i + 1] = 'b';
  out_name[i + 2] = 'f';
  out_name[i + 3] = '\0';

  FILE *out;
  out = fopen(out_name, "w");

  switch (BF_FACTOR) {
  case 0:
    O0(src, out);
    break;
  case 1:
    O1(src, out);
    break;
  case 2:
    O2(src, out);
    break;
  }

  fclose(src);
  fclose(out);

  return 0;
}

// really long file, easy to read
void O0(FILE *src, FILE *out) {
  char c; // why not call it c
  while (1) {
    c = fgetc(src);
    if (c == EOF)
      break;

    for (int i = 0; i < c; i++) {
      plus(out);
    }
    dot(out);
    open(out);
    minus(out);
    close(out);
  }
}

void O1(FILE *src, FILE *out) {
  char c = 0;
  char last_c;
  char delta;
  while (1) {
    last_c = c;
    c = fgetc(src);
    if (c == EOF)
      break;
    delta = c - last_c;

    if (delta > 0) {
      for (int i = 0; i < delta; i++)
        plus(out);
    } else if (delta < 0) {
      for (int i = 0; i > delta; i--)
        minus(out);
    }
    dot(out);
  }
}

unsigned int get_prime(int *prime, int number) {
  unsigned int len = 0;
  // clear the array
  for (int i = 0; prime[i] != 0; i++)
    prime[i] = 0;

  // prime factors of the number
  for (int i = 2; i <= number; i++) {
    if (number % i == 0) {
      number /= i;
      prime[len] = i;
      len++;
      i -= 1;
    }
  }

  return len;
}

void loop(FILE *out, int const *prime, int n) {
  if (prime[n] == 0)
    return;

  open(out);
  right(out);
  for (int i = 0; i < prime[n]; i++) {
    plus(out);
  }
  loop(out, prime, n + 1);
  left(out);
  minus(out);
  close(out);
}
int MCD(int last_c, int delta) {
  delta = delta * (delta > 0) - delta * (delta < 0);
  for (int i = delta; i > 1; i--) {
    if (last_c % i == 0 && delta % i == 0)
      return i;
  }
  return 1;
}

void print_prime(int const *prime, FILE *out) {
  int n;

  n = prime[0] * (prime[0] > 0) - prime[0] * (prime[0] < 0); // brachless

  for (int i = 0; i < n; i++) {
    if (prime[0] > 0) {
      plus(out);
    } else {
      minus(out);
    };

    loop(out, prime, 1);
  }
}

// 0 is last_c, 1 is delta
void fix(int *prime, int *edit) {
  int mcd = MCD(edit[0], edit[1]);
  if (mcd == 1) {
    if (get_prime(prime, edit[0]) > get_prime(prime, edit[1])) {
      edit[1] += (edit[1] > 0) - (edit[1] < 0);
      fix(prime, edit);
    } else {
      edit[0]++;
      fix(prime, edit);
    }
  }
}
void reset_position(FILE *out) {
  while (POS > 0) {
    open(out);
    minus(out);
    close(out);
    left(out);
    plus(out);
  }
}

void O2(FILE *src, FILE *out) {
  char last_c;
  char delta;
  int mcd;
  int last_c_molt, c_molt;
  int edit[2] = {0};

  int prime[16] = {0};
  unsigned len;

  // first char
  char c = fgetc(src);
  len = get_prime(prime, c);
  for (int i = 0; i < prime[0]; i++)
    plus(out);
  loop(out, prime, 1);
  for (int i = 1; i < len; i++) {
    right(out);
  }
  dot(out);

  while (1) {
    last_c = c;
    c = fgetc(src);
    if (c == EOF)
      break;

    if (POS > MAX_POS) {
      reset_position(out);
      minus(out); // to compensate for the last +
      len = get_prime(prime, c);
      for (int i = 0; i < prime[0]; i++)
        plus(out);
      loop(out, prime, 1);
      for (int i = 1; i < len; i++) {
        right(out);
      }
      dot(out);
      continue;
    }
    delta = c - last_c;

    if (delta * delta <= MAX_LENGHT * MAX_LENGHT) { // yeah, I'm lazy
      for (int i = 0; i < delta * delta; i += delta * ((delta > 0) * 2 - 1)) {
        fputc('-' * (delta < 0) + '+' * (delta > 0), out); // sorry:)
      }
      dot(out);

    } else {
      edit[0] = last_c;
      edit[1] = c;

      fix(prime, edit);

      mcd = MCD(edit[0], edit[1]);
      last_c_molt = edit[0] / mcd;
      c_molt = edit[1] / mcd;
      // fix last_c
      for (int i = 0; i < edit[0] - last_c; i++)
        plus(out);
      // start loop
      open(out);
      right(out);
      POS++;
      // builds c
      for (int i = 0; i < c_molt; i++)
        plus(out);
      left(out);
      POS--;
      // builds last_c
      for (int i = 0; i < last_c_molt; i++)
        minus(out);
      close(out);
      // for printing
      right(out);
      // fix for c
      for (int i = 0; i < edit[1] - c; i++)
        minus(out);
      // print
      dot(out);
    }
  }
}
