#include "arguments.h"
#include <stdio.h>

void print_arguments_struct(struct arguments arguments)
{
    printf("struct arguments"
           "\n{"
           "\n\targs = %d"
           "\n\targ v= char**"
           "\n\tprogram_name = %s"
           "\n}"
           "\n",
           arguments.argc,
           arguments.program_name);
}
