#ifndef PROPERTY_WIDGETS_H
#define PROPERTY_WIDGETS_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>

class DecimalPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    DecimalPropertyWidget(
        const QString &text,
        double value,
        double min = -1000000.0,
        double max = 1000000.0,
        QWidget *parent = nullptr
);
    void setEnabled(bool enabled);
    void setValue(double);
    double getValue() const;


protected:
    QFrame *frame;
    QLabel *label;
    QDoubleSpinBox *spinner;
};


class IntegerPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    IntegerPropertyWidget(
        const QString &text,
        int value,
        int min = -1000000,
        int max = 1000000,
        QWidget *parent = nullptr
);
    void setEnabled(bool enabled);
    void setValue(double);
    double getValue() const;

protected:
    QFrame *frame;
    QLabel *label;
    QSpinBox *spinner;
};

#endif
