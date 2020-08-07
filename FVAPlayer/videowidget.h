#ifndef FVAVIDEOWIDGET_H
#define FVAVIDEOWIDGET_H

#include <QtMultimediaWidgets/QVideoWidget>
#include <QtGui/QKeyEvent>
#include <QtGui/QMouseEvent>

class VideoWidget : public QVideoWidget
{
    Q_OBJECT

public:
    VideoWidget(QWidget *parent = 0)
	: QVideoWidget(parent)
	{
		setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

		QPalette p = palette();
		p.setColor(QPalette::Window, Qt::black);
		setPalette(p);

		setAttribute(Qt::WA_OpaquePaintEvent);
	}
protected:
    void keyPressEvent(QKeyEvent *event)
	{
		if (event->key() == Qt::Key_Escape && isFullScreen()) 
		{
			setFullScreen(false);
			event->accept();
		} 
		else if (event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt) 
		{
			setFullScreen(!isFullScreen());
			event->accept();
		} 
		else 
		{
			QVideoWidget::keyPressEvent(event);
		}
	}
    void mouseDoubleClickEvent(QMouseEvent *event)
	{
		setFullScreen(!isFullScreen());
		event->accept();
	}	
    void mousePressEvent(QMouseEvent *event)
	{
		QVideoWidget::mousePressEvent(event);
	}
};

#endif // FVAVIDEOWIDGET_H
