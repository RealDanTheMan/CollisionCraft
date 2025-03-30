#include "logwidget.h"
#include "logging.h"
#include <QVBoxLayout>
#include <QListWidget>
#include <QListWidgetItem>
#include <QString>

LogWidget::LogWidget(Logger &logger, QWidget *parent) :	QWidget(parent)
{
	this->setLayout(new QVBoxLayout());
	this->layout()->setContentsMargins(0, 0, 0, 0);
	this->layout()->setSpacing(0);

	this->log_list = new QListWidget(this);
	this->layout()->addWidget(this->log_list);

	connect(&logger, &Logger::messageLogged, this, &LogWidget::onMessageLogged);
}

void LogWidget::onMessageLogged(const std::string &message, LogLevel level)
{
	QString msg = QString::fromStdString(message);
	this->log_list->addItem(msg);
}
