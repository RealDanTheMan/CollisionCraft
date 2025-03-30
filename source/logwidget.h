#ifndef LOG_WIDGET_H
#define LOG_WIDGET_H

#include <QWidget>
#include <QListWidget>
#include <qlistwidget.h>
#include <string>

#include "logging.h"

class LogWidget : public QWidget
{
	Q_OBJECT

public:
	LogWidget(Logger &logger, QWidget *parent = nullptr);

protected:
	QListWidget* log_list;

private:
	void onMessageLogged(const std::string &message, LogLevel level);
};

#endif
