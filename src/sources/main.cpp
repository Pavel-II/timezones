#include <QApplication>
#include <QTranslator>
#include "src/headers/timezones.h"
#define MAXZONES 10

QStringList townnames;
int timeshifts[MAXZONES];
int timeFreq[MAXZONES];
int towncnt;
int prev_towncnt;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
    QTranslator qtTranslator;
    qtTranslator.load(a.applicationDirPath() + "/timezones_" + QLocale::system().name() + ".qm");
    a.installTranslator(&qtTranslator);
	timezones w;
	a.connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
    w.show();
    return a.exec();
}
