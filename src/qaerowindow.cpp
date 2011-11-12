/******************************************************************************
	Name: description
	Copyright (C) 2011 Wang Bin <wbsecg1@gmail.com>

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 2 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License along
	with this program; if not, write to the Free Software Foundation, Inc.,
	51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
******************************************************************************/

#include "qaerowindow.h"

QAeroWidget::QAeroWidget(Effect e, QWidget *parent, Qt::WFlags flags)
	:QWidget(parent, flags),effect(e),effect_alpha(0)
{
	if (effect != QAeroWidget::None) {
		setAttribute(Qt::WA_ContentsPropagated);
		setAttribute(Qt::WA_TranslucentBackground, true);
	}
	init();
}

QAeroWidget::QAeroWidget(QWidget *parent, Qt::WFlags flags)
	:QWidget(parent, flags),effect(Blur),effect_alpha(0)
{
	init();
}

QAeroWidget::~QAeroWidget()
{
}

void QAeroWidget::setEffect(Effect effect)
{
	this->effect = effect;
#ifdef Q_OS_WIN
	if (this->effect == QAeroWidget::SemiTransparent) {
		setWindowFlags(Qt::FramelessWindowHint);
	}
#endif
	if (this->effect != QAeroWidget::None) {
		setAttribute(Qt::WA_ContentsPropagated);
		setAttribute(Qt::WA_TranslucentBackground, true);
	}
}

void QAeroWidget::setEffectOpacity(qreal level)
{
	if (level>1.0)
		level = 1.0;
	if (level<0)
		level = 0;
	effect_alpha = 255*level;
}


