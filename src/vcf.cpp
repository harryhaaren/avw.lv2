#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

#include <lvtk-1/lvtk/plugin.hpp>

using namespace lvtk;

#include "synthdata.hpp"

#include "vcf.hpp"

Vcf::Vcf(double rate):
Plugin<Vcf>(p_n_ports)
{
	synthdata = new SynthData(1);
	//QString qs;

	//setGeometry(MODULE_NEW_X, MODULE_NEW_Y, MODULE_VCF_WIDTH, MODULE_VCF_HEIGHT);
	gain = 1.0;
	freq = 5;
	resonance = 0.8;
	dBgain = 3.0;
	initBuf();
	freq_const = 2.85f / 20000.0f;
	fInvertRandMax= 1.0f/(float)RAND_MAX ;
	b_noise = 19.1919191919191919191919191919191919191919;
	pi2_rate = 2.0f * M_PI / rate; // how often changes the rate? I guess once on init, or?
	inv2_rate = 2.0 / rate;// this double seems unnecessary

	//port_M_in = new Port(tr("In"), PORT_IN, 0, this);
	//port_M_freq = new Port(tr("Freq"), PORT_IN, 1, this);
	//port_M_exp = new Port(tr("Exp. FM"), PORT_IN, 2, this);
	//port_M_lin = new Port(tr("Lin. FM"), PORT_IN, 3, this);
	//port_M_resonance = new Port(tr("Resonance"), PORT_IN, 4, this);
	//cv.out_off = 130;
	//port_out = new Port(tr("Out"), PORT_OUT, 0, this);

	//QStringList vcfTypeNames;
	//vcfTypeNames << tr("Resonant Lowpass");
	//vcfTypeNames << tr("Lowpass");
	//vcfTypeNames << tr("Highpass");
	//vcfTypeNames << tr("Bandpass I");
	//vcfTypeNames << tr("Bandpass II");
	//vcfTypeNames << tr("Notch");
	//vcfTypeNames << tr("24 dB Lowpass I");
	//vcfTypeNames << tr("24 dB Lowpass II");
	//configDialog->addComboBox(tr("VCF Type"), vcfType, vcfTypeNames);
	//QObject::connect(configDialog->midiComboBoxList.at(0)->comboBox, SIGNAL(highlighted(int)), this, SLOT(initBuf(int)));
	//configDialog->addSlider(tr("Input Gain"), gain, 0, 10);
	//configDialog->addSlider(tr("Frequency"), freq, 0, 10);
	//configDialog->addSlider(tr("Exp. FM Gain"), vcfExpFMGain, 0, 10);
	//configDialog->addSlider(tr("Lin. FM Gain"), vcfLinFMGain, 0, 10);
	//configDialog->addSlider(tr("Resonance"), resonance, 0.01, 1);
	//configDialog->addSlider(tr("Resonance Gain"), resonanceGain, 0, 1);
}

void Vcf::initBuf()
{
	//enum vcfTypeEnum { VCF_LR, VCF_LPF, VCF_HPF, VCF_BPF_I, VCF_BPF_II, VCF_NF, VCF_MOOG1, VCF_MOOG2 };
	int l2;

	for (l2 = 0; l2 < 5; ++l2)
	{
		buf_lr[l2] = 0;
		in_lr[l2] = 0;
	}
	for (l2 = 0; l2 < 5; ++l2)
	{
		buf_lpf[l2] = 0;
		in_lpf[l2] = 0;
	}
	for (l2 = 0; l2 < 5; ++l2)
	{
		buf_hpf[l2] = 0;
		in_hpf[l2] = 0;
	}
	for (l2 = 0; l2 < 5; ++l2)
	{
		buf_bpf_i[l2] = 0;
		in_bpf_i[l2] = 0;
	}
	for (l2 = 0; l2 < 5; ++l2)
	{
		buf_bpf_ii[l2] = 0;
		in_bpf_ii[l2] = 0;
	}
	for (l2 = 0; l2 < 5; ++l2)
	{
		buf_nf[l2] = 0;
		in_nf[l2] = 0;
	}
	for (l2 = 0; l2 < 5; ++l2)
	{
		buf_moog1[l2] = 0;
		in_moog1[l2] = 0;
	}
	for (l2 = 0; l2 < 5; ++l2)
	{
		buf_moog2[l2] = 0;
		in_moog2[l2] = 0;
	}
}

