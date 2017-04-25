#ifndef TIMEZONES_H
#define TIMEZONES_H

#ifndef MAXZONES
#define MAXZONES 10
#endif
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QMenu>
#include <QMouseEvent>
#include <QSettings>
#include <QMessageBox>
#include "ui_timezones.h"
#include "src/headers/lcdclock.h"
#include "src/headers/townset.h"

extern QStringList townnames;
extern int timeshifts[MAXZONES];
extern int towncnt;
extern int prev_towncnt;

class timezones : public QDialog
{
	Q_OBJECT
public:
    timezones();
	~timezones();
private:
	Ui::timezonesClass ui;
	lcdclock *lcd;
	lcdclock *lcds[MAXZONES];
	QLabel *town;
	QLabel *towns[MAXZONES];
    QLayout *layouts[MAXZONES];
	QPoint dragPosition;
	QMenu *menu;
    QVBoxLayout *layout_v;
    QHBoxLayout *layout_h;
    QHBoxLayout *layout_h1;
	QPalette plt;
    townset *ts;

private slots:
    void menuSetup();
	void restoreSettings();
	void saveSettings();
    void saveOrientation();
	void setupDialog();
    void _showFullScreen();
    void _switch_orientation();
	void stayOnTop();
	void resetObjects();
public slots:
    void showAbout();
protected:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void contextMenuEvent(QContextMenuEvent *event);
};

#endif // TIMEZONES_H
