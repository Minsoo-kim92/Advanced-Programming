#include "header.h"


extern void TypoSetInputAnswer() {
	printf("tell me what your answer is: ");
	tcflush(0, TCIFLUSH);
	scanf("%s", strAnswerCorrect);
	getchar();
	printf("the answer is %s\n", strAnswerCorrect);
	tcflush(0, TCIFLUSH);
	SndAnswerCorrect(strAnswerCorrect);
}


extern void TypoInputLoopWriter() {
	int i;
	for (;!bGameOver;sleep(3));
	for (i = 0;i < 5;++i) {
		printf("%s, your mate got that one right\n", strAnswerCorrect);
	}
}

extern void TypoInputLoopReader() {
	int i;
	char tmp_strAnswer[100];
	for (;;tcflush(0, TCIFLUSH)) {
		printf("\ntell me what you think the answer is: \n");
		tcflush(0, TCIFLUSH);
		scanf("%s", tmp_strAnswer); // .. ...
		if (strcmp(strAnswerCorrect, tmp_strAnswer) == 0) { // ....
			for (i = 0;i < 5;++i) {
				printf("%s, you got that right\n", tmp_strAnswer);
			}
			SndGameOver();
			return;
		} else { // ....
			printf("your answer is %s, which is incorrect. lol\n", tmp_strAnswer);
		}
	}
}