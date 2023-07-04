#ifndef USER_IO_H
#define USER_IO_H

int input_integer(int range_min, int range_max, const char* input_message);
char *input_string(int max_len, const char* input_message);
double input_double(const char* input_message);

#endif
