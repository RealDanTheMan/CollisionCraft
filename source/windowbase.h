/********************************************************************************
** Form generated from reading UI file 'mainwindowhnDCyY.ui'
**
** Created by: Qt User Interface Compiler version 6.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef MAINWINDOWHNDCYY_H
#define MAINWINDOWHNDCYY_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionImportModel;
    QAction *actionExportCollision;
    QAction *actionFrameAll;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout_3;
    QHBoxLayout *horizontalLayout;
    QVBoxLayout *verticalLayout_3;
    QFrame *ViewportFrame;
    QFrame *ConsoleFrame;
    QHBoxLayout *horizontalLayout_2;
    QFrame *PropertyPanelFrame;
    QMenuBar *menubar;
    QMenu *menuFile;
    QMenu *menuView;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(800, 600);
        actionImportModel = new QAction(MainWindow);
        actionImportModel->setObjectName("actionImportModel");
        actionExportCollision = new QAction(MainWindow);
        actionExportCollision->setObjectName("actionExportCollision");
        actionFrameAll = new QAction(MainWindow);
        actionFrameAll->setObjectName("actionFrameAll");
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        horizontalLayout_3 = new QHBoxLayout(centralwidget);
        horizontalLayout_3->setObjectName("horizontalLayout_3");
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName("horizontalLayout");
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setObjectName("verticalLayout_3");
        ViewportFrame = new QFrame(centralwidget);
        ViewportFrame->setObjectName("ViewportFrame");
        ViewportFrame->setFrameShape(QFrame::Shape::StyledPanel);
        ViewportFrame->setFrameShadow(QFrame::Shadow::Raised);

        verticalLayout_3->addWidget(ViewportFrame);

        ConsoleFrame = new QFrame(centralwidget);
        ConsoleFrame->setObjectName("ConsoleFrame");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Preferred, QSizePolicy::Policy::Preferred);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(ConsoleFrame->sizePolicy().hasHeightForWidth());
        ConsoleFrame->setSizePolicy(sizePolicy);
        ConsoleFrame->setMaximumSize(QSize(16777215, 16777215));
        ConsoleFrame->setFrameShape(QFrame::Shape::StyledPanel);
        ConsoleFrame->setFrameShadow(QFrame::Shadow::Raised);

        verticalLayout_3->addWidget(ConsoleFrame);

        verticalLayout_3->setStretch(0, 6);
        verticalLayout_3->setStretch(1, 1);

        horizontalLayout->addLayout(verticalLayout_3);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        PropertyPanelFrame = new QFrame(centralwidget);
        PropertyPanelFrame->setObjectName("PropertyPanelFrame");
        sizePolicy.setHeightForWidth(PropertyPanelFrame->sizePolicy().hasHeightForWidth());
        PropertyPanelFrame->setSizePolicy(sizePolicy);
        PropertyPanelFrame->setFrameShape(QFrame::Shape::StyledPanel);
        PropertyPanelFrame->setFrameShadow(QFrame::Shadow::Raised);

        horizontalLayout_2->addWidget(PropertyPanelFrame);


        horizontalLayout->addLayout(horizontalLayout_2);

        horizontalLayout->setStretch(0, 5);
        horizontalLayout->setStretch(1, 2);

        horizontalLayout_3->addLayout(horizontalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 24));
        menuFile = new QMenu(menubar);
        menuFile->setObjectName("menuFile");
        menuView = new QMenu(menubar);
        menuView->setObjectName("menuView");
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        menubar->addAction(menuFile->menuAction());
        menubar->addAction(menuView->menuAction());
        menuFile->addAction(actionImportModel);
        menuFile->addAction(actionExportCollision);
        menuView->addAction(actionFrameAll);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        actionImportModel->setText(QCoreApplication::translate("MainWindow", "Import Model", nullptr));
#if QT_CONFIG(tooltip)
        actionImportModel->setToolTip(QCoreApplication::translate("MainWindow", "Import USD Model", nullptr));
#endif // QT_CONFIG(tooltip)
        actionExportCollision->setText(QCoreApplication::translate("MainWindow", "Export Collision", nullptr));
        actionFrameAll->setText(QCoreApplication::translate("MainWindow", "Frame All", nullptr));
        menuFile->setTitle(QCoreApplication::translate("MainWindow", "File", nullptr));
        menuView->setTitle(QCoreApplication::translate("MainWindow", "View", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // MAINWINDOWHNDCYY_H
