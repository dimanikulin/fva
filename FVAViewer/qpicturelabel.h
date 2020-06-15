#ifndef QPICTURELABEL_H
#define QPICTURELABEL_H

#include <QtGui/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include "ui_qpicturelabel.h"

class QPictureLabel : public QLabel
{
	Q_OBJECT

private:

    QPixmap _qpSource; //preserve the original, so multiple resize events won't break the quality
    QPixmap _qpCurrent;

    void _displayImage();

public:
    QPictureLabel(QWidget *aParent ) : QLabel( aParent ) 
	{
		
	}
    void setPixmap(QPixmap aPicture);
    void paintEvent(QPaintEvent *aEvent);
private:
	Ui::QPictureLabel ui;
};

#endif // QPICTURELABEL_H
