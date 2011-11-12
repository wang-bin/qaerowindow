#include "qaerowindow.h"
#include <QtCore/QLibrary>
#include <QtGui/QBitmap>
#include <QtGui/QPainter>
#include <QtGui/QResizeEvent>
#include <qt_windows.h>

#define CONFIG_LOAD_DLL 1

#if CONFIG_LOAD_DLL
// Blur behind data structures
#define DWM_BB_ENABLE                 0x00000001  // fEnable has been specified
#define DWM_BB_BLURREGION             0x00000002  // hRgnBlur has been specified
#define DWM_BB_TRANSITIONONMAXIMIZED  0x00000004  // fTransitionOnMaximized has been specified
#define WM_DWMCOMPOSITIONCHANGED        0x031E    // Composition changed window message

typedef struct _DWM_BLURBEHIND
{
	DWORD dwFlags;
	BOOL fEnable;
	HRGN hRgnBlur;
	BOOL fTransitionOnMaximized;
} DWM_BLURBEHIND, *PDWM_BLURBEHIND;

// 这个结构是从 VS2010 中的 atlglobal.h 中弄到的.SMALL_RECT 一个意不过 SMALL_RECT 用的是短整形
// 我用 SMALL_RECT 试了一虽然没错但是运行的时候只有部分窗口被Glass
typedef struct{
	int nLeft;
	 int nRight;
	 int nTop;
	 int nBottom;
} MARGINS;

// VC6 Win98 时代的东 DWM 这么华丽的东西他怎么可能会有,-_-!
// 自己定义然后 LoadLibrary ...
typedef HRESULT (WINAPI *PtrDwmIsCompositionEnabled)(BOOL* pfEnabled);
typedef HRESULT (WINAPI *PtrDwmExtendFrameIntoClientArea)(HWND hWnd, const MARGINS* pMarInset);
typedef HRESULT (WINAPI *PtrDwmEnableAeroWindow)(HWND hWnd, const DWM_BLURBEHIND* pBlurBehind);
typedef HRESULT (WINAPI *PtrDwmGetColorizationColor)(DWORD *pcrColorization, BOOL *pfOpaqueBlend);
typedef HRESULT (WINAPI *PtrDwmEnableBlurBehindWindow)(HWND hWnd, const DWM_BLURBEHIND *pBlurBehind);

static PtrDwmIsCompositionEnabled DwmIsCompositionEnabled= 0;
static PtrDwmEnableAeroWindow DwmEnableAeroWindow = 0;
static PtrDwmExtendFrameIntoClientArea DwmExtendFrameIntoClientArea = 0;
//static PtrDwmGetColorizationColor DwmGetColorizationColor = 0;
static PtrDwmEnableBlurBehindWindow DwmEnableBlurBehindWindow = 0;
#else
#include <dwmapi.h>     // DWM APIs. mingw _WIN32_WINNT >= 0x0600
//#pragma comment(lib, "dwmapi") //DWM library
#endif


QAeroWidget::QAeroWidget(QWidget *parent, Qt::WFlags flags)
	: QWidget(parent, flags)
{
	effect = Blur;
	effect_alpha = 0;

	QLibrary dwmapiLib("dwmapi.dll", this);
	if (!dwmapiLib.load()) {
		qWarning("Failed to load dwmapi.dll: %s", qPrintable(dwmapiLib.errorString()));
	}

	DwmIsCompositionEnabled =(PtrDwmIsCompositionEnabled)dwmapiLib.resolve("DwmIsCompositionEnabled");
	if (!DwmIsCompositionEnabled)
		qWarning("Resolve 'DwmIsCompositionEnabled()' error: %s", qPrintable(dwmapiLib.errorString()));
	DwmExtendFrameIntoClientArea = (PtrDwmExtendFrameIntoClientArea)dwmapiLib.resolve("DwmExtendFrameIntoClientArea");
	if (!DwmExtendFrameIntoClientArea)
		qWarning("Resolve 'DwmExtendFrameIntoClientArea()' error: %s", qPrintable(dwmapiLib.errorString()));
	//DwmEnableAeroWindow = (PtrDwmEnableAeroWindow)dwmapiLib.resolve("DwmEnableAeroWindow");
	//if (!DwmEnableAeroWindow)
		//qWarning("Resolve 'DwmEnableAeroWindow()' error: %s", qPrintable(dwmapiLib.errorString()));
	//DwmGetColorizationColor = (PtrDwmGetColorizationColor)dwmapiLib.resolve("DwmGetColorizationColor");
	DwmEnableBlurBehindWindow = (PtrDwmEnableBlurBehindWindow)dwmapiLib.resolve("DwmEnableBlurBehindWindow");
	if (!DwmEnableBlurBehindWindow)
		qWarning("Resolve 'DwmEnableBlurBehindWindow()' error: %s", qPrintable(dwmapiLib.errorString()));
	//dwmapiLib.unload();
}

