#include "App.h"

#include <QApplication>
#include "FileSelectionWidget.h"

#include <QMessageBox>
#include <QDirIterator>

#include <json/json.h>
#include <fstream>


#include <QProgressDialog>

#include <functional>
#include "NObjectHelper.h"

//#include <QFile>
//#include <QJsonArray>
//#include <QJsonObject>
//#include <QJsonDocument>


int getAllFilesNum(QString dir)
{
	int count = 0;
	QDirIterator it(dir, QDir::Files, QDirIterator::Subdirectories);
	while (it.hasNext())
	{
		it.next();
		count++;
	}
	return count;
}

static bool coreFunc(
	QString dst_file_dir, QString dst_file_prefix,
	QString src_file_dir, QString src_file_prefix,
	std::function<void(int all_count, int now_count)> one_finished
)
{
	QDir dst_dir(dst_file_dir);
	QDir src_dir(src_file_dir);
	if (dst_file_dir.isEmpty())
	{
		QMessageBox::warning(nullptr, "error", "目标目录不能为空");
		return false;
	}
	if (src_file_dir.isEmpty())
	{
		QMessageBox::warning(nullptr, "error", "源目录不能为空");
		return false;
	}
	if (!dst_dir.exists())
	{
		QMessageBox::warning(nullptr, "错误", "路径：" + dst_file_dir + "不存在");
		return false;
	}
	if (!src_dir.exists())
	{
		QMessageBox::warning(nullptr, "错误", "路径：" + src_file_dir + "不存在");
		return false;
	}
	QDirIterator src_it(
		src_file_dir, QDir::Files | QDir::NoDotAndDotDot,
		QDirIterator::Subdirectories
	);
	int all_count = getAllFilesNum(src_file_dir);
	int now_count = 0;
	int copy_file = 0;
	while (src_it.hasNext())
	{
		QString _src_file_path = src_it.next();
		QString _relative_file_dir =
			src_dir.relativeFilePath(QFileInfo(_src_file_path).absolutePath());
		QString _dst_file_name = QFileInfo(_src_file_path).fileName();
		if (_dst_file_name.startsWith(src_file_prefix))
		{
			_dst_file_name = _dst_file_name.mid(src_file_prefix.length());
		}
		_dst_file_name = dst_file_prefix + _dst_file_name;
		QString _relative_file_path = _relative_file_dir + "/" + _dst_file_name;

		QFileInfo _src_file_info(_src_file_path);
		QFileInfo _dst_file_info(dst_dir.absolutePath() + "/" + _relative_file_path);
		if (!_dst_file_info.exists())
		{
			QDir _temp_dir(_dst_file_info.absoluteDir());
			if (!_temp_dir.exists())
			{
				_temp_dir.mkpath(_temp_dir.absolutePath());
			}
			copy_file++;
			QFile::copy(_src_file_path, _dst_file_info.absoluteFilePath());

			continue;
		}

		Json::Value _src_json;
		//QJsonObject _src_json;
		Json::Value _dst_json;
		//QJsonObject _dst_json;
		{
			std::ifstream _dst_ifs;
			_dst_ifs.open(_dst_file_info.absoluteFilePath().toStdString());
			Json::Reader().parse(_dst_ifs, _dst_json);
			_dst_ifs.close();
		}
		//{
		//	QFile _dst_file(_dst_file_info.absoluteFilePath());
		//	_dst_file.open(QIODevice::ReadOnly);
		//	QByteArray _dst_data = _dst_file.readAll();
		//	_dst_file.close();
		//	QJsonParseError error; 
		//	_dst_json = QJsonDocument::fromJson(_dst_data, &error).object();
		//}
		if (!_dst_json.isObject() || !_dst_json.isMember("dataList") ||
			!_dst_json["dataList"].isArray())
		{
			continue;
		}
		{
			std::ifstream _src_ifs;
			_src_ifs.open(_src_file_info.absoluteFilePath().toStdString());
			Json::Reader().parse(_src_ifs, _src_json);
			_src_ifs.close();
		}
		//{
		//	QFile _src_file(_src_file_info.absoluteFilePath());
		//	_src_file.open(QIODevice::ReadOnly);
		//	QByteArray _src_data = _src_file.readAll();
		//	_src_file.close();
		//	QJsonParseError error;
		//	_src_json = QJsonDocument::fromJson(_src_data, &error).object();
		//}
		if (!_src_json.isObject() || !_src_json.isMember("dataList") ||
			!_src_json["dataList"].isArray())
		{
			continue;
		}
		Json::Value& _dst_data_list = _dst_json["dataList"];
		std::unordered_map<std::string, int> _dst_data_list_map;
		for (int i = 0; i < _dst_data_list.size(); ++i)
		{
			const Json::Value& _item = _dst_data_list[i];
			if (!_item.isMember("id"))
			{
				continue;
			}
			_dst_data_list_map[_item["id"].asString()] = i;
		}
		//QJsonArray _dst_data_list = _dst_json["dataList"].toArray();
		//std::unordered_map<std::string, int> _dst_data_list_map;
		//for (int i = 0; i < _dst_data_list.size(); ++i)
		//{
		//	const QJsonObject& _item = _dst_data_list[i].toObject();
		//	if (!_item.contains("id"))
		//	{
		//		continue;
		//	}
		//	_dst_data_list_map[_item["id"].toString().toStdString()] = i;
		//}

		const Json::Value& _src_data_list = _src_json["dataList"];
		for (int i = 0; i < _src_data_list.size(); ++i)
		{
			const Json::Value& _item = _src_data_list[i];
			if (!_item.isMember("id"))
			{
				continue;
			}
			if (_dst_data_list_map.find(_item["id"].asString()) ==
				_dst_data_list_map.end())
			{
				_dst_data_list.append(_item);
			}
		}
		//const QJsonArray _src_data_list = _src_json["dataList"].toArray();
		//for (int i = 0; i < _src_data_list.size(); ++i)
		//{
		//	const QJsonObject& _item = _src_data_list[i].toObject();
		//	if (!_item.contains("id"))
		//	{
		//		continue;
		//	}
		//	if (_dst_data_list_map.find(_item["id"].toString().toStdString()) ==
		//		_dst_data_list_map.end())
		//	{
		//		_dst_data_list.append(_item);
		//	}
		//}
		//_dst_json["dataList"] = _dst_data_list;


		Json::StreamWriterBuilder builder;
		builder["commentStyle"] = "None";
		builder["indentation"] = "   ";  // 紧凑格式 
		builder["enableYAMLCompatibility"] = false;
		builder["dropNullPlaceholders"] = false;
		builder["emitUTF8"] = true;   // 关键设置：直接输出UTF-8字符 
		builder["sortKeys"] = false;
		std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
		std::ofstream ofs;
		ofs.open(_dst_file_info.absoluteFilePath().toStdString());
		writer->write(_dst_json, &ofs);
		ofs.close();
		//QJsonDocument _write_doc;
		//_write_doc.setObject(_dst_json);
		//QFile _write_file(_dst_file_info.absoluteFilePath());
		//_write_file.open(QFile::WriteOnly | QFile::Truncate);

		//QTextStream _write_stream(&_write_file);
		//_write_stream.setEncoding(QStringConverter::Utf8);
		//_write_stream << _write_doc.toJson();
		//_write_file.close();

		now_count++;
		if (one_finished) { one_finished(all_count, now_count); }
		QCoreApplication::processEvents(); // 处理事件循环
	}
	qDebug() << "copy file: " << copy_file;
	return true;
}

