#pragma once

#include <QLineEdit>
#include <QFileDialog>

#include <QPushButton>

class NFileEdit : public QLineEdit
{
public:
	NFileEdit(QWidget* parent = nullptr);

	QSize sizeHint() const override;
	void resizeEvent(QResizeEvent* event) override;
protected:
	QPushButton* m_browse_button;
	QFileDialog* m_file_diag;
	int m_button_width = 30;
};