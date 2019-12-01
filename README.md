# Trials on handling custom CSV files
> used Dev c++ 5.11

----
## Input CSV file requirements (provided by the user)
>  * File structure, TAB and comma delimitted
>  * Column name					// Column number / index
>  *******************************************************
>  * RegName 						// 0
>  * WriteFieldName 				// 1
>  * ReadFieldName 				    // 2
>  * CommonFieldName 				// 3
>  * Address 						// 4
>  * NrBits 						// 5
>  * BitOffs 						// 6

## Output file requirements (as a resulting file created by this application):
> * Description of fields in the bit definition table
> * - bit field name (enum parameters)
> * - bit field name (const char*)
> * - bit field size (int, nr of bits)
> * - encoding type (enum MMTypes)
> * - table with all bit definitions
> *   * bit 0 till 3 is the bitposition
> *   * bit 4 till 11 is the address. Expressed in bytes - start address.
> * example:

 	const int EEPROM_MAP_DATA_BUS_WIDTH = 2;  	// Expressed in nr of bytes
	const int EEPROM_MAP_SIZE = 64;  			// Expressed in nr of bytes
	const int EEPROM_MAP_START = 0x00C0;  		// Expressed in bytes
	const MBitPosDefinition EEPROM_MAP_POS[] = {
  		// Note that the order of bits is from LSB -> MSB. This is the opposite from the convention.
  		//                                       b0     b1,    b2     b3     b4     b5     b6     b7     b8     b9     b10    b11    b12    b13    b14    b15
        
  		{ D_ID0, "D_ID0", 16, MM_UNSIGNED, { 0x3e0, 0x3e1, 0x3e2, 0x3e3, 0x3e4, 0x3e5, 0x3e6, 0x3e7, 0x3f0, 0x3f1, 0x3f2, 0x3f3, 0x3f4, 0x3f5, 0x3f6, 0x3f7 } },
  		{ D_ID1, "D_ID1", 16, MM_UNSIGNED, { 0x3c0, 0x3c1, 0x3c2, 0x3c3, 0x3c4, 0x3c5, 0x3c6, 0x3c7, 0x3d0, 0x3d1, 0x3d2, 0x3d3, 0x3d4, 0x3d5, 0x3d6, 0x3d7 } },
  		{ D_ID2, "D_ID2", 16, MM_UNSIGNED, { 0x3a0, 0x3a1, 0x3a2, 0x3a3, 0x3a4, 0x3a5, 0x3a6, 0x3a7, 0x3b0, 0x3b1, 0x3b2, 0x3b3, 0x3b4, 0x3b5, 0x3b6, 0x3b7 } },
  		{ D_XPOS_ID, "D_XPOS_ID", 8, MM_UNSIGNED, { 0x3e0, 0x3e1, 0x3e2, 0x3e3, 0x3e4, 0x3e5, 0x3e6, 0x3e7 } },
  		{ D_YPOS_ID, "D_YPOS_ID", 8, MM_UNSIGNED, { 0x3f0, 0x3f1, 0x3f2, 0x3f3, 0x3f4, 0x3f5, 0x3f6, 0x3f7 } },
  		{ I2C_TEMPERATURE, "I2C_TEMPERATURE", 8, MM_UNSIGNED, { 0xa50, 0xa51, 0xa52, 0xa53, 0xa54, 0xa55, 0xa56, 0xa57 } },
		{ I2C_DIAG, "I2C_DIAG", 14, MM_UNSIGNED, { 0xa20, 0xa21, 0xa22, 0xa23, 0xa24, 0xa25, 0xa26, 0xa27, 0xa30, 0xa31, 0xa32, 0xa33, 0xa34, 0xa35 } },
		{ DIRECT_PWM_VAL, "DIRECT_PWM_VAL", 9, MM_UNSIGNED, { 0xa07, 0xa10, 0xa11, 0xa12, 0xa13, 0xa14, 0xa15, 0xa16, 0xa17 } },
		{ APPL_I2C, "APPL_I2C", 1, MM_UNSIGNED, { 0x9f7 } },
		{ PWM_ONOFFB, "PWM_ONOFFB", 1, MM_UNSIGNED, { 0x9f6 } },
		{ DIS_PROG_I2C, "DIS_PROG_I2C", 1, MM_UNSIGNED, { 0x9f5 } },
		{ SLEEP_MODE_ACTIVATED, "SLEEP_MODE_ACTIVATED", 1, MM_UNSIGNED, { 0x9f4 } },
		{ MS_TR_OFF_TEMPSENS, "MS_TR_OFF_TEMPSENS", 3, MM_UNSIGNED, { 0x8f1, 0x8f2, 0x8f3 } },
		{ MS_SELDIO_TEMPSENS, "MS_SELDIO_TEMPSENS", 3, MM_UNSIGNED, { 0x8e6, 0x8e7, 0x8f0 } },
		...
		...
	};
>