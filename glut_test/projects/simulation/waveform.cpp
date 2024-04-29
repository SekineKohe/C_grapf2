#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <iomanip>
using namespace std;

string filename = "waveform F060 rd7.dat";

// 定数
int timer_period = 50;   //< タイマ周期 ミリ秒
int k = 0;

vector<double> time_N(10000),x(10000,100000),y(10000);
double t , now;

int time_stop = 0 , time_stop2;
ifstream ifs2(filename);
// ウィンドウサイズ
int window_width ;
int window_height;

void keyboard_func(unsigned char ch, int x, int y){

	if(ch=='d'){timer_period -=5; cout << "timer_period = " << timer_period <<endl;}
	if(ch=='a'){timer_period +=5; cout << "timer_period = " << timer_period <<endl;}
}

void timer_func(int value){

        ifs2 >> x[0] >> y[0];

        int i = 1;
    while(1){
        ifs2 >> t;
        if(now == t){
            ifs2 >> x[i] >> y[i];
            i++;
        }else{
            now = t;
            break;
        }
    }


	// 再描画要求
	glutPostRedisplay();

	// タイマコールバックを再登録
	glutTimerFunc(timer_period, timer_func, 0);
}


// キーボードコールバック関数


// 画面描画コールバック関数
void display_func(){

	// 描画処理のお膳立て
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0, 1.0, 1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-4, 6, -5, 5, -1.0, 1.0);

	// 座標軸を描画
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2d(100,  0);
        glVertex2d(-100,  0);
        glVertex2d(0,  100);
        glVertex2d(0,  -100);

        glEnd();

	//const double scale = 100.0;

    for(int i = 0; i < 1000 ; i++){
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2d(x[i]*0.09,  y[i]*90);
        glVertex2d(x[i+1]*0.09,  y[i+1]*90);
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

    ifs2 >> t ;
    now = 0.05;


	// OpenGLを初期化
	glutInit(&argc, argv);

    int windowWidth = 1000;
    int windowHeight = 500;

	// ウィンドウを作成
    const char* filename_data = filename.data();
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow(filename_data);

    k = 0;
	// コールバック関数を登録
	glutTimerFunc(timer_period, timer_func, 0);
	glutKeyboardFunc(keyboard_func);
	glutDisplayFunc(display_func);

	// メインループ（無限ループ）
	glutMainLoop();

}
