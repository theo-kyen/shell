#include <stdio.h>
#include <stdlib.h>
#include "cmd.h"

int main(int argc, char **argv)
{
	simple_cmd_t *cmd = simple_cmd_init();
	set_simple_cmd(cmd, "ls");
	insert_arg(cmd, "-al");
	simple_execute(cmd);
	simple_cmd_free(cmd);
	return 0;
}
