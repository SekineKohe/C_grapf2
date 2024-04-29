#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include <deque>
using namespace std;

void deviceOpen();

void deviceClose();

/// set D/A analog voltage
void daOut(float v1, float v2);

/// set cart motor drive voltage
void cartOut(float v);

/// reset encoder count
void encReset(int index);

/// get raw encoder count
int encIn(int index);

/// get cart encoder count
int cartEnc();

/// get pole encoder count
int poleEnc();

/// get cart position in [m]
double cartPos();

/// get cart velocity in [m/s]
double cartVel();

/// get pole angle in [rad]
double poleAngle();

/// get pole angular velocity in [rad/s]
double poleAngvel();

/// set cart drive force in [N]
void setCartForce(double u);

/// get elapsed time in micro seconds
int getUs();

/// update buffer for calculating velocities
void updateBuffer();
extern deque<double>  cartBuf;
extern deque<double>  poleBuf;