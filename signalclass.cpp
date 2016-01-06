#include <signalclass.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define PI 3.14
#define am     1
basesignal::basesignal(int * digital , int fc1,int Ts,float RB,double n0 ,float K ){
    this->Frequency1 = fc1;
    this->Frequency2 = 2*fc1;
    this->digital = new char[8];
    this->n0 = n0;
    this->RB = RB;
    this->Ts = Ts;
    if(K>1)
        K = 1;
    this->K = K;
    memcpy(this->digital,digital,8);


}
void basesignal::taxis(){                   //时间坐标轴函数
    this->T_axis=new float[8*Ts];
    for (int i=0 ; i<8*Ts;i++)
    {
        this->T_axis[i]=((float)i/Ts);
    }
    this->size = Ts*8;
}

void basesignal::squarewave(){           //方波
    this->Square = new float[8*Ts];
    for (int i=0;i<8;i++)
    {
        for (int j=0;j<Ts;j++)
        {
            if(digital[i]==0)
                Square[j+i*Ts]=0;
            else
                Square[j+i*Ts]=1;
        }
    }
}
void basesignal::carrywave1(){             //载波
    this->Sine = new float[8*Ts];
    for (int i=0;i<8*Ts;i++)
    {
        Sine[i] = sin((float)i/Ts*PI*2*Frequency1);
        Sine_11[i] = sin((float)i/Ts*PI*2*Frequency1+PI/4);
        Sine_01[i] = sin((float)i/Ts*PI*2*Frequency1+3*PI/4);
        Sine_00[i] = sin((float)i/Ts*PI*2*Frequency1+5*PI/4);
        Sine_10[i] = sin((float)i/Ts*PI*2*Frequency1+7*PI/4);
    }
}
void basesignal::carrywave2()              //载波2
{

    this->Sine2 = new float[size];

    for(int i=0;i<size;i++)
    {
        this->Sine2[i]=sin((float)i/Ts*PI*2*Frequency2);
    }

}
void basesignal::ASKwave(){
    this->Ask = new float[8*Ts];
    for (int i=0;i<size;i++)
    {
        Ask[i] = Sine[i]*Square[i];
    }
}
void basesignal::FSKwave(){
    this->fsk = new float[size];
    for(int i=0;i<size;i++)
    {
        if(Square[i] == 0)
            this-> fsk[i]=this-> Sine2[i];
        else
            this-> fsk[i]=this-> Sine[i];
    }

}
void basesignal::QPskwave(){
    int temp;
    this->qpsk = new float[size];

    for(int j=0;j<8;j+=2)
    {
       temp = digital[j]*10+digital[j+1];
          for(int i=0;i<2*Ts;i++)
          {
            if(temp == 00)
                qpsk[i+j*Ts] = Sine_00[i+j*Ts]*Square[i+j*Ts];
            else if(temp == 01)
                qpsk[i+j*Ts] = Sine_01[i+j*Ts]*Square[i+j*Ts];
            else if(temp == 11)
                qpsk[i+j*Ts] = Sine_11[i+j*Ts]*Square[i+j*Ts];
            else if(temp == 10)
                qpsk[i+j*Ts] = Sine_10[i+j*Ts]*Square[i+j*Ts];
          }
     }

}

void basesignal::SNR(){
    float B,delta;                //带宽，噪声功率
    B = 2*RB;
    delta = n0*B;
    r = (K*am)^2/(delta*2);

}

void basesignal::peask(){          //ask误码率


    pe = 1/(sqrt(PI*r))*exp(-r/4);
    unpe = 0.5*exp(-r/4);

}

void basesignal::pefsk(){           //fsk误码率

    pe = 1/(sqrt(2*PI*r))*exp(-r/4);
    unpe=0.5*exp(-r/2);
}
void basesignal::peQpsk()           //QPsk误码率
{

    pe = exp(-r*pow(sin(PI/4),2));
    unpe = 0;
}
void basesignal::spectrumfunc(){                                         //频谱方波函数
    this->spec_Square = new float [size];
    for (int i=0;i<8*Ts;i++)
    {
        int j= i-4*Ts;
        if(j==0)
        spec_Square[i] = 1.5*Ts;                                        //模拟载波抽样点脉冲
        else
        spec_Square[i] = Ts*sin((float)j/Ts*PI)/((float)j/Ts*PI);       //sa函数
    }
}

void basesignal::ASKspectrum(){                                          //ask频谱函数
    this->spec_Ask = new float [size];
    for (int i=0;i<8*Ts;i++){
        int j= i-(4-this->Frequency1)*Ts;
        int k= i-(4+this->Frequency1)*Ts;
        if(j==0 || k==0)
        spec_Ask[i] = 1.5*Ts;
        else
        spec_Ask[i] = pow((Ts/16)*(sin((float)j/Ts*PI)/((float)j/Ts*PI)),2)
                     +pow((Ts/16)*(sin((float)k/Ts*PI)/((float)k/Ts*PI)),2)
    }
}

void basesignal::FSKspectrum(){                                      //fsk频谱函数
    this->spec_Fsk = new float [size];
    for (int i=0;i<8*Ts;i++){
        int j= i-(4-this->Frequency1)*Ts;
        int k= i-(4+this->Frequency1)*Ts;
        int m= i-(4-this->Frequency2)*Ts;
        int n= i-(4+this->Frequency2)*Ts;
        if(j==0 || k==0 || m==0 || n==0)
        spec_Fsk[i] = 1.5*Ts;
        else
        spec_Fsk[i] = pow(Ts/16*(sin((float)j/Ts*PI)/((float)j/Ts*PI)),2)
                     +pow(Ts/16*(sin((float)k/Ts*PI)/((float)k/Ts*PI)),2)
                     +pow(Ts/16*(sin((float)m/Ts*PI)/((float)m/Ts*PI)),2)
                     +pow(Ts/16*(sin((float)n/Ts*PI)/((float)n/Ts*PI)),2);
    }
}
void basesignal::QPSKspectrum(){
    this->spec_QPsk = new float [size];
    for (int i=0;i<8*Ts;i++){
        int j= i-(4-this->Frequency1)*Ts;
        int k= i-(4+this->Frequency1)*Ts;
        if(j==0 || k==0)
        spec_QPsk[i] = Ts;
        else
        spec_QPsk[i] = pow((Ts/16)*(sin((float)j/Ts*PI)/((float)j/Ts*PI)),2)
                      +pow((Ts/16)*(sin((float)k/Ts*PI)/((float)k/Ts*PI)),2)
                      +pow((Ts/16)*(sin((float)j/Ts*PI)/((float)j/Ts*PI)),2)
                      +pow((Ts/16)*(sin((float)k/Ts*PI)/((float)k/Ts*PI)),2);

}
