#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "wrightvaluedialog.h"
#include <QToolButton>
#include <QTreeWidgetItem>




MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{

	ui->setupUi(this);


//----------Graphic Setup---------
	setupPlot();
	ui->tab_2->setStyleSheet("background-color:QColor(200,200,200);");	//tab_2'nin arka planı mavi yapıldı.
	ui->plot->setBackground(QColor(120,120,120,255));
    ui->pushButton_3->setToolTip( "Copy Received Package to Clipboard"  );

//	ui->tab_2->setStyleSheet("background-color:rgb(58,157,231)");	//tab_2'nin arka planı mavi yapıldı.
//	ui->plot->setBackground(QColor(165,165,165,255));
  //  ui->plot->set
	// configure scroll bars:
	// Since scroll bars only support integer values, we'll set a high default range of -500..500 and
	// divide scroll bar position values by 100 to provide a scroll range -5..5 in floating point
	// axis coordinates. if you want to dynamically grow the range accessible with the scroll bar,
	// just increase the the minimum/maximum values of the scroll bars as needed.
//	ui->horizontalScrollBar->setRange(-500, 500);
//	ui->verticalScrollBar->setRange(-500, 500);
//	ui->verticalSlider->setRange(0,100);
	ui->horizontalSlider->setRange(0,500);
//    ui->statusBar->addWidget( statusImage,0 );
//    QLabel *statusImage2 = new QLabel;
//    statusImage2->adjustSize(55,15);

//    int osman = statusEcount->width();
    ui->statusBar->addPermanentWidget(statusLimitCounter,0);
    ui->statusBar->addPermanentWidget(statusPtimer,0);
    ui->statusBar->addPermanentWidget(statusPcount,0);
    ui->statusBar->addPermanentWidget(statusImage2,0);
    ui->statusBar->addPermanentWidget(statusEcount,0);



    int h = 15;

    redI = new QImage(":/images/red.png");
    greenI = new QImage(":/images/green.png");

    podngred = redI->scaled(h,h,Qt::KeepAspectRatio);
    podnggreen = greenI->scaled(h,h,Qt::KeepAspectRatio);


	connect(ui->plot->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
	connect(ui->plot->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
	connect(ui->plot,SIGNAL(mousePress(QMouseEvent*)), this, SLOT(plotMousePress(QMouseEvent*)));

	// initialize axis range (and scroll bar positions via signals we just connected):
	ui->plot->xAxis->setRange(0, 100, Qt::AlignCenter);
	ui->plot->yAxis->setRange(0, 10, Qt::AlignCenter);

	ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
//--------------------------------
//    StatusImage = new QImage(":/images/green.png");
//    QImage podng = StatusImage->scaled(w,h,Qt::KeepAspectRatio);
//    ui->Resultlabel->setPixmap(QPixmap::fromImage(podng,Qt::AutoColor));
//    ui.statusBar->insertPermanentWidget(0, statusImage,0 );
//    ui->statusBar->insertPermanentWidget(0, statusImage,0 );
    setupRealtimeDataDemo(ui->plot);

	serial = new QSerialPort(this);
	//Main window settings
    this->setWindowTitle("Redkit Poll");
	this->setWindowIcon(QIcon("://images/favicon.ico"));

    connect(TimeOuttimer, SIGNAL(timeout()), this, SLOT(TimeOuttimerSlot()));
    connect(Polltimer, SIGNAL(timeout()), this, SLOT(PollingTimetimerSlot()));

	connect(timer, SIGNAL(timeout()), this, SLOT(timerSlot()));
	timer->start(1);

	connect(timer_poll, SIGNAL(timeout()), this, SLOT(PolltimerSlot()));

	QValidator *inputRange = new QDoubleValidator(0.00, 100.00, 2, this);
	ui->PollTimelineEdit->setValidator(inputRange);

	ui->comboBox->addItem("01 Read Coils 0x");
	ui->comboBox->addItem("02 Read Discrete Inputs 1x");
	ui->comboBox->addItem("03 Read Holding Registers 4x");
	ui->comboBox->addItem("04 Read Input Registers 3x");
    ui->comboBox->addItem("43 Read Special Registers");
//	ui->comboBox->addItem("05 Write Single Coils 0x");
//	ui->comboBox->addItem("06 Write Single Register 0x");
    ui->comboBox->setCurrentIndex(3);
    ui->ID_lineEdit->setText("1");
    ui->Add_lineEdit->setText("0");
    ui->Qua_lineEdit->setText("3");
    ui->PollTimelineEdit->setText("500");

	ui->DataTypecomboBox->addItem("Signed 8");
	ui->DataTypecomboBox->addItem("UnSigned 8");
	ui->DataTypecomboBox->addItem("Signed 16");
	ui->DataTypecomboBox->addItem("UnSigned 16");
	ui->DataTypecomboBox->addItem("Hex");
	ui->DataTypecomboBox->addItem("Binary");

    ui->tableWidget->setColumnCount(1);
//    ui->tableWidget->selectRow(1);
//	ui->tableWidget->setRowCount(10);
//	WrightValueDialog *write = new WrightValueDialog;
	connect(this,SIGNAL(osman(QString,QString,QString,int)),&write,SLOT(openingslot(QString,QString,QString,int)));
	connect(this,SIGNAL(setupopen()),&setupdialog,SLOT(openingslot()));
	connect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));

}

MainWindow::~MainWindow()
{
    serial->close();
    TimeOuttimer->stop();
    Polltimer->stop();
    timer_poll->stop();
    timer->stop();
	delete ui;
}

void MainWindow::setupPlot()
{
	ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom);
	ui->plot->xAxis->setLabel("Time");
	ui->plot->yAxis->setLabel("Recevied Data");

	ui->plot->xAxis2->setLabel("Time 2");
	ui->plot->yAxis2->setLabel("Recevied Data 2");
}


