#pragma once

#include <QLineEdit>

class NLineEdit : public QLineEdit
{
public:
	NLineEdit(QWidget* parnet = nullptr);

	QSize sizeHint() const override;
protected:
};