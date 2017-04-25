#include <QtGui>

#include "src/headers/lcdclock.h"

lcdclock::lcdclock(QWidget *parent, int frq, int shift)
: QLCDNumber(parent)
{
	setSegmentStyle(Flat);
    setDigitCount(11);
	freq = frq;
	shft = shift;
	timer = new QTimer(this);
    timer->start(freq);
	connect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
    updateTime();
}
lcdclock::~lcdclock()
{
	disconnect(timer, SIGNAL(timeout()), this, SLOT(updateTime()));
	if(timer) delete timer;
}

void lcdclock::updateTime()
{
    QTime time = (QTime::currentTime()).addSecs(60*(this->shft));
    display(time.toString("hhmm.ss.zzz"));
}
