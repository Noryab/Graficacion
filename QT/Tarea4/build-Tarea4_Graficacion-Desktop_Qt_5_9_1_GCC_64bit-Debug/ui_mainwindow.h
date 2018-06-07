/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QRadioButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include <renderObj.h>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout_5;
    QSpinBox *spinBox_4;
    QLabel *label_6;
    QHBoxLayout *horizontalLayout_4;
    QSpinBox *spinBox_3;
    QLabel *label_4;
    QHBoxLayout *horizontalLayout_6;
    QSpinBox *spinBox_5;
    QLabel *label_8;
    QRadioButton *radioButton;
    QRadioButton *radioButton_2;
    QRadioButton *radioButton_4;
    QRadioButton *radioButton_5;
    QRadioButton *radioButton_6;
    renderObj *widget;
    QSpinBox *spinBox;
    QLabel *label;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(1359, 1134);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayoutWidget = new QWidget(centralWidget);
        verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(1020, 520, 211, 109));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        spinBox_4 = new QSpinBox(verticalLayoutWidget);
        spinBox_4->setObjectName(QStringLiteral("spinBox_4"));
        spinBox_4->setMinimum(-360);
        spinBox_4->setMaximum(360);
        spinBox_4->setSingleStep(12);

        horizontalLayout_5->addWidget(spinBox_4);

        label_6 = new QLabel(verticalLayoutWidget);
        label_6->setObjectName(QStringLiteral("label_6"));
        QPalette palette;
        QBrush brush(QColor(255, 255, 255, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        QBrush brush1(QColor(190, 190, 190, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_6->setPalette(palette);

        horizontalLayout_5->addWidget(label_6);


        verticalLayout->addLayout(horizontalLayout_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        spinBox_3 = new QSpinBox(verticalLayoutWidget);
        spinBox_3->setObjectName(QStringLiteral("spinBox_3"));
        spinBox_3->setMinimum(-360);
        spinBox_3->setMaximum(360);
        spinBox_3->setSingleStep(12);

        horizontalLayout_4->addWidget(spinBox_3);

        label_4 = new QLabel(verticalLayoutWidget);
        label_4->setObjectName(QStringLiteral("label_4"));
        QPalette palette1;
        palette1.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette1.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_4->setPalette(palette1);

        horizontalLayout_4->addWidget(label_4);


        verticalLayout->addLayout(horizontalLayout_4);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        spinBox_5 = new QSpinBox(verticalLayoutWidget);
        spinBox_5->setObjectName(QStringLiteral("spinBox_5"));
        spinBox_5->setMinimum(-360);
        spinBox_5->setMaximum(360);
        spinBox_5->setSingleStep(12);

        horizontalLayout_6->addWidget(spinBox_5);

        label_8 = new QLabel(verticalLayoutWidget);
        label_8->setObjectName(QStringLiteral("label_8"));
        QPalette palette2;
        palette2.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette2.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label_8->setPalette(palette2);

        horizontalLayout_6->addWidget(label_8);


        verticalLayout->addLayout(horizontalLayout_6);

        radioButton = new QRadioButton(centralWidget);
        radioButton->setObjectName(QStringLiteral("radioButton"));
        radioButton->setGeometry(QRect(1020, 450, 133, 26));
        QPalette palette3;
        palette3.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette3.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        radioButton->setPalette(palette3);
        radioButton_2 = new QRadioButton(centralWidget);
        radioButton_2->setObjectName(QStringLiteral("radioButton_2"));
        radioButton_2->setGeometry(QRect(1020, 420, 133, 26));
        QPalette palette4;
        palette4.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette4.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        radioButton_2->setPalette(palette4);
        radioButton_2->setChecked(false);
        radioButton_4 = new QRadioButton(centralWidget);
        radioButton_4->setObjectName(QStringLiteral("radioButton_4"));
        radioButton_4->setGeometry(QRect(1020, 360, 133, 26));
        QPalette palette5;
        palette5.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette5.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        radioButton_4->setPalette(palette5);
        radioButton_4->setChecked(true);
        radioButton_5 = new QRadioButton(centralWidget);
        radioButton_5->setObjectName(QStringLiteral("radioButton_5"));
        radioButton_5->setGeometry(QRect(1020, 390, 133, 26));
        QPalette palette6;
        palette6.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette6.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        radioButton_5->setPalette(palette6);
        radioButton_5->setChecked(false);
        radioButton_6 = new QRadioButton(centralWidget);
        radioButton_6->setObjectName(QStringLiteral("radioButton_6"));
        radioButton_6->setGeometry(QRect(1020, 330, 133, 26));
        QPalette palette7;
        palette7.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette7.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        radioButton_6->setPalette(palette7);
        radioButton_6->setChecked(false);
        widget = new renderObj(centralWidget);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(20, 10, 961, 1051));
        spinBox = new QSpinBox(centralWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));
        spinBox->setGeometry(QRect(1260, 330, 52, 29));
        spinBox->setMaximum(5);
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(1170, 330, 80, 20));
        QPalette palette8;
        palette8.setBrush(QPalette::Active, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette8.setBrush(QPalette::Disabled, QPalette::WindowText, brush1);
        label->setPalette(palette8);
        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1359, 25));
        MainWindow->setMenuBar(menuBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);

        retranslateUi(MainWindow);
        QObject::connect(spinBox_4, SIGNAL(valueChanged(int)), MainWindow, SLOT(RotateX(int)));
        QObject::connect(spinBox_3, SIGNAL(valueChanged(int)), MainWindow, SLOT(RotateY(int)));
        QObject::connect(spinBox_5, SIGNAL(valueChanged(int)), MainWindow, SLOT(RotateZ(int)));
        QObject::connect(radioButton, SIGNAL(clicked()), MainWindow, SLOT(Puntos()));
        QObject::connect(radioButton_4, SIGNAL(clicked()), MainWindow, SLOT(Gouraud()));
        QObject::connect(radioButton_2, SIGNAL(clicked()), MainWindow, SLOT(Mallas()));
        QObject::connect(radioButton_5, SIGNAL(clicked()), MainWindow, SLOT(Poligonos()));
        QObject::connect(radioButton_6, SIGNAL(clicked()), MainWindow, SLOT(Textura()));
        QObject::connect(spinBox, SIGNAL(valueChanged(int)), MainWindow, SLOT(ChooseTextura(int)));

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", Q_NULLPTR));
        label_6->setText(QApplication::translate("MainWindow", "Rotaci\303\263n (X)", Q_NULLPTR));
        label_4->setText(QApplication::translate("MainWindow", "Rotaci\303\263n (Y)", Q_NULLPTR));
        label_8->setText(QApplication::translate("MainWindow", "Rotaci\303\263n (Z)", Q_NULLPTR));
        radioButton->setText(QApplication::translate("MainWindow", "Puntos", Q_NULLPTR));
        radioButton_2->setText(QApplication::translate("MainWindow", "Mallas", Q_NULLPTR));
        radioButton_4->setText(QApplication::translate("MainWindow", "Gouraud", Q_NULLPTR));
        radioButton_5->setText(QApplication::translate("MainWindow", "Poligonos", Q_NULLPTR));
        radioButton_6->setText(QApplication::translate("MainWindow", "Textura", Q_NULLPTR));
        label->setText(QApplication::translate("MainWindow", "Textura #", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
