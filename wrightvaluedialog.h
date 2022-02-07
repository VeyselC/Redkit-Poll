#ifndef WRIGHTVALUEDIALOG_H
#define WRIGHTVALUEDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>

#include "modbusmaster.h"
//#include "setupdialog.h"


#define ILLEGAL_FUNC_ERR		1	/*illegal function error*/
#define ILLEG_DAT_ADR_ERR		2	/*illegal data address*/
#define ILLEGAL_DATA_ERR		3	/*illegal data value*/
#define ACKNOWLEDGE_ERR			5	/*acknowledge*/
#define MEMORY_PARITY_ERR		8	/*memory parity error*/

#define ASCII_WRITE_EEPROM		102
#define ASCII_READ_EEPROM		100
#define ASCII_READ_RAM			101

#define ASCII_READ_COILS		110
#define ASCII_READ_DIS_IN		111
#define ASCII_READ_HOLD_REG		112
#define ASCII_IN_REG			113

#define READ_STATUS_COM			2	/*read status from device*/
#define READ_HOLD_COM			3	/*read holding register from eeprom*/
#define READ_INP_REG_COM		4	/*read input register*/
#define PRESET_SINGLE_COM		6	/*write preset single holding register*/


namespace Ui {
class WrightValueDialog;
//class MainWindow;
}

class WrightValueDialog : public QDialog
{
	Q_OBJECT
public slots:
    void openingslot(QString,QString,QString,int);
    void WriteTimeOuttimerSlot();
    void timerSlot();

public:
	explicit WrightValueDialog(QWidget *parent = 0);
	~WrightValueDialog();
//	void openingslot(QString &ID,QString &Address,QString &Quantity);
    QImage *greenI,*redI,*yellowI;
    QImage podngred,podnggreen,podngyellow;

    QTimer *writetimer = new QTimer(this);
    QTimer *WriteTimeOuttimer = new QTimer(this);
    bool error = false,good = false, Limited = false;
	ModBusMaster ModBusMaster_new;
    QString ID,Function,Address,Quantity,CRC,ByteCount,receivedData;
    QString writetable,received;
//    QSerialPort *writeserial;
    bool Sci_New_Packet = false, packageStarted = false, Charerror = false , pollbool = false, pollbutonbool = false;
    bool polltimerbool = false, SetupEntered = false, GraphicSetupEntered = false, osmanb = false,firstvalueB = false;
    int TimeOuttime_write;
    QString writeserialPortName;
    int SelectedConnection, SelectedPort, SelectedBaud, SelectedDataBits, SelectedStop, SelectedParity,SelectedTimeOut;
    bool SelectedComMode = false,savesets = false;
    bool sendwritingvalue_flag = false,writingsuccesful_flag = false,unsuccesful_flag = false;
    signed int Min_Limit_S= 0,Max_Limit_S=0;
//    unsigned int Min_Limit_US,Max_Limit_US;

private slots:
	void on_pushButton_clicked();
//    void writeserialReceived();

    void on_pushButton_Limit_clicked();

private:
	Ui::WrightValueDialog *ui;
    //SetupDialog setupdialog;
    unsigned char gs_ucErrortReg;
    unsigned int gs_uiExor1;
};

#endif // WRIGHTVALUEDIALOG_H
