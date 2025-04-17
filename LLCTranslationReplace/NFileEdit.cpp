#include "NFileEdit.h"

#include <QStyle>
#include <QResizeEvent>

NFileEdit::NFileEdit(QWidget* parent) :
	QLineEdit(parent)
{

	m_file_diag = new QFileDialog();


	// 设置初始属性
	setPlaceholderText(tr("请选择文件"));

	// 创建浏览按钮
	m_browse_button = new QPushButton(this);
	m_browse_button->setText("...");
	m_browse_button->setCursor(Qt::PointingHandCursor);

	// 设置按钮样式和位置
	int frame_width = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
	setStyleSheet(
		QString("QLineEdit { padding-right: %1px; }").arg(m_button_width + 1)
	);

	// 按钮大小和位置
	QSize msz = minimumSizeHint();
	int buttonHeight = msz.height() - frame_width * 2;
	m_browse_button->resize(m_button_width, buttonHeight);
	m_browse_button->move(rect().right() - frame_width - m_browse_button->width(),
		(rect().bottom() - m_browse_button->height() + 1) / 2);

	// 连接信号
	connect(m_browse_button, &QPushButton::clicked, this,
		[this]() {

			m_file_diag->setFileMode(QFileDialog::Directory); 
			QString _last_dir = text();
			QFileInfo _last_dir_info(_last_dir);
			if (!_last_dir.isEmpty() && _last_dir_info.exists() &&
				_last_dir_info.isDir())
			{
				m_file_diag->setDirectory(_last_dir);  // 或任何其他初始路径 
			}
			if (m_file_diag->exec() != QDialog::Accepted)
			{
				return;
			}
			auto _select_dir = m_file_diag->selectedFiles();
			if (_select_dir.size() > 0) 
			{
				QString _select_dir_str = _select_dir.at(0);
				QFileInfo _select_dir_info(_select_dir_str);
				if (_select_dir_str.isEmpty() || !_select_dir_info.exists()
					|| !_select_dir_info.isDir())
				{
					return;
				}
				setText(_select_dir_str);
			}
		}
	);
}

QSize NFileEdit::sizeHint() const
{
	QSize _size = QLineEdit::sizeHint();
	_size.setHeight(25);

	return _size;
}

void NFileEdit::resizeEvent(QResizeEvent* event)
{
	QLineEdit::resizeEvent(event);

	int frameWidth = style()->pixelMetric(QStyle::PM_DefaultFrameWidth);
	QSize msz = minimumSizeHint();
	QSize _size = event->size();
	int buttonHeight = _size.height();
	m_browse_button->resize(m_button_width, buttonHeight);
	// 调整按钮位置
	m_browse_button->move(rect().right() - m_browse_button->width(),
		(rect().bottom() - m_browse_button->height() + 1) / 2);
}