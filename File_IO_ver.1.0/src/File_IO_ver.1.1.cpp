#include "stdafx.h"
#include <iostream>

#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <windows.h>
#include <ctime>

#include "rs232.h"

using namespace std;


int vtoi(vector<char> vec, int beg, int end) // vector to int
{
	int ret = 0;
	int mult = pow(10, (end - beg));

	for (int i = beg; i <= end; i++) {
		ret += (vec[i] - '0') * mult;
		mult /= 10;
	}
	return ret;
}


int main()
{
	cout << ".csv file relay to Serial COM ver.1.1" << endl;

	cout << "======================================" << endl;
	cout << "Reading the setup file" << endl;
	cout << "======================================" << endl;

	//read the setup file.
	fstream setupFile;

	setupFile.open("setupFile.txt");

	if (setupFile.is_open() != true)
	{
		cout << "setupFile.txt is not open." << endl;
	}

	//Read the COM Port Number from the setupFile.txt
	string comPortString;

	getline(setupFile, comPortString);

	int comPortNumber = stoi(comPortString);

	cout << "Use COM Port "<< comPortNumber << endl;

	//Read the time interval from the setupFile.txt
	string timeIntervalString;

	getline(setupFile, timeIntervalString);

	int timeInterval = stoi(timeIntervalString);

	cout << "Time interval = " << timeInterval << " ms." << endl << endl;

	
	//read the csv file.
	fstream inputFile;

	inputFile.open("inputCsvFile.csv");
	
	if (inputFile.is_open() != true)
	{
		cout << "inputCsvFile.csv is not open." << endl;
	}

	std::vector<std::vector<std::string> > parsedCsv;
	std::string line;

	while (std::getline(inputFile, line))
	{
		std::stringstream lineStream(line);
		std::string cell;
		std::vector<std::string> parsedRow;
		while (std::getline(lineStream, cell, ','))
		{
			parsedRow.push_back(cell);
		}

		parsedCsv.push_back(parsedRow);
	}

	//display the loaded array
	cout << "======================================" << endl;
	cout << "Loaded .csv file (fixed to 2 colume only):" << endl;
	cout << "======================================" << endl;
	
	for (int i = 0; i < parsedCsv.size(); i++)
	{
		for (int j = 0; j <= 1; j++)
		{
			cout << parsedCsv[i][j] << " ";
		}
		cout << endl;
	}

	cout << "======================================" << endl;
	cout << "End of the .csv file." << endl;
	cout << "======================================" << endl;
	
	//Serial COM setup
	int cport_nr = comPortNumber -1,        /* COM Port 0 = 0*/
		bdrate = 9600;       /* 9600 baud */

	char mode[4] = { '8','N','1',0 };
	
	if (RS232_OpenComport(cport_nr, bdrate, mode))
	{
		cout << "Can not open COM Port " << comPortNumber << endl << endl;;

		//system("pause");

		//return(0);
	}
	else
	{
		cout << "COM Port " << comPortNumber << " is opened successfully." << endl << endl;
	}
	
	cout << "======================================" << endl;
	cout << "Start of main program." << endl;
	cout << "======================================" << endl;

	//Timer setup
	clock_t clockStart;
	clock_t clockDifferece;

	//Serial write here ......
	for (int i = 0; i < parsedCsv.size(); i++)
	{
		//For example, print the output
		cout << "Output = " << parsedCsv[i][1] << endl;

		//Need work here
		//convert the vecter< vecter <string>> to char to be sent out by :
		//int RS232_SendByte(cport_nr, unsigned char);
		
		//delay interval , unit  = millisecond.
		clockStart = clock();
		Sleep(timeInterval);
		clockDifferece = clock() - clockStart;
		cout << "Time interval (dt)= " << clockDifferece / (double)CLOCKS_PER_SEC << " s."<< endl;
	}
			
	system("pause");
	return 0;
}