//void MainWindow::horzScrollBarChanged(int value)
//{
//  if (qAbs(ui->plot->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
//  {
//	ui->plot->xAxis->setRange(value/100.0, ui->plot->xAxis->range().size(), Qt::AlignCenter);
//	ui->plot->replot();
//  }
//	xkey = value*10.0;
//}

//void MainWindow::vertScrollBarChanged(int value)
//{
//  if (qAbs(ui->plot->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
//  {
//	ui->plot->yAxis->setRange(-value/100.0, ui->plot->yAxis->range().size(), Qt::AlignCenter);
//	ui->plot->replot();
//  }
//  ykey = value*10.0;
//}

void MainWindow::xAxisChanged(QCPRange range)
{
//  ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
//  ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
//  ui->horizontalSlider->setRange(0,500);
  ui->horizontalSlider->setValue(qRound(ui->plot->xAxis->range().size()));
  xrange = range;
}

void MainWindow::yAxisChanged(QCPRange range)
{
//  ui->verticalScrollBar->setValue(qRound(-range.center()*100.0)); // adjust position of scroll bar slider
//  ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
  //  ui->verticalSlider->setRange(0,100);

//  ui->verticalSlider->setValue(qRound(ui->plot->yAxis->range().size()));
  yrange = range;
}

void MainWindow::on_pushButton_clicked()	//paket hazırla
{
	//:010300FF000AF3
	//ID = 01
	//Function = 03
	//Address = 0x00FF
	//Quantity = 0x000A
	//CRC = F3
	int polltime;
	if(!SetupEntered)
	{

	QMessageBox::information(this, "Information!", "<p>Please, first setup the connection!</p>");
//		QMessageBox msgBox;
//			msgBox.setIcon(QMessageBox::Information);
//			msgBox.set
//			msgBox.setWindowTitle("Information!");
//			msgBox.setText("Information!");
//			msgBox.setInformativeText("Please, first setup the connection");
//			msgBox.setStandardButtons(QMessageBox::Ok/*Save | QMessageBox::Discard | QMessageBox::Cancel*/);
//			msgBox.setDefaultButton(QMessageBox::Ok);
//			/*int ret = */msgBox.exec();
			goto label1;
	}

	if(!pollbutonbool)
	{
        statusLimitCounter->setText("Lexceeded:");
		sendtable = ModbusMaster_new.PreparePack(ui->ID_lineEdit->text(),(ui->comboBox->currentIndex()+1),ui->Add_lineEdit->text(),ui->Qua_lineEdit->text());
		ui->res_lineEdit->setText(sendtable + "\r\n");
		polltime = ui->PollTimelineEdit->text().toInt();
		polltimerbool = false;
		timer_poll->start(polltime);
        polltimerSec = 0;
        polltimerMin = 0;
        polltimerHour = 0;
        Polltimer->start(1000);
        timer->start(1);
		pollbutonbool = true;
		ui->pushButton->setText("Stop Polling");
        write.TimeOuttime_write = TimeOuttime;

	}
	else
	{
		pollbutonbool = false;
        TimeOuttimer->stop();
        timer_poll->stop();
        Polltimer->stop();
        timer->stop();
		ui->pushButton->setText("Start Polling");
		serial->close();
        errorcount = 0;
        packagecount = 0;
        limitcounter = 0;
        write.Limited = false;
	}
	label1:
	return;

}


void MainWindow::on_pushButton_2_clicked()		//paket al
{
//	QByteArray bytess;
//	ui->tableWidget->setItem(0, 0, new QTableWidgetItem("123"));
	//SentReading
	//:010300FF0003F3
	//ReceivedReading
	//:0103 0006 0001 0002 0003
	//ID - Function - ByteCount - 1stRegValHi,RegValLo - 2ndRegValHi,RegValLo - 3thRegValHi,RegValLo
//	received = ui->res_lineEdit->text();
//	int length = received.length();

	// Example use QSerialPort

	foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		portlist+=info.portName();
	}

	QString serialPortName = portlist.at(0);	//com1 seçildi
	serial->setPortName(serialPortName);
	if (serial->open(QIODevice::ReadWrite))
	{
		serial->setBaudRate(QSerialPort::Baud19200);
		serial->setDataBits(QSerialPort::Data8);
		serial->setParity(QSerialPort::NoParity);
		serial->setStopBits(QSerialPort::OneStop);
		serial->setFlowControl(QSerialPort::NoFlowControl);
//		serial.write("Hello Qt Serial");
//		qDebug() << "acildi";
	}
//	else
//		qDebug() << "acilmadi";
}

//unsigned char MainWindow::asciiToBin(unsigned char Bin)
//{
//	if((Bin >= '0') && (Bin <= '9')) return Bin - '0';
//	else if((Bin >= 'A') && (Bin <= 'F')) return Bin - ('A' - 10);
//	else return Bin;
//}
void MainWindow::PollingTimetimerSlot()
{
    if(!packageerror)       //paket hatalı gelince timer dursun
    {
        if(++polltimerSec >= 60)
        {
            polltimerSec = 0;
            if(++polltimerMin >= 60)
            {
                polltimerMin = 0;
                ++polltimerHour;
            }
        }
    }
    QString polltiming;
    if(polltimerHour == 0 && (polltimerMin == 0))
        polltiming = (QString::number( polltimerSec, 10 ).toUpper());
    else if(polltimerHour == 0)
        polltiming = (QString::number( polltimerMin, 10 ).toUpper()) + ":" + (QString::number( polltimerSec, 10 ).toUpper());
    else
        polltiming = (QString::number( polltimerHour, 10 ).toUpper()) + ":" + (QString::number( polltimerMin, 10 ).toUpper()) + ":" + (QString::number( polltimerSec, 10 ).toUpper());

    statusPtimer->setText("Ptime: " + polltiming);
}
void MainWindow::TimeOuttimerSlot()
{
    statusImage2->setPixmap(QPixmap::fromImage(podngred,Qt::AutoColor));
    ui->statusBar->showMessage("Time Out Error");
    ++errorcount;
    packageerror = true;
    statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
    if(write.sendwritingvalue_flag)
    write.unsuccesful_flag = true;
}


