#include <device.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool  logging = false;
static int   time = 0;
static FILE* file;
static int   mode = 0;

void TimerCallback(){
	if(!logging)
		return;
	if(!file)
		return;

	fprintf(file  , "%d, %d, %d\n", time, cartEnc(), poleEnc());
	fprintf(stdout, "%d, %d, %d\n", time, cartEnc(), poleEnc());

	time++;
}

int main(int argc, char** argv){
	// デバイスオープン
	deviceOpen();

	// モード選択
	int mode = 0;
	while(true){
		printf("[1]measure cart encoder, [2]measure pendulum encoder: ");
		fflush(stdin);
		mode = getchar() - '0';
		if(mode == 1 || mode == 2)
			break;
	}

	file = fopen( (mode == 1 ? "log_cart.csv" : "log_pole.csv"), "w");

	// モータに一定電圧を加えて台車エンコーダを記録
	if(mode == 1){
		printf("input voltage?: ");
		float v;
		scanf("%f", &v);
		printf("press enter when ready: ");
		fflush(stdin);
		getchar();

		cartOut(v);
		logging = true;

		fflush(stdin);
		printf("press enter to stop:");
		getchar();

		cartOut(0.0f);
		logging = false;
	
	}
	// 振り子エンコーダを記録
	if(mode == 2){
		printf("press enter when ready: ");
		fflush(stdin);
		getchar();

		logging = true;

		printf("press enter to stop:");
		fflush(stdin);
		getchar();

		logging = false;

	}

	/*
	// アナログ電圧出力テスト
	char line[256];
	while(true){
		printf("output voltage[V] or [q]uit: ");
		scanf("%s", line);
		if(strcmp(line, "q") == 0)
			break;
		float v = atof(line);
		daOut(v, 0.0f);
	}

	// カウンタテスト
	while(true){
		int cnt0 = encIn(0);
		int cnt1 = encIn(1);
		printf("counter: %d %d\n", cnt0, cnt1);
		//scanf("%s", line);
		//if(strcmp(line, "q") == 0)
		//	break;
	}
	*/

	// ファイルクローズ
	fclose(file);

	// デバイスクローズ
	deviceClose();
}
