#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int
main(int argc, char * const argv[], char * const envp[]) {
	int pid;
	int sid;
        FILE *f;
	char buffer[16];
	
	if(argc < 2) {
		fprintf(stderr, "Usage: %s <executable> [OPTS]\n", argv[0]);
		return 1;
	}

	sid = setsid();
	while(1) {
		pid = fork();
		if(pid==0) {
			execve(argv[1], &argv[1], envp);
			exit(0);
		} else if(pid > 0) {
			int wstatus;
			int options;
			int n;
			do {

				f = fopen("/opt/watcher.txt","w+");        
	                        if(!f) {                                                
        	                        printf("Unable to open /opt/watcher.txt\n");
                	                return 1;         
                        	}
				fseek(f, 0, SEEK_SET);

				fprintf(f, "0"); 
 
        	                fflush(f);              
 	                        sleep(7);
				fseek(f, 0, SEEK_SET);
				n = fread(buffer,1,1,f);
				printf("Signs: %d\n", n);
				putchar(buffer[0]);
				putchar('\n');

				fclose(f);

				options = WNOHANG;
				if(buffer[0] != '1') {
					printf("Killing proccess\n");
					kill(pid, SIGTERM);
					options = 0;
				}
			
				
			} while(waitpid(pid, &wstatus, options)==0);

			printf("Proccess has died, waitpid() = %d, wstatus=%d, WIFEXITED: %d\n", n, wstatus, WIFEXITED(wstatus));
		} else {
			perror("fork");
			return 1;
		}
	}

	return 0;
}
			
