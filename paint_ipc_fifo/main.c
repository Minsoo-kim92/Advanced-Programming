#include "header.h"

#define N_THREADS 2
static pthread_t thid[N_THREADS];

//externs//
char strAnswerCorrect[100] = "nn45nn3c8n";
bool bGameOver = false;
bool isDrawer = false;

extern int main(int argc, char* argv[]){
	if (argc == 1){
		printf(
				"not enough argument. \n"
				"usage: \n"
				"[%s -w] for writer \n"
				"[%s -r] for reader \n",
			  argv[0], argv[0]
		);
		return 1;
	}

	if (strcmp(argv[1],"-w") == 0) {
		isDrawer = true;
		printf("starting as a writer. \n");
	} else if (strcmp(argv[1], "-r") == 0) {
		isDrawer = false;
		printf("starting as a reader. \n");
	} else {
		printf("invalid argument \n");
		return 1;
	}

	printf("createing threads.. \n");

	if(isDrawer){	
		InitDisplay();
		TypoSetInputAnswer();
		pthread_create(&thid[0], NULL, Thread1, NULL);  // user_input
		pthread_create(&thid[1], NULL, Thread2Writer, NULL);  // ipc
		printf("hello there \n");
		TypoInputLoopWriter();
	} else{
		InitDisplay();
		pthread_create(&thid[0], NULL, Thread1, NULL);  // user_input
		pthread_create(&thid[1], NULL, Thread2Reader, NULL);  // ipc
		printf("hello there \n");
		TypoInputLoopReader();
	}
	
/*
	for (i = 0; i < N_THREADS; ++i) {
		if (pthread_join(thid[i], NULL) != 0) {
			printf("end of process (-1) \n");
			return -1;
		}
	}
*/
	printf("end of process (0) \n");
	return 0;
}