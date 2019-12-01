#include <cctype> 			// std::toupper and std::tolower
#include<bits/stdc++.h>		// transform
#include <iostream>
#include <fstream>

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sstream>      	// std::stringstream
#include <vector>

using namespace std;
//using namespace IO_port_config;

/*! Customer File structure
 * 
 * File structure, TAB and comma delimitted
 * Column name					// Column number / index
 *******************************************************
 * RegName 						// 0
 * WriteFieldName 				// 1
 * ReadFieldName 				// 2
 * CommonFieldName 				// 3
 * Address 						// 4
 * NrBits 						// 5
 * BitOffs 						// 6
 * 
 * Filename: predefined, inputFile_map.csv
 * location: application working directory
 *
 * Important notice: This application requires memory map register's NrBits + BitOffs <= 16 
 * thus the application is not applicable for Chip ID description (typicaly minimum WaferNr-YPos-XPos = 21bits)
 *
 */
 
 #define inHEX	1 // to get ascii number in hex format
 #define inDEC	0 // to get ascii number in dec format
 
// input and output (result) files
fstream myFile;
fstream resultFile;

unsigned int	AddressMin = 0x0;
 
unsigned int get_intNumber_from_asciiString(string asciiNumber, bool hexNumber);
void calcMemoryFileds(string portName, string portAddress, string portNrBits, string portOffset) ; 

int main(int argc, char** argv) {	
/*!
 * Description of fields in the bit definition table
 * - bit field name (enum parameters)
 * - bit field name (const char*)
 * - bit field size (int, nr of bits)
 * - encoding type (enum MMTypes)
 * - table with all bit definitions
 *   * bit 0 till 3 is the bitposition
 *   * bit 4 till 11 is the address. Expressed in bytes - start address.
 * example:
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
 */
 
	string 			baseName, baseAddress, baseNrBits, baseBitOffs;
	unsigned int	Address, NrBits, BitOffs;
	
	int last, count = 0; 
	
	// Read the Data from the file as String Vector  
    vector<string> row; 			// one row from the file, vector elements separated by comma
    vector<string> map;				// the whole map file, one row from the file become an element of the vector
    string AddrLine, AddressWord, temp;
	
	char buff[1024];
	int NrPortsRegisters = 1;
		
	resultFile.open("resultFile_map.csv", ios::out);
	if (!resultFile.is_open())
	{
		cerr << "Error: result file not OK" << endl;
		system("pause");
		return 1;	
	}

	myFile.open("inputFile_map.csv", ios::in);

	if (!myFile.is_open())
	{
		cerr << "Error: File Not Found" << endl;
		system("pause");
		return 1;	
	}
	else 
	{
		map.clear();
		while (getline(myFile, AddrLine)) // read an entire row and store it in a string variable 'AddrLine' 
		{
			// remove not relevant lines: comments, empty lines or errors
			sprintf(buff, AddrLine.c_str());
			if (buff[0] =='#' or buff[0] ==';' or buff[0] =='\0' or buff[0] =='\r' or buff[0] =='\n') continue;
			
			// Entire file row is an element of the vecor. Entire file is in the vector
			map.push_back(AddrLine); 
		}		
		
		// find the start address (minimum Address) -> consider this is the last address in an input input csv file descendind sorted !!!!
		last = map.size() - 1;
		temp = map[last];
		cout << "map.size(): " << last << ", lastRow: " << temp << endl;
		stringstream last_s(temp);
		int address_index = 0;
		while (getline(last_s, AddressWord, ',')) {
			if (address_index == 4) {
				AddressMin = get_intNumber_from_asciiString(AddressWord, inHEX);
				break;
			}
			address_index++;
		}
			
		for (int i = 0; i < map.size(); i++) 
		{
		 	row.clear();
			AddrLine = map[i];
			 
 			stringstream s(AddrLine);  // string stream to be used to split the line into words	
        	
			// read every column data of a row and store it in a string variable, 'AddressWord', comma separated
        	int wordNr = 0;        	
        	while (getline(s, AddressWord, ',')) 
			{ 
				//convert to Upper case format but don't change this for HEX address 0x0....
            	if (wordNr != 4) transform(AddressWord.begin(), AddressWord.end(), AddressWord.begin(), ::toupper);
				wordNr++;								
        		row.push_back(AddressWord); // add all the column data of a row to a vector
        	}
            // load found data into the resultFIle after memory field values are calculated  		
			bool validRow = true;				
			for (int index = 3; index < row.size(); index++)
			{					
				switch(index) {
					case 3  :
						// base registers naming on the CommonFieldName.
      					baseName = row[index];
      					if (row[3].size() == 0 or row[3] == "-" ) validRow = false; ; // ignore rows with empty or dummy "-" content of CommonFieldName
      					break; 	// assign memory register name
   					case 4  :
      					baseAddress = row[index];
      					break; 	// assign memory base address
					case 5  :
      					baseNrBits = row[index];
      					break; 	// assign register's bits number
   					case 6  :
      					baseBitOffs = row[index];
      					break; 	// assign register's bits offset  
   					default : 
      				{
      					cout << "Error: not valid memory field structure !!!" << endl;
						// close the files in case of an error and exit
						resultFile.close();
						myFile.close();  
						system("pause");    						
						return 1;	
					}
				}
 				if (!validRow) break;	// ignore rows with empty or dummy "-" content of CommonFieldName												
			}
			if (validRow) {
				calcMemoryFileds(baseName, baseAddress, baseNrBits, baseBitOffs);
				count++;
			}
		}
	
		cout << "Number of Address rows: " << std::dec << count << endl;
		cout << "Start Address [hex]: 0x" << std::hex << AddressMin << endl;	
	
		//close the files
		resultFile.close();
		myFile.close();	
		system("pause");
		return 0;
	}
}


