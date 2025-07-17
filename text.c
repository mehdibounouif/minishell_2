int global(int state)
{
	static int value;
	if(state != -1)
		value = state;
	return value;
}
/*
t_mini *get_global_pointer(t_mini *pointer)
{
	static t_mini *returned_ptr;
	if(pointer != NULL)
		returned_ptr = pointer;
	return returned_ptr;
}

int value = 0;
global(0);
globl(-1);
*/
