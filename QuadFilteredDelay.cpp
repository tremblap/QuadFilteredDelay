#include "QuadFilteredDelay.h"
#include "IPlug_include_in_plug_src.h"
#include "IControls.h"

QuadFilteredDelay::QuadFilteredDelay(const InstanceInfo& info)
: Plugin(info, MakeConfig(kNumParams, kNumPresets)) {
  GetParam(kFreqLL)->InitFrequency("Freq-L-L", 440., 20., 10000.);
  GetParam(kFreqLR)->InitFrequency("Freq-L-R", 440., 20., 10000.);
  GetParam(kFreqLLs)->InitFrequency("Freq-L-Ls", 440., 20., 10000.);
  GetParam(kFreqLRs)->InitFrequency("Freq-L-Rs", 440., 20., 10000.);
  GetParam(kFreqRL)->InitFrequency("Freq-R-L", 440., 20., 10000.);
  GetParam(kFreqRR)->InitFrequency("Freq-R-R", 440., 20., 10000.);
  GetParam(kFreqRLs)->InitFrequency("Freq-R-Ls", 440., 20., 10000.);
  GetParam(kFreqRRs)->InitFrequency("Freq-R-Rs", 440., 20., 10000.);
  GetParam(kFreqLsL)->InitFrequency("Freq-Ls-L", 440., 20., 10000.);
  GetParam(kFreqLsR)->InitFrequency("Freq-Ls-R", 440., 20., 10000.);
  GetParam(kFreqLsLs)->InitFrequency("Freq-Ls-Ls", 440., 20., 10000.);
  GetParam(kFreqLsRs)->InitFrequency("Freq-Ls-Rs", 440., 20., 10000.);
  GetParam(kFreqRsL)->InitFrequency("Freq-Rs-L", 440., 20., 10000.);
  GetParam(kFreqRsR)->InitFrequency("Freq-Rs-R", 440., 20., 10000.);
  GetParam(kFreqRsLs)->InitFrequency("Freq-Rs-Ls", 440., 20., 10000.);
  GetParam(kFreqRsRs)->InitFrequency("Freq-Rs-Rs", 440., 20., 10000.);
  GetParam(kDurLL)->InitSeconds("Delay-L-L", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurLR)->InitSeconds("Delay-L-R", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurLLs)->InitSeconds("Delay-L-Ls", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurLRs)->InitSeconds("Delay-L-Rs", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurRL)->InitSeconds("Delay-R-L", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurRR)->InitSeconds("Delay-R-R", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurRLs)->InitSeconds("Delay-R-Ls", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurRRs)->InitSeconds("Delay-R-Rs", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurLsL)->InitSeconds("Delay-Ls-L", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurLsR)->InitSeconds("Delay-Ls-R", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurLsLs)->InitSeconds("Delay-Ls-Ls", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurLsRs)->InitSeconds("Delay-Ls-Rs", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurRsL)->InitSeconds("Delay-Rs-L", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurRsR)->InitSeconds("Delay-Rs-R", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurRsLs)->InitSeconds("Delay-Rs-Ls", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kDurRsRs)->InitSeconds("Delay-Rs-Rs", DELDEF, DELMIN, DELMAX, 0.001);
  GetParam(kFeedL)->InitDouble("Feedback-L", 0., 0., 100.0, 0.001, "%");
  GetParam(kFeedR)->InitDouble("Feedback-R", 0., 0., 100.0, 0.001, "%");
  GetParam(kFeedLs)->InitDouble("Feedback-Ls", 0., 0., 100.0, 0.001, "%");
  GetParam(kFeedRs)->InitDouble("Feedback-Rs", 0., 0., 100.0, 0.001, "%");
  GetParam(kPingPong)->InitDouble("PingPong-Width", 0., 0., 100.0, 0.001, "%");
  GetParam(kFilterWidth)->InitDouble("Filter-Width", 1., 0.01, 5.0, 0.01, "Octaves");
  GetParam(kFilterQ)->InitDouble("Filter-Q", 1., 0.001, 5.0, 0.001, "Q");
  
  mMakeGraphicsFunc = [&]() {
    return MakeGraphics(*this, PLUG_WIDTH, PLUG_HEIGHT, PLUG_FPS, GetScaleForScreen(PLUG_WIDTH, PLUG_HEIGHT));
  };
  
  mLayoutFunc = [&](IGraphics* pGraphics) {
    pGraphics->AttachCornerResizer(EUIResizerMode::Scale, false);
    pGraphics->AttachPanelBackground(COLOR_GRAY);
    pGraphics->LoadFont("Roboto-Regular", ROBOTO_FN);
    const IRECT b = pGraphics->GetBounds().GetPadded(-10.f);
    for (int i = 0;i<kNumParams;i++){
//      pGraphics->AttachControl(new ITextControl(b.GetGridCell(i, 1, kNumParams, 5), GetParam(i)->GetName(), IText(14.f).WithAlign(EAlign::Center)));
      pGraphics->AttachControl(new ITextControl(b.GetGridCell((i * 9)+1, kNumParams, 9, EDirection::Horizontal, 3), GetParam(i)->GetName(), IText(14.f).WithAlign(EAlign::Center)));
//      pGraphics->AttachControl(new ICaptionControl(b.GetGridCell(i, 3, kNumParams, 5), i, IText(14.f).WithAlign(EAlign::Center)));
      pGraphics->AttachControl(new ICaptionControl(b.GetGridCell((i * 9)+4, kNumParams, 9, EDirection::Horizontal, 4), i, IText(14.f).WithAlign(EAlign::Center)));
    };
  };
}

