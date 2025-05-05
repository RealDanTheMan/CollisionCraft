#ifndef EXPANDER_WIDGET_H
#define EXPANDER_WIDGET_H

#include <QWidget>
#include <QToolButton>
#include <QVBoxLayout>

class ExpanderWidget : public QWidget
{
    Q_OBJECT

public:
    ExpanderWidget(const QString &title, QWidget *parent = nullptr);
    void addWidget(QWidget *widget);

protected:
    void onToggleButtonClick(bool toggle_value);

private:
    QToolButton *toggle_button;
    QWidget     *content_widget;
};

#endif