void MainWindow::PolltimerSlot()
{
    packagecount++;
	if(packageStarted) polltimerbool = false;
	if(!polltimerbool)
	{
		polltimerbool = true;
		foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
		{
			portlist+=info.portName();
		}

		QString serialPortName = portlist.at(setupdialog.SelectedPort);	//com1 seçildi
		serial->setPortName(serialPortName);
        write.writeserialPortName = serialPortName;
        write.SelectedBaud = setupdialog.SelectedBaud;
        write.SelectedDataBits = setupdialog.SelectedDataBits;
        write.SelectedParity = setupdialog.SelectedParity;
        write.SelectedStop = setupdialog.SelectedStop;


		if (serial->open(QIODevice::ReadWrite))
		{
			switch (setupdialog.SelectedBaud) {
			case 0:
				serial->setBaudRate(QSerialPort::Baud115200);
				break;
			case 1:
				serial->setBaudRate(QSerialPort::Baud57600);
				break;
			case 2:
				serial->setBaudRate(QSerialPort::Baud38400);
				break;
			case 3:
				serial->setBaudRate(QSerialPort::Baud19200);
				break;
			case 4:
				serial->setBaudRate(QSerialPort::Baud9600);
				break;
			case 5:
				serial->setBaudRate(QSerialPort::Baud4800);
				break;
			case 6:
				serial->setBaudRate(QSerialPort::Baud2400);
				break;
			case 7:
				serial->setBaudRate(QSerialPort::Baud1200);
				break;
			default:
				break;
			}

			if(setupdialog.SelectedDataBits == 0)
				serial->setDataBits(QSerialPort::Data8);
			else
				serial->setDataBits(QSerialPort::Data7);

			if(setupdialog.SelectedParity == 0)
				serial->setParity(QSerialPort::NoParity);
			else if(setupdialog.SelectedParity == 1)
				serial->setParity(QSerialPort::OddParity);
			else if(setupdialog.SelectedParity == 2)
				serial->setParity(QSerialPort::EvenParity);

			if(setupdialog.SelectedStop == 0)
				serial->setStopBits(QSerialPort::OneStop);
			else
				serial->setStopBits(QSerialPort::TwoStop);

//			if(setupdialog.sele == 0)
			serial->setFlowControl(QSerialPort::NoFlowControl);
	//		serial.write("Hello Qt Serial");
	//		qDebug() << "acildi";
		}
	}
//	QByteArray array (sendtable.toStdString().c_str());
    if(!(write.sendwritingvalue_flag))
    {
        sendtable = ModbusMaster_new.PreparePack(ui->ID_lineEdit->text(),(ui->comboBox->currentIndex()+1),ui->Add_lineEdit->text(),ui->Qua_lineEdit->text());
        ui->res_lineEdit->setText(sendtable + "\r\n");
        serial->write(sendtable.toStdString().c_str());
        TimeOuttime = setupdialog.SelectedTimeOut;
        if(TimeOuttime >= ui->PollTimelineEdit->text().toInt()) TimeOuttime = ui->PollTimelineEdit->text().toInt() - 1;
        TimeOuttimer->stop();
        TimeOuttimer->start(TimeOuttime);
    }
    else
    {
        serial->write(write.writetable.toStdString().c_str());
        TimeOuttime = setupdialog.SelectedTimeOut;
        if(TimeOuttime >= ui->PollTimelineEdit->text().toInt()) TimeOuttime = ui->PollTimelineEdit->text().toInt() - 1;
        TimeOuttimer->stop();
        TimeOuttimer->start(TimeOuttime);
    }
}

