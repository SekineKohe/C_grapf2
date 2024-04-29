#include <GL/freeglut.h>

#include <stdio.h>
#include <math.h>

// 定数
const int timer_period = 1;   //< タイマ周期 ミリ秒
const double dt = 0.001;  //< 数値積分の時間刻み 秒

// 物理定数
const double m = 1.0;  //< 振子の質量
const double M = 1.0;  //< 台車の質量
const double l = 1.0;  //< 振子の長さ
const double J = 1.0;  //< 振子の慣性モーメント
const double F = 0.1;  //< 台車の粘性抵抗
const double c = 0.1;  //< 振子の粘性抵抗
const double g = 9.8;

// 変数
double z[4]  = {0.0, -80, 0, 0};   //< 状態変数
double dz[4] = {0.0, 0.0, 0.0, 0.0};   //< 状態変数の微分
double u     = 0.0;                    //< 制御入力 台車の駆動力
double t     = 0.0;                    //< 現在時刻
	double brox[1000];
	double broy[1000];

// ウィンドウサイズ
int window_width ;
int window_height;

// ファイル
FILE* file = NULL;

// タイマコールバック関数
void timer_func(int value){
	// 状態変数から各成分を取得
	double x      = z[0];
	double y      = z[1];
	double dx     = z[2];
	double xx=dx;
	double dy     = z[3];
	double yy=dy;
	//ブロックについて
	brox[0] = 10;
	broy[0] = 0;
	brox[1] = 100;
	broy[1] = 0;
	brox[2] = 100;
	broy[2] = 50;
	brox[3] = -20;
	broy[3] = -60;



	// ここで制御入力を決定
	//double u = 0.0;

	// ここに運動方程式を記述する
	//double A[2][2];
	//double b[2];
	if(x-100>0&&dx>0){z[2]=-0.8*xx;}
	if(x+100<0&&dx<0){z[2]=-0.8*xx;}
	if(y-100>0&&dy>0){z[3]=-0.8*yy;}
	if(y+100<0&&dy<0){z[3]=-0.5*yy;}
	if(y+100<0&&dy<0.01){z[1]=-100;z[2]-=0.003*xx;}

	//ブロックについて
	for(int b=0;b<1000;b++){
		if(x-brox[b]-10>0&&dx>0){if(x+brox[b]+10<0&&y-broy[b]-10>0&&y+broy[b]+10<0){z[2]=-0.8*xx;}}
	}
	// 水平加速度
	double ddx     =0 ;
	// 重力加速度
	double ddy     =-500;

	// 状態変数の時間微分
	dz[0] = dx;
	dz[1] = dy;
	dz[2] = ddx;
	dz[3] = ddy;

	
	// 数値積分（オイラー則）
	for(int i = 0; i < 4; i++)
		z[i] += dz[i] * dt;

	// 時刻の更新
	t += dt;

	// ファイルに出力
	

	// 再描画要求
	glutPostRedisplay();

	// タイマコールバックを再登録
	glutTimerFunc(timer_period, timer_func, 0);
}

// キーボードコールバック関数
void keyboard_func(unsigned char ch, int x, int y){
	if(ch=='a'){z[2]-=10;}
	if(ch=='d'){z[2]+=10;}
	if(ch=='s'){z[1]-=1;}
	if(ch=='w'){z[1]+=1;}
	if(ch==' '){if(z[1]+100<0){z[3]+=250;}}

}

// ウィンドウサイズ変更コールバック関数
void reshape_func(int width, int height){
	window_width  = width;
	window_height = height;

	// 描画範囲を設定
	glViewport(0, 0, width, height);
}

// 画面描画コールバック関数
void display_func(){
	//double x      = z[0];
	//double y      = z[1];
	double w = (double)window_width ;
	double h = (double)window_height;

	// 描画処理のお膳立て
	glClear(GL_COLOR_BUFFER_BIT);
	 glClearColor(1.0, 1.0, 1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-w/2.0, w/2.0, -h/2.0, h/2.0, -1.0, 1.0);

	// 座標軸を描画
	glBegin(GL_LINES);
	glColor3f(0.0f, 0.0f, 0.0f);
	glVertex2d(100,  100);
	glVertex2d(-100,  100);
	glVertex2d(100,  100);
	glVertex2d(100,  -100);
	glVertex2d(-100,  -100);
	glVertex2d(100,  -100);
	glVertex2d(-100,  -100);
	glVertex2d(-100,  100);
	glEnd();

	//const double scale = 100.0;


	glBegin(GL_POLYGON);
	glColor3f(1.0f, 0.1f, 0.1f);
	for(int n=0;n<37;n++){
	glVertex2d((z[0])-3*l*sin(n*M_PI/18), (z[1])-3*l*cos(n*M_PI/18));
	}
	glEnd();

	//ブロック
	for(int b=0;b<1000;b++){
	glBegin(GL_POLYGON);
	glColor3f(0.0f, 1.0f, 1.0f);
		for(int n=0;n<4;n++){
			glVertex2d(brox[b]+10*l*sin(n*M_PI/2+M_PI/4), (broy[b]+10*l*cos(n*M_PI/2+M_PI/4)));
		}
		glEnd();
	}
	glFlush();
	glutSwapBuffers();
}

// ウィンドウが閉じられたときに呼ばれるコールバック関数
void close_func(){
	// ファイルを閉じる
	
}

int main(int argc, char** argv){

	// OpenGLを初期化
	glutInit(&argc, argv);

	// ウィンドウを作成
	glutCreateWindow("pendulum");

	// コールバック関数を登録
	glutTimerFunc(timer_period, timer_func, 0);
	glutKeyboardFunc(keyboard_func);
	glutReshapeFunc(reshape_func);
	glutDisplayFunc(display_func);

	// ファイルを開く

	
	// メインループ（無限ループ）
	glutMainLoop();

}
