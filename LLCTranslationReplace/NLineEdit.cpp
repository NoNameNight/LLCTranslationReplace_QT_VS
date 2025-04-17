#include "NLineEdit.h"

NLineEdit::NLineEdit(QWidget* parnet) :
	QLineEdit(parnet)
{
	//this->setStyleSheet("border: 0;border-radius: 6px;");
}

QSize NLineEdit::sizeHint() const
{
	QSize _size = QLineEdit::sizeHint();
	_size.setHeight(25);

    return _size;
}