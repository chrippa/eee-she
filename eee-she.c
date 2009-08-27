#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/utsname.h>
#include <string.h>

#define CPUFV_PATH1 "/sys/devices/platform/eeepc/cpufv"
#define CPUFV_PATH2 "/sys/devices/platform/eeepc/she"

#define STREQ(s1, s2) (strcmp(s1, s2) == 0)

#define POWERSAVE   2
#define NORMAL      1
#define PERFORMANCE 0

static char g_cpufv_path[256];

static FILE *
fopen2 (char *filename, char *mode)
{
	int mask;

	mask = R_OK;

	if (mode[0] == 'w')
		mask += W_OK;

	if (access(filename, mask) < 0) {
		fprintf(stderr, "%s: Permission denied (%s)\n", filename, mode);

		exit(EXIT_FAILURE);
	}

	return fopen(filename, mode);
}

static void
cpufv_print (int mode)
{
	char *str;

	switch (mode) {
		case PERFORMANCE:
			str = "Performance";
			break;

		case NORMAL:
			str = "Normal";
			break;

		case POWERSAVE:
			str = "Power Save";
			break;

		default:
			str = "Unknown";
			break;
	}

	printf("%s\n", str);
}

static void
cpufv_set (int mode)
{
	FILE *f;
	char buf[10];

	snprintf(buf, sizeof(buf), "%d", mode);

	f = fopen2(g_cpufv_path, "w");
	fputs(buf, f);
	fflush(f);
	fclose(f);
}

static int
cpufv_get (void)
{
	FILE *f;
	int mode;

	f = fopen2(g_cpufv_path, "r");
	fscanf(f, "%d", &mode);
	fclose(f);

	return mode;
}

static int
cpufv_toggle (void)
{
	int mode, next;

	mode = cpufv_get();
	next = (mode % 3) + 1;
	if (next > 2)
		next = 0;

	return next;
}

static void
show_help (void)
{
	printf(
		"Usage: eee-she <OPTION>\n"
		"Controls Asus eee Super Hybrid Engine\n\n"

		"-h,  --help            This help message\n"
		"-p,  --performance     Set SHE to Performance mode\n"
		"-n,  --normal          Set SHE to Normal mode\n"
		"-ps, --powersave       Set SHE to Power Save mode\n"
		"-t,  --toggle          Toggles through all modes\n"
	);

	exit(EXIT_SUCCESS);
}


int
main (int argc, char **argv)
{
	char *cmd = argv[1];
	int mode;

	/* Check that file exists */
	if (access(CPUFV_PATH1, F_OK) == 0)
		strcpy(g_cpufv_path, CPUFV_PATH1);
	else if (access(CPUFV_PATH2, F_OK) == 0)
		strcpy(g_cpufv_path, CPUFV_PATH2);
	else {
		fprintf(stderr, "Unable open file, make sure eeepc_laptop module is loaded\n");

		return EXIT_FAILURE;
	}

	/* Handle commands */
	if (argc < 2)
		show_help();

	if (STREQ(cmd, "-p") || STREQ(cmd, "--performance"))
		mode = PERFORMANCE;
	else if (STREQ(cmd, "-n") || STREQ(cmd, "--normal"))
		mode = NORMAL;
	else if (STREQ(cmd, "-ps") || STREQ(cmd, "--powersave"))
		mode = POWERSAVE;
	else if (STREQ(cmd, "-t") || STREQ(cmd, "--toggle"))
		mode = cpufv_toggle();
	else
		show_help();

	cpufv_set(mode);
	cpufv_print(mode);

	return EXIT_SUCCESS;
}
