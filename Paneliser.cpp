#include "Process.h"

extern "C" int main(int ac, char** av)
{
	CProcess P;
	return P.Process(ac, av);
}
