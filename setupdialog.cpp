#include "setupdialog.h"
#include "ui_setupdialog.h"


SetupDialog::SetupDialog(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SetupDialog)
{
	ui->setupUi(this);
	//main window settings
	this->setWindowTitle("Connection Setup");
	this->setWindowIcon(QIcon("://images/setting tools.ico"));

	//Connection Type
	ui->ConnectionTypecomboBox->addItem("Serial Port");
	ui->ConnectionTypecomboBox->addItem("TCP");
	ui->ConnectionTypecomboBox->addItem("UDP");

	//Com port
	//ui->ComPortcomboBox
	foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
	{
		ui->ComPortcomboBox->addItem(info.description()+ " " + info.portName());
	}


	//Baud rate
	ui->BaudcomboBox->addItem("115200 Baud");
	ui->BaudcomboBox->addItem("57600 Baud");
	ui->BaudcomboBox->addItem("38400 Baud");
	ui->BaudcomboBox->addItem("19200 Baud");
	ui->BaudcomboBox->addItem("9600 Baud");
	ui->BaudcomboBox->addItem("4800 Baud");
	ui->BaudcomboBox->addItem("2400 Baud");
	ui->BaudcomboBox->addItem("1200 Baud");
	ui->BaudcomboBox->setCurrentIndex(4);

	//Data bits
	ui->DataBitscomboBox->addItem("8 Data Bits");
	ui->DataBitscomboBox->addItem("7 Data Bits");

	//stop bits
	ui->StopBitcomboBox->addItem("1 Stop Bit");
	ui->StopBitcomboBox->addItem("2 Stop Bits");

	//Parity
	ui->ParitycomboBox->addItem("None Parity");
	ui->ParitycomboBox->addItem("Odd");
	ui->ParitycomboBox->addItem("Even");

	//com mode
	ui->ASCIIradioButton->setChecked(true);

	//timeout
	ui->TimeoutlineEdit->setText("200");


	writeSettings();
	//set states
//	ui->ConnectionTypecomboBox->setCurrentIndex(SelectedConnection);
//	ui->ComPortcomboBox->setCurrentIndex(SelectedPort);
//	ui->BaudcomboBox->setCurrentIndex(SelectedBaud);
//	ui->DataBitscomboBox->setCurrentIndex(SelectedDataBits);
//	ui->StopBitcomboBox->setCurrentIndex(SelectedStop);
//	ui->ParitycomboBox->setCurrentIndex(SelectedParity);
//	if(!SelectedComMode) ui->ASCIIradioButton->setChecked(true);
}

SetupDialog::~SetupDialog()
{
	delete ui;
}

void SetupDialog::on_pushButton_clicked()
{
	writeSettings();
//	SetupEntered = true;
	this->close();
}

void SetupDialog::keyPressEvent(QKeyEvent *event)
{
	if ( event->key() == Qt::Key_Escape )
	{
		this->close();
//		SetupEntered = true;
//		qDebug() << "esc";
		LoadSettings();
	}
}

void SetupDialog::on_CancelpushButton_clicked()
{
	this->close();
//	SetupEntered = true;
	LoadSettings();
}

void SetupDialog::writeSettings()
{
	SelectedConnection = ui->ConnectionTypecomboBox->currentIndex();
	SelectedPort = ui->ComPortcomboBox->currentIndex();
	SelectedBaud = ui->BaudcomboBox->currentIndex();
	SelectedDataBits = ui->DataBitscomboBox->currentIndex();
	SelectedStop = ui->StopBitcomboBox->currentIndex();
	SelectedParity = ui->ParitycomboBox->currentIndex();
    SelectedTimeOut = ui->TimeoutlineEdit->text().toInt();
	if(ui->ASCIIradioButton->isChecked())
		SelectedComMode = false;
	else SelectedComMode = true;
}

void SetupDialog::openingslot()
{
//	LoadSettings();
}

void SetupDialog::LoadSettings()
{
	ui->ConnectionTypecomboBox->setCurrentIndex(SelectedConnection);
	ui->ComPortcomboBox->setCurrentIndex(SelectedPort);
	ui->BaudcomboBox->setCurrentIndex(SelectedBaud);
	ui->DataBitscomboBox->setCurrentIndex(SelectedDataBits);
	ui->StopBitcomboBox->setCurrentIndex(SelectedStop);
	ui->ParitycomboBox->setCurrentIndex(SelectedParity);
	if(!SelectedComMode) ui->ASCIIradioButton->setChecked(true);
}
