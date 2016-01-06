#ifndef SIGNALCLASS_H
#define SIGNALCLASS_H
class basesignal
{
 public:
    basesignal(int * digital , int fc1,int Ts,float RB,double n0 ,float K);
    /*
       digital  传入的数字基带序列；
       fs1      载波1频率；
       n0       平均噪声密度；
       Ts       码元长度；
       RB       码元速率；
       K        信道衰减系数
    */

    void taxis();
    void squarewave();
    void ASKwave();
    void FSKwave();
    void QPskwave();
    void peask();
    void pefsk();
    void peQpsk();
    void carrywave1();
    void carrywave2();
    void spectrumfunc();
    void ASKspectrum();
    void FSKspectrum();
    void QPSKspectrum();
    /*
        taxis();        时间轴函数
        squarewave();   数字基带序列化成方波函数
        ASKwave();      ask调制后的波形函数
        FSKwave();      FSK
        QPskwave();     QPsk
        peask();        ask误码率
        pefsk();        fsk误码率
        peQpsk();       QPsk误码率
        carrywave1();   载波函数1
        carrywave2();   载波函数2
        spectrumfunc(); 用于计算的频谱函数
        ASKspectrum();  ASK频谱
        FSKspectrum();  FSK频谱
        QPSKspectrum(); QPSK频谱
    */
public:

    int  *digital;

    float * T_axis;
    float * Square;
    float * Sine;
    float * Sine_11;
    float * Sine_01;
    float * Sine_10;
    float * Sine_00;
    float * Sine2;
    float * Ask;
    float * fsk;
    float * qpsk;
    float * n;
    float * spec_Square;
    float * spec_Ask;
    float * spec_Fsk;
    float * spec_QPsk;

    int  size;
    int Frequency1;
    int Frequency2;
    int Ts;

    float  n0;
    float RB;
    float K;
    float r;
    float pe;
    float upe;

    /*
        n0      噪声功率密度
        RB      码元速率
        K       信道衰减系数
        r       信噪比
        pe      相干解调误码率
        unpe    非相干解调误码率
    */

}



#endif // SIGNALCLASS_H