void MainWindow::timerSlot()
{
    int length = received.length(), kuzeroch, function,recFunction;
	bool ok;
	if(Sci_New_Packet)
	{
//		qDebug() << "received";
		Sci_New_Packet = false;
		gs_uiExor1 = 0;

//		qDebug() << received;

		ID = received.mid(1,2);
//		qDebug() << "ID:" << ID;

        Function = received.mid(3,2);
//      qDebug() << "Func:" << Function;

		ByteCount = received.mid(5,2);
//		qDebug() << "ByteCount:" << ByteCount;

		receivedData = received.mid(7,length-11);
//		qDebug() << "Data:" << receivedData;

		CRC = received.mid(length-4,2);
//		qDebug() << "CRC:" << CRC;


		function = ui->comboBox->currentIndex()+1;

        recFunction = (Function.toUInt(&ok,16));

		for(kuzeroch = 1; kuzeroch < length-4; kuzeroch++)
		{
			if(kuzeroch%2 == 1)
			{
				QString asciiToBinTemp = received.at(kuzeroch);
				gs_uiExor1 += (16 * asciiToBinTemp.toUInt(&ok,16));
			}
			else
			{
				QString asciiToBinTemp = received.at(kuzeroch);
				gs_uiExor1 += asciiToBinTemp.toUInt(&ok,16);
			}
			if(gs_uiExor1 >= 256)
				gs_uiExor1 -= 256;
		}
		gs_uiExor1 = (255 ^ gs_uiExor1) + 1;

		QString asciiToBinTemp0 = CRC.at(0);
		QString asciiToBinTemp1 = CRC.at(1);

		if((asciiToBinTemp0.toUInt(&ok,16) != (unsigned char)((gs_uiExor1 & 0x00F0) >> 4))
				|| (asciiToBinTemp1.toUInt(&ok,16) != (unsigned char)(gs_uiExor1 & 0x000F)))
			gs_ucErrortReg = ACKNOWLEDGE_ERR;	//crc error
        else if(((6 != recFunction) && (receivedData.length() != (ByteCount.toUInt(&ok,16)*2)))/* || (6 != function && )*/)	gs_ucErrortReg = ILLEGAL_DATA_ERR;
//		else if(serial->error())										gs_ucErrortReg = ACKNOWLEDGE_ERR;
        else if(1 == recFunction)										gs_ucErrortReg = ASCII_READ_COILS;
        else if(2 == recFunction)										gs_ucErrortReg = ASCII_READ_DIS_IN;
        else if(3 == recFunction)										gs_ucErrortReg = ASCII_READ_HOLD_REG;
        else if(4 == recFunction)										gs_ucErrortReg = ASCII_IN_REG;
//		else if(5 == recFunction)										gs_ucErrortReg = ASCII_WRITE_EEPROM;
        else if(6 == recFunction)										gs_ucErrortReg = ASCII_WRITE_EEPROM;
		else															gs_ucErrortReg = ILLEGAL_FUNC_ERR;


//        int w = 15/*statusImage->width()*/;
        int h = 15/*statusImage->height()*/;
//        int h  = ui->statusBar->height();





		switch(gs_ucErrortReg)
		{
		case ACKNOWLEDGE_ERR:
//			qDebug() << "CRC error";
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podngred,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            ui->statusBar->showMessage(received+" - Acknowledge ERROR");
            if(write.sendwritingvalue_flag)
            {
                write.sendwritingvalue_flag = false;
                write.unsuccesful_flag = true;
            }
            ++errorcount;
            packageerror = true;
		break;

        case MEMORY_PARITY_ERR:
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podngred,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            ui->statusBar->showMessage(received+" - Memory Parity ERROR");
            if(write.sendwritingvalue_flag)
            {
                write.sendwritingvalue_flag = false;
                write.unsuccesful_flag = true;
            }
            ++errorcount;
            packageerror = true;
        break;

		case ILLEGAL_DATA_ERR:
//			qDebug() << "byte error";
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podngred,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            ui->statusBar->showMessage(received+" - Illegal Data ERROR");
            if(write.sendwritingvalue_flag)
            {
                write.sendwritingvalue_flag = false;
                write.unsuccesful_flag = true;
            }
            ++errorcount;
            packageerror = true;
		break;

		case ILLEGAL_FUNC_ERR:
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podngred,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            ui->statusBar->showMessage(received+" - Illegal Function ERROR");
            if(write.sendwritingvalue_flag)
            {
                write.sendwritingvalue_flag = false;
                write.unsuccesful_flag = true;
            }
            ++errorcount;
            packageerror = true;
        break;

		case ILLEG_DAT_ADR_ERR:
//			qDebug() << "data ERROR";
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podngred,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            ui->statusBar->showMessage(received+" - Illegal Data Address ERROR");
            if(write.sendwritingvalue_flag)
            {
                write.sendwritingvalue_flag = false;
                write.unsuccesful_flag = true;
            }
            ++errorcount;
            packageerror = true;
		break;

		case  ASCII_READ_COILS:
//			qDebug() << "ASCII_READ_COILS";
//            packagecount++;
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podnggreen,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            ui->statusBar->showMessage(received);
            if(!(write.sendwritingvalue_flag))
                PrintResults();
            else
                write.sendwritingvalue_flag = false;
            packageerror = false;
		break;

		case  ASCII_READ_DIS_IN:
//			qDebug() << "ASCII_READ_DIS_IN";
//            packagecount++;
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podnggreen,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            ui->statusBar->showMessage(received);
            if(!(write.sendwritingvalue_flag))
                PrintResults();
            else
                write.sendwritingvalue_flag = false;
            packageerror = false;
		break;

		case  ASCII_READ_HOLD_REG:
//			qDebug() << "ASCII_READ_HOLD_REG";
//            packagecount++;
            if(write.Limited && ((ui->tableWidget->item(limitRow,limitColumn)->text().toInt() < write.Min_Limit_S) ||  (ui->tableWidget->item(limitRow,limitColumn)->text().toInt() > write.Max_Limit_S)))
            {
                limitcounter++;
                statusLimitCounter->setText("Lexceeded:" + QString::number( limitcounter, 10 ).toUpper() + ": " + ui->tableWidget->item(limitRow,limitColumn)->text().toInt());
            }
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podnggreen,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            ui->statusBar->showMessage(received);
            if(!(write.sendwritingvalue_flag))
                PrintResults();
            else
                write.sendwritingvalue_flag = false;
            packageerror = false;
		break;

		case  ASCII_IN_REG:
//			qDebug() << "ASCII_IN_REG";
//            packagecount++;
            if(write.Limited && ((ui->tableWidget->item(limitRow,limitColumn)->text().toInt() < write.Min_Limit_S) ||  (ui->tableWidget->item(limitRow,limitColumn)->text().toInt() > write.Max_Limit_S)))
            {
//                if(ui->tableWidget->item(limitRow,limitColumn)->text().toInt() < write.Min_Limit_S)
//                    qDebug() << (ui->tableWidget->item(limitRow,limitColumn)->text().toInt()) << " " << "min" << write.Min_Limit_S;
//                if (ui->tableWidget->item(limitRow,limitColumn)->text().toInt() > write.Max_Limit_S)
//                    qDebug() << "max" << write.Max_Limit_S;
                limitcounter++;
                statusLimitCounter->setText("Lexceeded:" + QString::number( limitcounter, 10 ).toUpper() + ": " + ui->tableWidget->item(limitRow,limitColumn)->text());
            }
//            statusLimitCounter->setText("Lexceeded:" + QString::number( limitcounter, 10 ).toUpper());
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podnggreen,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            ui->statusBar->showMessage(received);
            if(!(write.sendwritingvalue_flag))
                PrintResults();
            else
                write.sendwritingvalue_flag = false;
            packageerror = false;
		break;

        case  ASCII_WRITE_EEPROM:
//            packagecount++;
            statusPcount->setText("Pcount:" + QString::number( packagecount, 10 ).toUpper());
            statusImage2->setPixmap(QPixmap::fromImage(podnggreen,Qt::AutoColor));
            statusEcount->setText("Ecount:" + QString::number( errorcount, 10 ).toUpper());
            write.sendwritingvalue_flag = false;
            write.writingsuccesful_flag = true;
            write.unsuccesful_flag = false;
//            qDebug() << "Hello world.";
        break;

		default:
		break;
		}
//		received.clear();
	}
}

