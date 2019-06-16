#include "CLogReader.h"

int main(int argc, char* argv[])
{
	char		buf[4096];

	int			i = 0;

	if (argc < 3)
	{
		fprintf(stderr, "Usage:\n ...CLogReader.exe 'log_file' 'filter'\n");
		return 1;
	}


	CLogReader	example;

	if (!example.Open(argv[1]))
	{
		fprintf(stderr, "Failed to open file '%s'\n", argv[1]);
		return 1;
	}

	if (!example.SetFilter(argv[2]))
	{
		fprintf(stderr, "Failed to set filter '%s'\n", argv[2]);
		return 1;
	}

	while (example.GetNextLine(buf, sizeof(buf)))
	{
		fprintf(stdout, "string %d : '%s'\n", i++, buf);
	}

	example.Close();

	return 0;
}


