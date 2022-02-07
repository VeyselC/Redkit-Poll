#ifndef SETUPDIALOG_H
#define SETUPDIALOG_H

#include <QDialog>
#include <QtSerialPort/QtSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QKeyEvent>

#include "modbusmaster.h"

namespace Ui {
class SetupDialog;
}

class SetupDialog : public QDialog
{
	Q_OBJECT

public:
	explicit SetupDialog(QWidget *parent = 0);
	~SetupDialog();
	void writeSettings();
	void LoadSettings();

	//variables
	int osman;
    int SelectedConnection, SelectedPort, SelectedBaud, SelectedDataBits, SelectedStop, SelectedParity,SelectedTimeOut;
	bool SelectedComMode,savesets;

protected:
	void keyPressEvent(QKeyEvent *);

public slots:
	void openingslot();

private:
	Ui::SetupDialog *ui;

private slots:
	void on_pushButton_clicked();

	void on_CancelpushButton_clicked();

};

#endif // SETUPDIALOG_H