void Vcf::run(uint32_t nframes)
{
	unsigned int l2;
	double t1, t2, fa, fb, q0, f, q, pbis, iv_sin, iv_cos, iv_alpha, a0, a1, a2, b0, b1, b2;

	inData = p(p_in);					//port_M_in->getinputdata();
	freqData = p(p_freqPort);			//port_M_freq->getinputdata();
	expFMData = p(p_expFM);				//port_M_exp->getinputdata();
	linFMData = p(p_linFM);				//port_M_lin->getinputdata();
	resonanceData = p(p_resonancePort);	//port_M_resonance->getinputdata();

	vcfExpFMGain = *p(p_expFMGain);
	vcfLinFMGain = *p(p_linFMGain);
	resonance = *p(p_resonance);
	resonanceGain = 0;
	gain = *p(p_inputGain);
	freq = *p(p_freq);
	vcfType = floor(*p(p_vcfType));

	switch (vcfType)
	{
	case VCF_LR:
	{
		double b_noiseout;
		int i_noise;
		q0 = resonance;
		freq_tune = 5.0313842f + freq;
		gain_linfm = 1000.0f * vcfLinFMGain;

		for (l2 = 0; l2 < nframes; ++l2)
		{
			f = freq_const * (synthdata->exp2_table(freq_tune + freqData[l2] + vcfExpFMGain * expFMData[l2]) + gain_linfm * linFMData[l2]);
			if (f < 0.0) f = 0.0;
			else if (f > 0.99) f = 0.99;
			q = q0 + resonanceGain * resonanceData[l2];
			if (q < 0.01) q = 0.01;
			else if (q > 1.0) q = 1.0;
			fa = 1.0 - f;
			fb = q * (1.0 + (1.0 / fa));
			// generate dither (?) noise
			b_noise = b_noise * b_noise;
			i_noise = (int)b_noise;
			b_noise = b_noise - i_noise;
			b_noiseout = b_noise - 1.5;// was 0.5
			b_noise = b_noise + 19.0;

			buf_lr[0] = fa * buf_lr[0] + f * (gain * inData[l2] + fb * (buf_lr[0] - buf_lr[1]) + 0.00001 * b_noiseout);
			buf_lr[1] = fa * buf_lr[1] + f * buf_lr[0];
			p(p_out)[l2] = buf_lr[1];
		}
		break;
	}
	case VCF_LPF:
	{
		double temp;

		q0 = resonance;

		freq_tune = 5.0313842f + freq;
		gain_linfm = 1000.0f * vcfLinFMGain;
		for (l2 = 0; l2 < nframes; ++l2)
		{
			f = synthdata->exp2_table(freq_tune + freqData[l2] + vcfExpFMGain * expFMData[l2]) + gain_linfm * linFMData[l2];
			if (f < MIN_FREQ) f = MIN_FREQ;
			else if (f > MAX_FREQ) f = MAX_FREQ;
			q = q0 + resonanceGain * resonanceData[l2];
			if (q < 0.01) q = 0.01;
			else if (q > 1.0) q = 1.0;
			iv_sin = sin(pi2_rate * f);
			iv_cos = cos(pi2_rate * f);
			iv_alpha = iv_sin/(64.0 * q);
			b0 = (1.0 - iv_cos) * 0.5;
			b1 = 1.0 - iv_cos;
			b2 = b0;
			a0 = 1.0 + iv_alpha;
			a1 = -2.0 * iv_cos;
			a2 = 1.0 - iv_alpha;
			temp = 1.0 / a0 * (b0 * gain * inData[l2] + b1 * buf_lpf[0] + b2 * buf_lpf[1] - a1 * buf_lpf[2] - a2 * buf_lpf[3]);
			p(p_out)[l2]=temp;
			buf_lpf[1] = buf_lpf[0];
			buf_lpf[0] = gain * inData[l2];
			buf_lpf[3] = buf_lpf[2];
			buf_lpf[2] = temp;//data[0][l2];
		}
		break;
	}
	case VCF_HPF:
	{
		double temp;

		q0 = resonance;
		freq_tune = 5.0313842f + freq;
		gain_linfm = 1000.0f * vcfLinFMGain;
		for (l2 = 0; l2 < nframes; ++l2)
		{
			f = synthdata->exp2_table(freq_tune + freqData[l2] + vcfExpFMGain * expFMData[l2]) + gain_linfm * linFMData[l2];
			if (f < MIN_FREQ) f = MIN_FREQ;
			else if (f > MAX_FREQ) f = MAX_FREQ;
			q = q0 + resonanceGain * resonanceData[l2];
			if (q < 0.01) q = 0.01;
			else if (q > 1.0) q = 1.0;
			iv_sin = sin(pi2_rate * f);
			iv_cos = cos(pi2_rate * f);
			iv_alpha = iv_sin/(64.0 * q);
			b0 = (1.0 + iv_cos) * 0.5;
			b1 = - 1.0 - iv_cos;
			b2 = b0;
			a0 = 1.0 + iv_alpha;
			a1 = -2.0 * iv_cos;
			a2 = 1.0 - iv_alpha;
			temp = 1.0 / a0 * (gain * b0 * inData[l2] + b1 * buf_hpf[0] + b2 * buf_hpf[1] - a1 * buf_hpf[2] - a2 * buf_hpf[3]);
			p(p_out)[l2]=temp;
			buf_hpf[1] = buf_hpf[0];
			buf_hpf[0] = gain * inData[l2];
			buf_hpf[3] = buf_hpf[2];
			buf_hpf[2] = temp;//data[0][l2];
		}
		break;
	}
	case VCF_BPF_I:
	{
		double temp;

		q0 = resonance;
		freq_tune = 5.0313842f + freq;
		gain_linfm = 1000.0f * vcfLinFMGain;
		for (l2 = 0; l2 < nframes; ++l2)
		{
			f = synthdata->exp2_table(freq_tune + freqData[l2] + vcfExpFMGain * expFMData[l2]) + gain_linfm * linFMData[l2];
			if (f < MIN_FREQ) f = MIN_FREQ;
			else if (f > MAX_FREQ) f = MAX_FREQ;
			q = q0 + resonanceGain * resonanceData[l2];
			if (q < 0.01) q = 0.01;
			else if (q > 1.0) q = 1.0;
			iv_sin = sin(pi2_rate * f);
			iv_cos = cos(pi2_rate * f);
			iv_alpha = iv_sin/(64.0 * q);
			b0 = q * iv_alpha;
			b1 = 0.0;
			b2 = -q * iv_alpha;
			a0 = 1.0 + iv_alpha;
			a1 = -2.0 * iv_cos;
			a2 = 1.0 - iv_alpha;

			temp = 1.0 / a0 * (gain * b0 * inData[l2] + b1 * buf_bpf_i[0] + b2 * buf_bpf_i[1] - a1 * buf_bpf_i[2] - a2 * buf_bpf_i[3]);
			p(p_out)[l2]=temp;
			buf_bpf_i[1] = buf_bpf_i[0];
			buf_bpf_i[0] = gain * inData[l2];
			buf_bpf_i[3] = buf_bpf_i[2];
			buf_bpf_i[2] = temp;//data[0][l2];
		}
		break;
	}
	case VCF_BPF_II:
	{
		double temp;
		q0 = resonance;
		freq_tune = 5.0313842f + freq;
		gain_linfm = 1000.0f * vcfLinFMGain;
		for (l2 = 0; l2 < nframes; ++l2)
		{
			f = synthdata->exp2_table(freq_tune + freqData[l2] + vcfExpFMGain * expFMData[l2]) + gain_linfm * linFMData[l2];
			if (f < MIN_FREQ) f = MIN_FREQ;
			else if (f > MAX_FREQ) f = MAX_FREQ;
			q = q0 + resonanceGain * resonanceData[l2];
			if (q < 0.01) q = 0.01;
			else if (q > 1.0) q = 1.0;
			iv_sin = sin(pi2_rate * f);
			iv_cos = cos(pi2_rate * f);
			iv_alpha = iv_sin/(64.0 * q);
			b0 = iv_alpha;
			b1 = 0.0;
			b2 = -iv_alpha;
			a0 = 1.0 + iv_alpha;
			a1 = -2.0 * iv_cos;
			a2 = 1.0 - iv_alpha;
			temp = 1.0 / a0 * (gain * b0 * inData[l2] + b1 * buf_bpf_ii[0] + b2 * buf_bpf_ii[1] - a1 * buf_bpf_ii[2] - a2 * buf_bpf_ii[3]);
			p(p_out)[l2] = temp;
			buf_bpf_ii[1] = buf_bpf_ii[0];
			buf_bpf_ii[0] = gain * inData[l2];
			buf_bpf_ii[3] = buf_bpf_ii[2];
			buf_bpf_ii[2] = temp;//data[0][l2];
		}
		break;
	}
	case VCF_NF:
	{
		double temp;
		q0 = resonance;
		freq_tune = 5.0313842f + freq;
		gain_linfm = 1000.0f * vcfLinFMGain;
		for (l2 = 0; l2 < nframes; ++l2)
		{
			f = synthdata->exp2_table(freq_tune + freqData[l2] + vcfExpFMGain * expFMData[l2]) + gain_linfm * linFMData[l2];
			if (f < MIN_FREQ) f = MIN_FREQ;
			else if (f > MAX_FREQ) f = MAX_FREQ;
			q = q0 + resonanceGain * resonanceData[l2];
			if (q < 0.01) q = 0.01;
			else if (q > 1.0) q = 1.0;
			iv_sin = sin(pi2_rate * f);
			iv_cos = cos(pi2_rate * f);
			iv_alpha = iv_sin/(64.0 * q);
			b0 = 1.0;
			b1 = -2.0 * iv_cos;
			b2 = 1.0;
			a0 = 1.0 + iv_alpha;
			a1 = -2.0 * iv_cos;
			a2 = 1.0 - iv_alpha;

			temp = 1.0 / a0 * (gain * b0 * inData[l2] + b1 * buf_nf[0] + b2 * buf_nf[1] - a1 * buf_nf[2] - a2 * buf_nf[3]);
			p(p_out)[l2] = temp;// conversion
			buf_nf[1] = buf_nf[0];
			buf_nf[0] = gain * inData[l2];
			buf_nf[3] = buf_nf[2];
			buf_nf[2] = temp;//data[0][l2];
		}
		break;
	}
	case VCF_MOOG1:
	{// Timo Tossavainen version
		double b_noiseout;
		int i_noise;
		q0 = resonance;
		freq_tune = 5.0313842f + freq;
		gain_linfm = 1000.0f * vcfLinFMGain;
		for (l2 = 0; l2 < nframes; ++l2)
		{
			f = synthdata->exp2_table(freq_tune + freqData[l2] + vcfExpFMGain * expFMData[l2]) + gain_linfm * linFMData[l2];
			if (f < MIN_FREQ) f = MIN_FREQ;
			else if (f > MAX_FREQ2) f = MAX_FREQ2;
			q = q0 + resonanceGain * resonanceData[l2];
			if (q < 0.01) q = 0.01;
			else if (q > 1.0) q = 1.0;
			fa = inv2_rate * f;
			moog_f = fa * 1.16f;
			revMoog = 1.0f - moog_f;
			moog2times= moog_f * moog_f;
			fb = 4.1 * q * (1.0 - 0.15 * moog2times);
			// generate dither (?) noise
			b_noise = b_noise * b_noise;
			i_noise = (int)b_noise;
			b_noise = b_noise - i_noise;

			b_noiseout = b_noise - 1.5; // was - 0.5 now with - 1.0

			b_noise = b_noise + 19.0;
			in_moog1[0] = gain * inData[l2] + 0.000001 * b_noiseout;//((float)rand() * fInvertRandMax - 1.0);
			in_moog1[0] -= fb * buf_moog1[4];
			in_moog1[0] *=0.35013 * (moog2times * moog2times);
			buf_moog1[1] = in_moog1[0] + 0.3 * in_moog1[1] + revMoog * buf_moog1[1];
			in_moog1[1] = in_moog1[0];
			buf_moog1[2] = buf_moog1[1] + 0.3 * in_moog1[2] + revMoog * buf_moog1[2];
			in_moog1[2] = buf_moog1[1];
			buf_moog1[3] = buf_moog1[2] + 0.3 * in_moog1[3] + revMoog * buf_moog1[3];
			in_moog1[3] = buf_moog1[2];
			buf_moog1[4] = buf_moog1[3] + 0.3 * in_moog1[4] + revMoog * buf_moog1[4];
			in_moog1[4] = buf_moog1[3];
			p(p_out)[l2] = buf_moog1[4];
		}
		break;
	}
	case VCF_MOOG2:                       // Paul Kellet version
	{
		double b_noiseout;
		int i_noise;

		q0 = resonance;
		freq_tune = 5.0313842f + freq;
		gain_linfm = 1000.0f * vcfLinFMGain;
		for (l2 = 0; l2 < nframes; ++l2)
		{
			f = synthdata->exp2_table(freq_tune + freqData[l2] + vcfExpFMGain * expFMData[l2]) + gain_linfm * linFMData[l2];
			if (f < MIN_FREQ) f = MIN_FREQ;
			else if (f > MAX_FREQ2) f = MAX_FREQ2;
			qr = q0 + resonanceGain * resonanceData[l2];
			if (qr < 0.01) qr = 0.01;
			else if (qr > 1.0) qr = 1.0;
			fb = inv2_rate * f;
			q = 1.0 - fb;
			pbis = fb + 0.8 * fb * q;
			fa = pbis + pbis - 1.0;
			q = qr * (1.0 + 0.5 * q * (1.0 - q + 5.6 * q * q));
			// generate dither (?) noise
			b_noise = b_noise * b_noise;
			i_noise = (int)b_noise;
			b_noise = b_noise - i_noise;

			b_noiseout = b_noise - 1.5;// was 0.5

			b_noise = b_noise + 19.0;

			in_moog2[0] = gain * inData[l2] + 0.000001 * b_noiseout;//* ((float)rand() * fInvertRandMax - 1.0);
			in_moog2[0] -= q * buf_moog2[4];
			if (in_moog2[0] < -1.0) in_moog2[0] = -1.0;
			if (in_moog2[0] > 1.0) in_moog2[0] = 1.0;
			t1 = buf_moog2[1];
			buf_moog2[1] = (in_moog2[0] + buf_moog2[0]) * pbis - buf_moog2[1] * fa;
			t2 = buf_moog2[2];
			buf_moog2[2] = (buf_moog2[1] + t1) * pbis - buf_moog2[2] * fa;
			t1 = buf_moog2[3];
			buf_moog2[3] = (buf_moog2[2] + t2) * pbis - buf_moog2[3] * fa;
			buf_moog2[4] = (buf_moog2[3] + t1) * pbis - buf_moog2[4] * fa;
			buf_moog2[4] -= buf_moog2[4] * buf_moog2[4] * buf_moog2[4] * 0.166667;
			buf_moog2[0] = in_moog2[0];
			p(p_out)[l2] = buf_moog2[4];
		}
		break;
	}
	}
}

static int _ = Vcf::register_class("http://avwlv2.sourceforge.net/plugins/avw/vcf");

