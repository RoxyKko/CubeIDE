#ifndef __AD9528_CONFIG_H
#define __AD9528_CONFIG_H
#include "type.h"
//
//static ad9528pll1Settings_t clockPll1Settings = {
//	30720000,
//	1,
//	3,
//	0,
//	1,
//	0,
//	122880000,
//	2,
//	4
//};
//
//static ad9528pll2Settings_t clockPll2Settings = {
//	3,
//	30
//};
//
//static ad9528outputSettings_t clockOutputSettings = {
//	53237,
//	{0,0,0,2,0,0,0,0,0,0,0,0,2,0},
//	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//	{0,0,0,0,0,0,0,0,0,0,0,0,0,0},
//	{5,5,5,5,5,5,5,5,5,5,5,5,5,5},
//	{245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000}
//};
//
//static ad9528sysrefSettings_t clockSysrefSettings = {
//	0,
//	2,
//	0,
//	0,
//	0,
//	0,
//	512
//};
//
static ad9528spiSettings_t spiSettings = {
	.LSBFirst      = 0,
	.autoIncAddrUp = 0,
	.fourWireMode  = 1
};


static ad9528pll1Settings_t pll1Settings = {
	.refA_Frequency_Hz = 0,
	.refA_Divider      = 4,
	.refA_bufferCtrl   = 0,

	.refB_Frequency_Hz = 0,
	.refB_Divider      = 0,
	.refB_bufferCtrl   = 0,

	.vcxo_Frequency_Hz = 0,
	.vcxoBufferCtrl    = 0,

	.nDivider          = 0,
	.bypassPll1        = 0,
};

static ad9528pll2Settings_t pll2Settings = {
	.rfDivider = 3,
	.n2Divider = 1,
	.totalNdiv = 10
};

static ad9528sysrefSettings_t sysrefSettings = {
	.sysrefRequestMethod = 0,
	.sysrefSource = 2,
	.sysrefPinEdgeMode = 0,
	.sysrefPinBufferMode = 0,
	.sysrefPatternMode = 0,
	.sysrefNshotMode = 0,
	.sysrefDivide = 512
};

static ad9528outputSettings_t outputSettings = {
	.outPowerDown    = 0x33FC,
	.outSource       = {0,0,0,2,0,0,0,0,0,0,0,0,2,0},
	.outBufferCtrl   = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	.outAnalogDelay  = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	.outDigitalDelay = {0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	.outChannelDiv   = {5,5,5,5,5,5,5,5,5,5,5,5,5,5},
	.outFrequency_Hz = {245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000, 245760000}
};
#endif
