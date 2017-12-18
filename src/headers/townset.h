#ifndef TOWNSET_H
#define TOWNSET_H
#ifndef MAXZONES
#define MAXZONES 10
#endif

#include <QDialog>
#include <QTableWidgetItem>
#include "src/headers/spinboxdelegate.h"
#include "ui_townset.h"

extern QStringList townnames;
extern int timeshifts[MAXZONES];
extern int timeFreq[MAXZONES];
extern int towncnt;
extern int prev_towncnt;

class townset : public QDialog
{
	Q_OBJECT

public:
	townset(QWidget *parent = 0);
	~townset();
signals:
    void _resetObjects();
    void _saveSettings();

private slots:
	void on_twTowns_currentItemChanged(QTableWidgetItem*,QTableWidgetItem*);
	void on_pbSave_clicked();
	void on_pbDown_clicked();
	void on_pbUp_clicked();
	void on_pbDel_clicked();
	void on_pbAdd_clicked();
	void on_pbExit_clicked();
	void updateButtons();

private:
	Ui::townsetClass ui;
};

#endif // TOWNSET_H
