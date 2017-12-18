#include <QSettings>
#include "src/headers/townset.h"

townset::townset(QWidget *parent)
	: QDialog(parent)
{
	ui.setupUi(this);
    //
    this->setWindowFlags(Qt::SplashScreen);
    //
    ui.twTowns->setColumnCount(3);
    ui.twTowns->setHorizontalHeaderLabels(QStringList()                                          
                                          << tr("All times are")                                          
                                          << tr("The difference with the current time\nin minutes")
                                          << tr("Freq"));
    ui.twTowns->resizeColumnsToContents();
    ui.twTowns->resizeRowsToContents();
    ui.twTowns->setColumnWidth(2,63);
    this->setWindowTitle(tr("Dials"));
	for (int i=0; i<towncnt; i++)
	{
		ui.twTowns->insertRow(i);
        QTableWidgetItem *itemleft  = new QTableWidgetItem(townnames[i],0);
        QTableWidgetItem *itemcenter= new QTableWidgetItem(QString::number(timeshifts[i]));
        QTableWidgetItem *itemright = new QTableWidgetItem(QString::number(timeFreq[i]));
		ui.twTowns->setItem(i,0,itemleft);
        ui.twTowns->setItem(i,1,itemcenter);
        ui.twTowns->setItem(i,2,itemright);
	}
    SpinBoxDelegate *d = new SpinBoxDelegate();
    SpinBoxDelegate *d_frq= new SpinBoxDelegate();
    d_frq->set_minmax(0,60000); // minute
    ui.twTowns->setItemDelegateForColumn(1,d);
    ui.twTowns->setItemDelegateForColumn(2,d_frq);
	updateButtons();
}

townset::~townset()
{

}

void townset::updateButtons()
{
    ui.pbAdd->setDisabled(towncnt>=MAXZONES);
	ui.pbDel->setEnabled(ui.twTowns->currentItem() && (ui.twTowns->currentRow()>=0));
	ui.pbUp->setDisabled(ui.twTowns->currentRow()==0 || ui.twTowns->rowCount()==0);
	ui.pbDown->setDisabled(ui.twTowns->currentRow()==ui.twTowns->rowCount()-1 || ui.twTowns->rowCount()==0);
}
void townset::on_pbExit_clicked()
{
	reject();
}

void townset::on_pbAdd_clicked()
{
	if (ui.twTowns->rowCount()<MAXZONES)
	{
		if (ui.twTowns->currentRow()>=0)
		{
			int cur = ui.twTowns->currentRow()+1;
			ui.twTowns->insertRow(cur);
            QTableWidgetItem *itemleft = new QTableWidgetItem(tr("Name"),0);
			QTableWidgetItem *itemright= new QTableWidgetItem(0);
            QTableWidgetItem *itemcenter= new QTableWidgetItem(0);

			ui.twTowns->setItem(cur,0,itemleft);
            ui.twTowns->setItem(cur,1,itemcenter);
            ui.twTowns->setItem(cur,2,itemright);
			ui.twTowns->setCurrentCell(cur, ui.twTowns->currentColumn());
		}
		else
		{
			ui.twTowns->insertRow(0);
            QTableWidgetItem *itemleft = new QTableWidgetItem(tr("Name"),0);
            QTableWidgetItem *itemright= new QTableWidgetItem(0);
            QTableWidgetItem *itemcenter= new QTableWidgetItem(0);

			ui.twTowns->setItem(0,0,itemleft);
            ui.twTowns->setItem(0,1,itemcenter);
            ui.twTowns->setItem(0,2,itemright);
			ui.twTowns->setCurrentCell(0,0);
		}
	}
	updateButtons();
}

void townset::on_pbDel_clicked()
{
	int cur = ui.twTowns->currentRow();
	if (cur>=0)
	{
		ui.twTowns->removeRow(ui.twTowns->currentRow());
		if (ui.twTowns->rowCount())
		{
			if (cur)
				ui.twTowns->setCurrentCell(cur-1,0);
			else
				ui.twTowns->setCurrentCell(cur,0);
		}
	}
	updateButtons();
}

void townset::on_pbUp_clicked()
{
	if (ui.twTowns->currentRow())
	{
		int cur = ui.twTowns->currentRow();
		QTableWidgetItem *interop = ui.twTowns->takeItem(cur-1,0);
		ui.twTowns->setItem(cur-1,0,ui.twTowns->takeItem(cur,0));
		ui.twTowns->setItem(cur,0, interop);
		interop = ui.twTowns->takeItem(cur-1,1);
		ui.twTowns->setItem(cur-1,1,ui.twTowns->takeItem(cur,1));
		ui.twTowns->setItem(cur,1, interop);
		ui.twTowns->setCurrentCell(cur-1, ui.twTowns->currentColumn());
	}
	updateButtons();
}

void townset::on_pbDown_clicked()
{
	int cur = ui.twTowns->currentRow();
	if (cur>=0 && cur<(ui.twTowns->rowCount()-1))
	{
		QTableWidgetItem *interop = ui.twTowns->takeItem(cur+1,0);
		ui.twTowns->setItem(cur+1,0,ui.twTowns->takeItem(cur,0));
		ui.twTowns->setItem(cur,0, interop);
		interop = ui.twTowns->takeItem(cur+1,1);
		ui.twTowns->setItem(cur+1,1,ui.twTowns->takeItem(cur,1));
		ui.twTowns->setItem(cur,1, interop);
		ui.twTowns->setCurrentCell(cur+1, ui.twTowns->currentColumn());
	}
	updateButtons();
}

void townset::on_pbSave_clicked()
{
//    QMessageBox::information(this,"",ui.twTowns->item(0,1)->text()+"," +ui.twTowns->item(0,2)->text());

	towncnt = ui.twTowns->rowCount();
    //for (int i=towncnt; i<prev_towncnt; i++)
    //{
    // не удалять имена, память для них была получена не по new
    //}
	townnames.clear();
	for (int i=0; (i<towncnt && i<MAXZONES); i++)
	{
		townnames.append(ui.twTowns->item(i,0)->text());
        timeFreq[i]   = ui.twTowns->item(i,2)->text().toInt();
        timeshifts[i] = ui.twTowns->item(i,1)->text().toInt();
	}
	if (towncnt==0)
	{
		towncnt=1;
		townnames.append(QString("local"));
		timeshifts[0]= 0;
        timeFreq[0]  = 1000;// Default
	}
    emit _saveSettings();
    emit _resetObjects();

}


void townset::on_twTowns_currentItemChanged(QTableWidgetItem*,QTableWidgetItem*)
{
	updateButtons();
}
