#include "qaerowindow.h"
#include <QtGui/QApplication>
#include <QtGui/QLabel>
#include <QtGui/QVBoxLayout>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);

	QLabel ww;
	ww.setPixmap(QPixmap("qt-logo.png"));
	//qDebug("wid=%d", (int)ww.winId());
	QVBoxLayout vl;
	ww.setLayout(&vl);

	QAeroWidget aero_widget;
	//w.setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);
	aero_widget.setEffect(QAeroWidget::Aero);
	aero_widget.setEffectOpacity(0.1);
	QLabel text(&aero_widget);
	text.setText("<font color=#ff0000 size=22>Aero Widget</font>");

	QAeroWidget blur_widget;
	blur_widget.setEffect(QAeroWidget::Blur);
	blur_widget.setEffectOpacity(0.1);
	QLabel bl(&blur_widget);
	bl.setText("<font color=#ff00ff size=22>Blur Widget</font>");

	QAeroWidget st_widget;
	st_widget.setEffect(QAeroWidget::SemiTransparent);
	st_widget.setEffectOpacity(0.1);
	QLabel stl(&st_widget);
	stl.setText("<font color=#ffff00 size=22>SemiTransparent Widget</font>");

	QLabel normal_text;
	normal_text.setText("<font color=#0000ff size=22>Normal Widget</font>");

	vl.addWidget(&aero_widget);
	vl.addWidget(&blur_widget);
	vl.addWidget(&st_widget);
	vl.addWidget(&normal_text);
	ww.show();

	QAeroWidget aero_window;
	aero_window.setEffect(QAeroWidget::Aero);
	//aero_window.setEffectOpacity(0.618);
	//aero_window.setWindowFlags(Qt::FramelessWindowHint);
	QLabel text_aw(&aero_window);
	text_aw.setText("<font color=#ff0000 size=22>Aero Window</font>");
	aero_window.show();
	aero_window.resize(text_aw.size());

	QAeroWidget blur_window;
	blur_window.setEffect(QAeroWidget::Blur);
	QLabel text_awbb(&blur_window);
	text_awbb.setText("<font color=#ff0000 size=22>Blur Window</font>");
	blur_window.show();
	blur_window.resize(text_awbb.size());

	QAeroWidget semitransparent_window;
	semitransparent_window.setEffect(QAeroWidget::SemiTransparent);
	semitransparent_window.setEffectOpacity(0.618);
	QLabel text_semitransparent_window(&semitransparent_window);
	text_semitransparent_window.setText("<font color=#ff0000 size=22>SemiTransparent Window</font>");
	semitransparent_window.show();
	semitransparent_window.resize(text_semitransparent_window.size());


	return a.exec();
}