void MainWindow::PrintResults()
{
	QString datatemp;
	short signedData;
	unsigned short UsignedData;
	char signedData8;
	unsigned char UsignedData8;
	bool ok;
    if((receivedData.length()/4)<=10)
    {
        ui->tableWidget->setRowCount(receivedData.length()/4);
        ui->tableWidget->setColumnCount(1);
    }
    else
    {
        ui->tableWidget->setRowCount(10);
        ui->tableWidget->setColumnCount((receivedData.length()/40)+1);
    }
    for(int i=0; i<receivedData.length()/4; i++)
    {
        datatemp = receivedData.mid(i*4,4);
        if(ui->DataTypecomboBox->currentIndex() == 0)			//signed
        {
            if(255 > datatemp.toUShort(&ok,16))
            {
                signedData8 = datatemp.toShort(&ok,16);
                ui->tableWidget->setItem(i%10, i/10, new QTableWidgetItem(QString::number(signedData8, 10 ).toUpper()));

            }
            else
                ui->tableWidget->setItem(i%10, i/10, new QTableWidgetItem("Out of Range"));
        }
        else if(ui->DataTypecomboBox->currentIndex() == 1)		//unsigned
        {
            if(255 > datatemp.toUShort(&ok,16))
            {
                UsignedData8 = datatemp.toUShort(&ok,16);
                ui->tableWidget->setItem(i%10, i/10, new QTableWidgetItem(QString::number(UsignedData8, 10 ).toUpper()));
            }
            else
                ui->tableWidget->setItem(i%10, i/10, new QTableWidgetItem("Out of Range"));
        }
        else if(ui->DataTypecomboBox->currentIndex() == 2)			//signed
        {
            signedData = datatemp.toUShort(&ok,16);
            ui->tableWidget->setItem(i%10, i/10, new QTableWidgetItem(QString::number(signedData, 10 ).toUpper()));
        }
        else if(ui->DataTypecomboBox->currentIndex() == 3)		//unsigned
        {
            UsignedData = datatemp.toUShort(&ok,16);
            ui->tableWidget->setItem(i%10, i/10, new QTableWidgetItem(QString::number(UsignedData, 10 ).toUpper()));
        }
        else if(ui->DataTypecomboBox->currentIndex() == 4)		//hex
            ui->tableWidget->setItem(i%10, i/10, new QTableWidgetItem("0x" + datatemp));
        else if(ui->DataTypecomboBox->currentIndex() == 5)		//binary
        {
            UsignedData = datatemp.toUShort(&ok,16);
            ui->tableWidget->setItem(i%10, i/10, new QTableWidgetItem(QString::number(UsignedData, 2 ).toUpper()));
        }
        else
            ui->tableWidget->setItem(i%10, i/10, new QTableWidgetItem(datatemp));
    }
}

void MainWindow::serialReceived()
{
//    qDebug() << "received";
    TimeOuttimer->stop();
	QByteArray ucUartTemp;
	ucUartTemp = serial->readAll();
	if (ucUartTemp.at(0) == ':')
	{
		received.clear();
		packageStarted = true;
		Charerror = false;
	}
	if (!((ucUartTemp.at(0) != ':' || ucUartTemp.at(0) != 10 || ucUartTemp.at(0) != 13 ||ucUartTemp.at(0) < '0' && ucUartTemp.at(0) > '9') || (ucUartTemp.at(0) < 'A' && ucUartTemp.at(0) > 'F')))
	{
		Charerror = true;
		received.clear();
	}

	if(packageStarted)
	{
		received.append(ucUartTemp);
		int length = received.length();
		if(received.at(0) == ':' && received.at(length-1) == 10 && received.at(length-2) == 13)
		{
			Sci_New_Packet = true;
			packageStarted = false;
		}
	}
	else
		received.clear();
}


//void MainWindow::customMenuRequested(QPoint pos){
//	QModelIndex index=ui->tableWidget->indexAt(pos);

//	QMenu *menu=new QMenu(this);
//	menu->addAction(new QAction("Action 1", this));
//	menu->addAction(new QAction("Action 2", this));
//	menu->addAction(new QAction("Action 3", this));
//	menu->popup(table->viewport()->mapToGlobal(pos));
//}


void MainWindow::on_tableWidget_customContextMenuRequested(const QPoint &pos)
{
//	QMenu *menu=new QMenu(this);
//	menu->addAction(new QAction("Action 1", this));
//	menu->popup(ui->tableWidget->viewport()->mapToGlobal(pos));
	WrightValueDialog write;
	write.setModal(true);
	write.exec();

}