void QuadFilteredDelay::ProcessBlock(sample** inputs, sample** outputs, int nFrames)
{
  const int nChans = NOutChansConnected();
  
  //silence all output so I can overdub
  for (int s = 0; s < nFrames; s++) {
    for (int c = 0; c < nChans; c++) {
      outputs[c][s] = 0;
    }
  }
  
  if (nChans >= 4) {
    for (int s = 0; s < nFrames; s++) {
      for (int c = 0; c < 4; c++) {
        for (int i = 0; i < 4; i++) {
          long index = (c*4) + i;//which of the 16 instances in order
          long indexf1 = (c*4) + (i^3);//getting the other 2 channels the dirty way
          long indexf2 = (c*4) + (i^2);//getting the other 2 channels the dirty way
          outputs[i][s] += mpBuffer[index][mRead[index]];
          mpBuffer[index][mWrite] = QuadFilterProcess((inputs[c][s] + (mFeed[c] * ((mpBuffer[indexf1][mRead[indexf1]] * mPingPong) + (mpBuffer[indexf2][mRead[indexf2]] * (1.0 - mPingPong))))), index);
          
          mRead[index]++;
          if(mRead[index] >= mBufferSize){
            mRead[index] = 0;
          }
        }
      }
      
      mWrite++;
      if(mWrite >= mBufferSize){
        mWrite = 0;
      }
    }
  }
}

void QuadFilteredDelay::OnReset()
{
  mBufferSize = DELMAX * GetSampleRate();
  
  if(mpBuffer) {
    for (int i = 0;i<16;i++)
      delete[] mpBuffer[i];
  }
  delete[] mpBuffer;
  
  mpBuffer = new double*[16];
  for (int i = 0;i<16;i++) {
    mpBuffer[i] = new double[mBufferSize];
  }
  resetDelay();
  computeParam();
}

void QuadFilteredDelay::resetDelay()
{
  if(mpBuffer)
  {
    for (int i = 0;i<16;i++) {
      memset(mpBuffer[i], 0, mBufferSize * sizeof(double));
    }
  }
  mWrite = 0;
  memset(mFilterBufs, 0, 256 * sizeof(double));
}

void QuadFilteredDelay::OnParamChange(int paramIdx)
{
  computeParam();
}

void QuadFilteredDelay::computeParam()
{
  //get the 16 delay lengths and store it in sample
  for (int i = 0;i<16;i++) {
    mDurSam[i] = long(GetParam(kDurLL+i)->Value() * GetSampleRate());
    mRead[i] = mWrite - mDurSam[i];
    if(mRead[i] < 0) {
      mRead[i] += mBufferSize;
    }
  }
  
  //get the 4 feedback values
  for (int i = 0;i<4;i++){
    mFeed[i] = GetParam(kFeedL+i)->Value() / 100.0;
  }
  
  //get the pingpong width
  mPingPong = GetParam(kPingPong)->Value() / 100.0;
  
  //get the Q and the samplingrate dependant constant
  double piDsr = (2. * SUPERPI) / GetSampleRate();
  double a0, a1, a2, b0, b1, b2;
  //compute the coefficients for each filter
  for (int i = 0;i<16;i++) {
    long index = i*10;
    //get the highpass freq
    double omega = piDsr * GetParam(kFreqLL+i)->Value();
    double tsin = sin(omega);
    double tcos = cos(omega);
    double alpha = tsin / (2.0 * GetParam(kFilterQ)->Value());
    a0 = 1;
    b0 = (1.0 + tcos) /2.0;
    b1 = -(1.0 + tcos);
    b2 = (1.0 + tcos) /2.0;
    a0 = 1.0 + alpha;
    a1 = -2.0 * tcos;
    a2 = 1.0 - alpha;
    a0 = 1/a0;
    mFilterCoefs[index]= b0 * a0;
    mFilterCoefs[index+1]= b1 * a0;
    mFilterCoefs[index+2]= b2 * a0;
    mFilterCoefs[index+3]= a1 * a0;
    mFilterCoefs[index+4]= a2 * a0;
    
    //derive the lowpass freq
    omega = omega * pow(2,GetParam(kFilterWidth)->Value());
    tsin = sin(omega);
    tcos = cos(omega);
    alpha = tsin / (2.0 * GetParam(kFilterQ)->Value());
    a0 = 1;
    b0=(1.0-tcos)/2.0;
    b1=1.0-tcos;
    b2=(1.0-tcos)/2.0;
    a0=1.0+alpha;
    a1=-2.0*tcos;
    a2=1.0-alpha;
    a0 = 1/a0;
    mFilterCoefs[index+5]= b0 * a0;
    mFilterCoefs[index+6]= b1 * a0;
    mFilterCoefs[index+7]= b2 * a0;
    mFilterCoefs[index+8]= a1 * a0;
    mFilterCoefs[index+9]= a2 * a0;
  }
}

