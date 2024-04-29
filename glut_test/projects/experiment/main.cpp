#include <device.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static bool  running = false;
static int   time = 0;
static FILE* file;
static int   mode = 0;

const double dt   = 0.001;
const double umax = 10.0;
const double zmax = 0.3;
const double pi = 3.141592;
//const double angleOffset = pi;  ///< 振り上げ制御の場合

double m = 0.1270;  //< 振子の質量
double M = 0.6320;  //< 台車の質量 PC1
//double M = 0.5200;  //< 台車の質量 PC2
double l = 0.1700;  //< 振子の長さ
double J = 0.0011;  //< 振子の慣性モーメント
double F = 3.65;   //< 台車の粘性抵抗 PC1
//double F = 3.53;   //< 台車の粘性抵抗 PC2
double c = 0.00043;  //< 振子の粘性抵抗
double g = 9.8;

double x[4] = { 0.0, 0.0, 0.0, 0.0 };
double u = 0.0;
double t = 0.0;
double zref = 0.0;
double angleOffset = 0.0;   ///< 安定化制御の場合 0,  振り上げ制御の場合 pi

double control(double z, double theta, double dz, double dtheta, double t);

void TimerCallback(){
	if (!running || !file){
		setCartForce(0.0);
		return;
	}
		
	double z = cartPos();
	x[0] = z;
	x[1] = poleAngle() + angleOffset;
	x[2] = cartVel();
	x[3] = poleAngvel();

	u = control(x[0] - zref, x[1], x[2], x[3], t);

	// input limit
	if (u >  umax) u =  umax;
	if (u < -umax) u = -umax;

	// cart position limit
	if (z > zmax || z < -zmax){
		printf("cart position exceeded limit!\n");
		setCartForce(0.0);
		running = false;
		return;
	}

	//u = 0.0;
	setCartForce(u);

	//fprintf(stdout, "%f, %f, %f, %f, %f, %f\n", t, x[0], x[1], x[2], x[3], u);
	fprintf(file  , "%f, %f, %f, %f, %f, %f, %f\n", t, x[0], x[1], x[2], x[3], u, zref);

	//printf("%d\n", getUs());

	updateBuffer();

	t += dt;
	time++;
}

int main(int argc, char** argv){
	// デバイスオープン
	deviceOpen();

	// モード選択
	int mode = 0;
	while (true){
		printf("[1]balance, [2]swing up: ");
		fflush(stdin);
		mode = getchar() - '0';
		if (mode == 1 || mode == 2)
			break;
	}

	file = fopen( "log.csv", "w");
	fprintf(file, "t, x0, x1, x2, x3, u\n");

	printf("bring the pendulum to %s posture and press enter when ready: ", mode == 1 ? "UPRIGHT" : "REST");

	fflush(stdin);
	getchar();

	// reset encoder counts
	encReset(0);
	encReset(1);
	angleOffset = (mode == 1 ? 0.0 : pi);

	running = true;

	while (true){
		fflush(stdin);
		printf("press enter to stop:");
		int c = getchar();
		if (c == 'a')
			zref = -0.1;
		else if (c == 's')
			zref =  0.0;
		else if (c == 'd')
			zref =  0.1;
		else break;
	}
	
	// ファイルクローズ
	fclose(file);

	// デバイスクローズ
	deviceClose();
}