unsigned int get_intNumber_from_asciiString(string asciiNumber, bool hexNumber) {
	unsigned int regAddress = 0;
	stringstream ss;
	ss << asciiNumber;
	if (hexNumber) {
		ss >> std::hex >> regAddress;
	} else {
		ss >> regAddress;
	}
	return regAddress;
}

void calcMemoryFileds(string portName, string portAddress, string portNrBits, string portOffset) 
{
	unsigned int prefix, regAddress = 0;
	unsigned int regNrBits = 0;
	unsigned int regOffset = 0;
	unsigned int lowCycle = 0;
	unsigned int highCycle = 0;
	unsigned int highOffset = 0;
	unsigned int count = 0;
	unsigned int result [16];
	char buff[10];
	
	regAddress = get_intNumber_from_asciiString(portAddress, inHEX);
	regAddress = regAddress - AddressMin; // will be OK if input ADI datafile was descending sorted 
	prefix = regAddress;
	regAddress *= 0x10;	
	
	regNrBits = get_intNumber_from_asciiString(portNrBits, inDEC);
	regOffset = get_intNumber_from_asciiString(portOffset, inDEC);
	
	if ((regNrBits + regOffset) <= 8) {
		lowCycle = regNrBits;
		highCycle = 0;
	} else if ((regOffset <= 8) and (regNrBits + regOffset <= 16)) {
		lowCycle = 8 - regOffset;
		highCycle = regNrBits - lowCycle;
		highOffset = 0;
	} else if ((regOffset > 8) and (regNrBits + regOffset <= 16)) {
		lowCycle = 0;
		highCycle = regNrBits;
		highOffset = regOffset - 8;

	} else {
		cerr << "Error: NrBits + BitOffs > 16 !!";
		system("pause");
	}
	
	resultFile << "{ " << portName << ", \"" << portName << "\", " << std::dec << regNrBits << ", MM_UNSIGNED, { "; 
	
	count = lowCycle;
	while (lowCycle) {
		result[count - lowCycle] = regAddress + regOffset + (count - lowCycle);
		prefix = result[count - lowCycle]; 
		if (prefix == 0x000 or prefix < 0x010) sprintf(buff,"0x00%x", result[count - lowCycle]);
		if (prefix > 0x00f and prefix < 0x100) sprintf(buff,"0x0%x", result[count - lowCycle]);
		if (prefix > 0x0ff) sprintf(buff,"0x%x", result[count - lowCycle]);
		resultFile << std::hex << buff;
		if (highCycle !=0 or lowCycle > 1) resultFile << ", ";
		lowCycle--;
	}

	count = highCycle;
	while (highCycle) {
		result[count - highCycle] = regAddress + 0x10 + (count - highCycle) + highOffset;
		prefix = result[count - highCycle];
		if (prefix == 0x000 or prefix < 0x010) sprintf(buff,"0x00%x", result[count - highCycle]);
		if (prefix > 0x00f && prefix < 0x100) sprintf(buff,"0x0%x", result[count - highCycle]);
		if (prefix > 0x0ff) sprintf(buff,"0x%x", result[count - highCycle]);
		resultFile  << std::hex << buff;
		if (highCycle > 1) resultFile << ", ";
		highCycle--;
	}
	resultFile << " } }," << endl;	

}

