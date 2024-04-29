#include <device.h>

#include <stdio.h>

#include <windows.h>
#include <FbiDa.h>
#include <IFUcnt.h>

HANDLE hDa = 0;
HANDLE hCounter = 0;

const size_t   bufferSize = 50;
deque<double>  timeBuf;
deque<double>  cartBuf;
deque<double>  poleBuf;

const double pulsePerRotation    = 1024.0;

// cart encoder gear radius [m]
const double cartEncoderGearRadius = 14.82975 / 1000.0;

// motor set current per D/A voltage
const double currentPerVoltage = 2.0 / 5.0;

// motor torque constant [Nm/A]
const double motorTorqueConstant = 7.44 / 1000.0;

// motor pinion gear radius [m]
const double cartDriveGearRadius = 6.35 / 1000.0;

// motor gearhead reduction ratio
const double motorGearRatio = 3.71;

// cart thrust force per D/A voltage
const double cartForcePerVoltage = currentPerVoltage * motorTorqueConstant * motorGearRatio / cartDriveGearRadius;

const double pi = 3.14159265358979;

extern void TimerCallback();

void CALLBACK EventProc(INT nChannel, DWORD dwEvent, DWORD dwUser){
	TimerCallback();
}

void deviceOpen(){
	// open D/A
	hDa = DaOpen("FBIDA1");
	if(hDa == INVALID_HANDLE_VALUE){
		printf("da board open failed\n");
		return;
	}
	
	// open Counter
	hCounter = UcntOpen("IFUCNT1");
	if(hCounter == INVALID_HANDLE_VALUE){
		printf("counter board open failed");
		return;
	}

	// �J�E���^�{�[�h�̐ݒ�
	int ret;
	//  CH1�`4�����Z�b�g
	ret = UcntReset(hCounter, 0x0F, IFUCNT_RESET_DEVICE | IFUCNT_RESET_COUNTER);
	//  CH1,CH2���p���X�J�E���g���[�h
	ret = UcntSetPulseCountMode(hCounter, 1, IFUCNT_COUNT_PHASE_1, 0, 0);
	ret = UcntSetPulseCountMode(hCounter, 2, IFUCNT_COUNT_PHASE_1, 0, 0);
	ret = UcntStartCount(hCounter, 0x0F, IFUCNT_CMD_START);
	
	//  CH3���^�C�}���[�h (10*100ns = 1us)
	ret = UcntSetTimerMode(hCounter, 3, 10);
	//  �v�����[�h�l��0�ɐݒ�
	ret = UcntSetLoadData(hCounter, 3, 0);
	//  CH3�̔�r�lA��1000�ɐݒ�
	DWORD cmpA[4] = {0,0,0,0};
	DWORD cmpB[4] = {0,0,0,0};
	cmpA[2] = 1000;
	ret = UcntSetComparator(hCounter, 0x01<<2, 0, cmpA, cmpB);
	ret = UcntSetComparatorConfig(hCounter, 3, 0x01<<2, 0x01<<3, 0);
	ret = UcntSetEventMask(hCounter, 3, 0x01<<4);
	UCNTEVENTREQ ev;
	ev.hWnd        = 0;
	ev.uMessage[0] = 0;
	ev.uMessage[1] = 0;
	ev.uMessage[2] = 0;
	ev.uMessage[3] = 0;
	ev.hEvent      = 0;
	ev.pCallBackProc = (PUCNTCALLBACK)&EventProc;
	ev.dwUser        = 0;
	ret = UcntSetEvent(hCounter, &ev);

	// CH4���t���[�����j���O�^�C�}
	ret = UcntSetTimerMode(hCounter, 4, 10);

}

void deviceClose(){
	daOut(0.0f, 0.0f);

	DaClose(hDa);
	//UcntClose(hCounter);
}

/*
  DA����d���o��

  ESCON���[�^�h���C�o���̐ݒ�ŁC�A�i���O�d�������[�^��d���ɂ��Ă���̂�
  ���̂܂܎w�肵���d���l�����[�^�d���ƂȂ�

  ���d���ō������ɋ쓮

 */
void daOut(float v1, float v2){
	DASMPLCHREQ	daSmplChReq[2];
	WORD data[2];

	// �g�p����D/A�{�[�h�̎d�l���}10V�Ȃ̂�DA_10V�ȊO���ƃG���[�ɂȂ�
	daSmplChReq[0].ulChNo  = 1;
	daSmplChReq[0].ulRange = DA_10V;
	daSmplChReq[1].ulChNo  = 2;
	daSmplChReq[1].ulRange = DA_10V;

	if(v1 < -10.0f) v1 = -10.0f;
	if(v1 >  10.0f) v1 =  10.0f;
	if(v1 < -10.0f) v1 = -10.0f;
	if(v1 >  10.0f) v1 =  10.0f;

	// convert voltage to word value
	data[0] = 0xffff * (v1 + 10.0f)/20.0f;
	data[1] = 0xffff * (v2 + 10.0f)/20.0f;
	
	int ret = DaOutputDA(hDa, 2, daSmplChReq, data);
	if( ret != DA_ERROR_SUCCESS ){
		printf("DaOutputDA failed\n");	
	}
}

// ��Ԃ��E�ɐ��̌����ɋ쓮����
void cartOut(float v){
	daOut(-v, 0.0f);
}

/*
	�G���R�[�_�J�E���g�l

	0 : �U��q�G���R�[�_�@���v��肪��
	1 : ��ԃG���R�[�_�@����������

 */
void encReset(int index){
	int ret;
	ret = UcntReset(hCounter, 0x01 << index, IFUCNT_RESET_COUNTER);
}
int encIn(int index){
	DWORD cnt[4];
	UcntReadCounter(hCounter, 0x0f, cnt);

	return cnt[index];
}

int cartEnc(){
	return -encIn(1);
}

int poleEnc(){
	return encIn(0);
}

double cartPos(){
	return (2.0 * pi * cartEncoderGearRadius) * (cartEnc() / pulsePerRotation);
}

double cartVel(){
	if (cartBuf.size() < bufferSize)
		return 0.0;
	int dt = timeBuf.back() - timeBuf.front();
	int dx = cartBuf.back() - cartBuf.front();
	return (2.0 * pi * cartEncoderGearRadius / pulsePerRotation) * (double)dx / (0.000001 * (double)dt);
}

double poleAngle(){
	return (2.0 * pi) * (poleEnc() / pulsePerRotation);
}

double poleAngvel(){
	if ( poleBuf.size() < bufferSize)
		return 0.0;
	int dt = timeBuf.back() - timeBuf.front();
	int dx = poleBuf.back() - poleBuf.front();
	return (2.0 * pi / pulsePerRotation) * (double)dx / (0.000001 * (double)dt);
}

void setCartForce(double u){
	cartOut(u / cartForcePerVoltage);
}

void updateBuffer(){
	timeBuf.push_back(getUs());
	cartBuf.push_back(cartEnc());
	poleBuf.push_back(poleEnc());
	if (timeBuf.size() > bufferSize)
		timeBuf.pop_front();
	if (cartBuf.size() > bufferSize)
		cartBuf.pop_front();
	if (poleBuf.size() > bufferSize)
		poleBuf.pop_front();
}

int getUs(){
	return encIn(3);
}
