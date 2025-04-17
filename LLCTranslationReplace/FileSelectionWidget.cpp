#include "FileSelectionWidget.h"

#include <QPainter>

void FileSelectionWidget::resizeEvent(QResizeEvent* event)
{
	int _x = this->rect().left();
	int _y = this->rect().top() +
		(this->rect().height() - m_real_widget_height) / 2;
	switch (m_widget_statue)
	{
	case FileSelectionWidget::WidgetStatue::Left:
	{
		_x = this->rect().left();
		_y = this->rect().top() +
			(this->rect().height() - m_real_widget_height) / 2;
		break;
	}
	case FileSelectionWidget::WidgetStatue::Right:
	{
		_x = this->rect().right() - m_real_widegt_width;
		_y = this->rect().top() +
			(this->rect().height() - m_real_widget_height) / 2;
		break;
	}
	case FileSelectionWidget::WidgetStatue::Top:
	{
		_x = this->rect().left() + 
			(this->rect().width() + m_real_widegt_width) / 2;
		_y = this->rect().top();
		break;
	}
	case FileSelectionWidget::WidgetStatue::Bottom:
	{
		_x = this->rect().left() +
			(this->rect().width() + m_real_widegt_width) / 2;
		_y = this->rect().bottom() - m_real_widget_height;
		break;
	}
	case FileSelectionWidget::WidgetStatue::Center:
	{
		_x = this->rect().left() +
			(this->rect().width() + m_real_widegt_width) / 2;
		_y = this->rect().top() +
			(this->rect().height() - m_real_widget_height) / 2;
		break;
	}
	}

	m_real_widget->setGeometry(
		_x, _y, m_real_widegt_width, m_real_widget_height
	);
}

void FileSelectionWidget::paintEvent(QPaintEvent* event)
{
	QRect _title_rect(
		m_real_widget->geometry().topLeft(), 
		QSize(m_real_widget->geometry().width(), m_title_height)
	);
	QPainter painter(this);
	Qt::Alignment _title_alignment = Qt::AlignVCenter;
	switch (m_title_statue)
	{
	case FileSelectionWidget::TitleStatue::Left:
		_title_alignment |= Qt::AlignLeft;
		break;
	case FileSelectionWidget::TitleStatue::Right:
		_title_alignment |= Qt::AlignRight;
		break;
	case FileSelectionWidget::TitleStatue::Center:
		_title_alignment |= Qt::AlignHCenter;
		break;
	}
	painter.drawText(_title_rect, m_title, _title_alignment); 

	QWidget::paintEvent(event);
}
