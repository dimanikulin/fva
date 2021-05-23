/*!
* \file QPictureLabel.h
* \copyright Copyright 2021 FVA Software. All rights reserved. This file is released under the XXX License.
* \author Dima Nikulin.
* \version 0.29
* \date  2014-2021
*/
#ifndef QPICTURELABEL_H
#define QPICTURELABEL_H

#include <QtWidgets/QLabel>
#include <QtGui/QImage>
#include <QtGui/QPixmap>

#include "ui_qpicturelabel.h"

/*!
* \brief impelements the drawing functions for static images
*/
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