double QuadFilteredDelay::QuadFilterProcess(double in0, long index){
  long coefIndex = index*10;
  long bufIndex = index*16;
  //first hp
  double yn = mFilterCoefs[coefIndex] * in0
  + mFilterCoefs[coefIndex+1] * mFilterBufs[bufIndex]
  + mFilterCoefs[coefIndex+2] * mFilterBufs[bufIndex+1]
  - mFilterCoefs[coefIndex+3] * mFilterBufs[bufIndex+2]
  - mFilterCoefs[coefIndex+4] * mFilterBufs[bufIndex+3];
  mFilterBufs[bufIndex+1] = mFilterBufs[bufIndex];
  mFilterBufs[bufIndex] = in0;
  mFilterBufs[bufIndex+3] = mFilterBufs[bufIndex+2];
  mFilterBufs[bufIndex+2] = yn;
  double out = yn;
  //second hp same coeffs
  yn = mFilterCoefs[coefIndex] * out
  + mFilterCoefs[coefIndex+1] * mFilterBufs[bufIndex+4]
  + mFilterCoefs[coefIndex+2] * mFilterBufs[bufIndex+5]
  - mFilterCoefs[coefIndex+3] * mFilterBufs[bufIndex+6]
  - mFilterCoefs[coefIndex+4] * mFilterBufs[bufIndex+7];
  mFilterBufs[bufIndex+5] = mFilterBufs[bufIndex+4];
  mFilterBufs[bufIndex+4] = out;
  mFilterBufs[bufIndex+7] = mFilterBufs[bufIndex+6];
  mFilterBufs[bufIndex+6] = yn;
  out = yn;
  //first lp
  yn = mFilterCoefs[coefIndex+5] * out
  + mFilterCoefs[coefIndex+6] * mFilterBufs[bufIndex+8]
  + mFilterCoefs[coefIndex+7] * mFilterBufs[bufIndex+9]
  - mFilterCoefs[coefIndex+8] * mFilterBufs[bufIndex+10]
  - mFilterCoefs[coefIndex+9] * mFilterBufs[bufIndex+11];
  mFilterBufs[bufIndex+9] = mFilterBufs[bufIndex+8];
  mFilterBufs[bufIndex+8] = out;
  mFilterBufs[bufIndex+11] = mFilterBufs[bufIndex+10];
  mFilterBufs[bufIndex+10] = yn;
  out = yn;
  //second lp
  yn = mFilterCoefs[coefIndex+5] * out
  + mFilterCoefs[coefIndex+6] * mFilterBufs[bufIndex+12]
  + mFilterCoefs[coefIndex+7] * mFilterBufs[bufIndex+13]
  - mFilterCoefs[coefIndex+8] * mFilterBufs[bufIndex+14]
  - mFilterCoefs[coefIndex+9] * mFilterBufs[bufIndex+15];
  mFilterBufs[bufIndex+13] = mFilterBufs[bufIndex+12];
  mFilterBufs[bufIndex+12] = out;
  mFilterBufs[bufIndex+15] = mFilterBufs[bufIndex+14];
  mFilterBufs[bufIndex+14] = yn;
  return(yn);
}

QuadFilteredDelay::~QuadFilteredDelay()
{
  if(mpBuffer) {
    for (int i = 0;i<16;i++)
      delete[] mpBuffer[i];
  }
  delete[] mpBuffer;
}

//some help indeed from https://larzeitlin.github.io/DDL/
//filters plundered from my butt (ha!)
