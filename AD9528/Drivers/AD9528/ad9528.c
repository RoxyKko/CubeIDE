/* Contains source code for configuring and initializing AD9528 clock device */
#include "./ad9528.h"
#include "./config.h"
#include "main.h"

static void delay_us(uint16_t us)
{
	while(us --)
		for(uint16_t i=0; i<1000; i++);
}

static void delay_ms(uint16_t ms)
{
	while(ms --)
		delay_us(10);
}


static void spi_write_and_read(uint8_t *txbuf, uint8_t *rxbuf,uint8_t bytes_number){
	for(uint8_t i=0; i<bytes_number; i++) {
		for(uint8_t j=0; j<8; j++) {
			if(txbuf[i] & 0x80) sdio_high();
			else                sdio_low();
			txbuf[i] <<=1;

			rxbuf[i] <<=1;
			if(sdo_read()) rxbuf[i] |= 0x01;

			sclk_high();
//			delay_us(1);
			sclk_low();
//			delay_us(1);

//			delay_us(1); //TODO: ´ýÉ¾
		}
	}
}

/* Write data to a register */
HAL_StatusTypeDef AD9528_spiWriteByte (uint16_t addr, uint8_t  data    ) {

	uint8_t txBuf[3];
	uint8_t rxBuf[3] = {0};
	txBuf[0] = (addr >> 8) & 0x7F;
	txBuf[1] = addr & 0xFF;
	txBuf[2] = data;

	csb_low();
//	delay_us(1);
	spi_write_and_read(txBuf, rxBuf, 3);
	csb_high();
	delay_us(100);

	return HAL_OK;
}

/* Read data from a register */
HAL_StatusTypeDef AD9528_spiReadByte  (uint16_t addr, uint8_t* readdata) {

	uint8_t txBuf[3];
	uint8_t rxBuf[3] = {0};
	txBuf[0] = 0x80 | ((addr >> 8) & 0x7F);
	txBuf[1] = addr & 0xFF;
	txBuf[2] = 0x00;

	csb_low();
//	delay_us(1);
	spi_write_and_read(txBuf, rxBuf, 3);
	csb_high();
	delay_us(100);

	*readdata = rxBuf[2];

	return HAL_OK;
}

/* Performs a hard reset on the AD9528 DUT */
HAL_StatusTypeDef AD9528_resetDevice() {
	reset_b_low();
	delay_ms(100);
	reset_b_high();
	delay_ms(100);

	AD9528_spiWriteByte(AD9528_ADDR_ADI_SPI_CONFIG_A, 0x81);
	delay_ms(100);

	return HAL_OK;
}

/* Sets the AD9528 device SPI settings (4wire, MSBFirst, etc). */
HAL_StatusTypeDef AD9528_setSpiSettings(ad9528spiSettings_t *settings) {
	uint8_t spiReg = 0;

	if(settings->LSBFirst) {
		spiReg |= 0x42;
	}

	if(settings->autoIncAddrUp) {
		spiReg |= 0x24;
	}

	if(settings->fourWireMode) {
		spiReg |= 0x18;
	}

	AD9528_spiWriteByte(AD9528_ADDR_ADI_SPI_CONFIG_A, spiReg);

	return HAL_OK;
}


uint32_t registerMap[200];

HAL_StatusTypeDef AD9528_Test() {
	uint16_t addr,i=0;

	for(addr=0; addr <= 0x0509; addr++) {
		if( (addr >= 0x0000 && addr <= 0x0001) ||
				(addr >= 0x0003 && addr <= 0x0006) ||
				(addr >= 0x000A && addr <= 0x000F) ||
				(addr >= 0x0100 && addr <= 0x010B) ||
				(addr >= 0x0200 && addr <= 0x0209) ||
				(addr >= 0x0300 && addr <= 0x032E) ||
				(addr >= 0x0400 && addr <= 0x0404) ||
				(addr >= 0x0500 && addr <= 0x0509))
		{
			uint8_t data;
			AD9528_spiReadByte(addr, &data);
			registerMap[i++] = (addr << 8) + data;
		}
	}

	return HAL_OK;
}


