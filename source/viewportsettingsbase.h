/********************************************************************************
** Form generated from reading UI file 'viewport_settingshJJxny.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef VIEWPORT_SETTINGSHJJXNY_H
#define VIEWPORT_SETTINGSHJJXNY_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ViewportSettingsBase
{
public:
    QVBoxLayout *verticalLayout;
    QFrame *main_frame;
    QVBoxLayout *verticalLayout_2;
    QFrame *model_frame;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QCheckBox *model_shaded_checkbox;
    QCheckBox *model_lighting_checkbox;
    QCheckBox *model_wireframe_checkbox;
    QFrame *collision_frame;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_2;
    QCheckBox *collision_shaded_checkbox;
    QCheckBox *collision_wireframe_checkbox;
    QFrame *button_frame;
    QHBoxLayout *horizontalLayout;
    QPushButton *apply_button;
    QPushButton *cancel_button;

    void setupUi(QWidget *ViewportSettingsBase)
    {
        if (ViewportSettingsBase->objectName().isEmpty())
            ViewportSettingsBase->setObjectName("ViewportSettingsBase");
        ViewportSettingsBase->resize(330, 335);
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ViewportSettingsBase->sizePolicy().hasHeightForWidth());
        ViewportSettingsBase->setSizePolicy(sizePolicy);
        ViewportSettingsBase->setAutoFillBackground(false);
        verticalLayout = new QVBoxLayout(ViewportSettingsBase);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(4, 4, 4, 4);
        main_frame = new QFrame(ViewportSettingsBase);
        main_frame->setObjectName("main_frame");
        QSizePolicy sizePolicy1(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(main_frame->sizePolicy().hasHeightForWidth());
        main_frame->setSizePolicy(sizePolicy1);
        main_frame->setFrameShape(QFrame::Shape::StyledPanel);
        main_frame->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_2 = new QVBoxLayout(main_frame);
        verticalLayout_2->setObjectName("verticalLayout_2");
        model_frame = new QFrame(main_frame);
        model_frame->setObjectName("model_frame");
        sizePolicy1.setHeightForWidth(model_frame->sizePolicy().hasHeightForWidth());
        model_frame->setSizePolicy(sizePolicy1);
        model_frame->setFrameShape(QFrame::Shape::StyledPanel);
        model_frame->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_3 = new QVBoxLayout(model_frame);
        verticalLayout_3->setSpacing(12);
        verticalLayout_3->setObjectName("verticalLayout_3");
        verticalLayout_3->setSizeConstraint(QLayout::SizeConstraint::SetDefaultConstraint);
        verticalLayout_3->setContentsMargins(4, 4, 4, 32);
        label = new QLabel(model_frame);
        label->setObjectName("label");
        QFont font;
        font.setBold(true);
        label->setFont(font);

        verticalLayout_3->addWidget(label);

        model_shaded_checkbox = new QCheckBox(model_frame);
        model_shaded_checkbox->setObjectName("model_shaded_checkbox");

        verticalLayout_3->addWidget(model_shaded_checkbox);

        model_lighting_checkbox = new QCheckBox(model_frame);
        model_lighting_checkbox->setObjectName("model_lighting_checkbox");

        verticalLayout_3->addWidget(model_lighting_checkbox);

        model_wireframe_checkbox = new QCheckBox(model_frame);
        model_wireframe_checkbox->setObjectName("model_wireframe_checkbox");

        verticalLayout_3->addWidget(model_wireframe_checkbox);


        verticalLayout_2->addWidget(model_frame);

        collision_frame = new QFrame(main_frame);
        collision_frame->setObjectName("collision_frame");
        sizePolicy1.setHeightForWidth(collision_frame->sizePolicy().hasHeightForWidth());
        collision_frame->setSizePolicy(sizePolicy1);
        collision_frame->setFrameShape(QFrame::Shape::StyledPanel);
        collision_frame->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_4 = new QVBoxLayout(collision_frame);
        verticalLayout_4->setObjectName("verticalLayout_4");
        verticalLayout_4->setContentsMargins(4, 4, 4, 32);
        label_2 = new QLabel(collision_frame);
        label_2->setObjectName("label_2");
        label_2->setFont(font);

        verticalLayout_4->addWidget(label_2);

        collision_shaded_checkbox = new QCheckBox(collision_frame);
        collision_shaded_checkbox->setObjectName("collision_shaded_checkbox");

        verticalLayout_4->addWidget(collision_shaded_checkbox);

        collision_wireframe_checkbox = new QCheckBox(collision_frame);
        collision_wireframe_checkbox->setObjectName("collision_wireframe_checkbox");

        verticalLayout_4->addWidget(collision_wireframe_checkbox);


        verticalLayout_2->addWidget(collision_frame);

        button_frame = new QFrame(main_frame);
        button_frame->setObjectName("button_frame");
        sizePolicy1.setHeightForWidth(button_frame->sizePolicy().hasHeightForWidth());
        button_frame->setSizePolicy(sizePolicy1);
        button_frame->setFrameShape(QFrame::Shape::NoFrame);
        button_frame->setFrameShadow(QFrame::Shadow::Raised);
        horizontalLayout = new QHBoxLayout(button_frame);
        horizontalLayout->setObjectName("horizontalLayout");
        apply_button = new QPushButton(button_frame);
        apply_button->setObjectName("apply_button");

        horizontalLayout->addWidget(apply_button);

        cancel_button = new QPushButton(button_frame);
        cancel_button->setObjectName("cancel_button");

        horizontalLayout->addWidget(cancel_button);


        verticalLayout_2->addWidget(button_frame);


        verticalLayout->addWidget(main_frame);


        retranslateUi(ViewportSettingsBase);

        QMetaObject::connectSlotsByName(ViewportSettingsBase);
    } // setupUi

    void retranslateUi(QWidget *ViewportSettingsBase)
    {
        ViewportSettingsBase->setWindowTitle(QCoreApplication::translate("ViewportSettingsBase", "Form", nullptr));
        label->setText(QCoreApplication::translate("ViewportSettingsBase", "Model Rendering", nullptr));
        model_shaded_checkbox->setText(QCoreApplication::translate("ViewportSettingsBase", "Shaded", nullptr));
        model_lighting_checkbox->setText(QCoreApplication::translate("ViewportSettingsBase", "Lighting", nullptr));
        model_wireframe_checkbox->setText(QCoreApplication::translate("ViewportSettingsBase", "Wireframe", nullptr));
        label_2->setText(QCoreApplication::translate("ViewportSettingsBase", "Collision Rendering", nullptr));
        collision_shaded_checkbox->setText(QCoreApplication::translate("ViewportSettingsBase", "Shaded", nullptr));
        collision_wireframe_checkbox->setText(QCoreApplication::translate("ViewportSettingsBase", "Wireframe", nullptr));
        apply_button->setText(QCoreApplication::translate("ViewportSettingsBase", "Apply", nullptr));
        cancel_button->setText(QCoreApplication::translate("ViewportSettingsBase", "Cancel", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ViewportSettingsBase: public Ui_ViewportSettingsBase {};
} // namespace Ui

QT_END_NAMESPACE

#endif // VIEWPORT_SETTINGSHJJXNY_H
