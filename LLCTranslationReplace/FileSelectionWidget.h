#pragma once
#include "NLineEdit.h"
#include "NFileEdit.h"

#include <QWidget>
#include <QFormLayout>

class FileSelectionWidget : public QWidget
{
public:
	enum class WidgetStatue
	{
		Left,
		Right,
		Top,
		Bottom,
		Center
	};
	enum class TitleStatue
	{
		Left,
		Right,
		Center
	};
public:
	FileSelectionWidget(QString title, QWidget* parent = nullptr) :
		QWidget(parent), m_title(title)
	{
		QFontMetrics fm(font());
		m_title_height = fm.height() + 10;
		m_title_height = std::max(m_title_height, m_form_padding);

		m_real_widget = new QWidget(this);
		//m_real_widget->setStyleSheet("background-color: red;");
		QFormLayout* _form = new QFormLayout(m_real_widget);
		_form->setLabelAlignment(Qt::AlignRight);
		_form->setContentsMargins(
			m_form_padding, m_title_height, m_form_padding, m_form_padding
		);
		m_file_dir = new NFileEdit(m_real_widget);
		_form->addRow("文件目录: ", m_file_dir);
		 m_file_prefix = new NLineEdit(m_real_widget);
		_form->addRow("文件前缀: ", m_file_prefix);
	}

	void setWidgetStatue(WidgetStatue statue)
	{
		m_widget_statue = statue;
	}
	void setTitleStatue(TitleStatue statue)
	{
		m_title_statue = statue;
	}
	void setFileDir(QString dir)
	{
		m_file_dir->setText(dir);
	}
	QString getFileDir() const
	{
		QString _dir = m_file_dir->text();
		return _dir;
	}
	void setFilePrefix(QString prefix)
	{
		m_file_prefix->setText(prefix);
	}
	QString getFilePrefix() const
	{
		QString _dir = m_file_prefix->text();
		return _dir;
	}
protected:
	void resizeEvent(QResizeEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
private:
	NFileEdit* m_file_dir = nullptr;
	NLineEdit* m_file_prefix = nullptr;
	QString m_title;
	int m_title_height = 0;
	QWidget* m_real_widget = nullptr;
	int m_real_widegt_width = 300;
	int m_real_widget_height = 150;
	int m_real_widget_min_widget = 300;
	int m_real_widget_min_height = 150;
	WidgetStatue m_widget_statue = WidgetStatue::Left;
	int m_form_padding = 10;
	TitleStatue m_title_statue = TitleStatue::Left;
};