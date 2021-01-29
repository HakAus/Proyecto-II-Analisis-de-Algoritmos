#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>   
#include <iomanip>
#include <string>

using namespace std;


class LogManager {
	
	string logName;
	string logContent;
	const string logPath = "StretchLogs\\";//Vuelto loco
	int i = 0;

public:

	void createLog(string pLogName) 
	{
		std::string xd = timeStamp();
		logName = xd+pLogName+".txt";
		logContent = "";
	}
	void addLine(string pLine)
	{
		logContent += pLine;
		logContent += "\n";
	}
	void addLine(int pLine)
	{
		logContent += to_string(pLine);
		logContent += "\n";
	}
	void addLine(unsigned short pLine)
	{
		logContent += to_string(pLine);
		logContent += "\n";
	}
	void addLine(string pLine,int number)
	{
		logContent += pLine+to_string(number);
		logContent += "\n";
	}
	void addLine(double number)
	{
		logContent += to_string(number);
		logContent += "\n";
	}
	void saveLog()
	{
		std::ofstream  txtLog;
		txtLog.open(logPath+logName);
		txtLog << logContent;
		txtLog.close();
	}

	string timeStamp()
	{
		std::string time = to_string(i++);
		return time;
	}

};