void MainWindow::on_tableWidget_cellDoubleClicked(int row, int column)
{
	QString DcId, DcAdd , Dcvalue;
//	ui->tableWidget->setItem(0, 0, new QTableWidgetItem("123"));
//	transferdata[1] = "asd";
//	WrightValueDialog *write = new WrightValueDialog;
	//write.setModal(true);
//	Quantity = ui->tableWidget->itemDoubleClicked();
    if(pollbutonbool)
    {
        DcId = ui->ID_lineEdit->text();
        DcAdd = ui->Add_lineEdit->text();
        int rowi,coli,indexi;
        coli = ui->tableWidget->columnCount();
        rowi = ui->tableWidget->rowCount();
        indexi = rowi*column+row;
    //	Dcvalue = ui->tableWidget->item(row, column)->text();
        QTableWidgetItem *itab = ui->tableWidget->item(row, column);
        if (itab) {
            Dcvalue = itab->text();
        }
    //	Quantity = ui->Qua_lineEdit->text();
        write.setWindowTitle("Write Value");
        write.show();
        emit osman(DcId,DcAdd,Dcvalue,indexi);
    //	qDebug() << ID;
    //	write.
    }
    else
    {
        QMessageBox::information(this, "Information!", "<p>Please, first start polling!</p>");
//                goto label1;
    }
    limitRow = row;
    limitColumn = column;
}

void MainWindow::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
//	WrightValueDialog write;
//	write.setModal(true);
//	write.exec();
}

void MainWindow::on_actionSetup_triggered()
{
	setupdialog.setModal(true);
	emit setupopen();
	setupdialog.exec();
	SetupEntered = true;

}

void MainWindow::on_actionOne_Register_Graphical_View_triggered()
{
	GraphicSettings.setModal(true);
	emit setupopen();
	GraphicSettings.exec();
	GraphicSetupEntered = true;
}

void MainWindow::on_actionStart_triggered()
{
	ui->actionStart->setDisabled(true);
	ui->actionPause->setEnabled(true);
	dataTimer.start(0);
}

void MainWindow::on_actionPause_triggered()
{
	ui->actionPause->setDisabled(true);
	ui->actionStart->setEnabled(true);
	dataTimer.stop();
}

void MainWindow::on_actionContinue_triggered()
{
	ui->actionPause->setEnabled(true);
	ui->actionStart->setEnabled(true);
	osmanint = osmanint*2;
}



void MainWindow::setupRealtimeDataDemo(QCustomPlot *customPlot)
{
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  QMessageBox::critical(this, "", "You're using Qt < 4.7, the realtime data demo needs functions that are available with Qt 4.7 to work properly");
#endif
//  demoName = "Real Time Data Demo";

  // include this section to fully disable antialiasing for higher performance:

  customPlot->setNotAntialiasedElements(QCP::aeAll);
  QFont font;
  font.setStyleStrategy(QFont::NoAntialias);
  customPlot->xAxis->setTickLabelFont(font);
  customPlot->yAxis->setTickLabelFont(font);
  customPlot->legend->setFont(font);

  customPlot->addGraph(); // blue line
  customPlot->graph(0)->setPen(QPen(Qt::blue));
  customPlot->graph(0)->setBrush(QBrush(QColor(240, 255, 200)));
  customPlot->graph(0)->setAntialiasedFill(false);
//  ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc, 5));
  ui->plot->graph(0)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, Qt::blue, Qt::white, 5));
  customPlot->addGraph(); // red line
  customPlot->graph(1)->setPen(QPen(Qt::red));
  customPlot->graph(0)->setChannelFillGraph(customPlot->graph(1));

  customPlot->addGraph(); // blue dot
  customPlot->graph(2)->setPen(QPen(Qt::blue));
  customPlot->graph(2)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(2)->setScatterStyle(QCPScatterStyle::ssDisc);
  customPlot->addGraph(); // red dot
  customPlot->graph(3)->setPen(QPen(Qt::red));
  customPlot->graph(3)->setLineStyle(QCPGraph::lsNone);
  customPlot->graph(3)->setScatterStyle(QCPScatterStyle::ssDisc);

  customPlot->xAxis->setTickLabelType(QCPAxis::ltDateTime);
  customPlot->xAxis->setDateTimeFormat("hh:mm:ss");
  customPlot->xAxis->setAutoTickStep(true);
