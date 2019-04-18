/********************************************************************************
** Form generated from reading UI file 'binoreconstruction.ui'
**
** Created by: Qt User Interface Compiler version 5.5.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_BINORECONSTRUCTION_H
#define UI_BINORECONSTRUCTION_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>
#include "QVTKWidget.h"

QT_BEGIN_NAMESPACE

class Ui_binoReconstructionClass
{
public:
    QAction *ReadCalibrationParameters;
    QAction *binoRestruction;
    QAction *singleRrestruction;
    QAction *Rgbdreconstruction;
    QAction *ReadPointCloud;
    QAction *SavePointCloud;
    QWidget *centralWidget;
    QVTKWidget *qvtkWidget;
    QMenuBar *menuBar;
    QMenu *menuReadCalibratePara;
    QMenu *menuRGBD;
    QMenu *menu;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *binoReconstructionClass)
    {
        if (binoReconstructionClass->objectName().isEmpty())
            binoReconstructionClass->setObjectName(QStringLiteral("binoReconstructionClass"));
        binoReconstructionClass->resize(1384, 896);
        ReadCalibrationParameters = new QAction(binoReconstructionClass);
        ReadCalibrationParameters->setObjectName(QStringLiteral("ReadCalibrationParameters"));
        binoRestruction = new QAction(binoReconstructionClass);
        binoRestruction->setObjectName(QStringLiteral("binoRestruction"));
        singleRrestruction = new QAction(binoReconstructionClass);
        singleRrestruction->setObjectName(QStringLiteral("singleRrestruction"));
        Rgbdreconstruction = new QAction(binoReconstructionClass);
        Rgbdreconstruction->setObjectName(QStringLiteral("Rgbdreconstruction"));
        ReadPointCloud = new QAction(binoReconstructionClass);
        ReadPointCloud->setObjectName(QStringLiteral("ReadPointCloud"));
        SavePointCloud = new QAction(binoReconstructionClass);
        SavePointCloud->setObjectName(QStringLiteral("SavePointCloud"));
        centralWidget = new QWidget(binoReconstructionClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        qvtkWidget = new QVTKWidget(centralWidget);
        qvtkWidget->setObjectName(QStringLiteral("qvtkWidget"));
        qvtkWidget->setGeometry(QRect(10, 10, 1361, 861));
        qvtkWidget->setAutoFillBackground(true);
        qvtkWidget->setStyleSheet(QStringLiteral(""));
        binoReconstructionClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(binoReconstructionClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 1384, 23));
        menuReadCalibratePara = new QMenu(menuBar);
        menuReadCalibratePara->setObjectName(QStringLiteral("menuReadCalibratePara"));
        menuRGBD = new QMenu(menuBar);
        menuRGBD->setObjectName(QStringLiteral("menuRGBD"));
        menu = new QMenu(menuBar);
        menu->setObjectName(QStringLiteral("menu"));
        binoReconstructionClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(binoReconstructionClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        binoReconstructionClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(binoReconstructionClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        binoReconstructionClass->setStatusBar(statusBar);

        menuBar->addAction(menu->menuAction());
        menuBar->addAction(menuReadCalibratePara->menuAction());
        menuBar->addAction(menuRGBD->menuAction());
        menuReadCalibratePara->addAction(ReadCalibrationParameters);
        menuReadCalibratePara->addAction(binoRestruction);
        menuRGBD->addAction(Rgbdreconstruction);
        menu->addAction(ReadPointCloud);

        retranslateUi(binoReconstructionClass);

        QMetaObject::connectSlotsByName(binoReconstructionClass);
    } // setupUi

    void retranslateUi(QMainWindow *binoReconstructionClass)
    {
        binoReconstructionClass->setWindowTitle(QApplication::translate("binoReconstructionClass", "binoReconstruction", 0));
        ReadCalibrationParameters->setText(QApplication::translate("binoReconstructionClass", "ReadCaliPara", 0));
        binoRestruction->setText(QApplication::translate("binoReconstructionClass", "binoRestruction", 0));
        singleRrestruction->setText(QApplication::translate("binoReconstructionClass", "singleRrestruction", 0));
        Rgbdreconstruction->setText(QApplication::translate("binoReconstructionClass", "Rgbdreconstruction", 0));
        ReadPointCloud->setText(QApplication::translate("binoReconstructionClass", "ReadpointCloud", 0));
        SavePointCloud->setText(QApplication::translate("binoReconstructionClass", "SavePointCloud", 0));
        menuReadCalibratePara->setTitle(QApplication::translate("binoReconstructionClass", "\345\217\214\347\233\256\351\207\215\345\273\272", 0));
        menuRGBD->setTitle(QApplication::translate("binoReconstructionClass", "RGBD\351\207\215\345\273\272", 0));
        menu->setTitle(QApplication::translate("binoReconstructionClass", "\350\257\273\345\217\226\347\202\271\344\272\221", 0));
    } // retranslateUi

};

namespace Ui {
    class binoReconstructionClass: public Ui_binoReconstructionClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_BINORECONSTRUCTION_H
