/*****************************************************************************
 * modbus4qt Library
 * Author: Leonid Kolesnik, l.kolesnik@m-i.ru
 * Copyright (C) 2012-2015
 * http://mt11.net.ru
 *****************************************************************************/

/*****************************************************************************
* This file is part of modbus4qt.
*
* modbus4qt is free software: you can redistribute it and/or modify it under the
* terms of the GNU Lesser General Public License as published by the Free
* Software Foundation, either version 3 of the License, or (at your option) any
* later version.
*
* modbus4qt is distributed in the hope that it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
* A PARTICULAR PURPOSE. See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along with
* modbus4qt.
*
* If not, see <https://www.gnu.org/licenses/>.
*****************************************************************************/

#include "main_window.h"
#include "main.h"

#include <QtGui>
#include <QAction>
#include <QGridLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QMenuBar>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QCheckBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* centralWidget = new QWidget();

    QVBoxLayout* mainLayout = new QVBoxLayout;
    QHBoxLayout* registersLayout = new QHBoxLayout;

    createServerGroupBox_();

    createCoilsGroupBox_();
    createDiscreteInputsGroupBox_();
    createInputRegistersGroupBox_();
    createHoldingRegistersGroupBox_();

    registersLayout->addWidget(coilsGroupBox_);
    registersLayout->addWidget(discreteInputsGroupBox_);
    registersLayout->addWidget(inputRegistersGroupBox_);
    registersLayout->addWidget(holdingRegistersGroupBox_);

    mainLayout->addWidget(serverGroupBox_);
    mainLayout->addStretch();
    mainLayout->addLayout(registersLayout);
    mainLayout->addStretch();
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
    centralWidget->setLayout(mainLayout);

    setCentralWidget(centralWidget);

    createActions_();
    createMenus_();
    statusBar();
    setWindowTitle(ApplicationName);

}

//-----------------------------------------------------------------------------

MainWindow::~MainWindow()
{
}

//-----------------------------------------------------------------------------

void
MainWindow::about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About"));
    msgBox.setText(ApplicationName);
    msgBox.setInformativeText(QString("Copyright \nhttp://www.") + OrganizationDomain);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

//-----------------------------------------------------------------------------