bool QAeroWidget::isCompositionEnabled() const
{
	if (!DwmIsCompositionEnabled)
		return false;

	HRESULT hr = S_OK;
	BOOL isEnabled = false;
	hr = DwmIsCompositionEnabled(&isEnabled);
	if (SUCCEEDED(hr))
		qDebug("Composition enabled=%d", isEnabled);
	else
		qWarning("DwmIsCompositionEnable() error: %d", (int)GetLastError());

	return isEnabled;
}


bool QAeroWidget::enableAeroWindow(bool enable)
{
	if (!DwmEnableAeroWindow)
		return false;

	bool result = false;
	DWM_BLURBEHIND bb = {0};
	HRESULT hr = S_OK;
	bb.fEnable = enable;
	bb.dwFlags = DWM_BB_ENABLE;
	bb.hRgnBlur = NULL;
	setAttribute(Qt::WA_TranslucentBackground, enable);
	setAttribute(Qt::WA_NoSystemBackground, enable);
	hr = DwmEnableAeroWindow(winId(), &bb);

	if (SUCCEEDED(hr))
		result = true;
	else
		qWarning("DwmenableAeroWindow() error: %d", (int)GetLastError());

	return result;
}


void QAeroWidget::resizeEvent(QResizeEvent *event)
{
	QWidget::resizeEvent(event);
	switch(effect) {
	case Aero:
	{
		//Why we must use top parent? It's weird!
		QWidget *top = this;
		while (top->parentWidget()) {
			top = top->parentWidget();
		}
		WId wid = top->winId();
		MARGINS mar = {top->size().width()/2, top->size().width()/2, top->size().height()/2, top->size().height()/2}; //left, right, top, bottom margins.
		HRESULT hr = DwmExtendFrameIntoClientArea(wid, &mar); //set the margins
		if (!SUCCEEDED(hr))
			qWarning("DwmExtendFrameIntoClientArea() error: %d", (int)GetLastError());
		break;
	}
	case Blur:
	{
		HRESULT hr = S_OK;
		// Create and populate the Blur Behind structure
		DWM_BLURBEHIND bb = {0};
		// Enable Blur Behind and apply to the entire client area
		bb.dwFlags = DWM_BB_ENABLE;
		bb.fEnable = true;
		bb.hRgnBlur = NULL;

		QWidget *top = this;
		while (top->parentWidget()) {
			top = top->parentWidget();
		}
		WId wid = top->winId();
		hr = DwmEnableBlurBehindWindow(wid, &bb);
		if (!SUCCEEDED(hr))
			qWarning("DwmEnableBlurBehindWindow() error: %d", (int)GetLastError());
		break;
	}
	case SemiTransparent:
	{
		//setAttribute(Qt::WA_TranslucentBackground);
		break;
	}
	case None:
	default:
		break;
	}
}

void QAeroWidget::paintEvent(QPaintEvent* event)
{
	//QWidget::paintEvent(event);
	QPainter p(this);
	if (effect==Aero || effect==Blur) {
		p.setCompositionMode(QPainter::CompositionMode_DestinationIn); //it's necessary!
	}else if (effect==SemiTransparent) {
		//setWindowFlag cause an paint error
		//setWindowFlags(Qt::FramelessWindowHint);
	}
	QColor backgroundColor = palette().light().color();
	backgroundColor.setAlpha(effect_alpha);
	p.fillRect(event->rect(), backgroundColor); //the RGB color is not the point, the alpha value is.
}


bool QAeroWidget::winEvent(MSG *message, long *result)
{
	if (message && message->message == WM_DWMCOMPOSITIONCHANGED) {
		bool compositionEnabled = isCompositionEnabled();
		setAttribute(Qt::WA_NoSystemBackground, compositionEnabled);
		update();
	}
	return QWidget::winEvent(message, result);
}
