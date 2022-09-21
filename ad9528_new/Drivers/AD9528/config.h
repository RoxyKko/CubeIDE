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
	.refA_Divider      = 0,
	.refA_bufferCtrl   = DISABLED,

	.refB_Frequency_Hz = 0,
	.refB_Divider      = 0,
	.refB_bufferCtrl   = DISABLED,

	.vcxo_Frequency_Hz = 122880000,
	.vcxoBufferCtrl    = NEG_SINGLE_ENDED,

	.nDivider          = 4,
	.bypassPll1        = 0,
};

static ad9528pll2Settings_t pll2Settings = {
	.rfDivider = 3,
	.n2Divider = 10,
	.totalNdiv = 30
};

static ad9528sysrefSettings_t sysrefSettings = {
	.sysrefRequestMethod = SPI,
	.sysrefSource = INTERNAL,
	.sysrefPinEdgeMode = LEVEL_ACTIVE_HIGH,
	.sysrefPinBufferMode = DISABLED,
	.sysrefPatternMode = NSHOT,
	.sysrefNshotMode = ONE_PULSE,
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
