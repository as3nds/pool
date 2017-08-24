#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

const char* get_process_name_by_pid (const int pid);

int
main (int argc, char *argv[])
{
	int pid, ppid;
	char *pname, *ppname;
	char *psh;

	pid = getpid();
	ppid = getppid();

	pname = get_process_name_by_pid (pid);
	ppname = get_process_name_by_pid (ppid);

	psh = strstr (ppname, "sh");
	if (!psh) { /* anti-debugger protection */
		printf ("The dark fire will not avail you, Flame of Udun!\n" \
				"Go back to the shadow. YOU SHALL NOT PASS!\n");
		exit(1);
	}

	if (pname) {
		printf ("pid:%d, pname:%s\n", pid, pname);
	}
	if (ppname) {
		printf ("ppid:%d, ppname:%s\n", ppid, ppname);
	}
	
	

	return 0;
}


const char* 
get_process_name_by_pid (const int pid) 
{
	char *name = (char *) calloc (1024, sizeof(char));
	if (name) {
		sprintf (name, "/proc/%d/cmdline", pid);
		FILE *fd = fopen (name, "r");
		if (fd) {
			size_t size;
			size = fread (name, sizeof(char), 1024, fd);
			if (size >0) {
				name[size - 1] = '\0';
			}
			fclose (fd);
		}
	}
	return name;
}
