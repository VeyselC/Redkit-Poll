#include "wrightvaluedialog.h"
#include "ui_wrightvaluedialog.h"
#include <QToolButton>
#include <QSplashScreen>
//#include "mainwindow.h"


WrightValueDialog::WrightValueDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::WrightValueDialog)
{
	ui->setupUi(this);
	//connect(d.pushButton, SIGNAL("clicked()"), this->clicked());
//	connect(, SIGNAL(clicked()),this,SLOT(openingslot());
//	connect(,SIGNAL(osman()),this,SLOT(open));
//    writeserial = new QSerialPort(this);
//    connect(writeserial,SIGNAL(readyRead()),this,SLOT(writeserialReceived()));

    connect(WriteTimeOuttimer, SIGNAL(timeout()), this, SLOT(WriteTimeOuttimerSlot()));
    connect(writetimer, SIGNAL(timeout()), this, SLOT(timerSlot()));
    writetimer->start(10);

//    int w = ui->Resultlabel->width();
    int h = 15/*ui->Resultlabel->height()*/;

    redI = new QImage(":/images/red.png");
    greenI = new QImage(":/images/green.png");
    yellowI = new QImage(":/images/yellow.png");

    podngred = redI->scaled(h,h,Qt::KeepAspectRatio);
    podnggreen = greenI->scaled(h,h,Qt::KeepAspectRatio);
    podngyellow = yellowI->scaled(h,h,Qt::KeepAspectRatio);


    ui->Resultlabel->setPixmap(QPixmap::fromImage(podngyellow,Qt::AutoColor));

}

WrightValueDialog::~WrightValueDialog()
{
	delete ui;
    writetimer->stop();
    WriteTimeOuttimer->stop();
}

void WrightValueDialog::WriteTimeOuttimerSlot()
{

//    ui->statusBar->showMessage("Time Out Error");

}


void WrightValueDialog::openingslot(QString ID1,QString Address1,QString Quantity1,int Index)
{
//	ID,Address,Quantity
	int addres = (Address1.toInt()+Index);
	ui->lineEdit->setText(ID1);
	ui->lineEdit_2->setText(Address1);
	ui->lineEdit_2->setText(QString::number(addres));
	ui->lineEdit_3->setText(Quantity1);
	ui->lineEdit_4->setText("");

    ui->lineEdit_Min_Limit->setText("---");
    ui->lineEdit_Max_Limit->setText("---");


    Limited = false;
    good = false;
    error = false;

//	ui->lineEdit->setText("asd");
//	qDebug() << "Hello world.";
//	qDebug() << ID1;
//	ui->lineEdit->setText("1");
}


void WrightValueDialog::on_pushButton_clicked()
{
      //fonksiyon listesi ile karışmaması için yazma fonksiyonu 666 ile çağırılıyor. daha sonradan '6' atanıyor.
    writetable = ModBusMaster_new.PreparePack(ui->lineEdit->text(),666/*6*/,ui->lineEdit_2->text(),ui->lineEdit_3->text());
    sendwritingvalue_flag = true;
}

void WrightValueDialog::timerSlot()
{
    int length = received.length(), kuzeroch, function;
    bool ok;

    if(writingsuccesful_flag)
    {
        writingsuccesful_flag = false;
        ui->lineEdit_4->setText(writetable);
        good = true;
        error = false;
    }
    if(unsuccesful_flag)
    {
        good = false;
        error = true;
    }

    if(good && !error)
    {
        ui->Resultlabel->setPixmap(QPixmap::fromImage(podnggreen,Qt::AutoColor));
    }
    else if(!good && error)
    {
        ui->Resultlabel->setPixmap(QPixmap::fromImage(podngred,Qt::AutoColor));
    }
    else
    {
        ui->Resultlabel->setPixmap(QPixmap::fromImage(podngyellow,Qt::AutoColor));
    }

    if(ui->checkBox->checkState() && good)
    {
        this->close();
    }
}

void WrightValueDialog::on_pushButton_Limit_clicked()
{
//    if((ui->lineEdit_Min_Limit->text().toInt() != 0) || (ui->lineEdit_Max_Limit->text().toInt() != 0))
//    {
//        if(ui->lineEdit_Min_Limit->text().toInt() < 0)
            Min_Limit_S = ui->lineEdit_Min_Limit->text().toInt();
//        else
//            Min_Limit_US = ui->lineEdit_Min_Limit->text().toInt();

//        if(ui->lineEdit_Max_Limit->text().toInt() < 0)
            Max_Limit_S = ui->lineEdit_Max_Limit->text().toInt();
//        else
//            Max_Limit_US = ui->lineEdit_Max_Limit->text().toInt();
        Limited = true;
        this->close();
//    }
}
