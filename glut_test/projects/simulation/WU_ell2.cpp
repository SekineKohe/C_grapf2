
// C	WaterUnst(Ellipse)70-2.f
// c   境界要素法(BEM)による半楕円棒を過ぎる流れの時間発展方程式の計算プログラム
// c   繰り返し計算のGCR法にあった誤り(L^tをかけていなかった)を正したversion
// c	計算法の説明ファイルは「航跡波(数値計算法)6.doc」を見よ．#include <iostream>

#include <GL/freeglut.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <math.h>
#include <iomanip>

using namespace std;

int IT = 3000 ; //(ITはIIよりも大きくとる．
int II, II0 ;
int IM , IG , nnn ;



//負の添え字を利用可能にする
//添え字が負の値の時、配列の最後からデータが挿入される
//size=20のとき　A[-4｣=A[16｣　となる

template <typename T>
class MyArray {
public:
    MyArray(int size) : data(size) {}

    T& operator[](int index) {
        if (index >= 0 && index < static_cast<int>(data.size())) {
            return data[index];
        } else if (index < 0 && -index <= static_cast<int>(data.size())) {
            return data[data.size() + index];
        } else {
            // 負の添え字が範囲外の場合の処理
            throw std::out_of_range("Index out of range");
        }
    }

private:
    std::vector<T> data;
};

string filename , filename01 , filename02 , filename11 , filename12, filename3 , filename4 , filename41 , filename42 , filename43;
MyArray<double> Xb(2*IT+1);
vector<double> dXb(IT+1) , d2Xb(IT+1);
vector<double> Yb(IT+1) , dYb(IT+1) , d2Yb(IT+1);
vector<double> RRb(IT+1),dpB(IT+1),dpBx(IT+1),dp2Bx(IT+1);
MyArray<double> pBx(IT+9);
MyArray<double> pBxL(IT+9),pBxC(IT+9),pBxR(IT+9);
vector<double> dpBxL(IT+1),dpBxC(IT+1),dpBxR(IT+1),dpnBx(IT+1);
vector<double> dp2BxL(IT+1),dp2BxC(IT+1),dp2BxR(IT+1);
vector<vector<double>> zIbb(IT+1,vector<double>(IT+1)),zRbb(IT,vector<double>(IT+1));
MyArray<double> X(IT+9),YY(IT+9),ph(IT+9);
vector<double> dx(IT+1),dy(IT+1),dp(IT+1),RR(IT+1);
vector<double> dx2(IT+1),dy2(IT+1),dp2(IT+1);
vector<vector<double>> zI00(IT+1,vector<double>(IT+1)),zR00(IT,vector<double>(IT+1)),zRb0(IT,vector<double>(IT+1));
vector<vector<double>> zR0b(IT+1,vector<double>(IT+1)),zIb0(IT,vector<double>(IT+1)),zI0b(IT,vector<double>(IT+1));
vector<vector<double>> aa1(IT+1,vector<double>(IT+1)),bb1(IT+1,vector<double>(IT+1)),aa2(IT+1,vector<double>(IT+1)),bb2(IT+1,vector<double>(IT+1));
vector<double> cc1(IT+1),cc2(IT+1);
vector<vector<double>> ca(2*IT+3,vector<double>(2*IT+3));
vector<double> cb(2*IT+3),SOL(2*IT+3),Res(2*IT+3);
vector<vector<double>> P(2*IT+3,vector<double>(2*IT+3)),dLP(2*IT+3,vector<double>(2*IT+3));
vector<double> dLPT(2*IT+3);
vector<double> dLR(2*IT+3),beta(2*IT+3),ReC(2*IT+3),LRes(2*IT+1);
vector<double> dpn(IT+1),Xt(IT+1),Yt(IT+1),phit(IT+1);
MyArray<double> pt(IT+9),px(IT+9);
vector<double> dpt(IT+1),dp2t(IT+1),dpx(IT+1),dp2x(IT+1);
vector<double> dpnt(IT+1),dpnx(IT+1),Xtt(IT+1),Ytt(IT+1),ptt(IT+1);

double Pi,c0,ts,rd,c,Am,Le,dt,tlimit,Xmin,Xmax,epsilon,t0,t,dX0;
double alpX,Xbb,Ybb,R2bb,X00,X0b,Xb0,Y00,Y0b,Yb0,R200,R20b,R2b0;
double c1,c2,alpha,ResT,ReCM, Amp,Lep,W1,W2,Wx1,Wx2,Upy,Yw1,Yw2;
double dII,Xe,Ye;
int K , L , KK , LL ;

// 定数
const int timer_period2 = 1;   //< タイマ周期 ミリ秒
const double dt2 = 0.001;  //< 数値積分の時間刻み 秒

// ウィンドウサイズ
int window_width ;
int window_height;

// ファイル
FILE* file = NULL;





// ウィンドウが閉じられたときに呼ばれるコールバック関数
void close_func(){
	// ファイルを閉じる
	
}