App::App(QWidget* parent) :
	QWidget(parent)
{
	m_progress_dialog = new QProgressDialog(
		"正在处理，请稍候...", QString(), 0, 100, this
	);
	m_progress_dialog->setWindowModality(Qt::NonModal);
	m_progress_dialog->setCancelButton(nullptr); // 关键代码：移除取消按钮
	m_progress_dialog->setWindowFlags(
		m_progress_dialog->windowFlags() & ~Qt::WindowCloseButtonHint
	); // 移除关闭按钮
	m_progress_dialog->setValue(0);
	m_progress_dialog->setMinimumDuration(-1);
	m_progress_dialog->hide();

	this->resize(700, 200);
	this->setFixedSize(700, 200);

	FileSelectionWidget* left = new FileSelectionWidget("目标文件", this);
	left->setGeometry(0, 0, this->rect().width() / 2, this->rect().height());
	left->setWidgetStatue(FileSelectionWidget::WidgetStatue::Right);
	left->setTitleStatue(FileSelectionWidget::TitleStatue::Center);
	FileSelectionWidget* right = new FileSelectionWidget("比对文件", this);
	right->setGeometry(
		this->rect().width() / 2, 0,
		this->rect().width() / 2, this->rect().height()
	);
	right->setWidgetStatue(FileSelectionWidget::WidgetStatue::Left);
	right->setTitleStatue(FileSelectionWidget::TitleStatue::Center);

	do
	{
		std::ifstream ifs;
		ifs.open("last_run.json");
		if (!ifs.is_open() || !ifs.good())
		{
			break;
		}
		Json::Value _json;
		Json::Reader().parse(ifs, _json);
		ifs.close();
		right->setFileDir(
			QString::fromStdString(_json["src_dir"].asString())
		);
		left->setFileDir(
			QString::fromStdString(_json["dst_dir"].asString())
		);
		right->setFilePrefix(
			QString::fromStdString(_json["src_prefix"].asString())
		);
		left->setFilePrefix(
			QString::fromStdString(_json["dst_prefix"].asString())
		);

	} while (0);

	QRect bottom_rect(0, this->height() - 50, this->width(), 50);
	QPushButton* _start_button = new QPushButton("start", this);
	int _start_button_width = 70;
	int _start_button_height = 30;
	_start_button->setGeometry(
		bottom_rect.left() + (bottom_rect.width() - _start_button_width) / 2,
		bottom_rect.top() + (bottom_rect.height() - _start_button_height) / 2,
		_start_button_width, _start_button_height
	);
	connect(_start_button, &QPushButton::clicked,
		[left, right, this, _start_button]() {
			NObjectHelper btn_helper(
				[_start_button]() {
					_start_button->setEnabled(false);
				}, 
				[_start_button]() {
					_start_button->setEnabled(true);
				}
			);

			m_progress_dialog->setValue(0);
			m_progress_dialog->show();
			bool _res = coreFunc(
				left->getFileDir(), left->getFilePrefix(),
				right->getFileDir(), right->getFilePrefix(),
				[this](int all_count, int now_count) {
					m_progress_dialog->setValue(now_count * 100 / all_count);
				}
			);
			m_progress_dialog->hide();

			if (!_res)
			{
				return;
			}

			Json::Value _json;
			_json["src_dir"] = right->getFileDir().toStdString();
			_json["dst_dir"] = left->getFileDir().toStdString();
			_json["src_prefix"] = right->getFilePrefix().toStdString();
			_json["dst_prefix"] = left->getFilePrefix().toStdString();
			std::ofstream ofs;
			ofs.open("last_run.json");
			ofs << _json.toStyledString();
			ofs.close();
		}
	);
}