HAL_StatusTypeDef AD9528_initialize() {

	uint8_t i=0,j=0;
	uint8_t reg108=0;
	uint8_t reg109=0x4;
	uint8_t pll2_Adiv=0;
	uint8_t pll2_Bdiv=0;

	// Set the default level
	ext_sysref_req_low();
	sclk_low();
	csb_high();

	// Setup SPI as serial conrtol port
	status0_low();
	status1_low();

	// Reset the AD9528
	AD9528_resetDevice();

	// Reset SPI Configuration A as 4 Wire, MSB
	AD9528_setSpiSettings(&spiSettings);

	/**********************************************************************
	 * Configure PLL1
	 **********************************************************************
	 */
	AD9528_spiWriteByte(AD9528_ADDR_REF_A_DIVIDER_LSB,  pll1Settings.refA_Divider);
	AD9528_spiWriteByte(AD9528_ADDR_REF_A_DIVIDER_MSB, (pll1Settings.refA_Divider >> 8) & 0x03);
	AD9528_spiWriteByte(AD9528_ADDR_REF_B_DIVIDER_LSB,  pll1Settings.refB_Divider);
	AD9528_spiWriteByte(AD9528_ADDR_REF_B_DIVIDER_MSB, (pll1Settings.refA_Divider >> 8) & 0x03);

	AD9528_spiWriteByte(AD9528_ADDR_PLL1_N_DIV_LSB,  pll1Settings.nDivider & 0xFF);
	AD9528_spiWriteByte(AD9528_ADDR_PLL1_N_DIV_MSB, (pll1Settings.nDivider >> 8) & 0x03);

	//TODO: AD9528_ADDR_PLL1_CHARGEPUMP && AD9528_ADDR_PLL1_CP_CTRL2
	AD9528_spiWriteByte(AD9528_ADDR_PLL1_CHARGEPUMP , 0x0A);
	AD9528_spiWriteByte(AD9528_ADDR_PLL1_CP_CTRL2   , 0x03);

	switch (pll1Settings.refA_bufferCtrl) {
		case DISABLED:
			reg108 &= ~0x28;
			reg109 &= ~0x01;
			break; //clear all REFA enable bits
		case SINGLE_ENDED:
			reg108 |=  0x08;
			reg109 &= ~0x01;
			break; //Set REFA receiver enable, clear REFA SE negative input enable
		case NEG_SINGLE_ENDED:
			reg108 |=  0x08;
			reg108 &= ~0x10;
			reg109 |= 0x01;
			break; //Disable REFA diff receiver, enable REFA receiver and use negative REFA input
		case DIFFERENTIAL:
			reg108 |=  0x28;
			reg109 &= ~0x01;
			break;
		default:
			reg108 |=  0x08;
			reg109 &= ~0x01;
			break; //default to single ended case
	}
	switch (pll1Settings.refB_bufferCtrl) {
		case DISABLED:
			reg108 &= ~0x50;
			reg109 &= ~0x02;
			break; //clear all REFB enable bits
		case SINGLE_ENDED:
			reg108 |=  0x10;
			reg109 &= ~0x02;
			break; //Set REFB receiver enable, clear REFB SE negative input enable
		case NEG_SINGLE_ENDED:
			reg108 |=  0x10;
			reg108 &= ~0x40;
			reg109 |= 0x02;
			break; //Disable REFB diff receiver, enable REFB receiver and use negative REFB input
		case DIFFERENTIAL:
			reg108 |=  0x50;
			reg109 &= ~0x02;
			break;
		default:
			reg108 |=  0x10;
			reg109 &= ~0x02;
			break; //default to single ended case
	}
	switch (pll1Settings.vcxoBufferCtrl) {
		case DISABLED:
			reg108 &= ~0x03;
			reg108 |=  0x04;
			break; //power down VCXO input
		case SINGLE_ENDED:
			reg108 &= ~0x07;
			break; //Clear VCXO receiver power down, defaults to Single ended mode
		case NEG_SINGLE_ENDED:
			reg108 &= ~0x07;
			reg108 |=  0x02;
			break; //Clear VCXO power down, enable Negative single ended mode[1]
		case DIFFERENTIAL:
			reg108 &= ~0x07;
			reg108 |=  0x01;
			break; //Clear VCXO power down, enable differential input buffer[0]
		default:
			reg108 &= ~0x07;
			break; //default to positive single ended case
	}


	//TODO: AD9528_ADDR_INPUT_RECEIVERS1 & AD9528_ADDR_INPUT_RECEIVERS2 & AD9528_ADDR_INPUT_RECEIVERS3
	AD9528_spiWriteByte(AD9528_ADDR_INPUT_RECEIVERS1, reg108); //Input receiver settings (single ended or differential clock inputs)
	AD9528_spiWriteByte(AD9528_ADDR_INPUT_RECEIVERS1, reg109); //Input receiver settings
	AD9528_spiWriteByte(AD9528_ADDR_INPUT_RECEIVERS3, 0x0A); //Reference selection mode
	AD9528_spiWriteByte(AD9528_ADDR_PLL1_FASTLOCK,    0x00); //Disable Fast lock

	/**********************************************************************
	 * Configure PLL2
	 **********************************************************************
	 */
	pll2_Bdiv = pll2Settings.totalNdiv / 4;
	pll2_Adiv = pll2Settings.totalNdiv % 4;

	if (pll2_Adiv > 3) {  //A div valid range (0 to 3)
		return HAL_ERROR;
	}
	if (pll2_Bdiv < 3 || pll2_Bdiv > 63) {
		return HAL_ERROR;
	}

	AD9528_spiWriteByte(AD9528_ADDR_PLL2_CHARGEPUMP, 0xE6); //PLL2 Charge Pump[7:0]
	AD9528_spiWriteByte(AD9528_ADDR_PLL2_N_DIV, ((pll2_Adiv & 0x3) << 6) | (pll2_Bdiv & 0x3F)); //PLL2 A and B divider
	AD9528_spiWriteByte(AD9528_ADDR_PLL2_CTRL, 0x03);     //PLL2 control
	AD9528_spiWriteByte(AD9528_ADDR_PLL2_VCO_CTRL, 0x01); //Manual VCO CAL

	if (pll2Settings.rfDivider < 3 || pll2Settings.rfDivider > 5) {
		return HAL_ERROR;
	}

	AD9528_spiWriteByte(AD9528_ADDR_PLL2_VCO_DIV , pll2Settings.rfDivider); //VCO divider
	AD9528_spiWriteByte(AD9528_ADDR_PLL2_LF_CTRL1, 0x3A); //PLL2 Loop filter [7:0]
	AD9528_spiWriteByte(AD9528_ADDR_PLL2_LF_CTRL2, 0x00); //PLL2 Loop filter [8]
	AD9528_spiWriteByte(AD9528_ADDR_PLL2_RDIV, 0x01); //R divider
	AD9528_spiWriteByte(AD9528_ADDR_PLL2_REPLICA_CHDIV, (pll2Settings.totalNdiv / pll2Settings.rfDivider) -1 ); //Replica channel divider: Ndivider = rfDivider * R divider
	AD9528_spiWriteByte(AD9528_ADDR_PLL2_REPLICA_DIV_PHASE, 0x00); //Replica Channel Divider phase

	/**********************************************************************
	 * Configure Output Distribution
	 **********************************************************************
	 */
	for (i=0; i<14; i++) {
		AD9528_spiWriteByte(AD9528_ADDR_CH_OUT0_CTRL1 + j,
		    ((outputSettings.outSource[i]       & 0x07) << 5) |
		     (outputSettings.outAnalogDelay[i]  & 0x1F));
		AD9528_spiWriteByte(AD9528_ADDR_CH_OUT0_CTRL2 + j,
		    ((outputSettings.outBufferCtrl[i]   & 0x03) << 6) |
		     (outputSettings.outDigitalDelay[i] & 0x3F));
		AD9528_spiWriteByte(AD9528_ADDR_CH_OUT0_CHDIV + j,
				outputSettings.outChannelDiv[i] - 1);
		j += 3;
	}

	AD9528_spiWriteByte(AD9528_ADDR_OUTPUT_SYNC, 0x00); //Sync outputs
	AD9528_spiWriteByte(AD9528_ADDR_MASK_SYNC1, 0x00); //Mask SYNC[7:0]
	AD9528_spiWriteByte(AD9528_ADDR_MASK_SYNC2, 0x00); //Mask SYNC[13:8]
	AD9528_spiWriteByte(AD9528_ADDR_EN_OUTPUT_PATH_SEL1, 0x00); //Bypass SYSREF resample [6:0], bit0:Enable VCXO to feed output distrubution
	AD9528_spiWriteByte(AD9528_ADDR_EN_OUTPUT_PATH_SEL2, 0x00); //Bypass SYSREF resample [13:7]

	/**********************************************************************
	 * Configure SYSREF operation
	 **********************************************************************
	 */
	AD9528_spiWriteByte(AD9528_ADDR_SYSERF_DIV_LSB,  sysrefSettings.sysrefDivide & 0xFF); //SYSREF divider [7:0]
	AD9528_spiWriteByte(AD9528_ADDR_SYSERF_DIV_MSB, (sysrefSettings.sysrefDivide >> 8) & 0xFF); //SYSREF divider [7:0]

	AD9528_spiWriteByte(AD9528_ADDR_SYSREF_CTRL3,
			    ((sysrefSettings.sysrefRequestMethod & 0x1) << 7) |
			    ((sysrefSettings.sysrefPinEdgeMode & 0x3) << 5) );

	AD9528_spiWriteByte(AD9528_ADDR_SYSREF_CTRL4,
			    ((sysrefSettings.sysrefSource & 0x03) << 6) |
			    ((sysrefSettings.sysrefPatternMode & 0x3) << 4 ) |
			    ((sysrefSettings.sysrefNshotMode & 0x7) << 1) );

	if(sysrefSettings.sysrefPinBufferMode == DISABLED) {
		AD9528_spiWriteByte(AD9528_ADDR_SYSREF_CTRL5, 0x04); //power down input buffer for external SYSREF
	}
	else if(sysrefSettings.sysrefPinBufferMode == SINGLE_ENDED) {
		AD9528_spiWriteByte(AD9528_ADDR_SYSREF_CTRL5, 0x00); //enable positive singled ended input
	}
	else if(sysrefSettings.sysrefPinBufferMode == NEG_SINGLE_ENDED) {
		AD9528_spiWriteByte(AD9528_ADDR_SYSREF_CTRL5, 0x02); //enable negative singled ended input
	}
	else if(sysrefSettings.sysrefPinBufferMode == DIFFERENTIAL) {
		AD9528_spiWriteByte(AD9528_ADDR_SYSREF_CTRL5, 0x01); //enable differential input buffer
	} else {
		AD9528_spiWriteByte(AD9528_ADDR_SYSREF_CTRL5, 0x04); //power down input buffer for external SYSREF
	}



	/**********************************************************************
	 * Power down control
	 **********************************************************************
	 */
	if (pll1Settings.bypassPll1 > 0) {
		AD9528_spiWriteByte(AD9528_ADDR_POWERDOWN_CTRL, 0x14);// power up PLL2 and output distribution, power down PLL1
	} else {
		AD9528_spiWriteByte(AD9528_ADDR_POWERDOWN_CTRL, 0x10);// power up PLL1, PLL2 and output distribution
	}
	AD9528_spiWriteByte(AD9528_ADDR_CH_POWERDOWN1,  outputSettings.outPowerDown & 0xFF );
	AD9528_spiWriteByte(AD9528_ADDR_CH_POWERDOWN2, (outputSettings.outPowerDown >> 8) & 0x3F);
	AD9528_spiWriteByte(AD9528_ADDR_LDO_ENABLES1 , 0xff);
	AD9528_spiWriteByte(AD9528_ADDR_LDO_ENABLES2 , 0xff);
	AD9528_spiWriteByte(AD9528_ADDR_STATUS0_CTRL , 0x07);
	AD9528_spiWriteByte(AD9528_ADDR_STATUS1_CTRL , 0x01);
	AD9528_spiWriteByte(AD9528_ADDR_STATUS_OE , 0x0C);
	AD9528_spiWriteByte(AD9528_ADDR_IO_UPDATE , 0x01);

	// spi test
	uint8_t data;
	AD9528_spiReadByte(0x03, &data);
	for(;;) {
		AD9528_Test();
	}

	return HAL_OK;
}