//  customPlot->xAxis->setTickStep(10);
  customPlot->axisRect()->setupFullAxesBox();

  // make left and bottom axes transfer their ranges to right and top axes:
  connect(customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), customPlot->yAxis2, SLOT(setRange(QCPRange)));

  // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
  connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
  dataTimer.start(0); // Interval 0 means to refresh as fast as possible
}
void MainWindow::realtimeDataSlot()
{
  // calculate two new data points:
#if QT_VERSION < QT_VERSION_CHECK(4, 7, 0)
  double key = 0;
#else
  double key = QDateTime::currentDateTime().toMSecsSinceEpoch()/1000.0;
#endif
  static double lastPointKey = 0;
  if (key-lastPointKey > 0.20) // at most add point every 200 ms
  {
    double value0 = qSin(key); //sin(key*1.6+cos(key*1.7)*2)*10 + sin(key*1.2+0.56)*20 + 26;
    double value1 = qCos(key); //sin(key*1.3+cos(key*1.2)*1.2)*7 + sin(key*0.9+0.26)*24 + 26;
    // add data to lines:
    ui->plot->graph(0)->addData(key, value0);
    ui->plot->graph(1)->addData(key, value1);
    // set data of dots:
//	ui->plot->graph(2)->clearData();
//	ui->plot->graph(2)->addData(key, value0);
//	ui->plot->graph(3)->clearData();
//	ui->plot->graph(3)->addData(key, value1);
    // remove data of lines that's outside visible range:
    if(GraphicSettings.RemTime1 != 0)
        ui->plot->graph(0)->removeDataBefore(key-GraphicSettings.RemTime1);
//	ui->plot->graph(0)->removeDataBefore(key-2);
    if(GraphicSettings.RemTime2 != 0)
        ui->plot->graph(1)->removeDataBefore(key-GraphicSettings.RemTime2);
//	ui->plot->graph(1)->removeDataBefore(key-2);
//	if(GraphicSettings.RemTime3 != 0)
//		ui->plot->graph(4)->removeDataBefore(key-GraphicSettings.RemTime3);

    //rescale
    // rescale value (vertical) axis to fit the current data:
    if(osmanint < 10) ui->plot->graph(0)->rescaleValueAxis();
    if(osmanint < 10) ui->plot->graph(1)->rescaleValueAxis(true);
    lastPointKey = key;



    double xtempvalratio, ytempvalratio;
//	xtempvalratio = (xmax - xmin) / xtempval;
//	ytempvalratio = (ui->verticalSlider->maximum() - ui->verticalSlider->minimum()) / ui->verticalSlider->value();

    //scale sliders and give new values
    if(!firstvalueB)
    {
        if(value0 < 0)
        {
            ymin = -value0;
            ymax = -value0;
        }
        firstvalueB = true;
//		yratio = (ymax - ymin);
    }
    if(value0 >= 0)
    {
        if(value0 < ymin)
            ymin = value0;
        if(value0 > ymax)
            ymax = value0;
    }
    else
    {
        if(-value0 < ymin)
            ymin = -value0;
        if(-value0 > ymax)
            ymax = -value0;
    }
//	yratio = (ymax - ymin)/yratio;
//	ui->verticalSlider->setValue(qRound(ui->plot->yAxis->range().size()/yratio));
//	qDebug() << yratio;
  }
  // make key axis range scroll with the data (at a constant range size of 8):
    if(osmanint < 10)
        ui->plot->xAxis->setRange(key+0.25,ui->plot->xAxis->range().size(), Qt::AlignLeft);
    osmanint++;
    ui->plot->replot();
    xkey = key+0.25;

  // calculate frames per second:
  static double lastFpsKey;
  static int frameCount;
  ++frameCount;
/*  if (key-lastFpsKey > 2) // average fps over 2 seconds
  {
    ui->statusBar->showMessage(
          QString("%1 FPS, Total Data points: %2")
          .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
          .arg(ui->plot->graph(0)->data()->count()+ui->customPlot->graph(1)->data()->count())
          , 0);
    lastFpsKey = key;
    frameCount = 0;
  }*/
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
//	ui->horizontalScrollBar->setValue(qRound(position*100.0)); // adjust position of scroll bar slider
//	ui->horizontalScrollBar->setPageStep(qRound(position*100.0)); // adjust size of scroll bar slider
//	ui->plot->xAxis->setRange(xkey, position, Qt::AlignCenter);			//position,size,alignment
    ui->plot->xAxis->setRange(ui->plot->xAxis->range().center(),position,Qt::AlignCenter);
}

//void MainWindow::on_verticalSlider_sliderMoved(int position)
//{
////	ui->verticalScrollBar->setValue(qRound(-position*100.0)); // adjust position of scroll bar slider
////	ui->verticalScrollBar->setPageStep(qRound(position*100.0)); // adjust size of scroll bar slider
////	ui->plot->yAxis->setRange(ykey, position, Qt::AlignCenter);		//position,size,alignment
////	ui->plot->yAxis->setRange(ui->plot->yAxis->range().center(),position, Qt::AlignCenter);	//position,size,alignment
////	qDebug() << ykey << "," << position;
//}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
//	ui->horizontalScrollBar->setValue(qRound(value*100.0)); // adjust position of scroll bar slider
//	ui->horizontalScrollBar->setPageStep(qRound(value*100.0)); // adjust size of scroll bar slider
//	ui->plot->xAxis->setRange(xkey, value, Qt::AlignCenter);
}

//void MainWindow::on_verticalSlider_valueChanged(int value)
//{
////	ui->verticalScrollBar->setValue(qRound(-value*100.0)); // adjust position of scroll bar slider
////	ui->verticalScrollBar->setPageStep(qRound(value*100.0)); // adjust size of scroll bar slider
////	ui->plot->yAxis->setRange(ykey, value, Qt::AlignCenter);
//}

