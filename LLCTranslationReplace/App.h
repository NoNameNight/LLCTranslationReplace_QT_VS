#pragma once

#include <QWidget>
#include <QProgressDialog>

class App : public QWidget
{
public:
	App(QWidget* parent = nullptr);
private:
	QProgressDialog* m_progress_dialog = nullptr;
};