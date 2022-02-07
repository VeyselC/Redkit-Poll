#ifndef GRAPH_SET_H
#define GRAPH_SET_H

#include <QDialog>
#include <QColorDialog>

namespace Ui {
class Graph_set;
}

class Graph_set : public QDialog
{
	Q_OBJECT

public:
	explicit Graph_set(QWidget *parent = 0);
	~Graph_set();
	void WriteSettings();

	bool Check1 = false,Check2 = false,Check3 = false,StartRequest = false;
	int ID1 = 1,ID2 = 1,ID3 = 1;
	int Add1 = 0,Add2 = 0,Add3 = 0;
	int Time1 = 1000,Time2 = 1000,Time3 = 1000;
	int RemTime1 = 0, RemTime2 = 0, RemTime3 = 0;
	int Function1 = 3,Function2 = 3,Function3 = 3;
	QColor Color1 = QColor(20,187,22),Color2 = QColor(241,19,32),Color3 = QColor(209,21,230);

private slots:
	void on_pushButton_clicked();

	void on_comboBox_currentIndexChanged(int index);

	void on_pushButton_2_clicked();

	void on_pushButton_3_clicked();

	void on_lineEdit_editingFinished();

	void on_lineEdit_2_editingFinished();

	void on_lineEdit_3_editingFinished();

	void on_checkBox_stateChanged(int arg1);

	void on_comboBox_2_currentIndexChanged(int index);

private:
	Ui::Graph_set *ui;

};

#endif // GRAPH_SET_H