void MainWindow::plotMousePress(QMouseEvent *event)
{
	if(event->buttons() == Qt::RightButton)
	{
		QCPAbstractPlottable *plottable =
		ui->plot->plottableAt(event->localPos());

		if(plottable)
		{
			double x = ui->plot->xAxis->pixelToCoord(event->localPos().x());
			double y = ui->plot->yAxis->pixelToCoord(event->localPos().y());

			QCPBars *bar =
			qobject_cast<QCPBars*>(plottable);

			if(bar)
			{
				double key = 0;
				double value = 0;

				bool ok = false;
				double m = std::numeric_limits<double>::max();

				foreach(QCPBarData data, bar->data()->values())
				{
					double d = qAbs(x - data.key);

					if(d < m)
					{
						key = data.key;
						value = data.value;

						ok = true;
						m = d;
					}
				}

				if(ok)
				{
					for(QCPBars *below = bar->barBelow();
					((below != NULL) && below->data()->contains(key));
					below = below->barBelow())
					{
						value += below->data()->value(key).value;
					}

//					QPoint pointtemp = QPoint(Qt::DateFormat(event->globalPos().x()), event->globalPos().y());
//					QToolTip::hideText();
//					QToolTip::showText(pointtemp,
					QToolTip::hideText();
					QToolTip::showText(event->globalPos(),
					tr("<table>"
						 "<tr>"
						   "<th colspan=\"2\">%L1</th>"
						 "</tr>"
						 "<tr>"
						   "<td>Key:</td>" "<td>%L2</td>"
						 "</tr>"
						 "<tr>"
						   "<td>Val:</td>" "<td>%L3</td>"
						 "</tr>"
					   "</table>").
					   arg(bar->name().isEmpty() ? "..." : bar->name()).
					   arg(key).
					   arg(value),
					   ui->plot, ui->plot->rect());
				}
			}

			QCPCurve *curve =
			qobject_cast<QCPCurve*>(plottable);

			if(curve)
			{
				double key = 0;
				double value = 0;

				bool ok = false;
				double m = std::numeric_limits<double>::max();

				foreach(QCPCurveData data, curve->data()->values())
				{
					double d = qSqrt(qPow(x-data.key,2)+qPow(y-data.value,2));

					if(d < m)
					{
						key = data.key;
						value = data.value;

						ok = true;
						m = d;
					}
				}

				if(ok)
				{
//					QPoint pointtemp = QPoint(Qt::DateFormat(event->globalPos().x()), event->globalPos().y());
//					QToolTip::hideText();
//					QToolTip::showText(pointtemp,
					QToolTip::hideText();
					QToolTip::showText(event->globalPos(),
					tr("<table>"
						 "<tr>"
						   "<th colspan=\"2\">%L1</th>"
						 "</tr>"
						 "<tr>"
						   "<td>Time:</td>" "<td>%L2</td>"
						 "</tr>"
						 "<tr>"
						   "<td>Val:</td>" "<td>%L3</td>"
						 "</tr>"
					   "</table>").
					   arg(curve->name().isEmpty() ? "..." : curve->name()).
					   arg(key).
					   arg(value),
					   ui->plot, ui->plot->rect());
				}
			}

			QCPGraph *graph =
			qobject_cast<QCPGraph*>(plottable);

			if(graph)
			{
				double key = 0;
				double value = 0;

				bool ok = false;
				double m = std::numeric_limits<double>::max();

				foreach(QCPData data, graph->data()->values())
				{
					double d = qAbs(x - data.key);

					if(d < m)
					{
						key = data.key;
						value = data.value;

						ok = true;
						m = d;
					}
				}

				if(ok)
				{
//					QPoint pointtemp = QPoint(Qt::DateFormat(event->globalPos().x()), event->globalPos().y());
//					QToolTip::hideText();
//					QToolTip::showText(pointtemp,		//burada yazının basılacağı koordinatlar veriliyor
					QToolTip::hideText();
					QToolTip::showText(event->globalPos(),
					tr("<table>"
						 "<tr>"
						   "<th colspan=\"2\">%L1</th>"
						 "</tr>"
						 "<tr>"
						   "<td>Time2:</td>" "<td>%L2</td>"
						 "</tr>"
						 "<tr>"
						   "<td>Val:</td>" "<td>%L3</td>"
						 "</tr>"
					   "</table>").
					   arg(graph->name().isEmpty() ? "..." : graph->name()).
					   arg(key).		//Time
					   arg(value),		//Val
					   ui->plot, ui->plot->rect());
				}
			}

			QCPStatisticalBox *box =
			qobject_cast<QCPStatisticalBox*>(plottable);

			if(box)
			{
				double key = box->key();
				double minimum = box->minimum();
				double lowerQuartile = box->lowerQuartile();
				double median = box->median();
				double upperQuartile = box->upperQuartile();
				double maximum = box->maximum();

//				QPoint pointtemp = QPoint(Qt::DateFormat(event->globalPos().x()), event->globalPos().y());
//				QToolTip::hideText();
//				QToolTip::showText(pointtemp,
				QToolTip::hideText();
				QToolTip::showText(event->globalPos(),
				tr("<table>"
					 "<tr>"
					   "<th colspan=\"2\">%L1</th>"
					 "</tr>"
					 "<tr>"
					   "<td>Key:</td>" "<td>%L2</td>"
					 "</tr>"
					 "<tr>"
					   "<td>Min:</td>" "<td>%L3</td>"
					 "</tr>"
					 "<tr>"
					   "<td>L-Q:</td>" "<td>%L4</td>"
					 "</tr>"
					 "<tr>"
					   "<td>Mid:</td>" "<td>%L5</td>"
					 "</tr>"
					 "<tr>"
					   "<td>U-Q:</td>" "<td>%L6</td>"
					 "</tr>"
					 "<tr>"
					   "<td>Max:</td>" "<td>%L7</td>"
					 "</tr>"
				   "</table>").
				   arg(box->name().isEmpty() ? "..." : box->name()).
				   arg(key).
				   arg(minimum).
				   arg(lowerQuartile).
				   arg(median).
				   arg(upperQuartile).
				   arg(maximum),
				   ui->plot, ui->plot->rect());
			}
		}
	}
}


QDateTime MainWindow::julianDoubleToDateTime(double julian)
{
	// The day number is the integer part of the date
	int julianDays = qFloor(julian);
	QDate d = QDate::fromJulianDay(julianDays);

	// The fraction is the time of day
	double julianMSecs = (julian - static_cast<double>(julianDays)) * 86400.0 * 1000;

	// Julian days start at noon (12:00 UTC)
	QTime t = QTime(12, 0, 0, 0).addMSecs(qRound(julianMSecs));

	return QDateTime(d, t, Qt::UTC);
}

void MainWindow::on_pushButton_3_clicked()
{
    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText(received);
}
