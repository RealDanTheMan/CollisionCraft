#ifndef PROPERTY_PANEL_H
#define PROPERTY_PANEL_H

#include "collisiongen.h"
#include <QWidget>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QSpinBox>
#include <QFrame>
#include <QVBoxLayout>


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


class PropertyPanelWidget : public QWidget
{
    Q_OBJECT

public:
    PropertyPanelWidget(QWidget *parent = nullptr);
    CollisionTechnique getSelectedTechnique() const;
    CollisionGenSettings getSettings() const;

    Q_SIGNAL
    void collisionGenerationRequested();

protected:
    void onTechniqueSelectionChanged();
    CollisionTechnique selected_technique;

private:
    QPushButton *generate_button;
    QComboBox *technique_menu;
    DecimalPropertyWidget *scale_property;
    DecimalPropertyWidget *resolution_property;
    IntegerPropertyWidget *hull_count_property;
    IntegerPropertyWidget *downsampling_property;
};


#endif
