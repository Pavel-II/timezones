#include "src/headers/timezones.h"

timezones::timezones() : QDialog()
{
	ui.setupUi(this);
    //
	plt = this->palette();
    plt.setColor(QPalette::Window, Qt::black);
	plt.setColor(QPalette::Text, Qt::green);
	plt.setColor(QPalette::WindowText, Qt::green);
	setPalette(plt);
    //
    connect(ui.actionclose, SIGNAL(triggered()), this, SLOT(accept()));
    connect(ui.actionsetup, SIGNAL(triggered()), this, SLOT(setupDialog()));
    connect(ui.actionstay, SIGNAL(triggered()), this, SLOT(stayOnTop()));
    connect(ui.actionFullScreen, SIGNAL(triggered()), this, SLOT(_showFullScreen()));
    connect(ui.actionOrientation_vertical, SIGNAL(triggered()), this, SLOT(_switch_orientation()));
    connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(showAbout()));
    //
    addAction(ui.actionclose);
    addAction(ui.actionsetup);
    addAction(ui.actionstay);
    addAction(ui.actionFullScreen);
    addAction(ui.actionOrientation_vertical);
    addAction(ui.actionOrientation_vertical);
    addAction(ui.actionAbout);
    //
    menuSetup();
    //
    layout_v  = new QVBoxLayout(this);
    layout_h  = new QHBoxLayout();
    layout_h1 = new QHBoxLayout();
    restoreSettings();
    ts = new townset(this);
    connect(ts, SIGNAL(_resetObjects()), this, SLOT(resetObjects()));
    connect(ts, SIGNAL(_saveSettings()), this, SLOT(saveSettings()));
    resetObjects();
}

timezones::~timezones()
{
	saveSettings();
	for (int i=0; i<towncnt; i++)
	{
		if (lcds[i])
		{
			delete lcds[i];
		}
		if (towns[i])
		{
			delete towns[i];
		}
	}
    if (layout_h) delete layout_h;
    if (layout_h1) delete layout_h1;
    if (layout_v) delete layout_v;
}

void timezones::menuSetup(){
    menu = new QMenu(this);
    menu->addAction(ui.actionsetup);
    menu->addAction(ui.actionstay);
    menu->addAction(ui.actionclose);
    menu->addAction(ui.actionFullScreen);
    menu->addAction(ui.actionOrientation_vertical);
    menu->addSeparator();
    menu->addAction(ui.actionAbout);
}

void timezones::mouseMoveEvent(QMouseEvent *event)
{
	if (event->buttons() == Qt::LeftButton) {
		move(event->globalPos() - dragPosition);
		event->accept();
	}

}

void timezones::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		dragPosition = event->globalPos() - frameGeometry().topLeft();
		event->accept();
	}
}

void timezones::contextMenuEvent(QContextMenuEvent *event)
{
	menu->exec(event->globalPos());
}

void timezones::resetObjects()
{
	restoreSettings();
    for (int i=0; i < prev_towncnt; i++)
	{
        if(ui.actionOrientation_vertical->isChecked()){
            layout_v->removeWidget(towns[i]);
            layout_v->removeWidget(lcds[i]);
        } else {
            if(layouts[i]){
                layout_h->removeWidget(lcds[i]);
                layout_h1->removeWidget(towns[i]);
            }
        }

        if (lcds[i]) delete lcds[i];
        if (towns[i]) delete towns[i];
	}
    if(!ui.actionOrientation_vertical->isChecked()){
        layout_v->removeItem(layout_h);
        layout_v->removeItem(layout_h1);
    }
    //
    layout_v->setMargin(0);
    layout_v->setSpacing(0);
    //
	for (int i=0; i<towncnt; i++)
	{
        lcd = new lcdclock(this, timeFreq[i], timeshifts[i]);
		lcd->setMinimumHeight(40);
		lcd->setPalette(plt);
		lcds[i] = lcd;
		town = new QLabel(this);
        town->setMinimumHeight(16);
        town->setMaximumHeight(16);
		town->setAlignment(Qt::AlignCenter);
		town->setText(townnames[i]);
		towns[i] = town;
        //
        if(ui.actionOrientation_vertical->isChecked()){
            layout_v->addWidget(towns[i]);
            layout_v->addWidget(lcds[i]);
        } else {
            layout_h->addWidget(towns[i]);
            layout_h1->addWidget(lcds[i]);
        }
    }
    if(!ui.actionOrientation_vertical->isChecked()){
        layout_v->addLayout(layout_h);
        layout_v->addLayout(layout_h1);
    }
    //
	update();
    prev_towncnt = towncnt;
}
void timezones::saveOrientation(){
    QSettings settings("timezones.ini", QSettings::IniFormat);
    settings.setPath(QSettings::IniFormat, QSettings::UserScope, ".\\");
    //
    settings.beginGroup("timezonesWindow");
        settings.setValue("Orientation_vertical", ui.actionOrientation_vertical->isChecked());
    settings.endGroup();
}

void timezones::saveSettings()
{
	QSettings settings("timezones.ini", QSettings::IniFormat);
	settings.setPath(QSettings::IniFormat, QSettings::UserScope, ".\\");
    // pos and size
    settings.beginGroup("timezonesWindow");
        settings.setValue("size", size());
        settings.setValue("pos", pos());
    settings.endGroup();
    //
	settings.setValue("towncount", towncnt);
	settings.beginGroup("towns");
	for (int i=1; i<=towncnt; i++)
	{
		settings.setValue(QString("town")+QString::number(i), townnames[i-1].toUtf8());
		settings.setValue(QString("shft")+QString::number(i), timeshifts[i-1]);
	}
	settings.endGroup();
}

void timezones::restoreSettings()
{
	QSettings settings("timezones.ini", QSettings::IniFormat);
	settings.setPath(QSettings::IniFormat, QSettings::UserScope, ".\\");
    //
    settings.beginGroup("timezonesWindow");
        this->move(settings.value("pos").toPoint());
        this->resize(settings.value("size").toSize());
        ui.actionOrientation_vertical->setChecked(settings.value("Orientation_vertical").toBool());
    settings.endGroup();
    //
	towncnt = settings.value("towncount", 0).toInt();
	if (towncnt>MAXZONES) towncnt = MAXZONES;
	townnames.clear();
	settings.beginGroup("towns");
	for (int i=1; i<=towncnt; i++)
	{
		townnames.append(QString::fromUtf8 ( settings.value(QString("town")+QString::number(i)).toByteArray().data() ));
		timeshifts[i-1]= settings.value(QString("shft")+QString::number(i), 0).toInt();
        timeFreq[i-1]  = settings.value(QString("freq")+QString::number(i), 0).toInt();
	}
	settings.endGroup();
	if (towncnt==0)
	{
		towncnt=1;
		townnames.append(QString("local"));
		timeshifts[0]= 0;
        timeFreq[0]  = 1000;
	}
}

void timezones::stayOnTop()
{
    ui.actionstay->isChecked()?
        setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint):
        setWindowFlags(Qt::FramelessWindowHint);
}

void timezones::setupDialog()
{
	prev_towncnt = towncnt;
    ts->show();
}
void timezones::_showFullScreen(){
    ui.actionFullScreen->isChecked()? this->showFullScreen():this->showNormal();
}

void timezones::_switch_orientation(){
    this->saveOrientation();
    this->resetObjects();
}

void timezones::showAbout(){
    QMessageBox::aboutQt(this, "");
}
