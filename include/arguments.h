#ifndef ARGUMENTS
#define ARGUMENTS

struct arguments
{
    int         argc;
    char      **argv;
    const char *program_name;
};

void print_arguments_struct(struct arguments);

#endif
