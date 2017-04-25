#include <QLCDNumber>
class lcdclock : public QLCDNumber
{
	Q_OBJECT

public:
	lcdclock(QWidget *parent = 0, int frq=1000, int shift=0);
	~lcdclock();
private:
	QTimer *timer;
		int freq;
		int shft;
	private slots:
		void updateTime();
};