int main(int argc, char** argv){

    // OpenGLを初期化
	glutInit(&argc, argv);

    int windowWidth = 500;
    int windowHeight = 500;
 

	// ウィンドウを作成
    glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("waveform");

    LL = 1;

//  計算条件

  double H = 0.09 ; //水深

  c0 = 0.50;
  rd = 5/H/1000;
  filename = "F050 rd5.dat";

  cout << filename << endl;



  ts = 1.5;
//   c=0(at t=0)からc=c0付近に到達するまでに要する無次元時間
//  (tsが小さいと，非定常の波が大きく発生してしまうため，定常状態までの時間がかかる．
// 	          ただし，定常航跡波が大きくなる場合は，小さくてもよい(ts=1〜2).
//                   定常航跡波が小さくなる場合は，tsは大きくとる必要がある)

//  半楕円棒
  Xe = rd * 4;  //半楕円棒水平軸
  Ye = rd *1.1; //半楕円棒鉛直軸

//　計算パラメータ
  dt = 0.05;
  tlimit = 50;
  int ns = 100;
  int nf = 200000000;
  int nP = 20;

// 計算領域
  Xmin = -40;
  Xmax = 40;
  alpX = 0.0625 * Xe;

// 計測位置
  W1 = 1.2;
  W2 = 1.5;

//　計算メッシュ数は全部でII
//　メッシュ配分は計算領域に合わせて調整される
  if(c0 >= 0.5) {
    II0 = 800 - 200 * ( c0 - 0.5 );
   } else {
     II0 = 800 - 400 * ( c0 - 0.5 );
   }

  II = II0;


  epsilon = 0.000000005;	//境界積分方程式の連立方程式を反復計算GCRで解く際の収束判定条件


//  ファイルの初期化
  filename01 = "W1mHp " + filename;
    ofstream w1ofs(filename01);

  filename02 = "W2mHp " + filename;
    ofstream w2ofs(filename02);

//  測定地点の無次元化
  Wx1 = -W1/H;
  Wx2 = -W2/H;
  t = 0;

//nは計算の時間ステップ
  int n = 0;

// 底面座標（x座標のメッシュ(時間に依存しsない)を不等間隔にとる
//	Xb: 底面のx座標 (Xmin<Xb<Xmax)
//	dXb : Xbのξによる1階微分
//  d2Xb : Xbのξによる2階微分

  Xmin= Xmin + Xe;
  Xmax= Xmax - Xe;
  int IX = 0;
  int Imax , Imin ;
  int IIc;

// Save the original state of std::cout
    ios originalState(nullptr);
    originalState.copyfmt(cout);



//　|Xmin|とXmaxの大きさに応じて，0〜IIのメッシュを地形(中心がx=0)の上下流に配分
  for(IX = 0 ; IX <= 11 ; IX++ ){
//　|Xmin|>Xmaxのとき，
    if(fabs(Xmin + 0.0001) > Xmax){

      for(int i= 0; i <= II; i++){
        t0 = static_cast<double> (i) / static_cast<double> (II) ;
        Xb[i] = -Xmin*( alpX*t0 +(1-alpX)*pow(t0,3) );

        if(Xb[i] >= Xmax){
          Imax = i;
          break;
        }

        Imax = II;

      }




//　Iminは上流側，Imaxは下流側のメッシュ数を指す．
      Imin = - int(static_cast<double>(pow(II,2))/static_cast<double>(II + Imax));
      Imax = II + Imin;

      for(int i = 0 ; i <= II ; i++){
          t0= static_cast<double>(i+Imin)/static_cast<double>(-Imin);
          Xb[i] = -Xmin*( alpX*t0 +(1-alpX)*pow(t0,3) );
          dXb.at(i)= -Xmin*( alpX +3*(1-alpX)*pow(t0,2) )/static_cast<double>(-Imin);
          d2Xb.at(i)= -Xmin*6*(1-alpX)*t0/static_cast<double>(pow(-Imin,2));
      }

    }

// //  |Xmin|<Xmaxのとき，
    else{

      for(int i= -1; i >= -II; --i){
        t0 = static_cast<double> (i) / static_cast<double> (II) ;
        Xb[i] = Xmax*( alpX*t0 +(1-alpX)*pow(t0,3) );

        if(Xb[i] <= Xmin){
          Imin = i;
          break;
        }

        Imin = -II;

      }


        Imax = int(static_cast<double>(pow(II,2))/static_cast<double>(II - Imin) + 0.0001);
        Imin = Imax - II;

        for(int j = 0 ; j <= II ; j++){
            t0= static_cast<double>(j+Imin)/static_cast<double>(Imax);
            Xb[j] = Xmax*( alpX*t0 +(1-alpX)*pow(t0,3) );
            dXb.at(j)= Xmax*( alpX +3*(1-alpX)*pow(t0,2) )/static_cast<double>(Imax);
            d2Xb.at(j)= Xmax*6*(1-alpX)*t0/static_cast<double>(pow(Imax,2));
        }

    }

//  IIcは半楕円棒上でのメッシュ数．この分のメッシュ数増加を抑えるため，
//  IIからIICを引いた上で17に戻って再計算する．
     IIc = int(M_PI*Ye/dXb.at(-Imin));

      cout << "Imin,IIc,Imax = " << Imin << " ," << IIc <<" ," << Imax <<  endl ;
      II = II0 - IIc;


  }

   II = II + IIc;
   cout << "Imin,IIc,Imax = " << Imin << " ," << IIc <<" ," << Imax << endl ;




//	最終的な底面のメッシュ分布(x,y)をここで決定
// 半楕円棒の上流底面
    for (int i= 0; i <= -Imin - 1; i++) {
        Xb[i] = Xb[i] - Xe;
        Yb.at(i) = -1.0;
        dYb.at(i) = 0.0;
        d2Yb.at(i) = 0.0;
    }

// 半楕円棒の下流底面
    for (int i= II; i >= -Imin + IIc + 1; --i) {
        Xb[i] = Xb[i - IIc] + Xe;
        dXb.at(i) = dXb[i - IIc];
        d2Xb.at(i) = d2Xb[i - IIc];
        Yb.at(i) = -1.0;
        dYb.at(i) = 0.0;
        d2Yb.at(i) = 0.0;
    }

// 角
    Xb[-Imin + IIc] = Xb[-Imin + IIc - IIc] + Xe;
    Yb.at(-Imin + IIc) = -1.0;
    dXb.at(-Imin + IIc) = dXb[-Imin + IIc - IIc];
    dYb.at(-Imin + IIc) = - M_PI * Ye / static_cast<double>(IIc);
    d2Xb.at(-Imin + IIc) = d2Xb[-Imin + IIc - IIc];
    d2Yb.at(-Imin + IIc) = 0.0;


    Xb[-Imin] = Xb[-Imin] - Xe;
    Yb.at(-Imin) = -1.0;
    dXb.at(-Imin) = dXb.at(-Imin);
    dYb.at(-Imin) =  M_PI * Ye / static_cast<double>(IIc);
    d2Xb.at(-Imin) = d2Xb.at(-Imin);
    d2Yb.at(-Imin) = 0.0;

// 半楕円棒
    for (int i= -Imin + 1; i <= -Imin + IIc - 1; i++) {
        double t0 = static_cast<double>(i + Imin) / static_cast<double>(IIc);
        Xb[i] = -Xe *  cos( M_PI * t0);
        Yb.at(i) = -1.0 + Ye *  sin( M_PI * t0);
        dXb.at(i) =  M_PI * Xe *  sin( M_PI * t0) / static_cast<double>(IIc);
        dYb.at(i) =  M_PI * Ye *  cos( M_PI * t0) / static_cast<double>(IIc);
        d2Xb.at(i) =  pow( M_PI, 2) * Xe *  cos( M_PI * t0) /  pow(static_cast<double>(IIc), 2);
        d2Yb.at(i) = - pow( M_PI, 2) * Ye *  sin( M_PI * t0) /  pow(static_cast<double>(IIc), 2);

        
    }

  

  for( int i = 0 ; i <= II ; i++) RRb.at(i) = dXb.at(i)*dXb.at(i)+dYb.at(i)*dYb.at(i);



	// ここでXminの値が最初の指定した値より少しずれるので，そのずれた値で定義し直しておく．
	Xmin = Xb[0];
	Xmax = Xb[II];

	cout << "Xmin,Xmax=" << fixed << setprecision(9) << Xmin << " ," << Xmax << endl;


//  底面の形状，微分形状などを保存して確認
  filename11 = "Tik " + filename;
    ofstream Tikofs(filename11);
    for(int i = 0 ; i <= II ; i++)  {
      Tikofs << fixed << setprecision(10) << Xb[i] << "  " << Yb.at(i) << "  " << dXb.at(i) << "  " << dYb.at(i) << endl;
    }
    Tikofs.close();

  filename12 = "Tik2 " + filename;
    ofstream Tik2ofs(filename12);
    for(int i = 0 ; i <= II ; i++){
        Tik2ofs << fixed << setprecision(10) << Xb[i] << "  " << d2Xb.at(i) << "  " << d2Yb.at(i) << endl;
    }
    Tik2ofs.close();


//  境界積分方程式の既知項（ここで時間に依存しない項を計算しておく．p.8-10を見よ）
  for (int i = 0; i <= II; i++) {
    for (int j = 0; j <= II; j++) {
      Xbb = Xb[j] - Xb[i];
      Ybb = Yb.at(j) - Yb.at(i);
      R2bb = Xbb * Xbb + Ybb * Ybb;

      if (j != i) {
          zIbb.at(i).at(j) = (Xbb * dYb.at(i) - Ybb * dXb.at(i)) / R2bb;
          zRbb.at(i).at(j) = (Xbb * dXb.at(i) + Ybb * dYb.at(i)) / R2bb;
      }
      else {
          zIbb.at(i).at(j) = (d2Xb.at(i) * dYb.at(i) - d2Yb.at(i) * dXb.at(i)) / (2.0 * RRb.at(i));
          zRbb.at(i).at(j) = -(d2Xb.at(i) * dXb.at(i) + d2Yb.at(i) * dYb.at(i)) / (2.0 * RRb.at(i));
      }
    }
  }


//  計算した項のチェック
  filename3 = "Ch " + filename;
    ofstream Chofs(filename3);
    for(int i = 0 ; i <= II ; i++)  {
      Chofs << fixed << setprecision(10) << Xb[i] << "  " << zIbb.at(II/3).at(i) << "  " << zRbb.at(II/3).at(i) << endl;
    }
  Chofs.close();

// 水表面の初期条件

    dX0 = (Xmax - Xmin) / static_cast<double>(II);

    for (int i = 0; i <= II; ++i) {
        X[i] = Xmin + dX0 * static_cast<double>(i);
        YY[i] = 0.0;
        ph[i] = 0.0;
    }


    cout << fixed << setprecision(9) << "dX0 = " << dX0 << endl;

// Restore the original state
    cout.copyfmt(originalState);


// C*************************************************************************
// C************************* 時間ステップを進める **************************
// C*************************************************************************


// c---- 計算粒子の下流側への排出，上流側からの投入，および既存粒子の番号付替え----
// c************* Renumber Lagrangian particles *************


// c------- 最下流の粒子が計算領域(X<Xmax)をはみ出た場合に，
// c	はみ出た粒子を消去して全体の粒子番号を１つ下げる．  
    while(true){ 

        if (X[II] > Xmax) {
            for (int i = II; i >= 1; --i) {
                X[i] = X[i - 1];
                YY[i] = YY[i - 1];
                ph[i] = ph[i - 1];
            }
            // 新たにI=0の粒子を投入する．投入する位置は，I=1の粒子から距離dX0だけ上流側．
            X[0] = X[1] - dX0;
            YY[0] = 0.0;
            ph[0] = 0.0;
        }

        // 一様流の速度cを時間発展
        c = c0 * tanh((t + 0.000001) / ts);

        // 地形の動く室内実験(H=9cm)を想定した場合の，
        // 計測地点に対する地形の相対位置 Wx(無次元)を時間発展
        Wx1 += c * dt;
        Wx2 += c * dt;

        // 水表面に沿って（計算座標ξに沿って）の(X,η,φ)微分項を計算しておく
        for (int i = -1; i >= -4; --i) {
            X[i] = X[i + 1] - dX0;
            YY[i] = 0.0;
            ph[i] = 0.0;
        }

        for (int i = II + 1; i <= II + 4; ++i) {
            X[i] = X[i - 1] + dX0;
            YY[i] = YY[II];
            ph[i] = ph[II];
        }

        for (int i = 0; i <= II; ++i) {
            dx[i] = (3.0 * (X[i - 4] - X[i + 4]) - 32.0 * (X[i - 3] - X[i + 3]) 
                    + 168.0 * (X[i - 2] - X[i + 2]) - 672.0 * (X[i - 1] - X[i + 1])) / 840.0;
            dy[i] = (3.0 * (YY[i - 4] - YY[i + 4]) - 32.0 * (YY[i - 3] - YY[i + 3]) 
                    + 168.0 * (YY[i - 2] - YY[i + 2]) - 672.0 * (YY[i - 1] - YY[i + 1])) / 840.0;
            dp[i] = (3.0 * (ph[i - 4] - ph[i + 4]) - 32.0 * (ph[i - 3] - ph[i + 3]) 
                    + 168.0 * (ph[i - 2] - ph[i + 2]) - 672.0 * (ph[i - 1] - ph[i + 1])) / 840.0;
            RR[i] = dx[i] * dx[i] + dy[i] * dy[i];

            dx2[i] = (-9.0 * (X[i - 4] + X[i + 4]) + 128.0 * (X[i - 3] + X[i + 3]) 
                    - 1008.0 * (X[i - 2] + X[i + 2]) + 8064.0 * (X[i - 1] + X[i + 1]) - 14350.0 * X[i]) / 5040.0;
            dy2[i] = (-9.0 * (YY[i - 4] + YY[i + 4]) + 128.0 * (YY[i - 3] + YY[i + 3]) 
                    - 1008.0 * (YY[i - 2] + YY[i + 2]) + 8064.0 * (YY[i - 1] + YY[i + 1]) - 14350.0 * YY[i]) / 5040.0;
            dp2[i] = (-9.0 * (ph[i - 4] + ph[i + 4]) + 128.0 * (ph[i - 3] + ph[i + 3]) 
                    - 1008.0 * (ph[i - 2] + ph[i + 2]) + 8064.0 * (ph[i - 1] + ph[i + 1]) - 14350.0 * ph[i]) / 5040.0;
        }

        n = n + 1;
        t = t + dt;

        if(n % nP == 1) cout << "calculating";

        cout << "." ;

        if (n % nP == 0) {
            cout << endl;
            cout << "     n = " << n << endl;
            cout << " Dimensionless Time  = " << static_cast<float>(t) << endl;
            cout << " Actual time (H=9cm) = " << static_cast<float>(t * sqrt(H / 9.8)) << " sec" << endl;
        }

        if (n % (nP * 20) == 0) {
            cout << endl;
            cout << "Xe,Ye,c0= " << static_cast<float>(Xe) << ", " << static_cast<float>(Ye) << ", " << static_cast<float>(c0) << endl;
            cout << endl;
        }

    // C*********************************************************
    // C*************** First-order calculation *****************
    // C*********************************************************


    // c----境界積分方程式(4.33)(4.34)を解くために必要な項を計算しておく(pp.7〜8を見よ)  

        for (int i = 0; i <= II; ++i) {
            for (int j = 0; j <= II; ++j) {
                X00 = X[j] - X[i];
                X0b = X[j] - Xb[i];
                Xb0 = Xb[j] - X[i];
                Y00 = YY[j] - YY[i];
                Y0b = YY[j] - Yb[i];
                Yb0 = Yb[j] - YY[i];
                R200 = X00 * X00 + Y00 * Y00;
                R20b = X0b * X0b + Y0b * Y0b;
                R2b0 = Xb0 * Xb0 + Yb0 * Yb0;

                if (j != i) {
                    zI00[i][j] = (X00 * dy[i] - Y00 * dx[i]) / R200;    // 式1マル(p.9)
                    zR00[i][j] = (X00 * dx[i] + Y00 * dy[i]) / R200;    // 式5マルのf(ξ)以外の部分
                } else {
                    zI00[i][j] = (dx2[i] * dy[i] - dy2[i] * dx[i]) / (2.0 * RR[i]);
                    zR00[i][j] = -(dx2[i] * dx[i] + dy2[i] * dy[i]) / (2.0 * RR[i]);
                }
                zRb0[i][j] = (Xb0 * dx[i] + Yb0 * dy[i]) / R2b0;    // 式3マル
                zR0b[i][j] = (X0b * dXb[i] + Y0b * dYb[i]) / R20b;  // 式4マル
                zIb0[i][j] = (Xb0 * dy[i] - Yb0 * dx[i]) / R2b0;    // 式3マル
                zI0b[i][j] = (X0b * dYb[i] - Y0b * dXb[i]) / R20b;  // 式4マル
            }
        }

    // C****************** ∂φ/∂νに対する積分方程式を解く (pp.6〜8)***********************
    // c	aa1:(4.33)式の∂φ/∂νの係数,       bb1:(4.33)式の(∂φ/∂ξ)y=bの係数
    // c	aa2:(4.34)式の∂φ/∂νの係数,       bb2:(4.34)式の(∂φ/∂ξ)y=bの係数
    // c	cc1:(4.33)式の非同次項を右辺に集めたもの, cc2:(4.34)式の非同次項を左辺に集めたもの
    // C*************************************************************************************

    // c*** 連立方程式(4.33)(4.34)の未知変数[∂φ/∂ν,(∂φ/∂ξ)y=b]の係数を計算

        for (int i = 0; i <= II; ++i) {
            for (int j = 0; j <= II; ++j) {
                aa1[i][j] = -zI00[i][j];

                if (j <= -Imin - 1) {
                    bb1[i][j] = zRb0[i][j];
                } else if (j >= -Imin + 1 && j <= -Imin + IIc - 1) {
                    bb1[i][j - 1] = zRb0[i][j];
                } else if (j >= -Imin + IIc + 1) {
                    bb1[i][j - 2] = zRb0[i][j];
                }
            }
        }

        for (int i = 0; i <= -Imin - 1; ++i) {
            for (int j = 0; j <= II; ++j) {
                aa2[i][j] = zR0b[i][j];

                if (j <= -Imin - 1) {
                    bb2[i][j] = zIbb[i][j];
                } else if (j >= -Imin + 1 && j <= -Imin + IIc - 1) {
                    bb2[i][j - 1] = zIbb[i][j];
                } else if (j >= -Imin + IIc + 1) {
                    bb2[i][j - 2] = zIbb[i][j];
                }
            }
        }

        for (int i = -Imin + 1; i <= -Imin + IIc - 1; ++i) {
            for (int j = 0; j <= II; ++j) {
                aa2[i - 1][j] = zR0b[i][j];

                if (j <= -Imin - 1) {
                    bb2[i - 1][j] = zIbb[i][j];
                } else if (j >= -Imin + 1 && j <= -Imin + IIc - 1) {
                    bb2[i - 1][j - 1] = zIbb[i][j];
                } else if (j >= -Imin + IIc + 1) {
                    bb2[i - 1][j - 2] = zIbb[i][j];
                }
            }
        }

        for (int i = -Imin + IIc + 1; i <= II; ++i) {
            for (int j = 0; j <= II; ++j) {
                aa2[i - 2][j] = zR0b[i][j];

                if (j <= -Imin - 1) {
                    bb2[i - 2][j] = zIbb[i][j];
                } else if (j >= -Imin + 1 && j <= -Imin + IIc - 1) {
                    bb2[i - 2][j - 1] = zIbb[i][j];
                } else if (j >= -Imin + IIc + 1) {
                    bb2[i - 2][j - 2] = zIbb[i][j];
                }
            }
        }

        for (int i = 0; i <= II; ++i) {
            aa1[i][i] -= M_PI;
        }

        for (int i = 0; i <= II - 2; ++i) {
            bb2[i][i] -= M_PI;
        }

        // 連立方程式(4.33)(4.34)の非同次項を計算
        for (int i = 0; i <= II; ++i) {
            double c1 = 0.0;
            for (int j = 0; j <= II; ++j) {
                if (j != -Imin && j != -Imin + IIc) {
                    c1 = zR00[i][j] * dp[j] - zIb0[i][j] * c * dYb[j] + c1;  // (4.33)の非同次項
                } else {
                    c1 = zR00[i][j] * dp[j] - zIb0[i][j] * c * dYb[j] / 2.0 + c1;
                }
            }
            // 式5マルのdf/dξ(=∂2φ/∂ξ2)の項と(4.34)の右辺第3項の角からの寄与を(4.33)の非同次項に加える
            cc1[i] = c1 + dp2[i] + zRb0[i][-Imin] * c * dXb[-Imin] / 2.0 +
                    zRb0[i][-Imin + IIc] * c * dXb[-Imin + IIc] / 2.0;
        }

        for (int i = 0; i <= -Imin - 1; ++i) {
            double c2 = 0.0;
            for (int j = 0; j <= II; ++j) {
                if (j != -Imin && j != -Imin + IIc) {
                    c2 = zI0b[i][j] * dp[j] + zRbb[i][j] * c * dYb[j] + c2;
                } else {
                    c2 = zI0b[i][j] * dp[j] + zRbb[i][j] * c * dYb[j] / 2.0 + c2;
                }
            }
            // 式5マルのdf/dξ(=c*d2b/dξ2)の項と(4.34)の右辺第1項の角からの寄与を(4.34)の非同次項に加える
            cc2[i] = c2 + c * d2Yb[i] + zIbb[i][-Imin] * c * dXb[-Imin] / 2.0 +
                    zIbb[i][-Imin + IIc] * c * dXb[-Imin + IIc] / 2.0;
        }

        for (int i = -Imin + 1; i <= -Imin + IIc - 1; ++i) {
            double c2 = 0.0;
            for (int j = 0; j <= II; ++j) {
                if (j != -Imin && j != -Imin + IIc) {
                    c2 = zI0b[i][j] * dp[j] + zRbb[i][j] * c * dYb[j] + c2;
                } else {
                    c2 = zI0b[i][j] * dp[j] + zRbb[i][j] * c * dYb[j] / 2.0 + c2;
                }
            }
            // 式5マルのdf/dξ(=c*d2b/dξ2)の項と(4.34)の右辺第1項の角からの寄与を(4.34)の非同次項に加える
            cc2[i - 1] = c2 + c * d2Yb[i] + zIbb[i][-Imin] * c * dXb[-Imin] / 2.0 +
                        zIbb[i][-Imin + IIc] * c * dXb[-Imin + IIc] / 2.0;
        }

        for (int i = -Imin + IIc + 1; i <= II; ++i) {
            double c2 = 0.0;
            for (int j = 0; j <= II; ++j) {
                if (j != -Imin && j != -Imin + IIc) {
                    c2 = zI0b[i][j] * dp[j] + zRbb[i][j] * c * dYb[j] + c2;
                } else {
                    c2 = zI0b[i][j] * dp[j] + zRbb[i][j] * c * dYb[j] / 2.0 + c2;
                }
            }
            // 式5マルのdf/dξ(=c*d2b/dξ2)の項と(4.34)の右辺第1項の角からの寄与を(4.34)の非同次項に加える
            cc2[i - 2] = c2 + c * d2Yb[i] + zIbb[i][-Imin] * c * dXb[-Imin] / 2.0 +
                        zIbb[i][-Imin + IIc] * c * dXb[-Imin + IIc] / 2.0;
        }

    // c-------aa1,aa2,bb1,bb2をcaに当てはめ，cc1,cc2をcbに当てはめる．
    // c	    なお，ca,cbは，この後解く連立方程式の未知数の係数行列，および非同次項を表す
    // c	ca:(4.33)の左辺，(4.34)の右辺の係数を並べた行列
    // c	cb:(4.33)，(4.34)の非同次項から成る列ベクトル

        int IM = II + 1;

        for (int i = 1; i <= IM; ++i) {
            for (int j = 1; j <= IM; ++j) {
                ca[i][j] = aa1[i - 1][j - 1];
            }
        }

        for (int i = IM + 1; i <= 2 * IM - 2; ++i) {
            for (int j = 1; j <= IM; ++j) {
                ca[i][j] = aa2[i - 1 - IM][j - 1];
            }
        }

        for (int i = 1; i <= IM; ++i) {
            for (int j = IM + 1; j <= 2 * IM - 2; ++j) {
                ca[i][j] = bb1[i - 1][j - 1 - IM];
            }
        }

        for (int i = IM + 1; i <= 2 * IM - 2; ++i) {
            for (int j = IM + 1; j <= 2 * IM - 2; ++j) {
                ca[i][j] = bb2[i - 1 - IM][j - 1 - IM];
            }
        }

        for (int i = 1; i <= IM; ++i) {
            cb[i] = cc1[i - 1];
        }

        for (int i = 1 + IM; i <= 2 * IM - 2; ++i) {
            cb[i] = cc2[i - 1 - IM];
        }

    // C*******ここよりGeneralized Conjugate Residual法による連立方程式求解******
    // c	  (中身は分からなくてよい．GCRという反復法で連立方程式を解いているだけ)
    // c	ca(I,J) :マトリックス係数 L
    // c	cb(I)   :非同次項 F
    // c	SOL(I)  :この連立方程式の解 V
    // c	Res(I)  :残差 R
    // c	dLP(I,nnn):nnnステップ目のL*P
    // c	dLR(I)  :L*R

        int IG = 2 * IM - 2;

        for (int i = 1; i <= IG; ++i) {
            SOL[i] = 0.0;
        }

        for (int i = 1; i <= IG; ++i) {
            Res[i] = cb[i];
            for (int j = 1; j <= IG; ++j) {
                Res[i] -= ca[i][j] * SOL[j];
            }
        }

        for (int i = 1; i <= IG; ++i) {
            P[i][1] = Res[i];
            dLP[i][1] = 0.0;
        }

        for (int i = 1; i <= IG; ++i) {
            for (int j = 1; j <= IG; ++j) {
                dLP[i][1] += ca[i][j] * P[j][1];
            }
        }

        int nnn = 0;

    // C************ Iteration **************
        while (true) {
            ++nnn;

            // (i)
            c1 = 0.0;
            dLPT[nnn] = 0.0;

            for (int i = 1; i <= IG; ++i) {
                c1 += Res[i] * dLP[i][nnn];
                dLPT[nnn] += pow(dLP[i][nnn], 2);
            }

            alpha = c1 / dLPT[nnn];

            for (int i = 1; i <= IG; ++i) {
                SOL[i] += alpha * P[i][nnn];
                Res[i] -= alpha * dLP[i][nnn];
            }

            // (ii)
            ResT = 0.0;
            for (int i = 1; i <= IG; ++i) {
                ResT += pow(Res[i], 2);
            }
            if (ResT < epsilon || nnn == IG) {
                break;
            }

            // (iii)
            for (int i = 1; i <= IG; ++i) {
                LRes[i] = 0;
                for (int j = 1; j <= IG; ++j) {
                    LRes[i] += ca[j][i] * Res[j];
                }
            }

    
            for (int i = 1; i <= IG; ++i) {
                dLR[i] = 0;
                for (int j = 1; j <= IG; ++j) {
                    dLR[i] += ca[i][j] * LRes[j];
                }
            }

            for (int j = 1; j <= nnn; ++j) {
                c1 = 0.0;
                for (int I = 1; I <= IG; ++I) {
                    c1 += dLR[I] * dLP[I][j];
                }
                beta[j] = -c1 / dLPT[j];
            }

            for (int I = 1; I <= IG; ++I) {
                P[I][nnn+1] = LRes[I];
                dLP[I][nnn+1] = dLR[I];
                for (int j = 1; j <= nnn; ++j) {
                    P[I][nnn+1] += beta[j] * P[I][j];
                    dLP[I][nnn+1] += beta[j] * dLP[I][j];
                }
            }
        }

        if (n % nP == 0) {
            for (int i = 0; i <= IG; ++i) {
                ReC[i] = cb[i];
                for (int j = 0; j < IG; ++j) {
                    ReC[i] -= ca[i][j] * SOL[j];
                }
            }
            ReCM = 0.0;
            for (int i = 0; i <= IG; ++i) {
                ReCM = max(fabs(ReC[i]), ReCM);
            }
            cout << " Iteration =" << nnn << "  Max Residual(dp ) =" << ReCM << endl;
        }

        // dpn=∂φ/∂ν, dpB=(∂φ/∂ξ)y=b.
        for (int i = 0; i <= II; ++i) {
            dpn[i] = SOL[i + 1];
        }

        dpB[-Imin] = -c * dXb[-Imin] * 2.0;
        dpB[-Imin + IIc] = -c * dXb[-Imin + IIc] * 2.0;
        for (int i = 0; i <= -Imin; ++i) {
            dpB[i] = SOL[II + i + 2];
        }
        for (int i = -Imin + 1; i <= -Imin + IIc - 1; ++i) {
            dpB[i] = SOL[II + i + 1];
        }
        for (int i = -Imin + IIc + 1; i <= II; ++i) {
            dpB[i] = SOL[II + i];
        }

        // 時間微分とx微分の計算((4.42)-(4.44),(4.56)を見よ)
        // Xt:DX/Dt, Yt:Dη/Dt, phit:Dφ/Dt,   pt:∂φ/∂t, px:∂φ/∂x,  pBx:(∂φ/∂x)y=b
        for (int i = 0; i <= II; ++i) {
            px[i] = (dx[i] * dp[i] - dy[i] * dpn[i]) / RR[i];
            Xt[i] = c + px[i];
            Yt[i] = (dp[i] * dy[i] + dpn[i] * dx[i]) / RR[i];
            phit[i] = (dp[i] * dp[i] + dpn[i] * dpn[i]) / (2.0 * RR[i]) - YY[i];
            pt[i] = phit[i] - (dp[i] * dp[i] + dpn[i] * dpn[i]) / RR[i] - c * px[i];
            pBx[i] = (dXb[i] * dpB[i] - c * dYb[i] * dYb[i]) / RRb[i];  // 式(4.56)
        }
        pBx[-Imin] = -c;
        pBx[-Imin + IIc] = -c;


    // C**********************************************************
    // C*************** Second-order calculation *****************
    // C**********************************************************

    // c----時間微分項とx微分項のξによる１階，２階微分をここで計算しておく
    // c	dpt:∂2φ/∂ξ∂t,  dp2t:∂3φ/∂ξ2∂t,
    // c	dpx:∂2φ/∂ξ∂x,  dp2x:∂3φ/∂ξ2∂x,
    // c	dpBx:(∂2φ/∂ξ∂x)y=b,  dp2Bx:(∂3φ/∂ξ2∂x)y=b,


        for (int i = -1; i >= -4; --i) {
            pt[i] = 0.0;
            px[i] = 0.0;
        }

        for (int i = II + 1; i <= II + 4; ++i) {
            pt[i] = pt[II];
            px[i] = px[II];
        }


        for (int i = 0; i <= II; ++i) {
            dpt[i] = (3.0 * (pt[i - 4] - pt[i + 4]) - 32.0 * (pt[i - 3] - pt[i + 3]) +
                    168.0 * (pt[i - 2] - pt[i + 2]) - 672.0 * (pt[i - 1] - pt[i + 1])) /
                    840.0;
            dp2t[i] = (-9.0 * (pt[i - 4] + pt[i + 4]) + 128.0 * (pt[i - 3] + pt[i + 3]) -
                    1008.0 * (pt[i - 2] + pt[i + 2]) + 8064.0 * (pt[i - 1] + pt[i + 1]) -
                    14350.0 * pt[i]) /
                    5040.0;
            dpx[i] = (3.0 * (px[i - 4] - px[i + 4]) - 32.0 * (px[i - 3] - px[i + 3]) +
                    168.0 * (px[i - 2] - px[i + 2]) - 672.0 * (px[i - 1] - px[i + 1])) /
                    840.0;
            dp2x[i] = (-9.0 * (px[i - 4] + px[i + 4]) + 128.0 * (px[i - 3] + px[i + 3]) -
                    1008.0 * (px[i - 2] + px[i + 2]) + 8064.0 * (px[i - 1] + px[i + 1]) -
                    14350.0 * px[i]) /
                    5040.0;
        }

        for (int i = 0; i <= -Imin - 1; ++i) {
            pBxL[i] = pBx[i];
        }
        pBxL[-Imin] = -c;
        for (int i = -1; i >= -4; --i) {
            pBxL[i] = 0.0;
        }

        // Do i=0,-Imin-4
        for (int i = 0; i <= -Imin - 4; ++i) {
            dpBxL[i] = (3.0 * (pBxL[i - 4] - pBxL[i + 4]) - 32.0 * (pBxL[i - 3] - pBxL[i + 3]) +
                        168.0 * (pBxL[i - 2] - pBxL[i + 2]) - 672.0 * (pBxL[i - 1] - pBxL[i + 1])) /840.0;
            dp2BxL[i] = (-9.0 * (pBxL[i - 4] + pBxL[i + 4]) + 128.0 * (pBxL[i - 3] + pBxL[i + 3]) -
                        1008.0 * (pBxL[i - 2] + pBxL[i + 2]) + 8064.0 * (pBxL[i - 1] + pBxL[i + 1]) -
                        14350.0 * pBxL[i]) /5040.0;
        }

        for (int i = -Imin - 3; i <= -Imin - 1; ++i) {
            dpBxL[i] = (pBxL[i + 1] - pBxL[i - 1]) / 2.0;
            dp2BxL[i] = pBxL[i + 1] - 2.0 * pBxL[i] + pBxL[i - 1];
        }
        dpBxL[-Imin] = pBxL[-Imin] - pBxL[-Imin - 1];
        dp2BxL[-Imin] = pBxL[-Imin] - 2.0 * pBxL[-Imin - 1] + pBxL[-Imin - 2];


        for (int i = -Imin + 1; i <= -Imin + IIc - 1; ++i) {
            pBxC[i] = pBx[i];
        }
        pBxC[-Imin] = -c;
        pBxC[-Imin + IIc] = -c;

        int I = -Imin;
        dpBxC[I] = pBxC[I + 1] - pBxC[I];
        dp2BxC[I] = pBxC[I + 2] - 2.0 * pBxC[I + 1] + pBxC[I];

        for (int i = -Imin + 1; i <= -Imin + 3; ++i) {
            dpBxC[i] = (pBxC[i + 1] - pBxC[i - 1]) / 2.0;
            dp2BxC[i] = pBxC[i + 1] - 2.0 * pBxC[i] + pBxC[i - 1];
        }


        for (int i = -Imin + 4; i <= -Imin + IIc - 4; ++i) {
            dpBxC[i] = (3.0 * (pBxC[i - 4] - pBxC[i + 4]) - 32.0 * (pBxC[i - 3] - pBxC[i + 3]) +
                        168.0 * (pBxC[i - 2] - pBxC[i + 2]) - 672.0 * (pBxC[i - 1] - pBxC[i + 1])) /
                    840.0;
            dp2BxC[i] = (-9.0 * (pBxC[i - 4] + pBxC[i + 4]) + 128.0 * (pBxC[i - 3] + pBxC[i + 3]) -
                        1008.0 * (pBxC[i - 2] + pBxC[i + 2]) + 8064.0 * (pBxC[i - 1] + pBxC[i + 1]) -
                        14350.0 * pBxC[i]) /
                        5040.0;
        }

        for (int i = -Imin + IIc - 3; i <= -Imin + IIc - 1; ++i) {
            dpBxC[i] = (pBxC[i + 1] - pBxC[i - 1]) / 2.0;
            dp2BxC[i] = pBxC[i + 1] - 2.0 * pBxC[i] + pBxC[i - 1];
        }
        I = -Imin + IIc;
        dpBxC[I] = pBxC[I] - pBxC[I - 1];
        dp2BxC[I] = pBxC[I] - 2.0 * pBxC[I - 1] + pBxC[I - 2];


        for (int i = -Imin + IIc + 1; i <= II; ++i) {
            pBxR[i] = pBx[i];
        }
        pBxR[-Imin + IIc] = -c;

        for (int i = II + 1; i <= II + 4; ++i) {
            pBxR[i] = pBxR[II];
        }

        I = -Imin + IIc;
        dpBxR[I] = pBxR[I + 1] - pBxR[I];
        dp2BxR[I] = pBxR[I + 2] - 2.0 * pBxR[I + 1] + pBxR[I];

        for (int i = -Imin + IIc + 1; i <= -Imin + IIc + 3; ++i) {
            dpBxR[i] = (pBxR[i + 1] - pBxR[i - 1]) / 2.0;
            dp2BxR[i] = pBxR[i + 1] - 2.0 * pBxR[i] + pBxR[i - 1];
        }

        // Do i=-Imin+IIc+4,II
        for (int i = -Imin + IIc + 4; i <= II; ++i) {
            dpBxR[i] = (3.0 * (pBxR[i - 4] - pBxR[i + 4]) - 32.0 * (pBxR[i - 3] - pBxR[i + 3]) +
                        168.0 * (pBxR[i - 2] - pBxR[i + 2]) - 672.0 * (pBxR[i - 1] - pBxR[i + 1])) /
                    840.0;
            dp2BxR[i] = (-9.0 * (pBxR[i - 4] + pBxR[i + 4]) + 128.0 * (pBxR[i - 3] + pBxR[i + 3]) -
                        1008.0 * (pBxR[i - 2] + pBxR[i + 2]) + 8064.0 * (pBxR[i - 1] + pBxR[i + 1]) -
                        14350.0 * pBxR[i]) /
                        5040.0;
        }

        // Do i=0,-Imin-1
        for (int i = 0; i <= -Imin - 1; ++i) {
            dpBx[i] = dpBxL[i];
            dp2Bx[i] = dp2BxL[i];
        }
        dpBx[-Imin] = (dpBxL[-Imin] + dpBxC[-Imin]) / 2.0;
        dp2Bx[-Imin] = (dp2BxL[-Imin] + dp2BxC[-Imin]) / 2.0;

        // Do i=-Imin+1,-Imin+IIc-1
        for (int i = -Imin + 1; i <= -Imin + IIc - 1; ++i) {
            dpBx[i] = dpBxC[i];
            dp2Bx[i] = dp2BxC[i];
        }
        dpBx[-Imin + IIc] = (dpBxC[-Imin + IIc] + dpBxR[-Imin + IIc]) / 2.0;
        dp2Bx[-Imin + IIc] = (dp2BxC[-Imin + IIc] + dp2BxR[-Imin + IIc]) / 2.0;

        // Do i=-Imin+IIc+1,II
        for (int i = -Imin + IIc + 1; i <= II; ++i) {
            dpBx[i] = dpBxR[i];
            dp2Bx[i] = dp2BxR[i];
        }

    //  C****∂2φ/∂ν∂tに対する積分方程式を解く((4.50)(4.51)とその前のコメントを見よ) *****
    // c		          ((4.33)(4.34)と同形で未知変数だけ異なる)
    // c	aa1:(4.50)の∂2φ/∂ν∂tの係数,   bb1:(4.50)の(∂2φ/∂ξ∂t)y=bの係数
    // c	aa2:(4.51)の∂2φ/∂ν∂tの係数,   bb2:(4.51)の(∂2φ/∂ξ∂t)y=bの係数
    // c	cc1:(4.50)の非同次項を右辺に集めたもの, cc2:(4.50)の非同次項を左辺に集めたもの
    // C*************************************************************************************
    // c	非同次項cc1,cc2の計算（未知変数の係数であるaa1,aa2,bb1,bb2は，既に計算した∂φ/∂νに
    // c	                          対する積分方程式のそれと同一なので，ここでは計算不要）

        for (int i = 0; i <= II; ++i) {
            c1 = 0.0;
            for (int j = 0; j <= II; ++j) {
                c1 = zR00[i][j] * dpt[j] + c1; // (4.50)の非同次項
            }
            cc1[i] = c1 + dp2t[i]; // 5マルのdf/dξ(=∂3φ/∂ξ2∂t)の項を(4.50)の非同次項に加える
        }

        for (int i = 0; i <= -Imin - 1; ++i) {
            double c2 = 0.0;
            for (int j = 0; j <= II; ++j) {
                c2 = zI0b[i][j] * dpt[i] + c2; // (4.51)の非同次項
            }
            cc2[i] = c2;
        }

        // Do i=-Imin+1,-Imin+IIc-1
        for (int i = -Imin + 1; i <= -Imin + IIc - 1; ++i) {
            double c2 = 0.0;
            // Do j=0,II
            for (int j = 0; j <= II; ++j) {
                c2 = zI0b[i][j] * dpt[i] + c2; // (4.51)の非同次項
            }
            cc2[i - 1] = c2;
        }

        // Do i=-Imin+IIc+1,II
        for (int i = -Imin + IIc + 1; i <= II; ++i) {
            double c2 = 0.0;
            // Do j=0,II
            for (int j = 0; j <= II; ++j) {
                c2 = zI0b[i][j] * dpt[i] + c2; // (4.51)の非同次項
            }
            cc2[i - 2] = c2;
        }

        // cc1,cc2をcb(連立方程式の非同次項)に当てはめる
        // cb:(4.50)，(4.51)の非同次項から成る列ベクトル
        for (int i = 1; i <= IM; ++i) {
            cb[i] = cc1[i - 1];
        }

        for (int i = 1 + IM; i <= 2 * IM - 2; ++i) {
            cb[i] = cc2[i - 1 - IM];
        }

    // C*******ここよりGeneralized Conjugate Residual法による連立方程式求解******
    // c	                      (中身は分からなくてよい)
    // c	ca(I,J) :マトリックス係数 L
    // c	cb(I)   :非同次項 F
    // c	SOL(I)  :この連立方程式の解 V
    // c	Res(I)  :残差 R
    // c	dLP(I,nnn):nnnステップ目のL*P
    // c	dLR(I)  :L*R

        for (int i = 1; i <= IG; ++i) {
            SOL[i] = 0.0;
        }

        for (int i = 1; i <= IG; ++i) {
            Res[i] = cb[i];
            // Do j=1,IG
            for (int j = 1; j <= IG; ++j) {
                Res[i] = Res[i] - ca[i][j] * SOL[j];
            }
        }

        for (int i = 1; i <= IG; ++i) {
            P[i][1] = Res[i];
            dLP[i][1] = 0.0;
        }

        for (int i = 1; i <= IG; ++i) {

            for (int j = 1; j <= IG; ++j) {
                dLP[i][1] = dLP[i][1] + ca[i][j] * P[j][1];
            }
        }
        nnn = 0;

    // C************ Iteration **************
        while (true) {
            ++nnn;

            // (i)
            c1 = 0.0;
            dLPT[nnn] = 0.0;

            for (int i = 1; i <= IG; ++i) {
                c1 += Res[i] * dLP[i][nnn];
                dLPT[nnn] += pow(dLP[i][nnn], 2);
            }

            alpha = c1 / dLPT[nnn];

            for (int i = 1; i <= IG; ++i) {
                SOL[i] += alpha * P[i][nnn];
                Res[i] -= alpha * dLP[i][nnn];
            }

            // (ii)
            ResT = 0.0;
            for (int i = 1; i <= IG; ++i) {
                ResT += pow(Res[i], 2);
            }
            if (ResT < epsilon || nnn == IG) {
                break;
            }

            // (iii)
            for (int i = 1; i <= IG; ++i) {
                LRes[i] = 0;
                for (int j = 1; j <= IG; ++j) {
                    LRes[i] += ca[j][i] * Res[j];
                }
            }

    
            for (int i = 1; i <= IG; ++i) {
                dLR[i] = 0;
                for (int j = 1; j <= IG; ++j) {
                    dLR[i] += ca[i][j] * LRes[j];
                }
            }

            for (int j = 1; j <= nnn; ++j) {
                c1 = 0.0;
                for (int I = 1; I <= IG; ++I) {
                    c1 += dLR[I] * dLP[I][j];
                }
                beta[j] = -c1 / dLPT[j];
            }

            for (int I = 1; I <= IG; ++I) {
                P[I][nnn+1] = LRes[I];
                dLP[I][nnn+1] = dLR[I];
                for (int j = 1; j <= nnn; ++j) {
                    P[I][nnn+1] += beta[j] * P[I][j];
                    dLP[I][nnn+1] += beta[j] * dLP[I][j];
                }
            }
        }


    // C********* 残差ReCを直接計算して十分小さいかをチェックする **********
        if (n % nP == 0) {
            double ReCM = 0.0;
            for (int i = 1; i <= IG; ++i) {
                ReC[i] = cb[i];
                for (int j = 1; j <= IG; ++j) {
                    ReC[i] = ReC[i] - ca[i][j] * SOL[j];
                }
                ReCM = max(fabs(ReC[i]), ReCM);
            }
            cout << " Iteration =" << nnn << "  Max Residual(dpt) =" << static_cast<double>(ReCM) << endl;
        }

        // dpnt=∂2φ/∂ν∂t.
        for (int i = 0; i <= II; ++i) {
            dpnt[i] = SOL[i + 1];
        }

    // C*****∂2φ/∂ν∂xに対する積分方程式を解く((4.53)(4.54)とその前のコメントを見よ) *****
    // c    ((4.33)の同形と，(4.34)ではなく(4.32)の実部をとった式．未知変数も異なるので注意!)
    // c	aa1:(4.53)の∂2φ/∂ν∂xの係数,   bb1:(4.53)の(∂2φ/∂ν∂x)y=bの係数
    // c	aa2:(4.54)の∂2φ/∂ν∂xの係数,   bb2:(4.54)の(∂2φ/∂ν∂x)y=bの係数
    // c	cc1:(4.53)の非同次項を右辺に集めたもの, cc2:(4.54)の非同次項を左辺に集めたもの
    // C**************************************************************************************
    // c	連立方程式の未知変数[∂2φ/∂ν∂x，(∂2φ/∂ν∂x)y=b]の係数を計算

        for (int i = 0; i <= II; ++i) {
            for (int j = 0; j <= II; ++j) {
                aa1[i][j] = -zI00[i][j];
            }
            for (int j = -Imin+1; j <= -Imin+IIc-1; ++j) {
                bb1[i][j + Imin - 1] = zIb0[i][j];
            }
        }

        for (int i = -Imin+1; i <= -Imin+IIc-1; ++i) {
            for (int j = 0; j <= II; ++j) {
                aa2[i + Imin - 1][j] = -zI0b[i][j];
            }
            for (int j = -Imin+1; j <= -Imin+IIc-1; ++j) {
                bb2[i + Imin - 1][j + Imin - 1] = zIbb[i][j];
            }
        }

        // Adjusting diagonal elements
        for (int i = 0; i <= II; ++i) {
            aa1[i][i] = aa1[i][i] - M_PI;
        }
        for (int i = 0; i <= IIc-2; ++i) {
            bb2[i][i] = bb2[i][i] - M_PI;
        }

        // Non-homogeneous term calculation
        for (int i = 0; i <= II; ++i) {
            double c1 = 0.0;
            for (int j = 0; j <= II; ++j) {
                c1 += zR00[i][j] * dpx[j] - zRb0[i][j] * dpBx[j]; // Non-homogeneous term of Eq. (4.53)
            }
            // Contributions from terms involving df/dξ (Eq. 5) and (∂²φ/∂ν∂x)y=b (Eqs. 4.58, 4.59)
            cc1[i] = c1 + dp2x[i] - zIb0[i][-Imin] * (-dpBxL[-Imin] / 2.0) - zIb0[i][-Imin+IIc] * (dpBxR[-Imin+IIc] / 2.0);
        }

        for (int i = -Imin+1; i <= -Imin+IIc-1; ++i) {
            double c2 = 0.0;
            for (int j = 0; j <= II; ++j) {
                c2 += zR0b[i][j] * dpx[j] - zRbb[i][j] * dpBx[j]; // Non-homogeneous term of Eq. (4.54)
            }
            // Contributions from terms involving df/dξ (Eq. 5) and (∂²φ/∂ν∂x)y=b (Eqs. 4.58, 4.59)
            cc2[i + Imin - 1] = c2 - dp2Bx[i] - zIbb[i][-Imin] * (-dpBxL[-Imin] / 2.0) - zIbb[i][-Imin+IIc] * (dpBxR[-Imin+IIc] / 2.0);
        }

        for (int i = 1; i <= IM; ++i) {
            for (int j = 1; j <= IM; ++j) {
                ca[i][j] = aa1[i - 1][j - 1];
            }
        }
        for (int i = IM + 1; i <= IM + IIc - 1; ++i) {
            for (int j = 1; j <= IM; ++j) {
                ca[i][j] = aa2[i - 1 - IM][j - 1];
            }
        }
        for (int i = 1; i <= IM; ++i) {
            for (int j = IM + 1; j <= IM + IIc - 1; ++j) {
                ca[i][j] = bb1[i - 1][j - 1 - IM];
            }
        }
        for (int i = IM + 1; i <= IM + IIc - 1; ++i) {
            for (int j = IM + 1; j <= IM + IIc - 1; ++j) {
                ca[i][j] = bb2[i - 1 - IM][j - 1 - IM];
            }
        }

        // Assigning values of cc1 and cc2 to cb
        for (int i = 1; i <= IM; ++i) {
            cb[i] = cc1[i - 1];
        }
        for (int i = 1 + IM; i <= IM + IIc - 1; ++i) {
            cb[i] = cc2[i - 1 - IM];
        }



    // C*******ここよりGeneralized Conjugate Residual法による連立方程式求解******
    // c	                      (中身は分からなくてよい)
    // c	ca(I,J) :マトリックス係数 L
    // c	cb(I)   :非同次項 F
    // c	SOL(I)  :この連立方程式の解 V
    // c	Res(I)  :残差 R
    // c	dLP(I,nnn):nnnステップ目のL*P
    // c	dLR(I)  :L*R

        IG = IM + IIc - 1;

        // Initializing SOL and Res arrays
        for (int i = 1; i <= IG; ++i) {
            SOL[i] = 0.0;
        }

        // Copying values from cb to Res
        for (int i = 1; i <= IG; ++i) {
            Res[i] = cb[i];
            for (int j = 1; j <= IG; ++j) {
                Res[i] -= ca[i][j] * SOL[j];
            }
        }

        // Initializing P and dLP arrays
        for (int i = 1; i <= IG; ++i) {
            P[i][1] = Res[i];
            dLP[i][1] = 0.0;
        }

        // Calculating dLP
        for (int i = 1; i <= IG; ++i) {
            for (int j = 1; j <= IG; ++j) {
                dLP[i][1] += ca[i][j] * P[j][1];
            }
        }

        nnn = 0;

    // C************ Iteration **************
        while (true) {
            ++nnn;

            // (i)
            c1 = 0.0;
            dLPT[nnn] = 0.0;

            for (int i = 1; i <= IG; ++i) {
                c1 += Res[i] * dLP[i][nnn];
                dLPT[nnn] += pow(dLP[i][nnn], 2);
            }

            alpha = c1 / dLPT[nnn];

            for (int i = 1; i <= IG; ++i) {
                SOL[i] += alpha * P[i][nnn];
                Res[i] -= alpha * dLP[i][nnn];
            }

            // (ii)
            ResT = 0.0;
            for (int i = 1; i <= IG; ++i) {
                ResT += pow(Res[i], 2);
            }
            if (ResT < epsilon || nnn == IG) {
                break;
            }

            // (iii)
            for (int i = 1; i <= IG; ++i) {
                LRes[i] = 0;
                for (int j = 1; j <= IG; ++j) {
                    LRes[i] += ca[j][i] * Res[j];
                }
            }

    
            for (int i = 1; i <= IG; ++i) {
                dLR[i] = 0;
                for (int j = 1; j <= IG; ++j) {
                    dLR[i] += ca[i][j] * LRes[j];
                }
            }

            for (int j = 1; j <= nnn; ++j) {
                c1 = 0.0;
                for (int I = 1; I <= IG; ++I) {
                    c1 += dLR[I] * dLP[I][j];
                }
                beta[j] = -c1 / dLPT[j];
            }

            for (int I = 1; I <= IG; ++I) {
                P[I][nnn+1] = LRes[I];
                dLP[I][nnn+1] = dLR[I];
                for (int j = 1; j <= nnn; ++j) {
                    P[I][nnn+1] += beta[j] * P[I][j];
                    dLP[I][nnn+1] += beta[j] * dLP[I][j];
                }
            }
        }

    // C*********** 残差ReCを直接計算して十分小さいかをチェックする ************

        if (n % nP == 0) {
            for (int i = 0; i <= IG; ++i) {
                ReC[i] = cb[i];
                for (int j = 0; j <= IG; ++j) {
                    ReC[i] -= ca[i][j] * SOL[j];
                }
            }

            double ReCM = 0.0;
            for (int i = 0; i <= IG; ++i) {
                ReCM = max(abs(ReC[i]), ReCM);
            }

            cout << " Iteration =" << nnn << "  Max Residual(dpx) =" << ReCM << endl;
        }

    // dpnx=∂²φ/∂ν∂x.
        for (int i = 0; i <= II; ++i) {
            dpnx[i] = SOL[i + 1];
        }

        dpnBx[-Imin] = -dpBxL[-Imin];
        dpnBx[-Imin + IIc] = dpBxR[-Imin + IIc];

        for (int i = 0; i <= -Imin - 1; ++i) {
            dpnBx[i] = 0.0;
        }

        for (int i = -Imin + 1; i <= -Imin + IIc - 1; ++i) {
            dpnBx[i] = SOL[II + i + 1 + Imin];
        }

        for (int i = -Imin + IIc + 1; i <= II; ++i) {
            dpnBx[i] = 0.0;
        }

        // Xtt: D²X/Dt²,  Ytt: D²η/Dt²,  ptt: D²φ/Dt²
        for (int i = 0; i <= II; ++i) {
            Xtt[i] = (dx[i] * dpt[i] - dy[i] * dpnt[i] +
                    Xt[i] * (dx[i] * dpx[i] - dy[i] * dpnx[i]) +
                    Yt[i] * (dy[i] * dpx[i] + dx[i] * dpnx[i])) / RR[i];

            Ytt[i] = (dy[i] * dpt[i] + dx[i] * dpnt[i] +
                    Xt[i] * (dy[i] * dpx[i] + dx[i] * dpnx[i]) -
                    Yt[i] * (dx[i] * dpx[i] - dy[i] * dpnx[i])) / RR[i];

            ptt[i] = (Xt[i] - c) * Xtt[i] + Yt[i] * Ytt[i] - Yt[i];
        }

        // X, η, φを１ステップ時間発展させる（(4.39)-(4.41)を見よ）
        for (int i = 0; i <= II; ++i) {
            X[i] += Xt[i] * dt + Xtt[i] * dt * dt / 2.0;
            YY[i] += Yt[i] * dt + Ytt[i] * dt * dt / 2.0;
            ph[i] += phit[i] * dt + ptt[i] * dt * dt / 2.0;
        }

        // Filtering (計算を安定させるための平滑化処理．本計算では不要と思われるので，
    //                    最初の計算条件の設定のところでnfに大きな値を入れておく)
        if (n % nf == 0) {
            cout << "filtering applied" << endl;
            for (int i = -1; i >= -2; --i) {
                X[i] = X[i + 1] - dX0;
                YY[i] = 0.0;
                ph[i] = 0.0;
            }

            for (int i = II + 1; i <= II + 2; ++i) {
                X[i] += X[i] - X[i - 1];
                YY[i] = YY[II];
                ph[i] = ph[II];
            }

            for (int i = 0; i <= II; ++i) {
                X[i] = (1.0 / 16.0) * (-X[i - 2] + 4.0 * X[i - 1] + 10.0 * X[i] + 4.0 * X[i + 1] - X[i + 2]);
                YY[i] = (1.0 / 16.0) * (-YY[i - 2] + 4.0 * YY[i - 1] + 10.0 * YY[i] + 4.0 * YY[i + 1] - YY[i + 2]);
                ph[i] = (1.0 / 16.0) * (-ph[i - 2] + 4.0 * ph[i - 1] + 10.0 * ph[i] + 4.0 * ph[i + 1] - ph[i + 2]);
            }
        }

    // 室内実験(H=9cm)で，地形からの初期距離が W[m] の地点における波形を保存
    // 何ステップごとに保存するか
        if (n % 2 == 0) {
            // 測定地点における水面鉛直変位Ywを内挿で求める (Wx:地形の無次元相対位置)
            for (int i = 1; i <= II; ++i) {
                if (X[i] >= Wx1) {
                    Yw1 = (YY[i] * (Wx1 - X[i - 1]) + YY[i - 1] * (X[i] - Wx1)) / (X[i] - X[i - 1]);
                    break;
                }
            }
        

    // c	実際の時間(秒)，水面の無次元鉛直変位，水面の有次元鉛直変位[mm]，
    // c	                測定地点に対する地形の相対位置(x座標[m])を保存

            w1ofs <<setprecision(16)<< t*sqrt(H/9.8) <<" "<< Yw1 <<" "<< Yw1*H*1000 <<" "<< Wx1*H <<endl;

            for (int i = 1; I <= II; ++I) {
                if (X[i] >= Wx2) {
                    Yw2 = (YY[i] * (Wx2 - X[i - 1]) + YY[i - 1] * (X[i] - Wx2)) / (X[i] - X[i - 1]);
                    break;
                }
            }

            w2ofs << t*sqrt(H/9.8) <<" "<< Yw2 <<" "<< Yw2*H*1000 <<" "<< Wx2*H <<endl;

        }


	// 描画処理のお膳立て
	glClear(GL_COLOR_BUFFER_BIT);
	 glClearColor(1.0, 1.0, 1.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.5, 3, -3, 3, -1.0, 1.0);

	// 座標軸を描画
	glBegin(GL_LINES);
	glColor3f(0.5f, 0.5f, 0.8f);
	glVertex2d(0,  100);
	glVertex2d(0,  -100);
	glVertex2d(100,  0);
	glVertex2d(-100,  0);
	glEnd();
    

    for(int i= 0; i <= II ; ++i){
        glBegin(GL_LINES);
        glColor3f(0.0f, 0.0f, 0.0f);
        glVertex2d(X[i]*0.09, YY[i] * 90);
        glVertex2d(X[i+1]*0.09,  YY[i+1] * 90);
        glEnd();
    }

	glFlush();
	glutSwapBuffers();


// c***********************************************************************
// c****************** Save the data **************************************
// c***********************************************************************

//         if( n % ns == 0 ) {
// // c----------------------------------------------------------------------
// // c             時間tによってファイル名が変わるようにする
// // c----------------------------------------------------------------------

//             if (t - 0.00001 <= 20.0) {
//                 filename4 = "020 ";
//             } else if (t - 0.00001 <= 40.0) {
//                 filename4 = "040 ";
//             } else if (t - 0.00001 <= 60.0) {
//                 filename4 = "060 ";
//             } else if (t - 0.00001 <= 80.0) {
//                 filename4 = "080 ";
//             } else if (t - 0.00001 <= 100.0) {
//                 filename4 = "100 ";
//             } else if (t - 0.00001 <= 150.0) {
//                 filename4 = "150 ";
//             } else if (t - 0.00001 <= 200.0) {
//                 filename4 = "200 ";
//             } else if (t - 0.00001 <= 300.0) {
//                 filename4 = "300 ";
//             } else if (t - 0.00001 <= 400.0) {
//                 filename4 = "400 ";
//             } else if (t - 0.00001 <= 500.0) {
//                 filename4 = "500 ";
//             } else if (t - 0.00001 <= 600.0) {
//                 filename4 = "600 ";
//             } else if (t - 0.00001 <= 700.0) {
//                 filename4 = "700 ";
//             } else if (t - 0.00001 <= 800.0) {
//                 filename4 = "800 ";
//             } else if (t - 0.00001 <= 900.0) {
//                 filename4 = "900 ";
//             } else if (t - 0.00001 <= 1000.0) {
//                 filename4 = "999 ";
//             } else {
//                 filename4 = "Lar ";
//             }

// //  c--- 結果（x座標，水面の無次元鉛直変位η，無次元速度ポテンシャルφ）の保存
// // c	まずはIIなどのパラメータ保存       

//             filename41 = "II " + filename4;
//             ofstream ofs41(filename41);
//             ofs41 << t << " " << Xmin << " " << Xmax << " " << Wx1 << " " << Wx2 << " " << II <<endl;
//             ofs41.close();

//     // c	計算結果の保存
//             filename42 = filename4 + filename;
//             ofstream ofs42(filename42);
//             for( int i = 0 ;i <= II ; ++i){
//                 ofs42 << X[i] << " " << YY[i] << " " << dpnx[i] <<endl;
//             }
//             ofs42.close();

//             filename43 = "ca " + filename;
//             ofstream ofs43(filename43);
//             for( int i = 0 ;i <= II ; ++i){
//                 ofs43 << Xb[i] << " " << dpnBx[i] <<endl;
//             }
//             ofs43.close();
        // }


        if (t >= tlimit - 0.0000001) {
        // 2000に戻って計算を継続する
        break;
        }

        if (L >= LL) {
    // --- LがLL未満なら，1000に戻って次ケースの時間発展計算を行う
        break;
        }
    }      

}