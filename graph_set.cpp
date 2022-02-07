#include "graph_set.h"
#include "ui_graph_set.h"

Graph_set::Graph_set(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Graph_set)
{
	ui->setupUi(this);

	this->setWindowTitle("Graphic Settings");
	this->setWindowIcon(QIcon("://images/graph.png"));

	ui->comboBox->addItem("1");
	ui->comboBox->addItem("2");
	ui->comboBox->addItem("3");

	ui->comboBox_2->addItem("01 Read Coils 0x");
	ui->comboBox_2->addItem("02 Read Discrete Inputs 1x");
	ui->comboBox_2->addItem("03 Read Holding Registers 4x");
	ui->comboBox_2->addItem("04 Read Input Registers 3x");
//	ui->comboBox_2->addItem("05 Write Single Coils 0x");
//	ui->comboBox_2->addItem("06 Write Single Register 0x");
//	if(StartRequest)
//		ui->pushButton->setText("Stop");
}

Graph_set::~Graph_set()
{
	delete ui;
}

void Graph_set::on_comboBox_currentIndexChanged(int index)
{
	if(index+1 == 1)
	{
		ui->checkBox->setChecked(Check1);
		ui->lineEdit->setText(QString::number(ID1,10));
		ui->lineEdit_2->setText(QString::number(Add1,10));
		ui->lineEdit_3->setText(QString::number(Time1,10));
		ui->lineEdit_4->setText(QString::number(RemTime1,10));
		ui->comboBox_2->setCurrentIndex(Function1);
		if(Color1.isValid())
		{
			QString qss = QString("background-color: %1").arg(Color1.name());
			ui->pushButton_3->setStyleSheet(qss);
		}
	}
	else if(index+1 == 2)
	{
		ui->checkBox->setChecked(Check2);
		ui->lineEdit->setText(QString::number(ID2,10));
		ui->lineEdit_2->setText(QString::number(Add2,10));
		ui->lineEdit_3->setText(QString::number(Time2,10));
		ui->lineEdit_4->setText(QString::number(RemTime2,10));
		ui->comboBox_2->setCurrentIndex(Function2);
		if(Color2.isValid())
		{
			QString qss = QString("background-color: %1").arg(Color2.name());
			ui->pushButton_3->setStyleSheet(qss);
		}
	}
	else if(index+1 == 3)
	{
		ui->checkBox->setChecked(Check3);
		ui->lineEdit->setText(QString::number(ID3,10));
		ui->lineEdit_2->setText(QString::number(Add3,10));
		ui->lineEdit_3->setText(QString::number(Time3,10));
		ui->lineEdit_4->setText(QString::number(RemTime3,10));
		ui->comboBox_2->setCurrentIndex(Function3);
		if(Color3.isValid())
		{
			QString qss = QString("background-color: %1").arg(Color3.name());
			ui->pushButton_3->setStyleSheet(qss);
		}
	}
}

void Graph_set::on_pushButton_clicked() //start request
{
	if(!StartRequest)
	{
		StartRequest = true;
		ui->pushButton->setText("Stop");
	}
	else
	{
		StartRequest = false;
		ui->pushButton->setText("Start");
	}
	WriteSettings();
	this->close();
}

void Graph_set::on_pushButton_2_clicked()		//cancel button
{
	this->close();
}

void Graph_set::on_pushButton_3_clicked()
{
	QColor Colortemp = QColorDialog::getColor(Qt::white,this);
	if(Colortemp.isValid())
	{
		QString qss = QString("background-color: %1").arg(Colortemp.name());
		ui->pushButton_3->setStyleSheet(qss);
	}
	if(ui->comboBox->currentIndex()+1 == 1)			Color1 = Colortemp;
	else if(ui->comboBox->currentIndex()+1 == 2)	Color2 = Colortemp;
	else if(ui->comboBox->currentIndex()+1 == 3)	Color3 = Colortemp;
}

void Graph_set::on_lineEdit_editingFinished()
{
	WriteSettings();
}

void Graph_set::on_lineEdit_2_editingFinished()
{
	WriteSettings();
}

void Graph_set::on_lineEdit_3_editingFinished()
{
	WriteSettings();
}

void Graph_set::WriteSettings()
{
	if(ui->comboBox->currentIndex()+1 == 1)
	{
		Check1 = ui->checkBox->isChecked();
		ID1 = ui->lineEdit->text().toInt();
		Add1 = ui->lineEdit_2->text().toInt();
		Time1 = ui->lineEdit_3->text().toInt();
		RemTime1 = ui->lineEdit_4->text().toInt();
		Function1 = ui->comboBox_2->currentIndex();
	}
	else if(ui->comboBox->currentIndex()+1 == 2)
	{
		Check2 = ui->checkBox->isChecked();
		ID2 = ui->lineEdit->text().toInt();
		Add2 = ui->lineEdit_2->text().toInt();
		Time2 = ui->lineEdit_3->text().toInt();
		RemTime2 = ui->lineEdit_4->text().toInt();
		Function2 = ui->comboBox_2->currentIndex();
	}
	else if(ui->comboBox->currentIndex()+1 == 3)
	{
		Check3 = ui->checkBox->isChecked();
		ID3 = ui->lineEdit->text().toInt();
		Add3 = ui->lineEdit_2->text().toInt();
		Time3 = ui->lineEdit_3->text().toInt();
		RemTime3 = ui->lineEdit_4->text().toInt();
		Function3 = ui->comboBox_2->currentIndex();
	}
}

void Graph_set::on_checkBox_stateChanged(int arg1)
{
	WriteSettings();
}

void Graph_set::on_comboBox_2_currentIndexChanged(int index)
{
	WriteSettings();
}
