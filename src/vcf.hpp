#ifndef VCF_H
#define VCF_H

#include "synthdata.hpp"
#include "lv2plugin.hpp"

using namespace LV2;

static const char p_uri[] = "http://avwlv2.sourceforge.net/plugins/avw/vcf";

enum p_port_enum {
  p_in,
  p_freqPort,
  p_expFM,
  p_linFM,
  p_resonancePort,
  p_vcfType,
  p_inputGain,
  p_freq,
  p_expFMGain,
  p_linFMGain,
  p_resonance,
  p_resonanceGain,
  p_out,
  p_n_ports
};

static const peg_data_t p_ports[] = {
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
  { 0, 7, 0, 0, 0, 0 },
  { 0, 10, 1, 0, 0, 0 },
  { 0, 10, 5, 0, 0, 0 },
  { 0, 10, 0, 0, 0, 0 },
  { 0, 10, 0, 0, 0, 0 },
  { 0.8, 1, 0.01, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0 },
  { -3.40282e+38, 3.40282e+38, -3.40282e+38, 0, 0, 0 },
};


#define MIN_FREQ                         20
#define MAX_FREQ                      20000
#define MAX_FREQ2                     22000

enum vcfTypeEnum { VCF_LR, VCF_LPF, VCF_HPF, VCF_BPF_I, VCF_BPF_II, VCF_NF, VCF_MOOG1, VCF_MOOG2 };

class Vcf : public Plugin<Vcf>
{
private:
    float gain, freq, resonance, dBgain;
    float vcfExpFMGain, vcfLinFMGain, resonanceGain;
    float freq_const;
    float fInvertRandMax ;
    float pi2_rate,inv2_rate;
    float freq_tune, gain_linfm, qr, moog_f, revMoog, moog2times;
    double b_noise;
    double in_lr[5], buf_lr[5];
    double in_lpf[5], buf_lpf[5];
    double in_hpf[5], buf_hpf[5];
    double in_bpf_i[5], buf_bpf_i[5];
    double in_bpf_ii[5], buf_bpf_ii[5];
    double in_nf[5], buf_nf[5];
    double in_moog1[5], buf_moog1[5];
    double in_moog2[5], buf_moog2[5];

    SynthData *synthdata;

    int vcfType;
    void initBuf();
    //Port *port_M_in, *port_M_resonance, *port_M_freq, *port_M_exp, *port_M_lin, *port_out;
//public:
    float *inData, *resonanceData, *freqData, *linFMData, *expFMData;

public:
    Vcf(double rate);
    void run(uint32_t nframes);

//public slots:
//    void initBuf(int index);
};

#endif
