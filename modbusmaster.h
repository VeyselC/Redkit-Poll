#ifndef MODBUSMASTER_H
#define MODBUSMASTER_H

#include <QString>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class ModBusMaster
{
public:
//Function prototypes
	ModBusMaster();
	char asc2hex(char);
	char int2hex(int x);
	int CrcCalculation(int,int,int,int);
	QString PreparePack(QString,int,QString,QString);

//Variables
	QString sendtable,ID,Address,Quantity,CRC;

};

#endif // MODBUSMASTER_H
