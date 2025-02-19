#pragma once

#define DELDEF 0.1
#define DELMIN 0.001
#define DELMAX 1.5
#define SUPERPI 3.141592653589793238462643383279502884L

#include "IPlug_include_in_plug_hdr.h"

const int kNumPresets = 1;

enum EParams
{
  kFreqLL = 0,
  kFreqLR,
  kFreqLLs,
  kFreqLRs,
  kFreqRL,
  kFreqRR,
  kFreqRLs,
  kFreqRRs,
  kFreqLsL,
  kFreqLsR,
  kFreqLsLs,
  kFreqLsRs,
  kFreqRsL,
  kFreqRsR,
  kFreqRsLs,
  kFreqRsRs,
  kDurLL,
  kDurLR,
  kDurLLs,
  kDurLRs,
  kDurRL,
  kDurRR,
  kDurRLs,
  kDurRRs,
  kDurLsL,
  kDurLsR,
  kDurLsLs,
  kDurLsRs,
  kDurRsL,
  kDurRsR,
  kDurRsLs,
  kDurRsRs,
  kFeedL,
  kFeedR,
  kFeedLs,
  kFeedRs,
  kPingPong,
  kFilterWidth,
  kFilterQ,
  kNumParams
};

using namespace iplug;
using namespace igraphics;

class QuadFilteredDelay final : public Plugin
{
public:
  QuadFilteredDelay(const InstanceInfo& info);
  ~QuadFilteredDelay();

  void ProcessBlock(sample** inputs, sample** outputs, int nFrames) override;
  void OnReset() override;
  void OnParamChange(int paramIdx) override;
  void computeParam();
  void resetDelay();
  double QuadFilterProcess(double in,long index);
  
private:
  double** mpBuffer = NULL;
  long mWrite = 0;
  long mBufferSize = 0;
  float mPingPong = 0;
  long mDurSam[16];
  long mRead[16];
  double mFeed[4];
  double mFilterCoefs[160];//5 coeffs x 16 filters x 2 freq (lp/hp)
  double mFilterBufs[256];//4 buffers x 16 x 2 freq x 2 filters
};
