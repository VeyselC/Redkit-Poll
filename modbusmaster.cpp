#include "modbusmaster.h"

ModBusMaster::ModBusMaster()
{
}


char ModBusMaster::asc2hex(char v)
{
	if (v <= '9' && v >= '0') return (char)(v - '0');
	else if (v >= 'A' && v <= 'F') return (char)(v - 'A' + 10);
	else if (v >= 'a' && v <= 'f') return (char)(v - 'a' + 10);
	return 0;
}

char ModBusMaster::int2hex(int x)
{

	return x;
}

int ModBusMaster::CrcCalculation(int a,int b,int c,int d)
{//    a=ID; b=function; c=Address; d=Quantity;
//	return (0-a-b-c-d);
    return(0-a-b-((unsigned char)(c & 0x00FF))-((unsigned char)((c>>8) & 0x00FF)) - ((unsigned char)(d & 0x00FF)) - ((unsigned char)((d>>8) & 0x00FF)));
    //CRC hesabında adres veya register adedi hex olarak 2 basamaktan fazlaysa yani 8bit'den fazlaysa yanlış crc hesaplıyordu.
}

QString ModBusMaster::PreparePack(QString ID_pack,int Function_pack,QString Address_pack,QString Quantity_pack)
{
	unsigned int Add_Int,Qua_Int,Function,ID_Int,length;
	QByteArray bytes;

	ID_Int = ID_pack.toInt();
	if(ID_Int > 255) ID_Int = 255;

#warning denenecek
    if(Function_pack < 5)
        Function = Function_pack;
    else if(Function_pack == 666)   //fonksiyon listesi ile karışmaması için yazma fonksiyonu 666 ile çağırılıyor. daha sonradan burada '6' atanıyor.
        Function = 6;
    else Function = 67;


	Add_Int = Address_pack.toInt();
	if(Add_Int > 65535) Add_Int = 65535;

	Qua_Int = Quantity_pack.toInt();
	if(Qua_Int > 65535) Qua_Int = 65535;

	ID =QString::number( ID_Int, 16 ).toUpper();
	bytes = ID.toUtf8();
	length = bytes.size();
	ID = QString((2-length), '0') + ID;

	Address = QString::number(Add_Int, 16 ).toUpper();
	bytes = Address.toUtf8();
	length = bytes.size();
	Address = QString((4-length), '0') + Address;

	Quantity = QString::number(Qua_Int, 16 ).toUpper();
	bytes = Quantity.toUtf8();
	length = bytes.size();
	Quantity = QString((4-length), '0') + Quantity;

	unsigned char Crc_Res = this->CrcCalculation(ID_Int,Function,Add_Int,Qua_Int);
//	CRC.append(QString("%1").arg(QString::number( Crc_Res, 16 ).toUpper()));

	CRC = QString::number(Crc_Res, 16 ).toUpper();
	bytes = CRC.toUtf8();
	length = bytes.size();
	CRC = QString((2-length), '0') + CRC;


	sendtable = ":" + ID + "0" + QString::number( Function, 16 ).toUpper()
			+ Address + Quantity + CRC + "\r\n";

//	ui->res_lineEdit->setText(sendtable);
	return sendtable;
}
