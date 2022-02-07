#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTableWidget>
#include <QObject>
#include <QImage>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QTimer>
#include <QMessageBox>
#include <QKeyEvent>
#include <QMouseEvent>


#include "wrightvaluedialog.h"
#include "modbusmaster.h"
#include "setupdialog.h"
#include "qcustomplot.h"
#include "graph_set.h"


//#include <QChar>

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

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();
	void setupPlot();
	void setupRealtimeDataDemo(QCustomPlot *customPlot);
	QDateTime julianDoubleToDateTime(double julian);

	//Variables
	ModBusMaster ModbusMaster_new;
	QTimer *timer_poll = new QTimer(this);
    QTimer *timer = new QTimer(this);
    QTimer *TimeOuttimer = new QTimer(this);
    QTimer *Polltimer = new QTimer(this);

    QSerialPort *serial;

	QCPRange xrange,yrange;
    QString ID,Function,Address,Quantity,CRC,ByteCount,receivedData;
	QString sendtable,received;

	QStringList portlist;
	unsigned char g_ucSciRxNbOfBytes;
	QChar *pucPtrToSCIBuffRx;
    QLabel *statusImage;
    QLabel *statusImage2 = new QLabel;
    QLabel *statusEcount = new QLabel;
    QLabel *statusPcount = new QLabel;
    QLabel *statusPtimer = new QLabel;
    QLabel *statusLimitCounter = new QLabel;


    QImage *greenI,*redI;
    QImage podngred,podnggreen;

	bool Sci_New_Packet = false, packageStarted = false, Charerror = false , pollbool = false, pollbutonbool = false;
    bool polltimerbool = false, SetupEntered = false, GraphicSetupEntered = false, osmanb = false,firstvalueB = false,packageerror = false;
	double xkey, ykey;
	int osmanint=1, xtempval, ytempval;
	double xmax, xmin, ymax, ymin, xratio=1.0, yratio=1.0;
    int TimeOuttime,errorcount=0,packagecount=0,polltimerSec=0,polltimerMin=0,polltimerHour=0;
    int limitRow=0,limitColumn=0, limitcounter=0;

public slots:
	void timerSlot();
	void PolltimerSlot();
    void TimeOuttimerSlot();
    void PollingTimetimerSlot();

signals:
    void osman(QString,QString,QString,int);
	void setupopen();

private slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	void on_tableWidget_customContextMenuRequested(const QPoint &pos);

	void on_tableWidget_itemDoubleClicked(QTableWidgetItem *item);

	void on_tableWidget_cellDoubleClicked(int row, int column);

	void on_actionSetup_triggered();

	void serialReceived();

//	void horzScrollBarChanged(int value);
//	void vertScrollBarChanged(int value);
	void xAxisChanged(QCPRange range);
	void yAxisChanged(QCPRange range);

	void on_actionOne_Register_Graphical_View_triggered();

	void on_actionStart_triggered();

	void on_actionPause_triggered();

	void on_actionContinue_triggered();

	void realtimeDataSlot();

	void on_horizontalSlider_sliderMoved(int position);

//	void on_verticalSlider_sliderMoved(int position);

//	void on_verticalSlider_valueChanged(int value);

	void on_horizontalSlider_valueChanged(int value);

	void plotMousePress(QMouseEvent *);

    void on_pushButton_3_clicked();

private:
	Ui::MainWindow *ui;
//	WrightValueDialog *write;
//	WrightValueDialog *write = new WrightValueDialog;
	WrightValueDialog write;
	SetupDialog setupdialog;
	Graph_set GraphicSettings;
//	SetupDialog *setupdialog = new SetupDialog(this);
	unsigned char gs_ucErrortReg;
	unsigned int gs_uiExor1;
	QTimer dataTimer;

	//function prototypes
//	unsigned char asciiToBin(unsigned char);
	void PrintResults();
};

#endif // MAINWINDOW_H
