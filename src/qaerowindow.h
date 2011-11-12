#ifndef AEROWIDGET_H
#define AEROWIDGET_H

#include <QtGui/QWidget>
//aero window
class QAeroWidget : public QWidget
{
	Q_OBJECT

public:
	enum Effect {
		None, Aero, Blur, SemiTransparent
	};

	QAeroWidget(Effect e, QWidget *parent = 0, Qt::WFlags flags = 0);
	QAeroWidget(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QAeroWidget();

	void setEffectOpacity(qreal level);

	bool isCompositionEnabled() const;
	bool enableAeroWindow(bool enable = true);
	void setEffect(Effect effect);

protected:
	virtual void paintEvent(QPaintEvent *);
	virtual void resizeEvent(QResizeEvent *);

#ifdef Q_OS_WIN
	virtual bool winEvent(MSG *message, long *result);
#endif //Q_OS_WIN
private:
	void init();

	Effect effect;
	qreal effect_alpha;
};

#endif // AEROWIDGET_H
