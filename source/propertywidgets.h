#ifndef PROPERTY_WIDGETS_H
#define PROPERTY_WIDGETS_H

#include <QWidget>
#include <QFrame>
#include <QLabel>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QCheckBox>

class TogglePropertyWidget : public QWidget
{
    Q_OBJECT

public:
    TogglePropertyWidget(
        const QString &text,
        bool value,
        const QString &tooltip,
        QWidget *parent = nullptr
    );

    void setEnabled(bool enabled);
    void setValue(bool value);
    bool getValue() const;

    Q_SIGNAL
    void valueChanged(Qt::CheckState state);

protected:
    QFrame      *frame;
    QLabel      *label;
    QCheckBox   *toggle;
};


class DecimalPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    DecimalPropertyWidget(
        const QString &text,
        double value,
        double min = -1000000.0,
        double max = 1000000.0,
        double step = 1.0,
        int decimals = 2,
        const QString& tooltip= "",
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
        int step = 1,
        const QString& tooltip= "",
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


class DropdownPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    DropdownPropertyWidget(const QString &text, const QString &tooltip, QWidget *parent = nullptr);
    void addItem(const QString &text, int value);
    void setEnabled(bool enabled);
    void setSelected(int value);
    void setSelectedIndex(int idx);
    int getSelected() const;
    int getSelectedIndex() const;

    Q_SIGNAL
    void selectedValueChanged(int selected_value);

    Q_SIGNAL
    void selectedIndexChanged(int idx);

protected:
    QFrame  *frame;
    QLabel *label;
    QComboBox *menu;

    void onMenuSelectionChanged(int index);
};

#endif