void
MainWindow::createActions_()
{
    aboutAction_ = new QAction(tr("&About..."), this);
    aboutAction_->setStatusTip(tr("Show this program's About box"));
    connect(aboutAction_, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAction_ = new QAction(tr("About &Qt..."), this);
    aboutQtAction_->setShortcut(tr("Ctrl+Shift+F1"));
    aboutQtAction_->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAction_, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    exitAction_ = new QAction(tr("E&xit"), this);
    exitAction_->setShortcut(tr("Ctrl+Q"));
    exitAction_->setStatusTip(tr("Exit the application"));
    connect(exitAction_, SIGNAL(triggered()), this, SLOT(close()));
}

//-----------------------------------------------------------------------------

void
MainWindow::createMenus_()
{
    fileMenu_ = menuBar()->addMenu(tr("&File"));
    fileMenu_->addAction(exitAction_);

    helpMenu_ = menuBar()->addMenu(tr("Help"));
    helpMenu_->addAction(aboutAction_);
    helpMenu_->addAction(aboutQtAction_);
}

//-----------------------------------------------------------------------------

void
MainWindow::createServerGroupBox_()
{
    serverGroupBox_ = new QGroupBox(tr("MODBUS TCP Server"));
    QHBoxLayout *layout = new QHBoxLayout;

    QLabel* serverAddressLabel = new QLabel(tr("Server address:"));
    serverAddress_ = new QLineEdit("127.0.0.1");

    layout->addWidget(serverAddressLabel);
    layout->addWidget(serverAddress_);

    layout->addStretch();

    QLabel* serverPortLabel = new QLabel(tr("Server port:"));
    serverPort_ = new QSpinBox();
    serverPort_->setRange(1, 65535);
    serverPort_->setValue(502);

    layout->addWidget(serverPortLabel);
    layout->addWidget(serverPort_);

    serverGroupBox_->setLayout(layout);
}

//-----------------------------------------------------------------------------

void
MainWindow::createCoilsGroupBox_() // r/w
{
    coilsGroupBox_ = new QGroupBox(tr("Coils"));
    QVBoxLayout* layout = new QVBoxLayout;

    QCheckBox* coil1_ = new QCheckBox("1");
    QCheckBox* coil2_ = new QCheckBox("2");
    QCheckBox* coil3_ = new QCheckBox("3");
    QCheckBox* coil4_ = new QCheckBox("4");
    QCheckBox* coil5_ = new QCheckBox("5");

    layout->addWidget(coil1_);
    layout->addWidget(coil2_);
    layout->addWidget(coil3_);
    layout->addWidget(coil4_);
    layout->addWidget(coil5_);

    coilsGroupBox_->setLayout(layout);
}

//-----------------------------------------------------------------------------

void
MainWindow::createDiscreteInputsGroupBox_()
{
    discreteInputsGroupBox_ = new QGroupBox(tr("DisInput"));
    QVBoxLayout* layout = new QVBoxLayout;

    QCheckBox* dIn1_ = new QCheckBox("1");
    QCheckBox* dIn2_ = new QCheckBox("2");
    QCheckBox* dIn3_ = new QCheckBox("3");
    QCheckBox* dIn4_ = new QCheckBox("4");
    QCheckBox* dIn5_ = new QCheckBox("5");

    layout->addWidget(dIn1_);
    layout->addWidget(dIn2_);
    layout->addWidget(dIn3_);
    layout->addWidget(dIn4_);
    layout->addWidget(dIn5_);

    discreteInputsGroupBox_->setLayout(layout);
}

//-----------------------------------------------------------------------------

void
MainWindow::createInputRegistersGroupBox_()
{
    inputRegistersGroupBox_ = new QGroupBox(tr("Input Registers"));
    QGridLayout* layout = new QGridLayout;

    QLineEdit* inReg1_ = new QLineEdit;
    QLabel* inRegLabel1 = new QLabel("1");
    QLineEdit* inReg2_ = new QLineEdit;
    QLabel* inRegLabel2 = new QLabel("2");
    QLineEdit* inReg3_ = new QLineEdit;
    QLabel* inRegLabel3 = new QLabel("3");
    QLineEdit* inReg4_ = new QLineEdit;
    QLabel* inRegLabel4 = new QLabel("4");
    QLineEdit* inReg5_ = new QLineEdit;
    QLabel* inRegLabel5 = new QLabel("5");

    layout->addWidget(inReg1_, 0, 0);
    layout->addWidget(inRegLabel1, 0, 1);
    layout->addWidget(inReg2_, 1, 0);
    layout->addWidget(inRegLabel2, 1, 1);
    layout->addWidget(inReg3_, 2, 0);
    layout->addWidget(inRegLabel3, 2, 1);
    layout->addWidget(inReg4_, 3, 0);
    layout->addWidget(inRegLabel4, 3, 1);
    layout->addWidget(inReg5_, 4, 0);
    layout->addWidget(inRegLabel5, 4, 1);

    inputRegistersGroupBox_->setLayout(layout);
}

//-----------------------------------------------------------------------------

void
MainWindow::createHoldingRegistersGroupBox_() // r/w
{
    holdingRegistersGroupBox_ = new QGroupBox(tr("Holding Registers"));
    QGridLayout* layout = new QGridLayout;

    QLineEdit* holdReg1_ = new QLineEdit;
    QLabel* holdRegLabel1 = new QLabel("1");
    QLineEdit* holdReg2_ = new QLineEdit;
    QLabel* holdRegLabel2 = new QLabel("2");
    QLineEdit* holdReg3_ = new QLineEdit;
    QLabel* holdRegLabel3 = new QLabel("3");
    QLineEdit* holdReg4_ = new QLineEdit;
    QLabel* holdRegLabel4 = new QLabel("4");
    QLineEdit* holdReg5_ = new QLineEdit;
    QLabel* holdRegLabel5 = new QLabel("5");

    layout->addWidget(holdReg1_, 0, 0);
    layout->addWidget(holdRegLabel1, 0, 1);
    layout->addWidget(holdReg2_, 1, 0);
    layout->addWidget(holdRegLabel2, 1, 1);
    layout->addWidget(holdReg3_, 2, 0);
    layout->addWidget(holdRegLabel3, 2, 1);
    layout->addWidget(holdReg4_, 3, 0);
    layout->addWidget(holdRegLabel4, 3, 1);
    layout->addWidget(holdReg5_, 4, 0);
    layout->addWidget(holdRegLabel5, 4, 1);

    holdingRegistersGroupBox_->setLayout(layout);

}

//-----------------------------------------------------------------------------
