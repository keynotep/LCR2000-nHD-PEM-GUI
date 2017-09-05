/*
 *
 *
 * Copyright (C) {2017} Keynote Photonics - www.keynotephotonics.com
 *
 *
 *    Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Redistribution and use of source is not permitted with out
 *  written authorizitaion.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Keynote Photonics nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/


/*
 *
 * Copyright (C) {2015} Texas Instruments Incorporated - http://www.ti.com/
 *
 *
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions
 *  are met:
 *
 *    Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 *    Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the
 *    distribution.
 *
 *    Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 *  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 *  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 *  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 *  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 *  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
*/

/**************************************************************************************************************************************************
*                                 Copyright © 2013 Texas Instruments Incorporated - http://www.ti.com/                                            *
***************************************************************************************************************************************************
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met: *
*                                                                                                                                                 *
*    Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.                 *
*                                                                                                                                                 *
*    Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the        *
*    documentation and/or other materials provided with the distribution.                                                                         *
*                                                                                                                                                 *
*    Neither the name of Texas Instruments Incorporated nor the names of its contributors may be used to endorse or promote products derived      *
*    from this software without specific prior written permission.                                                                                *
*                                                                                                                                                 *
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT          *
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT     *
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT         *
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY    *
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE      *
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                                           *
***************************************************************************************************************************************************/

/*!
 * \file mainwindow.cpp
 *
 * \brief This provides the implementation of MainWindow class.
 * \n It provides the implementation of GUI control functionalities.
 * \n Manages the sending of commands (using PacketHandler class which manages packet communication)
 * \n according to the required functionality and interpreting the response.
 */


#include <QFileDialog>
#include <QProgressDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QPainter>
#include <QDate>
#include <QTime>
#include <QImage>
#include <QBuffer>
#include <QFile>
#include <QFileInfo>
#include <QIODevice>
#include <QStringList>
#include <QDir>


#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "PacketHandler.h"
#include "Version.h"
#include "MBMCReadmeTxtParser.h"
#include "bmpparser.h"
#include "temperature.h"

#include <math.h>
#include <cmath>

#define LED_MAX_CURRENT_PASSIVE     274
#define LED_MAX_CURRENT_ACTIVE      758


#define VERSION_MSG_SKIP_DEFAULT        true               // Skip version checking
#define LEDSelect_MULTIPLE 7                                // Determines what item number of pull-down menu refers to MTB
#define DEBUG_MODE                      true                // If TRUE => Enable all hidden tabs when in Debug mode
                                                            // i.e. when ENV variable LC3000_GUI=KP_ENABLE_SPECIAL_TOOLS or KP_ENABLE_DEBUG

// For new version, be sure latest pkg version reference info is updated along with the version here
#define LATEST_LCR_FW_SF_PKG            "0.0.9"

//For the latest pkg version reference (obsolete)
#define LATEST_DM365_VER                PKG_KP0DOT09_DPEM_COMPATIBLE_VER
#define LATEST_MSP430_VER               PKG_KP0DOT09_MSP430_COMPATIBLE_VER
#define LATEST_FPGA_VER                 PKG_KP0DOT09_FPGA_COMPATIBLE_VER

#define LATEST_PEM_MAJOR_VER              5
#define LATEST_PEM_MINOR_VER              3
#define LATEST_PEM_BUILD_VER              10

#define LATEST_MSP430_MAJOR_VER             0
#define LATEST_MSP430_MINOR_VER             0
#define LATEST_MSP430_BUILD_VER             0

// KP 0.9 PKG version info (Keynote Photonics Release)
#define PKG_KP0DOT09_PEM_COMPATIBLE_VER    "0.0.9"
#define PKG_KP0DOT09_FPGA_COMPATIBLE_VER     "2.6.43"
#define PKG_KP0DOT09_MSP430_COMPATIBLE_VER   "KP3.1"


// Variable typ definition for loading and changing splash images in DLPC300
typedef struct splashInfo
{
   unsigned int Resolution;
   unsigned int StartAddress;
   unsigned int Size;
} SPLASH;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),
    settings("Keynote Photonics", "LC2000 nHD GUI")
{
    char versionStr[128];
    int i;
    int index;

    ui->setupUi(this);
    connect(&packetHandler,
            SIGNAL(setStatus(const char *, PacketHandler::ConnectionStatus)),
            this,
            SLOT(receiveStatus(const char *, PacketHandler::ConnectionStatus)));

    //char *LCrStr = getenv("LIGHT_CRAFTER_GUI");
    char *LCrStr = getenv("LC2000 nHD_GUI");

    for(i = 0; i < ARRAY_SIZE(HWPtns); i++)
    {
        HWPtns[i].number = i & 0xF;
        HWPtns[i].invert = i >> 4;
    }

    camImage = NULL;
    camImageData = NULL;



    // Display GUI Version #
    sprintf(versionStr, "Keynote Photonics LC2000 nHD Control Software - v%d.%d.%d %s", GUI_V_MAJOR, GUI_V_MINOR,GUI_V_BUILD,GUI_V_OPTIONAL);
    setWindowTitle(versionStr);

    ptnImagePath = settings.value("PtnImagePath", "").toString();
    imagePath = settings.value("ImagePath", "").toString();
    seqFilePath = settings.value("SeqFilePath", "").toString();
    readmeFilePath = settings.value("readmeFilePath", "").toString();
    imagelistFilePath = settings.value("imagelistFilePath", "").toString();
    ui->IPtext->setText(settings.value("TargetIP", "192.168.7.2").toString());

    ui->startSeqBut->setEnabled(0);
    ui->stopSeqBut->setEnabled(0);
    ui->nextSeqBut->setEnabled(0);

    ui->modeList->setCurrentIndex(2);
    ui->tabWidget->setCurrentIndex(0);

    //ui->GuiVersion->setText(versionStr);

    ui->trigType->setCurrentIndex(1);
    ui->newIPText->setEnabled(false);

    ui->radioVideoMode->setChecked(1);
    ui->groupBox_VideoStreaming->setEnabled(1);
    ui->groupBox_ExtPatSettings->setEnabled(0);
    ui->xResInput_Pat->setEnabled(0);
    ui->yResInput_Pat->setEnabled(0);
    ui->extPatPatternRate->setEnabled(0);
    ui->extPatPatperFrame->setEnabled(0);

    ui->MBMCReadme_Exposure->setEnabled(false);
    ui->MBMCReadme_MinTriggerPeriod->setEnabled(false);
    ui->MBMCReadme_Patterns->setEnabled(false);
    ui->MBMCSeqStartVec->setEnabled(false);
    ui->MBMCSeqNumVec->setEnabled(false);

    emit on_extPatLEDSelect_currentIndexChanged(0);
    emit on_bitDepth_currentIndexChanged(0);
    emit on_enableCustSeq_clicked(0);

    fillNumPatterns();

    // Hide tabs not needed for release version of the tool
    // When ENV variable LC2000 nHD_GUI=KP_ENABLE_SPECIAL_TOOLS or KP_ENABLE_DEBUG, enable some
    //if((LCrStr == NULL || strcmp(LCrStr, "TI_INTERNAL")) && (!DEBUG_MODE) )
    if (1)
    {
    //    index = ui->tabWidget->indexOf(ui->tabDebug);
    //    if(index > 0) ui->tabWidget->removeTab(index);
   //     ui->rotate->setVisible(false);

        index = ui->tabWidget->indexOf(ui->tabSpectro);
        if(index > 0) ui->tabWidget->removeTab(index);


        index = ui->tabWidget->indexOf(ui->tabCamera);
        if(index > 0) ui->tabWidget->removeTab(index);

        index = ui->tabWidget->indexOf(ui->tabVideo);
        ui->tabWidget->removeTab(index);


    }
    else if(!strcmp(LCrStr, "KP_ENABLE_DEBUG"))
    {


        index = ui->tabWidget->indexOf(ui->tabCamera);
        if(index > 0) ui->tabWidget->removeTab(index);
        ui->rotate->setVisible(false);
    }
    else if(!strcmp(LCrStr, "KP_ENABLE_SPECIAL_TOOLS"))
    {
        ui->rotate->setVisible(true);
    } else {
        index = ui->tabWidget->indexOf(ui->tabDebug);
        if(index > 0) ui->tabWidget->removeTab(index);
        ui->rotate->setVisible(false);



        index = ui->tabWidget->indexOf(ui->tabCamera);
        if(index > 0) ui->tabWidget->removeTab(index);
        ui->rotate->setVisible(false);
    }

    extendedMode = true;
    CalDataSet = false;
    splashLayoutError=false;

}

MainWindow::~MainWindow()
{
    settings.setValue("PtnImagePath", ptnImagePath);
    settings.setValue("ImagePath", imagePath);
    settings.setValue("SeqFilePath", seqFilePath);
    settings.setValue("readmeFilePath", readmeFilePath);
    settings.setValue("imagelistFilePath", imagelistFilePath);
    settings.setValue("TargetIP", ui->IPtext->text());
    settings.setValue("TabIndex", ui->tabWidget->currentIndex());
    delete ui;
}

/*!
 * \brief MainWindow::on_connectButton_clicked
 * \n Connect to / Disconnect from the board.
 */
void MainWindow::on_connectButton_clicked()
{
    bool enable;

    if(packetHandler.connectionStatus() == PacketHandler::CON_STATUS_CONNECTED)
        enable = false;                             //Disconnect
    else
    {
        enable = true;                              //Connect
        packetHandler.setIP(ui->IPtext->text());    //Default = 192.168.1.100
    }
    packetHandler.connectToBoard(enable);
}



/*!
 * \brief MainWindow::on_dppGetButton_clicked
 * \n Reads the value of DLPC300 register.
 */
void MainWindow::on_dppGetButton_clicked()
{
    quint8 regAddr = strToNum(ui->dppRegAddress->text());

    ui->dppRegAddress->setText(numToStr(regAddr, 2, 1));

    //Command : 0xff 0x00 -> Reads DLPC300 register
    if(packetHandler.readCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(regAddr, 1))
        return;

    //Send the packet with command to read DLPC300 register and register address as payload
    if(packetHandler.sendPacket())
        return;

    //Should return 4-byte value of DLPC300 register if the command is successful
    if(packetHandler.responseSize() != 4)
        return;

    quint32 regVal = packetHandler.getInt(4);

    ui->dppRegValue->setText(numToStr(regVal, 8, 1));
}

/*!
 * \brief MainWindow::on_dppSetButton_clicked
 * \n Sets the value of DLPC300 register.
 */
void MainWindow::on_dppSetButton_clicked()
{
    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(strToNum(ui->dppRegAddress->text()), 1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData(strToNum(ui->dppRegValue->text()), 4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_FPGAGetButton_clicked
 * \n Get the value of the FPGA register.
 * \n Command 0xff 0x03
 */
void MainWindow::on_FPGAGetButton_clicked()
{
    unsigned regAddr = strToNum(ui->FPGARegAddress->text());

    ui->FPGARegAddress->setText(numToStr(regAddr, 4, 1));

    //command to read FPGA reg value
    if(packetHandler.readCommand(0xFF03))
        return;

    //send the FPGA register address
    if(packetHandler.putData(regAddr, 2))
        return;

    if(packetHandler.sendPacket())
        return;

    //response contains the 4-byte FPGA register value
    if(packetHandler.responseSize() != 4)
        return;

    quint32 regVal = packetHandler.getInt(4);

    ui->FPGARegValue->setText(numToStr(regVal, 8, 1));

}

/*!
 * \brief MainWindow::on_FPGASetButton_clicked
 * \n Sets the FPGA register value
 * \n Command 0xff 0x03
 */
void MainWindow::on_FPGASetButton_clicked()
{
    //command to set the FPGA reg value
    if(packetHandler.writeCommand(0xFF03))
        return;

    //send the address of the FPGA register
    if(packetHandler.putData(strToNum(ui->FPGARegAddress->text()), 2))
        return;

    //send the value to be written to FPGA register
    if(packetHandler.putData(strToNum(ui->FPGARegValue->text()), 4))
        return;

    if(packetHandler.sendPacket())
        return;
}

/*!
 * brief MainWindow::on_dmGetBut_clicked
 * \n Gets the value of DM365 register value.
 * \n Command 0xff 0x01
 */
void MainWindow::on_dmGetBut_clicked()
{
    quint32 dmValue;
    quint32 dmAddr;

    dmAddr = strToNum(ui->dmAddr->text());

    ui->dmAddr->setText(numToStr(dmAddr, 8, 1));

    //send command to read DM365 register value
    if(packetHandler.readCommand(0xFF01))
        return;

    //send the address of the DM365 register in the payload
    if(packetHandler.putData(dmAddr, 4))
        return;

    if(packetHandler.sendPacket())
        return;

    //Get the 4-byte value of the DM365 register
    if(packetHandler.responseSize() != 4)
        return;

    dmValue = packetHandler.getInt(4);

    ui->dmValue->setText(numToStr(dmValue, 8, 1));
}

/*!
 * \brief MainWindow::on_dmSetBut_clicked
 * \n Set the value of DM365 register.
 * \n Command 0xff 0x01
 */
void MainWindow::on_dmSetBut_clicked()
{
    quint32 dmValue;
    quint32 dmAddr;

    dmAddr = strToNum(ui->dmAddr->text());
    dmValue = strToNum(ui->dmValue->text());

    ui->dmAddr->setText(numToStr(dmAddr, 8, 1));
    ui->dmValue->setText(numToStr(dmValue, 8, 1));

    //send the command to set the value of DM365 register
    if(packetHandler.writeCommand(0xFF01))
        return;

    //send the address of the DM365 register
    if(packetHandler.putData(dmAddr, 4))
        return;

    //send the value to be written to the DM365 register.
    if(packetHandler.putData(dmValue, 4))
        return;

    if(packetHandler.sendPacket())
        return;
}

////////////////////////////////////////////////////////////////////////////////////////////////
/// TAB: Debug - misc. commands
////////////////////////////////////////////////////////////////////////////////////////////////

/*!
 * \brief MainWindow::on_pb_RunFrameBufferMemTest_clicked
 * \n Push Button to issue the DM365 command to execute the Frame Buffer memory test.
 * \n Command 0xFF 0x20
 */
void MainWindow::on_pb_RunFrameBufferMemTest_clicked()
{
    quint8 ErrorCode;

    if(packetHandler.writeCommand(0xFF20))
        return;

    if(packetHandler.sendPacket())
        return;

    if(packetHandler.responseSize() != 1)
        return;

    ErrorCode = packetHandler.getInt(1);

}

/*!
 * \brief MainWindow::on_pb_DM365MemTestSet_clicked
 * \n Loads the DM365 setting for Frame buffer memory test from the check box value
 * \n Command 0xFF 0x21
 */
void MainWindow::on_pb_DM365MemTestSet_clicked()
{
    if(packetHandler.writeCommand(0xFF21))
        return;

    if (ui->checkBox_DM365MemTestStatus->isChecked())
        packetHandler.putData(1, 1);
    else  packetHandler.putData((quint32)0, 1);

    if(packetHandler.sendPacket())
        return;
}


/*!
 * \brief MainWindow::on_pb_DM365MemTestGet_clicked
 * \n Reads the current DM365 setting for Frame buffer memory test ands loads it into the check box setting.
 * \n Command 0xFF 0x21
 */
void MainWindow::on_pb_DM365MemTestGet_clicked()
{
    if(packetHandler.readCommand(0xFF21))
        return;

    if(packetHandler.sendPacket())
        return;

    if(packetHandler.responseSize() != 1)
        return;

    if (packetHandler.getInt(1) == 1)
        ui->checkBox_DM365MemTestStatus->setChecked(true);
    else
        ui->checkBox_DM365MemTestStatus->setChecked(false);
}


/*!
 * \brief MainWindow::on_pb_ResetFPGA_clicked
 * \n Push Button to issue the DM365 command to reset the FPGA.
 */
void MainWindow::on_pb_ResetFPGA_clicked()
{
    quint8 ErrorCode;

    if(packetHandler.writeCommand(0xFF22))
        return;

    if(packetHandler.sendPacket())
        return;

    if(packetHandler.responseSize() != 1)
        return;

    ErrorCode = packetHandler.getInt(1);

}

////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 * \brief MainWindow::on_patternGetSetting_clicked
 * \n Reads pattern sequence setting.
 * \n Command : 0x04 0x00
 * \n           0x04 0x80 [extended mode - supported in DM365 firmware version 3.0 or greater]
 */
void MainWindow::on_patternGetSetting_clicked()
{
    int bitDepth;
    int numPtns;

    //Command to read pattern sequence setting (0x04 0x00)
    //or extended pattern seq. setting (0x04 0x80)
    if(packetHandler.readCommand(extendedMode ? 0x0480 : 0x0400))
        return;

    if(packetHandler.sendPacket())
        return;

    bitDepth = packetHandler.getInt(1);
    if(bitDepth > 0)
    {
        ui->bitDepth->setCurrentIndex(bitDepth - 1);
        emit on_bitDepth_currentIndexChanged(bitDepth - 1);
    }

    numPtns = packetHandler.getInt(extendedMode ? 2 : 1);

    ui->PatternType->setCurrentIndex(packetHandler.getInt(1));

    ui->trigType->setCurrentIndex(packetHandler.getInt(1));

    ui->trigDelay->setText(getTextIntData(4));

    ui->trigPeriod->setText(getTextIntData(4));

    ui->exposureTime->setText(getTextIntData(4));

    ui->LEDSelect->setCurrentIndex(packetHandler.getInt(1));

    if(extendedMode)
    {
        ui->playCount->setChecked(packetHandler.getInt(1));
    }

    if(ui->LEDSelect->currentIndex()== LEDSelect_MULTIPLE)
    {
        ui->enableCustSeq->setChecked(true);
        emit on_enableCustSeq_clicked(true);

        // Get the vector information
        if(packetHandler.readCommand(0x0A01))
            return;

        if(packetHandler.sendPacket())
            return;

        QString startVector = getTextIntData(1);
        QString numVectors = getTextIntData(1);

        ui->MBMCSeqStartVec->setText(startVector);
        ui->MBMCSeqNumVec->setText(numVectors);

        // Mark file names as __stored__
        ui->MBMCImageList->setText("__stored__");
        ui->MBMCReadmeFile->setText("__stored__");
        ui->MBMCSeqFile->setText("__stored__");

    }
    else
    {
        ui->enableCustSeq->setChecked(false);
        emit on_enableCustSeq_clicked(false);
    }

    int index = ui->numPatterns->findText(numToStr(numPtns));
    if(numPtns > 0 && index >= 0)
    {
        ui->numPatterns->setCurrentIndex(index);
    }
    else
    {
        ui->numPatterns->setEditText(numToStr(numPtns));
    }

    if(numPtns > 0 && bitDepth > 0)
    {
        bool playing = checkPlaying();
        fillNumPatterns();

        ui->startSeqBut->setEnabled(!playing);
        ui->stopSeqBut->setEnabled(playing);
        ui->nextSeqBut->setEnabled(playing && ((ui->trigType->currentIndex() == 0) || (ui->trigType->currentIndex() == 6)));
    }


}

bool MainWindow::checkPlaying(void)
{
    static bool failed = 0;
    if(failed || extendedMode == 0)
        return 0;

    //Command 0x04 0x02 -> Checks the display of currently defined pattern sequence
    if(packetHandler.readCommand(0x0402))
    {
        failed = 1;
        return 0;
    }

    if(packetHandler.sendPacket())
    {
        failed = 1;
        return 0;
    }

    return packetHandler.getInt(1); // 1 ->playing , 0 -> not playing

}

/*!
 * \brief MainWindow::on_patternSetSetting_clicked
 * \n Defines pattern sequence setting
 * \n Command : 0x04 0x00
 * \n           0x04 0x80 [extended mode - supported in DM365 firmware version 3.0 or greater]
 */
void MainWindow::on_patternSetSetting_clicked()
{    
    // Check that lightcrafter is in correct mode
    if(ui->modeList->currentIndex() != 3)
    {
        showError("Please set the LC2000 nHD's Display Mode to 'Stored Pattern Sequence'");
        return;
    }

    // If in MBMC mode check that exposure and trigger meet minimum requirement
    if(ui->enableCustSeq->isChecked())
    {
        int         error = 0;
        QFileInfo   file;
        QString     warningMBMC("MBMC Settings Invalid\n");

        // Check that sequence file has been selected
        file.setFile(ui->MBMCSeqFile->text());
        if((!file.exists()) && (ui->MBMCSeqFile->text() != "__stored__"))
        {
            error = 1;
            warningMBMC.append("MBMC Sequence File was not selected\n");
        }

        // Check that readme file has been selected
        file.setFile(ui->MBMCReadmeFile->text());
        if((!file.exists()) && (ui->MBMCReadmeFile->text() != "__stored__"))
        {
            error = 1;
            warningMBMC.append("MBMC Readme File was not selected\n");
        }

        // Check that image list file has been selected
        file.setFile(ui->MBMCImageList->text());
        if((!file.exists()) && (ui->MBMCImageList->text() != "__stored__"))
        {
            error = 1;
            warningMBMC.append("MBMC Image List File was not selected\n");
        }

        if((ui->exposureTime->text().toInt() < ui->MBMCReadme_Exposure->text().toInt()) &&
           (ui->MBMCReadmeFile->text() != "__stored__"))
        {
            error = 1;
            warningMBMC.append("The minimum exposure time is "+ ui->MBMCReadme_Exposure->text()+"\n");
        }

        if(((ui->trigPeriod->text().toInt() < ui->MBMCReadme_MinTriggerPeriod->text().toInt()) &&
            (ui->trigType->currentIndex() == 1)) &&
            (ui->MBMCReadmeFile->text() != "__stored__"))
        {
            error = 1;
            warningMBMC.append("The minimum trigger time is "+ ui->MBMCReadme_MinTriggerPeriod->text()+"\n");
        }

        if(error==1)
        {
            QString title("LC2000 nHD Error Message");
            QMessageBox msgBox(QMessageBox::Warning, title, warningMBMC, QMessageBox::NoButton, this);
            msgBox.exec();
          //  return;
        }
  //      else
        {
            // MBMC settings are valid
            if(ui->MBMCSeqFile->text() != "__stored__")
            {
                // Send sequence file
                if(packetHandler.writeCommand(0x0A00))  return;
                if(sendFile(ui->MBMCSeqFile->text()))   return;
            }

            // Send vector information
            if(packetHandler.writeCommand(0x0A01))              return;
            if(putTextIntData(ui->MBMCSeqStartVec->text(), 1))  return;
            if(putTextIntData(ui->MBMCSeqNumVec->text(), 1))    return;
            if(packetHandler.sendPacket())return;

        }

    }
    else if( ui->LEDSelect->currentIndex()== LEDSelect_MULTIPLE )
    {
        // MBMC is not enabled but multiple leds is selected
        showError("MBMC must be enabled for LED Select 'Multiple' setting to be valid");
        return;
    }

    //Command to define pattern sequence setting (0x04 0x00)
    //or extended pattern seq. setting (0x04 0x80)
    if(packetHandler.writeCommand(extendedMode ? 0x0480 : 0x0400))
        return;

    //Payload

    //pattern bit depth
    if(packetHandler.putData(ui->bitDepth->currentIndex() + 1, 1))
        return;

    //Number of patterns in the sequence (1-96) [(1-1000) for extended]
    if(putTextIntData(ui->numPatterns->currentText(), extendedMode ? 2 : 1))
        return;

    //pattern type : 0 -> display every pattern ; 1 -> display every pattern followed by inverted pattern
    if(packetHandler.putData(ui->PatternType->currentIndex(), 1))
        return;

    //Input trigger type
    if(packetHandler.putData(ui->trigType->currentIndex(), 1))
        return;

    //Input Trigger Delay in micro seconds
    if(putTextIntData(ui->trigDelay->text(), 4))
        return;

    //Trigger Period in micro second (only for Auto Trigger mode)
    if(ui->trigPeriod->isEnabled())
    {
        if(putTextIntData(ui->trigPeriod->text(), 4))
            return;
    }
    else
    {
        if(packetHandler.putData((quint32)0, 4))
            return;
    }

    //Exposure time in micro seconds
    if(putTextIntData(ui->exposureTime->text(), 4))
        return;

    //LED select 0->red, 1->green, 2->blue, 3->MBMC

    qint8 mask;

    switch (ui->LEDSelect->currentIndex())
    {
        case 0: //red
            mask = 1;
            break;
        case 1: //green
            mask = 0x2;
        break;
        case 2: //blue
            mask = 0x4;
            break;
        case 3: //white
            mask = 0x7;
            break;
        case 4: //cyan
            mask = 0x6;
            break;
        case 5: //yellow
            mask = 0x3;
            break;
        case 6: //magenta
            mask = 0x4;
            break;
        case 7: // mbmc ??
            mask = 0x8;
            break;
        }

    if(packetHandler.putData(mask, 1))
        return;

    if(extendedMode)
    {
        /* Extended mode */
        if(packetHandler.putData(ui->playCount->isChecked(), 1))
            return;
    }

    if(packetHandler.sendPacket())
        return;

    ui->startSeqBut->setEnabled(1);
    ui->stopSeqBut->setEnabled(0);
    ui->nextSeqBut->setEnabled(0);

    fillNumPatterns();

}

void MainWindow::showError(const char *str)
{
    QString title("LightCrafter Error Message");
    QString text(str);
    QMessageBox msgBox(QMessageBox::Warning, title, text, QMessageBox::NoButton, this);
    msgBox.exec();
}

void MainWindow::showVerMismatchwarning(QString titleName, QString str)
{
    QMessageBox msgBox(this);
    msgBox.setWindowTitle(titleName);
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
//    msgBox.setText(str+QString("<a href=http://www.ti.com/tool/DLPLIGHTCRAFTER>visit website</a>"));
    msgBox.setText(str+QString("<a href=https://keynotephotonics.filecamp.com/files/24ap-1a932kdc/24ap-1a932kdc>Click Here for Link to Website</a>"));
    msgBox.exec();
}

/*!
 * \brief MainWindow::receiveStatus
 * \n Check the connection status and populate the related controls.
 * \param str
 * \param status Current connection status
 */
void MainWindow::receiveStatus(const char *str, PacketHandler::ConnectionStatus status)
{
    ui->statusBar->showMessage(str);


    if(status == PacketHandler::CON_STATUS_CONNECTED)
    {
        ui->IPtext->setEnabled(false);
        ui->newIPText->setEnabled(true);
        ui->connectButton->setText("Disconnect");
        // Populate version #s
        emit on_versionGetBut_clicked();
        // Populate LED currents
        emit on_LEDCurGet_clicked();
        // Populate display mode
        emit on_dispModeGetBut_clicked();
        // Populate image setting
        emit on_imageSetGetBut_clicked();
    }
    else if(status == PacketHandler::CON_STATUS_NOT_CONNECTED)
    {
        ui->IPtext->setEnabled(true);
        ui->newIPText->setEnabled(false);
        ui->connectButton->setText("Connect");
        // X out version #s
        ui->DM365Version->setText("XXX.XXX");
        ui->FPGAVersion->setText("XXX.XXX");
        ui->MSPVersion->setText("XX.XX");
        ui->MSPBuild->setText(".XX");
        ui->MfgVersion->setText("XXXX");
        ui->ModelNumVersion->setText("XXXX");
        ui->LEDTemp->setText("XXX.X C");
        ui->SNumVersion->setText("XXXX");
        // Clear LED current settings
        ui->LEDCurRed->clear();
        ui->LEDCurGreen->clear();
        ui->LEDCurBlue->clear();
    }
    else if(status == PacketHandler::CON_STATUS_ERROR)
    {
        showError(str);
    }
}

/*!
 * \brief MainWindow::on_browseButton_clicked
 * \n Search for the files to upload.
 */
void MainWindow::on_browseButton_clicked()
{
    int patNum = ui->patternNum->currentIndex();
    QStringList fileNames;
    QString startPath;

    QString value = (ui->numPatterns->currentText()); //number of patterns USER selected
    int maxPatternNum = value.toInt();
    QString text;
    QString title;


    //SELECTING FILES
    if(patNum < 0)
        patNum = 0;
    if(patNum >= MAX_PATTERNS)
        patNum = MAX_PATTERNS - 1;

    if(patternFile[patNum].isFile())
        startPath = patternFile[patNum].absolutePath();
    else
        startPath = ptnImagePath;

    fileNames = QFileDialog::getOpenFileNames(this,
                                              QString("Select Image for Pattern : ") + numToStr(patNum),
                                              startPath,
                                              "*.bmp");
    fileNames.sort();

    //ASSIGNING EACH FILE A NUMBER
    for(int i = 0; i < fileNames.size() && i < MAX_PATTERNS; i++)
    {
        patternFile[patNum + i].setFile(fileNames.at(i));
    }

    if(fileNames.size())
    {
        on_patternNum_currentIndexChanged(0);
        ptnImagePath = patternFile[patNum].absolutePath();
    }

    //CHECKING NUMBER OF FILES SELECTED
    if((fileNames.size()) > maxPatternNum )
    {
        text = "Too many patterns selected (";
        text.append(QString("%1").arg(fileNames.size()));
        text = text + "). \nExtra patterns will be ignored.";
        title = "Warning";
        QMessageBox msgBox(QMessageBox::Warning, title, text, QMessageBox::NoButton, this);
        msgBox.exec();
    }
    else if((fileNames.size()) < maxPatternNum )
    {
        text = "Number of files selected is less than pattern count (";
        text.append(QString("%1").arg(maxPatternNum));
        text = text + ").";
        title = "Warning";
        QMessageBox msgBox(QMessageBox::Warning, title, text, QMessageBox::NoButton, this);
        msgBox.exec();
    }
}

/**
 * \brief MainWindow::sendHWPattern
 * \n Display HW pattern.
 */
int MainWindow::sendHWPattern(int start, int count)
{
    int i;

    setHWPattern();

    if(packetHandler.writeCommand(0x0406))
        return -1;

    if(packetHandler.putData(start, 1))
        return -1;

    if(packetHandler.putData(count, 1))
        return -1;

    for(i = start; i < count + start; i++)
    {
        if(packetHandler.putData(HWPtns[i].number, 1))
            return -1;
        if(packetHandler.putData(HWPtns[i].invert, 1))
            return -1;
    }

    if(packetHandler.sendPacket())
        return -1;

    return 0;
}

/*!
 * \brief MainWindow::on_uploadButton_clicked
 * \n Displays the patterns from the pattern sequence.
 *
 * \n Command 0x04 0x01
 * \n         0x04 0x81 - [extended mode]
 */
void MainWindow::on_uploadButton_clicked()
{
    int i;
    int count;

    // Disable button until upload is complete
    ui->uploadButton->setEnabled(false);
    ui->uploadThisButton->setEnabled(false);
    ui->displayThisButton->setEnabled(false);

    count = ui->patternNum->count();

    /* if HW Pattern upload */
    if(ui->PatternType->currentIndex() == 2)
    {
        sendHWPattern(0, count);
    }
    else
    {
        for(i = 0; i < count; i++)
        {
            if(i >= MAX_PATTERNS)
                break;

            if(!patternFile[i].exists())
                break;

            //Define one pattern from the pattern sequence
            //Command : 0x04 0x01
            //          0x04 0x81 [extended mode]
            if(packetHandler.writeCommand(extendedMode ? 0x0481 : 0x0401))
                break;

            //Pattern number
            else if(packetHandler.putData(i, extendedMode ? 2 : 1))
                break;

            if(extendedMode)
            {
                //Column position and Row position to display this pattern within DMD
                if(packetHandler.putData((quint32)0, 4))
                    break;
            }

            //Pattern bitmap in Windows BMP format
            if(sendFile(patternFile[i].absoluteFilePath()))
                break;
        }
    }

    // Enable button after upload is complete
    ui->displayThisButton->setEnabled(true);
    ui->uploadThisButton->setEnabled(true);
    ui->uploadButton->setEnabled(true);
}

/*!
 * \brief MainWindow::on_dispModeGetBut_clicked
 * \n Reads the current display mode.
 *
 * \n 01 - static image
 * \n 02 - internal test pattern
 * \n 03 - HDMI video input
 * \n 04 - reserved
 * \n 05 - pattern sequence display
 */
void MainWindow::on_dispModeGetBut_clicked()
{
    int modeNum;
    int modeNumLookup[] = { 0, 1 , 2, 0, 3 };

    //Command 0x01 0x01 reads the current display mode
    if(packetHandler.readCommand(0x0101))
        return;

    if(packetHandler.sendPacket())
        return;

    //response contains 1-byte value of current display mode
    if(packetHandler.responseSize() != 1)
        return;

    modeNum = packetHandler.getInt(1);

    if(modeNum < 0 || modeNum > ARRAY_SIZE(modeNumLookup))
        return;

    ui->modeList->setCurrentIndex(modeNumLookup[modeNum]);
    setTab( modeNum );
}

/*!
 * \brief MainWindow::on_dispModeSetBut_clicked
 * \n Sets the current display mode.
 */
void MainWindow::on_dispModeSetBut_clicked()
{
    int modeNumLookup[] = { 0, 1, 2, 4 }; // 0->static image ; 1->internal test pattern ; 2->HDMI video input ; 4->pattern sequence display

    int modeNum = ui->modeList->currentIndex();

    if(modeNum < 0 || modeNum > ARRAY_SIZE(modeNumLookup))
        return;

    modeNum = modeNumLookup[modeNum];

    if(modeNum==4)
    {
        // Requires 608x684 because this is a pixel accurate mode
        // Send resolution, active pixels, and active lines
        if(packetHandler.writeCommand(0x0200))
            return;
        if(putTextIntData(QString::number(608), 2))
            return;
        if(putTextIntData(QString::number(684), 2))
            return;
        if(putTextIntData(QString::number(0), 2))
            return;
        if(putTextIntData(QString::number(0), 2))
            return;
        if(putTextIntData(QString::number(608), 2))
            return;
        if(putTextIntData(QString::number(684), 2))
            return;
        if(packetHandler.sendPacket())
            return;
    }


    //Command 0x01 0x01 sets the current display mode
    if(packetHandler.writeCommand(0x0101))
        return;

    //The mode to be displayed
    if(packetHandler.putData(modeNum, 1))
        return;

    if(packetHandler.sendPacket())
        return;

    setTab( modeNum );
}

/*!
 * \brief MainWindow::on_imageSetGetBut_clicked
 * \n Reads the current display settings.
 * \n Command 0x01 0x07
 */
void MainWindow::on_imageSetGetBut_clicked()
{
    if(packetHandler.readCommand(0x0107))
        return;

    if(packetHandler.sendPacket())
        return;

    if(packetHandler.responseSize() != 3)
        return;
    ui->longAxisFlip->setChecked(packetHandler.getInt(1));
    ui->shortAxisFlip->setChecked(packetHandler.getInt(1));
    ui->rotate->setChecked(packetHandler.getInt(1));
}

/*!
 * \brief MainWindow::on_imageSetSetBug_clicked
 * \n Sets the current display settings.
 * \n Command 0x01 0x07
 */
void MainWindow::on_imageSetSetBug_clicked()
{
    if(packetHandler.writeCommand(0x0107))
        return;

    if(packetHandler.putData(ui->longAxisFlip->isChecked(), 1))
        return;
    if(packetHandler.putData(ui->shortAxisFlip->isChecked(), 1))
        return;
    if(packetHandler.putData(ui->rotate->isChecked(), 1))
        return;

    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_testPtnGetBut_clicked
 * \n Reads the currently selected Internal Test Pattern
 * \n Command 0x01 0x03
 */
void MainWindow::on_testPtnGetBut_clicked()
{
    if(packetHandler.readCommand(0x0103))
        return;

    if(packetHandler.sendPacket())
        return;

    if(packetHandler.responseSize() != 1)
        return;

    ui->testPtnList->setCurrentIndex(packetHandler.getInt(1));
}

/*!
 * \brief MainWindow::on_testPtnSetBut_clicked
 * \n Sets the current Internal Test Pattern
 * \n Command 0x01 0x03
 */
void MainWindow::on_testPtnSetBut_clicked()
{    
    // Check that lightcrafter is in correct mode
    if(ui->modeList->currentIndex() != 1)
    {
        showError("Please set the LC2000 nHD's Display Mode to 'Test Pattern'");
        return;
    }

    if(packetHandler.writeCommand(0x0103))
        return;

    if(packetHandler.putData(ui->testPtnList->currentIndex(), 1))
        return;

    if(packetHandler.sendPacket())
        return;
}



/*!
 * \brief MainWindow::on_GetTempBut_clicked
 * \n Reads the Temperature of the LED from the LC2000 nHD.
 */
void MainWindow::on_GetTempBut_clicked()
{
    // For now just do the same thing as Populate version #s
//    emit on_versionGetBut_clicked();

    quint8 temperatureData[32];                         // Data read from target
    char temperatureStr[32];                            // Data presented to user on GUI

//    int j;

    int TempCode;
    char TempValueStr[32];

    float TempValue = -1;
    double TempScale = 1;
    int TempCode2;
    int TempCodeBase = 1024;

    // Get DM365 Version #
    if(packetHandler.readCommand(0x0100))
        return;

    if(packetHandler.putData(0x70, 1))                  // Set command for Temperature reading
        return;

    if(packetHandler.sendPacket())
        return;

    if(packetHandler.getBytes(temperatureData, 32))         //get the temperature string
        return;

    //Convert ASCI to Decimal
    TempCode = (temperatureData[0] - 48) * 100 + (temperatureData[1]-48)*10 + temperatureData[2]-48;

    if (TempCode >= T_DAC_1)
      { TempScale =  T_Scale1; TempCode2 = T_DAC_0; TempCodeBase = T_DAC_1;}
    else if (TempCode >= T_DAC_2)
      { TempScale = T_Scale2; TempCode2 = T_DAC_1; TempCodeBase = T_DAC_2;}
    else if (TempCode >= T_DAC_3)
      { TempScale = T_Scale3; TempCode2 = T_DAC_2; TempCodeBase = T_DAC_3;}
    else if (TempCode >= T_DAC_4)
      { TempScale = T_Scale4; TempCode2 = T_DAC_3; TempCodeBase = T_DAC_4;}
    else if (TempCode >= T_DAC_5)
      { TempScale = T_Scale5; TempCode2 = T_DAC_4; TempCodeBase = T_DAC_5;}
    else if (TempCode >= T_DAC_6)
      { TempScale = T_Scale6; TempCode2 = T_DAC_5; TempCodeBase = T_DAC_6;}
    else if (TempCode >= T_DAC_7)
      { TempScale = T_Scale7;TempCode2 = T_DAC_6; TempCodeBase = T_DAC_7;}
    else if (TempCode >= T_DAC_8)
      { TempScale = T_Scale8;TempCode2 = T_DAC_7; TempCodeBase = T_DAC_8;}
    else if (TempCode >= T_DAC_9)
      { TempScale = T_Scale9;TempCode2 = T_DAC_8; TempCodeBase = T_DAC_9;}
    else if (TempCode >= T_DAC_A)
      { TempScale = T_ScaleA;TempCode2 = T_DAC_9; TempCodeBase = T_DAC_A;}
    else if (TempCode >= T_DAC_B)
      { TempScale = T_ScaleB;TempCode2 = T_DAC_A; TempCodeBase = T_DAC_B;}
    else if (TempCode >= T_DAC_C)
      { TempScale = T_ScaleC;TempCode2 = T_DAC_B; TempCodeBase = T_DAC_C;}
    else if (TempCode >= T_DAC_D)
      { TempScale = T_ScaleD; TempCode2 = T_DAC_C; TempCodeBase = T_DAC_D; }
    else if (TempCode >= T_DAC_E)
      { TempScale = T_ScaleE; TempCode2 = T_DAC_D; TempCodeBase = T_DAC_E;}
    else if (TempCode >= T_DAC_F)
      { TempScale = T_ScaleF; TempCode2 = T_DAC_E; TempCodeBase = T_DAC_F;}
    else if (TempCode >= T_DAC_G)
      { TempScale = T_ScaleG; TempCode2 = T_DAC_F; TempCodeBase = T_DAC_G;}
    else if (TempCode >= T_DAC_H)
      { TempScale = T_ScaleH ; TempCode2 = T_DAC_G; TempCodeBase = T_DAC_H;}

    TempValue =  TempCode/TempScale + (TempCode - TempCodeBase)/TempScale;
    if (TempCode == 0)
        TempValue = 0.0;
    sprintf(TempValueStr,"%3.1f C",TempValue);
//            sprintf_s(TempValueStr, "%0x", TempCode); // for debug

    for (int j = 0; j < 32; j++)
    {
        temperatureStr[j] = TempValueStr[j];
    }

    ui->LEDTemp->setText(temperatureStr);

}




/*!
 * \brief MainWindow::on_versionGetBut_clicked
 * \n Reads the LightCrafter software and firmware versions.
 * \n Displays warning in case of version mismatch.
 * \n Command 0x10 0x00
 */
void MainWindow::on_versionGetBut_clicked()
{
    quint8 versionData[32];
    char versionStr[32];
    QLabel *verLables[] = {ui->DM365Version, \
                            ui->FPGAVersion, \
                            ui->MSPVersion, \
                            ui->MfgVersion, \
                            ui->ModelNumVersion, \
                            ui->SNumVersion, \
//                            ui->LEDTemp,
                            ui->MSPBuild };
    quint8 verTypes[] = { 0x00,    /* DM365 SW version*/  \
                           0x10,    /* FPGA Firmware Revision*/  \
                           0x20,    /* MSP430 SW Revision*/  \
                           0x30,    /* [KN] Manufacturer  */ \
                           0x40,    /* [KN] Model */ \
                           0x50,    /* [KN] Serial Number */ \
//                           0x70,    /* [KN] LED Temperature */
                           0x80     /* [KN] MSP Build */ };
    int i,j;

    for(i = 0; i < ARRAY_SIZE(verLables); i++)
    {
        // Get DM365 Version #
        if(packetHandler.readCommand(0x0100))
            return;

        if(packetHandler.putData(verTypes + i, 1))
            return;

        if(packetHandler.sendPacket())
            return;

        if(packetHandler.getBytes(versionData, 32)) //get the revision string
            return;

        for(j = 0; j < 32; j++)
        {
            versionStr[j] = versionData[j];
        }
        verLables[i]->setText(versionStr);
    }


    QString verDM365 = ui->DM365Version->text();
    QString verFPGA = ui->FPGAVersion->text();
    QString verMSP430 = ui->MSPVersion->text();
    QString verMSP430BldStr = ui->MSPBuild->text();

    //Compare for the version mismatch
    QString tempString;
    QByteArray tempByteArray;
    const char *tempStr;
    bool skipWarningMsg = VERSION_MSG_SKIP_DEFAULT;



    ////////////////////////////////////////////////////////////////////////////////////////////
    // Calculate version numbers for KP Version checking options
    ////////////////////////////////////////////////////////////////////////////////////////////
    verDM365Major = verDM365.split(".").at(0).toInt();
    verDM365Minor = verDM365.split(".").at(1).toInt();
    if(verDM365.split(".").at(2).endsWith('X'))                      // account for internal build versions ending in 'x'
    {
//        showError("Internal ('x') DM365 Build version detected.");
        verDM365Build = verDM365.split(".").at(2).left(verDM365.split(".").at(2).length()-1).toInt();
    } else {
//        showError("Normal DM365 Build version detected.");
        verDM365Build = verDM365.split(".").at(2).toInt();
    }
    verMSP430Major = verMSP430.split(".").at(0).right(1).toInt();
    verMSP430Minor = verMSP430.right(verMSP430.length()-verMSP430.split(".").at(0).length()-1).toInt();
    verMSP430Build = verMSP430BldStr.right(verMSP430BldStr.length()-1).toInt();
    char TempStrFormattedPart[10];

    // Check if there was a version mismatch
    if(!skipWarningMsg)
    {
        tempString = "";
//        if(verDM365.compare(LATEST_PEM_VER) != 0)
        if((verDM365Major < LATEST_PEM_MAJOR_VER)         ||
           ((verDM365Major == LATEST_PEM_MAJOR_VER) &&
            (verDM365Minor < LATEST_PEM_MINOR_VER)     )  ||
           ((verDM365Major == LATEST_PEM_MAJOR_VER) &&
            (verDM365Minor == LATEST_PEM_MINOR_VER) &&
            (verDM365Build < LATEST_PEM_BUILD_VER)     )  )
        {
            tempString.append("The PEM firmware version detected ");
            sprintf(TempStrFormattedPart,"(%d.%d.%d)",verDM365Major,verDM365Minor,verDM365Build);
            tempString.append(QString(TempStrFormattedPart));
            tempString.append(" is out of date!!!<br> Recommend updating to the latest version.<br><br>");
        }

//        if(verMSP430.compare(LATEST_MSP430_VER) != 0)
        if((verMSP430Major < LATEST_MSP430_MAJOR_VER)       ||
           ((verMSP430Major == LATEST_MSP430_MAJOR_VER) &&
            (verMSP430Minor < LATEST_MSP430_MINOR_VER)    ) ||
           ((verMSP430Major == LATEST_MSP430_MAJOR_VER) &&
            (verMSP430Minor == LATEST_MSP430_MINOR_VER) &&
            (verMSP430Build < LATEST_MSP430_BUILD_VER)     )  )
        {
            tempString.append("The MSP430 firmware version detected ");
            sprintf(TempStrFormattedPart,"(%d.%d.%d)",verMSP430Major,verMSP430Minor,verMSP430Build);
            tempString.append(QString(TempStrFormattedPart));
            tempString.append(" is out of date!!!<br> Recommend updating to the latest version.<br><br>");
//            tempString.append("The MSP430 firmware version is out of date!!!<br> Recommend updating to the latest version.<br><br>");
        }

        if(verFPGA.compare(LATEST_FPGA_VER) != 0)
        {
            tempString.append("FPGA firmware version mismatch!!! Please update to v");
            tempString.append(QString(LATEST_FPGA_VER));
            tempString.append(QString(" or newer.<br><br>"));
        }

        if( tempString != "")
        {
            tempString.append("Please download the latest DLP LC2000 nHD Firmware and Software Bundle");
//            tempString.append(QString(LATEST_LCR_FW_SF_PKG));
            tempString.append(" from the LC2000 nHD website in the Available Versions section. ");
            this->showVerMismatchwarning("Warning: LC2000 nHD Version Mismatch", tempString);
        }
    }

    if(verDM365Major >= 3)
        extendedMode = true;
    else
        extendedMode = false;

    ui->numPatterns->setEditable(extendedMode);
}


/*!
 * \brief MainWindow::on_DM365BuildVersionGetBut_clicked
 * \n Reads the LightCrafter software and firmware versions.
 * \n Displays warning in case of version mismatch.
 * \n Command 0x10 0x00
 */
void MainWindow::on_DM365BuildVersionGetBut_clicked()
{
    quint8 versionData[32];
    char versionStr[32];
    QLabel *verLable = ui->DM365_BuildVersion;
    quint8 verType =  0x08;    /* DM365 SW Build version*/

    int j;

        if(packetHandler.readCommand(0x0100))
            return;

        if(packetHandler.putData(verType, 1))
            return;

        if(packetHandler.sendPacket())
            return;

        if(packetHandler.getBytes(versionData, 32)) //get the revision string
            return;

           // AJK: Hack for Mac OS
       // verLable->setText(versionData);
}


/*!
 * \brief MainWindow::on_startSeqBut_clicked
 * \n Starts the display of currently defined pattern sequence.
 * \n Command 0x04 0x02
 */
void MainWindow::on_startSeqBut_clicked()
{
    if(packetHandler.writeCommand(0x0402))
        return;

    if(packetHandler.putData(1, 1)) //data byte 0 value = 1 (start the pattern sequence)
        return;

    if(packetHandler.sendPacket())
        return;

    ui->startSeqBut->setEnabled(0);
    ui->stopSeqBut->setEnabled(1);
    ui->nextSeqBut->setEnabled((ui->trigType->currentIndex() == 0) || (ui->trigType->currentIndex() == 6));
}

/*!
 * \brief MainWindow::on_stopSeqBut_clicked
 * \n Stops the display of currently defined pattern sequence.
 * \n Command 0x04 0x02
 */
void MainWindow::on_stopSeqBut_clicked()
{
    quint32 value = 0;
    if(packetHandler.writeCommand(0x0402))
        return;

    if(packetHandler.putData(value, 1)) //data byte 0 value = 0 (stop the pattern sequence)
        return;

    if(packetHandler.sendPacket())
        return;

    ui->startSeqBut->setEnabled(1);
    ui->stopSeqBut->setEnabled(0);
    ui->nextSeqBut->setEnabled(0);
}

/*!
 * \brief MainWindow::on_nextSeqBut_clicked
 * \n Advances the pattern to the next stored pattern.
 * \n (Valid only in SW trigger mode)
 * \n Command 0x04 0x03
 */
void MainWindow::on_nextSeqBut_clicked()
{
    if(packetHandler.writeCommand(0x0403))
        return;

    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_I2Cread_clicked
 * \n Reads from I2C interface.
 */
void MainWindow::on_I2Cread_clicked()
{
    //command to read from I2C interface
    if(packetHandler.readCommand(0xFF04))
        return;

    //send the I2C slave address
    if(putTextIntData(ui->I2CSlaveAddr->text(), 1))
        return;

    //number of bytes to be read
    if(putTextIntData(ui->I2CNumBytes->text(), 2))
        return;

    if(packetHandler.sendPacket())
        return;

    QString data;
    for(int i = 0; i < packetHandler.responseSize(); i++)
    {
        char str[5];
        sprintf(str, "%02X ", packetHandler.getInt(1));

        data.append(str);
    }

    ui->I2CData->setText(data);
}

/*!
 * \brief MainWindow::on_I2Cwrite_clicked
 * \n Writes data to I2C interface.
 */
void MainWindow::on_I2Cwrite_clicked()
{
    QString data;

    //command to write to I2C interface
    if(packetHandler.writeCommand(0xFF04))
        return;

    //send the I2C slave address
    if(putTextIntData(ui->I2CSlaveAddr->text(), 1))
        return;

    data = ui->I2CData->text();

    data.remove(QChar(' '));

    //send the data to be written
    for(int i = 0; i < data.size(); i += 2)
    {
        bool ok;
        packetHandler.putData(data.mid(i, 2).toInt(&ok, 16), 1);
    }

    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_LEDCurGet_clicked
 * \n Reads the red, green, and blue LED current values.
 * \n Command 0x01 0x04
 */
void MainWindow::on_LEDCurGet_clicked()
{
    if(packetHandler.readCommand(0x0104))
        return;

    if(packetHandler.sendPacket())
        return;

    ui->LEDCurRed->setText(getTextIntData(2)); //Red LED current value.
    ui->LEDCurGreen->setText(getTextIntData(2)); //Green LED current value.
    ui->LEDCurBlue->setText(getTextIntData(2)); //Blue LED current value.
}

/*!
 * \brief MainWindow::on_LEDCurSet_clicked
 * \n Sets the red, green, and blue LED current values.
 * \n (Range -> 0 – 1024 ; Default = 274)
 */
void MainWindow::on_LEDCurSet_clicked()
{
    // Get current settings
    int currentRed   = ui->LEDCurRed->text().toInt();
    int currentGreen = ui->LEDCurGreen->text().toInt();
    int currentBlue  = ui->LEDCurBlue->text().toInt();

    // Check that each LED setting is above 0. If it is, change to 1 to prevent oscillations in LED driver circuit.
    if (currentRed == 0 || currentGreen == 0 || currentBlue == 0 )
    {
        if (currentRed == 0)
        {
            currentRed = 1;
            ui->LEDCurRed->setText(QString::number(currentRed));
        }
        if (currentGreen == 0)
        {
            currentGreen = 1;
            ui->LEDCurGreen->setText(QString::number(currentGreen));
        }
        if (currentBlue == 0)
        {
            currentBlue = 1;
            ui->LEDCurBlue->setText(QString::number(currentBlue));
        }
        // Popup warning that 0 is not a valid value
        QMessageBox::warning(this,"LC2000 Warning Message","LED Setting of 0 not valid.\nValue changed to minimum value of 1.");
    }

    // Send command to LCr
    if(packetHandler.writeCommand(0x0104))
        return;

    if(putTextIntData(ui->LEDCurRed->text(), 2))
        return;

    if(putTextIntData(ui->LEDCurGreen->text(), 2))
        return;

    if(putTextIntData(ui->LEDCurBlue->text(), 2))
        return;

    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_staticImageBrowse_Button_clicked
 * \n Select the static bitmapped image to be uploaded.
 */
void MainWindow::on_staticImageBrowse_Button_clicked()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(this,
                                            QString("Select Image to load"),
                                            imagePath,
                                            "*.bmp");
    if(!fileName.isEmpty())
    {
        ui->staticImageFile->setText(fileName);
        imagePath = fileName;
    }
}

/*!
 * \brief MainWindow::on_staticImageBrowseIndexed_Button_clicked
 * \n Select the static bitmapped image to be uploaded.
 */
void MainWindow::on_staticImageBrowseIndexed_Button_clicked()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(this,
                                            QString("Select Image to load"),
                                            imagePath,
                                            "*.bmp");
    if(!fileName.isEmpty())
    {
        ui->staticImageFile_Indexed->setText(fileName);
        imagePath = fileName;
    }
}

/*!
 * \brief MainWindow::on_StaticImageUpload_Button_clicked
 * \n Uploads the static bitmapped image into the LightCrafter's DLPC300 memory buffer.
 * \n When no image is loaded a 24-bit red, green, and blue vertical color bar is displayed.
 * \n Command 0x01 0x05
 */
void MainWindow::on_StaticImageUpload_Button_clicked()
{
    // Check that lightcrafter is in correct mode
    if(ui->modeList->currentIndex() != 0)
    {
        showError("Please set the LC2000 nHD's Display Mode to 'Static Image / Color'");
        return;
    }

    if(packetHandler.writeCommand(0x0105))
        return;

    //Send the BMP image file contents
    if(sendFile(ui->staticImageFile->text()))
        return;
}

/*!
 * \brief MainWindow::on_StaticImageUploadIndexed_Button_clicked
 * \n Uploads the static bitmapped image into the LightCrafter's DLPC300 memory buffer
 * \n at the selected index.
 * \n When no image is loaded a 24-bit red, green, and blue vertical color bar is displayed.
 * \n Command 0x01 0x15
 */
void MainWindow::on_StaticImageUploadIndexed_Button_clicked()
{
    // Check that lightcrafter is in correct mode
    if(ui->modeList->currentIndex() != 0)
    {
        showError("Please set the LC2000 nHD's Display Mode to 'Static Image / Color'");
        return;
    }

    if(packetHandler.writeCommand(0x0115))
        return;

    //send the Index value
    if(packetHandler.putData((quint8)ui->StaticImageIndex->currentIndex(), 1))
        return;

    //Send the BMP image file contents
    if(sendFile(ui->staticImageFile_Indexed->text()))
        return;
}

/*!
 * \brief MainWindow::on_staticImageDisplay_Button_clicked
 * \n Displays the static image from memory buffer at the selected index.
 * \n Command 0x01 0x16
 */
void MainWindow::on_staticImageDisplay_Button_clicked()
{
    // Check that lightcrafter is in correct mode
    if(ui->modeList->currentIndex() != 0)
    {
        showError("Please set the LC2000 nHD's Display Mode to 'Static Image / Color'");
        return;
    }

    if(packetHandler.writeCommand(0x0116))
        return;

    //send the Index value
    if(packetHandler.putData((quint8)ui->StaticImageIndex->currentIndex(), 1))
        return;

    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_staticColorSetBut_clicked
 * \n Fills the screen with given color (24 bit) when set to "Static Image mode".
 */
void MainWindow::on_staticColorSetBut_clicked()
{
    quint8 red;
    quint8 green;
    quint8 blue;
    quint32 color;


    // Check that lightcrafter is in correct mode
    if(ui->modeList->currentIndex() != 0)
    {
        showError("Please set the LC2000 nHD's Display Mode to 'Static Image / Color'");
        return;
    }

    if(packetHandler.writeCommand(0x0106))
        return;

    red =  strToNum(ui->staticColorRed->text());
    green =  strToNum(ui->staticColorGreen->text());
    blue =  strToNum(ui->staticColorBlue->text());

    color = (red << 16) |  (green << 8) | (blue);

    //send the 24 bit Color in 00RRGGBB format
    if(packetHandler.putData(color, 4))
        return;

    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_bitDepth_currentIndexChanged
 * \n Calculate and Populate the pattern count according to the current bit depth selected.
 */
void MainWindow::on_bitDepth_currentIndexChanged(int index)
{
    int bitDepth = index + 1;
    int ptnNum = 1;
    int ptnsPerFrame = 24/(index + 1);
    int curNumPtns = ui->numPatterns->currentIndex();
    int maxPattn;

    if(bitDepth == 1)
    {
        ui->PatternType->setEnabled(true);
    }
    else
    {
        ui->PatternType->setCurrentIndex(0);
        ui->PatternType->setEnabled(false);
        ui->patternFileGroup->setEnabled(true);
    }


    /* HW Pattern Mode */
    if (ui->PatternType->currentIndex() == 2)
    {
        ui->numPatterns->setEditable(0);
        maxPattn = 32;
    }
    else
    {
        ui->numPatterns->setEditable(extendedMode);
        maxPattn = 4 * ptnsPerFrame;
    }

    ui->numPatterns->setMaxCount(maxPattn);
    ui->numPatterns->clear();

    while(1)
    {
        ui->numPatterns->addItem(numToStr(ptnNum));

        if( (ptnNum >= 4 * ptnsPerFrame) || (ptnNum >= maxPattn) )
        {
            break;
        }
        else if(ptnNum >= 2 * ptnsPerFrame)
        {
            ptnNum = (ptnNum + 4) & ~3;
        }
        else if(ptnNum >= ptnsPerFrame)
        {
            ptnNum = (ptnNum + 2) & ~1;
        }
        else
        {
            ptnNum++;
        }
    }

    if ( maxPattn < curNumPtns )
        ui->numPatterns->setCurrentIndex(maxPattn);
    else
        ui->numPatterns->setCurrentIndex(curNumPtns);

    fillNumPatterns();
}

/*!
 * \brief MainWindow::fillNumPatterns
 * \n Populate the pattern numbers.
 */
void MainWindow::fillNumPatterns(void)
{
    int prevIndex = ui->patternNum->currentIndex();
    int numPtns = strToNum(ui->numPatterns->currentText());

    if (ui->PatternType->currentIndex() == 2)
    {
        ui->HWPatternLabel->setVisible(1);
        ui->HWPtnSelect->setVisible(1);

        ui->patternFileLabel->setVisible(0);
        ui->patternFile->setVisible(0);
        ui->browseButton->setVisible(0);
    }
    else
    {
        ui->HWPatternLabel->setVisible(0);
        ui->HWPtnSelect->setVisible(0);

        ui->patternFileLabel->setVisible(1);
        ui->patternFile->setVisible(1);
        ui->browseButton->setVisible(1);
    }

    ui->patternNum->clear();

    for(int i = 0; i < numPtns; i++)
    {
        ui->patternNum->addItem(numToStr(i));
    }

    if(prevIndex < 0)
        prevIndex = 0;
    else if(prevIndex > numPtns)
        prevIndex = numPtns - 1;

    ui->patternNum->setCurrentIndex(prevIndex);
}

/*!
 * \brief MainWindow::on_solutionSaveBut_clicked
 * \n Saves the current settings of the displayed mode in persistent memory storage with the given solution name.
 * \n Command 0x06 0x00
 */
void MainWindow::on_solutionSaveBut_clicked()
{
    //command to save the solution
    if(packetHandler.writeCommand(0x0600))
        return;

    //send the solution name string
    if(packetHandler.putData(ui->solutionName->text(), SOLUTION_NAME_LEN))
        return;

    if(packetHandler.sendPacket())
        return;

    ui->solutionName->clear();

    on_solutionGetAllBut_clicked();
}

/*!
 * \brief MainWindow::on_solutionGetAllBut_clicked
 * \n Get all the stored solutions and populate the solution list.
 */
void MainWindow::on_solutionGetAllBut_clicked()
{
    char name[SOLUTION_NAME_LEN + 1] = {0};
    quint8 numSolutions;
    quint8 defSolution;

    //command to get the saved solutions
    if(packetHandler.readCommand(0x0600))
        return;

    if(packetHandler.sendPacket())
        return;

    //get the number of stored solutions
    if(packetHandler.getBytes(&numSolutions, 1))
        return;

    //get the number of default solution
    if(packetHandler.getBytes(&defSolution, 1))
        return;

    ui->solutionList->clear();

    for(int i = 0; i < numSolutions; i++)
    {
        QListWidgetItem *item;

        if(packetHandler.getBytes(name, SOLUTION_NAME_LEN))
            return;

        item = new QListWidgetItem(QString(name));

        if(i == defSolution)
        {
            item->setTextColor(QColor(0, 0, 255));
        }

        ui->solutionList->addItem(item);
    }
    if(ui->solutionList->count()!=0)
    {
        ui->solutionList->item(0)->setSelected(true);
        ui->solutionList->setFocus();
    }

}

/*!
 * \brief MainWindow::on_solutionLoadBut_clicked
 * \n Loads the previously saved solution of the given solution name.
 */
void MainWindow::on_solutionLoadBut_clicked()
{
    ui->solutionList->setFocus();

    if(packetHandler.writeCommand(0x0601))
        return;

    //data byte 0 = 1 -> load the solution
    if(packetHandler.putData(1, 1))
        return;


    if(ui->solutionList->count()!=0)
    {
            // send the name of the solution to be loaded.
            if(packetHandler.putData(ui->solutionList->currentItem()->text(), SOLUTION_NAME_LEN))
                return;

            if(packetHandler.sendPacket())
                return;
    }
    else
        QMessageBox::warning(this,"LC2000 nHD warning message","Solution list is empty!");


}

/*!
 * \brief MainWindow::on_solutionDeleteBut_clicked
 * \n Deletes the previously saved solution of the given solution name.
 */
void MainWindow::on_solutionDeleteBut_clicked()
{

    ui->solutionList->setFocus();
    if(packetHandler.writeCommand(0x0601))
        return;

    //data byte 0 = 0 -> delete the solution
    if(packetHandler.putData((quint32)0, 1))
        return;

    if(ui->solutionList->count()!=0)
    {
        ui->solutionList->setFocus();

        //send the name of the solution to be deleted.
        if(packetHandler.putData(ui->solutionList->currentItem()->text(), SOLUTION_NAME_LEN))
                return;

            if(packetHandler.sendPacket())
                return;
    }
    else
        QMessageBox::warning(this,"LC2000 nHD warning message","Solution list is empty!");

    on_solutionGetAllBut_clicked();

}


#if QT_VERSION <= 0x040804
void MainWindow::on_tabWidget_currentChanged(QWidget* currentWidget)
{
    static bool firstTime = true;
    int index = ui->tabWidget->indexOf(currentWidget);

    if(firstTime && currentWidget == ui->solutionTab)
    {
        if(packetHandler.connectionStatus()
                == PacketHandler::CON_STATUS_CONNECTED)
        {
            on_solutionGetAllBut_clicked();
            firstTime = false;
        }
    }
    ui->tabWidget->setCurrentIndex(index);
}
#else
void MainWindow::on_tabWidget_currentChanged(int curIndex)
{
    static bool firstTime = true;
    int index = curIndex;

    if(firstTime)
    {
        if(packetHandler.connectionStatus()
                == PacketHandler::CON_STATUS_CONNECTED)
        {
            on_solutionGetAllBut_clicked();
            firstTime = false;
        }
    }
    ui->tabWidget->setCurrentIndex(index);
}
#endif

/*!
 * \brief MainWindow::on_solutionDefaultBut_clicked
 * \n Sets the selected solution from the previously saved solution as the default solution.
 */
void MainWindow::on_solutionDefaultBut_clicked()
{

    if(packetHandler.writeCommand(0x0601))
        return;

    //data byte 0 = 2 -> set the solution as default
    if(packetHandler.putData(2, 1))
        return;

    if(ui->solutionList->count()!=0)
    {
        ui->solutionList->setFocus();

        //send the name of the solution to be set as default.
        if(packetHandler.putData(ui->solutionList->currentItem()->text(), SOLUTION_NAME_LEN))
            return;
        if(packetHandler.sendPacket())
            return;
    }
    else
        QMessageBox::warning(this,"LC2000 nHD warning message","Solution list is empty!");



    on_solutionGetAllBut_clicked();

}


quint32 MainWindow::getFileChecksum(QString fileName)
{
    QFile file(fileName);
    quint32 sum;

    if(!file.open(QIODevice::ReadOnly))
        return 0;

    sum = 0;
    while(!file.atEnd())
    {
        char data[1];

        file.read((char *)data, 1);
        sum += data[0] && 0xFF;

    }

    file.close();

    return sum;
}


/*!
 * \brief MainWindow::on_camTrigSet_clicked
 * \n Sets the trigger output setting used for camera trigger.
 * \n Command 0x04 0x04
 */
void MainWindow::on_camTrigSet_clicked()
{
    if(packetHandler.writeCommand(0x0404))
        return;

    //enable/disable trigger output
    if(packetHandler.putData(ui->camTrigEnable->isChecked(), 1))
        return;

    //trigger source
    if(packetHandler.putData((quint32)0, 1))
        return;

    //trigger polarity
    if(packetHandler.putData(ui->camTrigInv->isChecked(), 1))
        return;

    //Trigger Delay in micro seconds
    if(putTextIntData(ui->camTrigDelay->text(), 4))
        return;

    //Trigger Pulse Width
    if(putTextIntData(ui->camTrigWidth->text(), 4))
        return;

    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_camTrigGet_clicked
 * \n Reads the trigger output setting used for camera trigger.
 * \n Command 0x04 0x04
 */
void MainWindow::on_camTrigGet_clicked()
{
    if(packetHandler.readCommand(0x0404))
        return;

    if(packetHandler.sendPacket())
        return;

    ui->camTrigEnable->setChecked(packetHandler.getInt(1));
    packetHandler.getInt(1); // trigger source
    ui->camTrigInv->setChecked(packetHandler.getInt(1)); //trigger polarity
    ui->camTrigDelay->setText(getTextIntData(4)); //trigger Delay in micro seconds
    ui->camTrigWidth->setText(getTextIntData(4)); //trigger pulse width
}

/*!
 * \brief MainWindow::on_videoModeSet_clicked
 * \n Sets the current video mode settings.
 * \n Command 0x02 0x01
 */
void MainWindow::on_videoModeSet_clicked()
{
    // Check that lightcrafter is in correct mode
    if(ui->modeList->currentIndex() != 2)
    {
        showError("Please set the LC2000 nHD Display Mode to 'HDMI Port'");
        return;
    }

    // Video streaming or External Pattern mode?
    if(ui->radioVideoMode->isChecked())
    {
        // Video Streaming Mode Selected

        //Added new version of this command in v5.3.7 DM365 code that allows non-RGB mode to be saved with a solution
        if( (ui->MonochromeSelect->currentIndex()==0)  ||
                VER_DM365LT(5,3,7)   )
        {
            // Send this command for version < 5.3.7 or MonochromeSelect = 0 (RGB)
            if(packetHandler.writeCommand(0x0201))
                return;

            // Send Frame Rate to packet
            if(putTextIntData(ui->videoFrameRate->currentText(), 1))
                return;

            // Send Bit Depth to packet (8bit per color = 24bpp)
            if(putTextIntData("8", 1))
                return;

            // Send "video mode" LED select to packet (RGB=1 for video streaming)
            if(packetHandler.putData(1, 1))
                return;

        } else {
            // Send command for non RGB (Mono) when using DM365 version > 5.3.7
            if(packetHandler.writeCommand(0x0281))
                return;

            // Send Frame Rate to packet
            if(putTextIntData(ui->videoFrameRate->currentText(), 1))
                return;

            // Send Bit Depth to packet (8bit per color = 24bpp)
            if(putTextIntData("8", 1))
                return;

            // Send "video mode" LED select to packet (RGB=1 for video streaming))
            if(packetHandler.putData((quint32)1, 1))
                return;

            // Add MonoColor & CSC table to command (new in v5.3.7 DM365 API) instead of ccaMono(ui->MonochromeSelect->currentIndex());
            // Send MonoColor selection (0=RGB, 1,2,3 = R,G,B, 0x81, 82, 83 = R,G,B with custom CSC)
            if(packetHandler.putData(ui->MonochromeSelect->currentIndex(), 1))
                return;

            // Send Dummy CSC table which are the defaults from the PG (need to change if Select index > 3 but this is not currently a user option)
            if(packetHandler.putData((quint32) 0x0100, 4))
                return;
            if(packetHandler.putData((quint32) 0x0000, 4))
                return;
            if(packetHandler.putData((quint32) 0x0000, 4))
                return;
            if(packetHandler.putData((quint32) 0x0000, 4))
                return;
            if(packetHandler.putData((quint32) 0x0100, 4))
                return;
            if(packetHandler.putData((quint32) 0x0000, 4))
                return;
            if(packetHandler.putData((quint32) 0x0000, 4))
                return;
            if(packetHandler.putData((quint32) 0x0000, 4))
                return;
            if(packetHandler.putData((quint32) 0x0100, 4))
                return;
        }

        // Send packet
        if(packetHandler.sendPacket())
            return;
//

        // Send resolution, active pixels, and active lines
        if(packetHandler.writeCommand(0x0200))
            return;
        if(putTextIntData(ui->xResInput->text(), 2))
            return;
        if(putTextIntData(ui->yResInput->text(), 2))
            return;
        if(putTextIntData(ui->firstPixInput->text(), 2))
            return;
        if(putTextIntData(ui->firstLineInput->text(), 2))
            return;
        if(putTextIntData(ui->activeWidthInput->text(), 2))
            return;
        if(putTextIntData(ui->activeHeightInput->text(), 2))
            return;
        if(packetHandler.sendPacket())
            return;

        // DM365 Versions before v5.3.7 and RGB mode need
        if( (ui->MonochromeSelect->currentIndex()==0)  ||
                VER_DM365LT(5,3,7)   )
        {
            // Update CCA
            ccaMono(ui->MonochromeSelect->currentIndex());
        }
    } else {
        // External Pattern Sequence Mode

        // Send command
        if(packetHandler.writeCommand(0x0201))
            return;

    //    ccaRGB();

        // Send Frame Rate to packet
        if(putTextIntData(ui->extPatFrameRate->currentText(), 1))
            return;

        // Send Bit Depth to packet
        if(putTextIntData(ui->extPatBitDepth->currentText(), 1))
            return;

        // Send "video mode" LED select to packet [1->RGB ; 2->Monochrome red ; 3->Monochrome green ; 4->Monochrome blue]
        if(packetHandler.putData(ui->extPatLEDSelect->currentIndex() + 1, 1))
            return;

        // Send packet
        if(packetHandler.sendPacket())
            return;

        // Requires 640x360 because this is a pixel accurate mode
        // Send resolution, active pixels, and active lines
        if(packetHandler.writeCommand(0x0200))
            return;
        if(putTextIntData(QString::number(640), 2))
            return;
        if(putTextIntData(QString::number(360), 2))
            return;
        if(putTextIntData(QString::number(0), 2))
            return;
        if(putTextIntData(QString::number(0), 2))
            return;
        if(putTextIntData(QString::number(608), 2))
            return;
        if(putTextIntData(QString::number(684), 2))
            return;
        if(packetHandler.sendPacket())
            return;




    }
}

void MainWindow::ccaRGB()

{
    int gain=0x100;
    int i;

    // Clear CCA

    for (i=0x5F;i<0x67;i++)
    {
        //Command : 0xff 0x00 -> Writes to DLPC300 register
        if(packetHandler.writeCommand(0xFF00))
            return;

        //send DLPC300 Register Address as byte 0 of payload
        if(packetHandler.putData(i, 1))
            return;

        //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
        if(packetHandler.putData((quint32)0x00, (int)0x4))
            return;

        //send the packet with the write command, DLPC300 register address and the value to be written.
        if(packetHandler.sendPacket())
            return;
    }



    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(0x5F, 1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData(gain, (int)0x4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;

    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(0x63, 1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData(gain, (int)0x4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;

    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(0x67, 1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData(gain, (int)0x4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;


}


// Configure C300 CCA to use either red, blue or green source data

void MainWindow::ccaMono(int color)

{
    int i;


    if (color <4) {

        // Clear CCA

        for (i=0x5F;i<0x67;i++)
        {
            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(i, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData((quint32)0x00, (int)0x4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;
        }
    }
    else return;

    switch (color) {

        case 0:
            ccaRGB();

        // Force to 8-bit 60 Hz RGB sequence

        //Command : 0xff 0x00 -> Writes to DLPC300 register
        if(packetHandler.writeCommand(0xFF00))
            return;

        //send DLPC300 Register Address as byte 0 of payload
        if(packetHandler.putData(0x82, 0x1))
            return;

        //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
        if(packetHandler.putData((quint32)0x0, 0x4))
            return;

        //send the packet with the write command, DLPC300 register address and the value to be written.
        if(packetHandler.sendPacket())
            return;

        //Command : 0xff 0x00 -> Writes to DLPC300 register
        if(packetHandler.writeCommand(0xFF00))
            return;

        //send DLPC300 Register Address as byte 0 of payload
        if(packetHandler.putData(0x83, 0x1))
            return;

        //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
        if(packetHandler.putData(0x100, 0x4))
            return;

        //send the packet with the write command, DLPC300 register address and the value to be written.
        if(packetHandler.sendPacket())
            return;

        //Command : 0xff 0x00 -> Writes to DLPC300 register
        if(packetHandler.writeCommand(0xFF00))
            return;

        //send DLPC300 Register Address as byte 0 of payload
        if(packetHandler.putData(0x39, 0x1))
            return;

        //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
        if(packetHandler.putData(2, 0x4))
            return;

        //send the packet with the write command, DLPC300 register address and the value to be written.
        if(packetHandler.sendPacket())
            return;

        //Command : 0xff 0x00 -> Writes to DLPC300 register
        if(packetHandler.writeCommand(0xFF00))
            return;

        //send DLPC300 Register Address as byte 0 of payload
        if(packetHandler.putData(0x3a, 0x1))
            return;

        //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
        if(packetHandler.putData((quint32)0x1, 0x4))
            return;

        //send the packet with the write command, DLPC300 register address and the value to be written.
        if(packetHandler.sendPacket())
            return;

        //Command : 0xff 0x00 -> Writes to DLPC300 register
        if(packetHandler.writeCommand(0xFF00))
            return;

        //send DLPC300 Register Address as byte 0 of payload
        if(packetHandler.putData(0x38, 0x1))
            return;

        //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
        if(packetHandler.putData((quint32)0xc1, 0x4))
            return;

        //send the packet with the write command, DLPC300 register address and the value to be written.
        if(packetHandler.sendPacket())
            return;

        //Command : 0xff 0x00 -> Writes to DLPC300 register
        if(packetHandler.writeCommand(0xFF00))
            return;

        //send DLPC300 Register Address as byte 0 of payload
        if(packetHandler.putData(0x82, 0x1))
            return;

        //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
        if(packetHandler.putData(0x1, 0x4))
            return;

        //send the packet with the write command, DLPC300 register address and the value to be written.
        if(packetHandler.sendPacket())
            return;



        //Command : 0xff 0x00 -> Writes to DLPC300 register
        if(packetHandler.writeCommand(0xFF00))
            return;

        //send DLPC300 Register Address as byte 0 of payload
        if(packetHandler.putData(0x4b, 1))
            return;

        //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
        if(packetHandler.putData(0x14151624, 4))
            return;

        //send the packet with the write command, DLPC300 register address and the value to be written.
        if(packetHandler.sendPacket())
            return;

        //Command : 0xff 0x00 -> Writes to DLPC300 register
        if(packetHandler.writeCommand(0xFF00))
            return;


        //send DLPC300 Register Address as byte 0 of payload
        if(packetHandler.putData(0x4c, 1))
            return;

        //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
        if(packetHandler.putData(0x25111213, 4))
            return;

        //send the packet with the write command, DLPC300 register address and the value to be written.
        if(packetHandler.sendPacket())
            return;

            break;

        case 1:
            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x5F, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x100, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x60, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x100, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x61, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData((quint32)0x0, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;


            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x4b, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x13151624, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;


            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x4c, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x25111312, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            break;
        case 2:
            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x62, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x100, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x63, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x100, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x64, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData((quint32)0x0, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;


            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x4b, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x14151624, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;


            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x4c, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x25111213, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            break;


        case 3:
            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x65, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x100, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x66, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x100, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;

            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x67, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData((quint32)0x000, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;




            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x4b, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x14151624, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;

            //Command : 0xff 0x00 -> Writes to DLPC300 register
            if(packetHandler.writeCommand(0xFF00))
                return;


            //send DLPC300 Register Address as byte 0 of payload
            if(packetHandler.putData(0x4c, 1))
                return;

            //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
            if(packetHandler.putData(0x25111312, 4))
                return;

            //send the packet with the write command, DLPC300 register address and the value to be written.
            if(packetHandler.sendPacket())
                return;
            break;

    }

    if (color>0) {
    // Force to 8-bit Monochrome sequence

    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(0x82, 0x1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData((quint32)0x0, 0x4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;

    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(0x83, 0x1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData(0x10F, 0x4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;

    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(0x39, 0x1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData(16, 0x4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;

    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(0x3a, 0x1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData((quint32)0x1, 0x4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;

    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(0x38, 0x1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData((quint32)0xc1, 0x4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;

    //Command : 0xff 0x00 -> Writes to DLPC300 register
    if(packetHandler.writeCommand(0xFF00))
        return;

    //send DLPC300 Register Address as byte 0 of payload
    if(packetHandler.putData(0x82, 0x1))
        return;

    //send the 4-byte value to be written to DLPC300 register in byte 1 - byte 4
    if(packetHandler.putData(0x1, 0x4))
        return;

    //send the packet with the write command, DLPC300 register address and the value to be written.
    if(packetHandler.sendPacket())
        return;

    }



}

/*!
 * \brief MainWindow::on_videoModeGet_clicked
 * \n Gets the current video mode settings.
 * \n Command 0x02 0x01
 */
void MainWindow::on_videoModeGet_clicked()
{
    int     index;
    int     LEDSelect;
    QString FrameRate;
    QString bitDepth;

    // Send command to get HDMI source settings
    if(packetHandler.readCommand(0x0201))
        return;
    if(packetHandler.sendPacket())
        return;

    // Get the video frame rate
    FrameRate = numToStr(packetHandler.getInt(1));

    // Get the video bit depth
    bitDepth = numToStr(packetHandler.getInt(1));

    // Get the "video mode" LED Select
    LEDSelect = packetHandler.getInt(1);
    if(LEDSelect == 0)
    {
        LEDSelect = 2;
    }
    else
    {
        LEDSelect -= 1;
    }

    // Video streaming or External Pattern mode?
    if((LEDSelect == 0) && (bitDepth == "8"))
    {
        // Video Streaming Mode Selected
        emit on_radioVideoMode_clicked();
        ui->radioVideoMode->setChecked(true);

        // Set Frame Rate combobox
        index = ui->videoFrameRate->findText(FrameRate);
        if(index >= 0)
        {
            ui->videoFrameRate->setCurrentIndex(index);
        }

        // Get resolution, active pixels, and active lines
        if(packetHandler.readCommand(0x0200))
            return;
        if(packetHandler.sendPacket())
            return;
        if(packetHandler.responseSize() != 12)
            return;

        // Set resolution, active pixels, and active lines in GUI
        ui->xResInput->setText(getTextIntData(2)); //input resolution x
        ui->yResInput->setText(getTextIntData(2)); //input resolution y
        ui->firstPixInput->setText(getTextIntData(2)); //first active pixel
        ui->firstLineInput->setText(getTextIntData(2)); //first active line
        ui->activeWidthInput->setText(getTextIntData(2)); //active width
        ui->activeHeightInput->setText(getTextIntData(2)); //active height
    }
    else
    {
        // External Pattern Sequence Mode
        emit on_radioExtPatternMode_clicked();
        ui->radioExtPatternMode->setChecked(true);

        // Send "video mode" LED select to packet [1->RGB ; 2->Monochrome red ; 3->Monochrome green ; 4->Monochrome blue]
        ui->extPatLEDSelect->setCurrentIndex(LEDSelect);

        // Set Bit Depth combobox
        index = ui->extPatBitDepth->findText(bitDepth);
        if(index >= 0)
        {
            ui->extPatBitDepth->setCurrentIndex(index);
        }

        // Set Frame Rate combobox
        index = ui->extPatFrameRate->findText(FrameRate);
        if(index >= 0)
        {
            ui->extPatFrameRate->setCurrentIndex(index);
        }
    }


}


// LC2000 nHD Firmware Upgrades
/*!
 * \brief MainWindow::on_Install_clicked
 * \n Downloads and installs the firmware file.
 * \n Command 0x07 0x00
 */
void MainWindow::on_Install_clicked()
{
    int fileType = ui->installFileType->currentIndex();         // Index into the current File Type selection drop-down
    QFileInfo fileInfo;
    fileInfo.setFile(ui->pkgFileName->text());                  // File Name/path which was added to the text box after browsing
    int test_enabled = ui->checkBox_FileLoadTestStatus->isChecked();
//    int TestLoop_count = 65535;
    int TestLoop_count = strToNum(ui->TestFile_RepeatCount->text());
    char const *fileID[] =
    {
 //     "IPKG",
        "FPGA",
        "DLPC",
        "MSPS",
        "EDID",
        "SITR",
        "SNUM",
        "SPEC",
        "TEST"
    };

    if(!fileInfo.isFile())
        return;

    if(packetHandler.writeCommand(0x0700))
        return;

    if(fileType != 7)
    {
        if((unsigned)fileType >= ARRAY_SIZE(fileID))            // Check for file type out of range
            return;

        if(packetHandler.putData(QString("IPKG"), 4))           //File identifier
            return;

        if(packetHandler.putData(fileInfo.baseName(), 32))      //Version string for this install package
            return;

        if(packetHandler.putData(1, 1))                         //Number of files in this package
            return;

        if(packetHandler.putData((quint32)0, 3))                //Reserved
            return;

        if(packetHandler.putData(QString(fileID[fileType]), 4)) //ID of the Installation File
            return;

        if(packetHandler.putData(fileInfo.baseName(), 32))      //Version string of the Installation File
            return;

        if(packetHandler.putData(fileInfo.size(), 4))           //Length of the First Installation File
            return;

        if(packetHandler.putData((quint32)0, 4))                //32 bit Checksum
            return;

        // only send this command for IPKG types
        if(sendFile(fileInfo.absoluteFilePath()))
            return;
    } else if(fileType == 7 && test_enabled)                    // TEST type is special case (KP Internal only)
    do {
        if((unsigned)fileType >= ARRAY_SIZE(fileID))            // Check for file type out of range
            return;

        if(packetHandler.putData(QString("IPKG"), 4))           //File identifier
            return;

        if(packetHandler.putData(fileInfo.baseName(), 32))      //Version string for this install package
            return;

        if(packetHandler.putData(1, 1))                         //Number of files in this package
            return;

        if(packetHandler.putData((quint32)0, 3))                //Reserved
            return;

        if(packetHandler.putData(QString(fileID[fileType]), 4)) //ID of the Installation File
            return;

        if(packetHandler.putData(fileInfo.baseName(), 32))      //Version string of the Installation File
            return;

        if(packetHandler.putData(fileInfo.size(), 4))           //Length of the First Installation File
            return;

        if(packetHandler.putData((quint32)0, 4))                //32 bit Checksum
            return;

        // only send this command for IPKG types
        if(sendFile(fileInfo.absoluteFilePath()))
            return;
    } while(TestLoop_count--);

}

void MainWindow::on_pkgBrowse_clicked()
{
    QString startPath = ui->pkgFileName->text();
    int fileType = ui->installFileType->currentIndex();
    QString fileName;

    //Full Package format (place holder for now)
    if(fileType == 8)
    {
        fileName = QFileDialog::getOpenFileName(0,
                                                QString("Select Package File"),
                                                startPath,
                                                "*.pkg");
    }
    //FPGA build in .rbf format
    else if(fileType == 0)
    {
        fileName = QFileDialog::getOpenFileName(0,
                                                QString("Select FPGA File"),
                                                startPath,
                                                "*.rbf");
    }
    //DLPC300 Binary file format
    else if((fileType == 1) || (fileType == 6))
    {
        fileName = QFileDialog::getOpenFileName(0,
                                                QString("Select DLPC300 binary File"),
                                                startPath,
                                                "*.bin");
    }
    //MSP430 in .txt file format
    else if(fileType == 2)
    {
        fileName = QFileDialog::getOpenFileName(0,
                                                QString("Select MSP430 File"),
                                                startPath,
                                                "*.txt");
    }
    //EDID in .bin format
    else if(fileType == 3)
    {
        fileName = QFileDialog::getOpenFileName(0,
                                                QString("Select EDID File"),
                                                startPath,
                                                "*.bin");
    }
    //Application binary
    else if(fileType == 4)
    {
        fileName = QFileDialog::getOpenFileName(0,
                                                QString("Select DM365 Binary"),
                                                startPath,
                                                "*");
    }
    //Serial Number in .txt format
    else if(fileType == 5)
    {
        fileName = QFileDialog::getOpenFileName(0,
                                                QString("Select Serial Number File"),
                                                startPath,
                                                "*.txt");
    }
    else if(fileType == 7)
    {
        fileName = QFileDialog::getOpenFileName(0,
                                                QString("Select Test File"),
                                                startPath,
                                                "*.*");
    }
    else
    {
        fileName = "";
    }

    if(!fileName.isEmpty())
    {
        ui->pkgFileName->setText(fileName);
    }
}

/* returns 1=error, 0=last packet (all data) sent ok ?? */
int MainWindow::sendFile(QString fileName)
{
    QFile file(fileName);
    quint8 data[PacketHandler::MAX_PACKET_SIZE];
    int ret = 0;

    if(!file.open(QIODevice::ReadOnly))
        return 1;

    while(1)
    {
        int readLen = PacketHandler::MAX_PACKET_SIZE;       // start with max data packet size

        readLen -= packetHandler.getDataLength();           // subtract number of bytes already in packet ready to send

        readLen = file.read((char *)data, readLen);         // pull data from file to fill out packet size

        if(readLen <= 0)                                    // if we failed to read any bytes then return with error (0)??
            break;

        if(packetHandler.putData(data, readLen))            // put the data in packet and return error (1) if we fail??
        {
            ret = 1;
            break;
        }

        if(packetHandler.sendPacket(!file.atEnd()))        // send data packet and return error (1) if we fail??
        {
            ret = 1;
            break;
        }
    }

    file.close();

    return ret;
}


/*!
 * \brief MainWindow::on_camCaptureBut_clicked
 * \n Captures one frame of the image from the integrated camera port and returns the raw bytes.
 * \n Command 0x05 0x00
 */
void MainWindow::on_camCaptureBut_clicked()
{
    int width;
    int height;
    int bytesPerPix;
    QSize imgSize(ui->camImage->size());

    if(packetHandler.readCommand(0x0500))
        return;

    //Capture mode 1 ->single frame capture
    if(packetHandler.putData(1, 1))
        return;

    if(packetHandler.sendPacket())
        return;

    width = packetHandler.getInt(2); //width of the image
    height = packetHandler.getInt(2); //height of the image
    bytesPerPix = packetHandler.getInt(1);//Image format (bytes per pixel)

    if(bytesPerPix != 1)//8-bit Gray Scale (1 byte per pixel)
    {
        showError("Un supported image format");
        return;
    }

    if(camImageData == NULL)
    {
        camImageData = new uchar[width * height];
    }
    if(camImage == NULL)
    {
        camImage = new QImage(camImageData, width, height, QImage::Format_Indexed8);
        camImage->setColorCount(256);
        for(unsigned i = 0; i < 256; i++)
        {
            camImage->setColor(i, 0xFF000000 | i | i << 8 | i << 16);
        }
    }

    if(packetHandler.getBytes(camImageData, width * height))
        return;

    ui->camImage->setPixmap(QPixmap::fromImage(*camImage).scaled(imgSize));
}

/*!
 * \brief MainWindow::on_camImageSave_clicked
 * \n Save the capturd image.
 */
void MainWindow::on_camImageSave_clicked()
{
    static QString imageFileName;
    QString fileName;

    if(camImage == NULL)
        return;

    fileName = QFileDialog::getSaveFileName(this,
                                            "Save Captured Image as",
                                            imageFileName,
                                            "Image File (*.bmp *.png *.jpg)");

    if(!fileName.isEmpty())
    {
        camImage->save(fileName);
        imageFileName = fileName;
    }
}


void MainWindow::on_trigType_currentIndexChanged()
{
    checkItems();
}

/*!
 * \brief MainWindow::on_uploadThisButton_clicked
 * \n Uploads the selected pattern.
 */
void MainWindow::on_uploadThisButton_clicked()
{
    int patNum = ui->patternNum->currentIndex();

    if(ui->patternNum->count() == 0) return;

    if(patNum >= MAX_PATTERNS)
        return;

    if(ui->PatternType->currentIndex() == 2)
    {
        sendHWPattern(patNum, 1);
    }
    else
    {
        if(!patternFile[patNum].exists())
            return;

        if(packetHandler.writeCommand(extendedMode ? 0x0481 : 0x0401))
            return;

        //Pattern number
        if(packetHandler.putData(patNum, extendedMode ? 2 : 1))
            return;

        if(extendedMode)
        {
            //column position and row position to display this pattern within DMD
            if(packetHandler.putData((quint32)0, 4))
                return;
        }

        //send the pattern bitmap in Windows BMP format to be loaded.
        if(sendFile(patternFile[patNum].absoluteFilePath()))
            return;
    }
}

/*!
 * \brief MainWindow::on_displayThisButton_clicked
 * \n Displays the selected pattern sequence with the inidcates exposure and trigger period settings.
 * \n (supported in DM365 firmware version 3.0 or greater)
 */
void MainWindow::on_displayThisButton_clicked()
{
    int patNum = ui->patternNum->currentIndex();

    if(packetHandler.writeCommand(0x0405))
        return;

    //pattern number
    if(packetHandler.putData(patNum, 2))
        return;

    if(packetHandler.sendPacket())
        return;
}

void MainWindow::on_playCount_clicked()
{
    checkItems();
}

void MainWindow::checkItems(void)
{
    int trigType = ui->trigType->currentIndex();
    bool enableDelay = true;
    bool enablePeriod = true;
    bool enablePlayCount = true;

    // Disable Trigger inputs when Command input trigger is active
    if(trigType == 1) // Auto Trigger
    {
        enableDelay = false;
//JFH 1-22        enablePlayCount = false;
                enablePlayCount = extendedMode;
    }
    else
    {
        enablePlayCount = extendedMode;

 //       if(ui->bitDepth->currentIndex() > 0  &&
 //               strToNum(ui->exposureTime->text()) > 20000)
 //           )
 //          enablePlayCount = false;

        if(trigType == 0) // Command Trigger
        {
            enableDelay = false;
        }

        if(ui->playCount->isChecked() == false)
        {
            enablePeriod = false;
        }
    }

    if(enablePlayCount == 0)
        ui->playCount->setChecked(0);

    ui->playCount->setEnabled(enablePlayCount);

    if(enablePeriod == false) {
        ui->trigPeriod->setText("0");
    }

    if(enableDelay == false) {
        ui->trigDelay->setText("0");
    }

    ui->trigDelay->setEnabled(enableDelay);
    ui->trigPeriod->setEnabled(enablePeriod);
}


void MainWindow::on_exposureTime_editingFinished()
{
    checkItems();
}

/*!
 * \brief MainWindow::on_pb_ChangeIP_clicked
 * \n Changes the IP address of the LightCrafter module.
 * \n Command 0x08 0x00
 */
void MainWindow::on_pb_ChangeIP_clicked()
{
    QString title("Changing BBB IP");
    QString text("Do you want to change the IP of the BeagleBone?\nThe device must be manually reset after the change.");
    QMessageBox msgBox(QMessageBox::Warning, title, text, QMessageBox::Yes | QMessageBox::No, this);
    if(msgBox.exec() != QMessageBox::Yes)
        return;

    QString IP(ui->newIPText->text());
    QTextStream ts(&IP, QIODevice::ReadOnly);

    if(packetHandler.writeCommand(0x0800))
        return;

    //send the IP address in 4 byte format
    for(int i = 0; i < 4; i++)
    {
        unsigned value;
        char ch;
        ts>>value;
        ts>>ch;
        if(packetHandler.putData(value, 1))
            return;
    }

    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_pb_ChangeIPandReboot_clicked
 * \n Changes the IP address of the LightCrafter module and then Reboots the target using software reset.
 * \n Command 0x08 0x01
 */
void MainWindow::on_pb_ChangeIPandReboot_clicked()
{
    QString title("Changing BeagleBone IP");
    QString text("Do you want to change the IP of this BeagleBone?\nSYSTEM WILL BE RESET WHEN YOU CLICK.");
    QMessageBox msgBox(QMessageBox::Warning, title, text, QMessageBox::Yes | QMessageBox::No, this);
    if(msgBox.exec() != QMessageBox::Yes)
        return;

    QString IP(ui->newIPText->text());
    QTextStream ts(&IP, QIODevice::ReadOnly);

    if(packetHandler.writeCommand(0x0801))
        return;

    //send the IP address in 4 byte format
    for(int i = 0; i < 4; i++)
    {
        unsigned value;
        char ch;
        ts>>value;
        ts>>ch;
        if(packetHandler.putData(value, 1))
            return;
    }

    if(packetHandler.sendPacketNoResponse())
        return;

}


void MainWindow::addExtPatFrameRate(void)
{
    int index;
    int mono = ui->extPatLEDSelect->currentIndex();
    int bitDepth = ui->extPatBitDepth->currentText().toInt();


    extPatFrameRate_prevText = ui->extPatFrameRate->currentText();

    ui->extPatFrameRate->clear();
    if(mono == 0 && bitDepth == 8)
    {
        ui->extPatFrameRate->addItem("60");
        ui->extPatFrameRate->addItem("50");
    }
    else if(mono != 0 && (bitDepth == 1 || bitDepth == 2 || bitDepth == 4 ||
                          bitDepth == 7))
    {
        ui->extPatFrameRate->addItem("60");
        ui->extPatFrameRate->addItem("40");
        ui->extPatFrameRate->addItem("30");
        ui->extPatFrameRate->addItem("15");
    }
    else
    {
        ui->extPatFrameRate->addItem("60");
        ui->extPatFrameRate->addItem("45");
        ui->extPatFrameRate->addItem("30");
        ui->extPatFrameRate->addItem("15");
    }

    // Check if previous frame rate can be selected for new LED select
    index = ui->extPatFrameRate->findText(extPatFrameRate_prevText);
    if(index >= 0)
    {
        ui->extPatFrameRate->setCurrentIndex(index);
    }
}

void MainWindow::on_extPatLEDSelect_currentIndexChanged(int index)
{    
    // Store old bit depth and frame rates before changing combo boxes
    extPatBitDepth_prevText  = ui->extPatBitDepth->currentText();

    updateExtPatCombos = 0;
    if(index == 0)
    {
        ui->extPatBitDepth->clear();
        ui->extPatBitDepth->addItem("1");
        ui->extPatBitDepth->addItem("2");
        ui->extPatBitDepth->addItem("4");
        // ui->extPatBitDepth->addItem("8"); // This mode is not currently enabled in External Pattern Sequence from DM365
    }
    else
    {
        ui->extPatBitDepth->clear();
        ui->extPatBitDepth->addItem("1");
        ui->extPatBitDepth->addItem("2");
        ui->extPatBitDepth->addItem("3");
        ui->extPatBitDepth->addItem("4");
        ui->extPatBitDepth->addItem("5");
        ui->extPatBitDepth->addItem("6");
        ui->extPatBitDepth->addItem("7");
        ui->extPatBitDepth->addItem("8");
    }

    // Update frame rates
    addExtPatFrameRate();

    // Check if previous bit depth can be selected for new LED select
    index = ui->extPatBitDepth->findText(extPatBitDepth_prevText);
    if(index >= 0)
    {
        ui->extPatBitDepth->setCurrentIndex(index);
    }

    // See Table 2-78 External Video Sequences and Pattern Rates in DLPU004B for details.
    addExtPatFrameRate();
}

void MainWindow::on_extPatBitDepth_currentIndexChanged(QString )
{
    int bitDepth = ui->extPatBitDepth->currentText().toInt();
    int mono = ui->extPatLEDSelect->currentIndex();

    if(mono)
    {
        // Monochrome external pattern sequence
        switch(bitDepth)
        {
            case 1:
                ui->extPatPatperFrame->setText("24");
                break;
            case 2:
                ui->extPatPatperFrame->setText("12");
                break;
            case 3:
                ui->extPatPatperFrame->setText("8");
                break;
            case 4:
                ui->extPatPatperFrame->setText("6");
                break;
            case 5:
                ui->extPatPatperFrame->setText("4");
                break;
            case 6:
                ui->extPatPatperFrame->setText("4");
                break;
            case 7:
                ui->extPatPatperFrame->setText("3");
                break;
            case 8:
                ui->extPatPatperFrame->setText("2");
                break;
        }
    }
    else
    {
        // RGB external pattern sequence
        switch(bitDepth)
        {
            case 1:
                ui->extPatPatperFrame->setText("8");
                break;
            case 2:
                ui->extPatPatperFrame->setText("4");
                break;
            case 4:
                ui->extPatPatperFrame->setText("2");
                break;
        }
    }

    // See Table 2-78 External Video Sequences and Pattern Rates in DLPS023B for details

    addExtPatFrameRate();
}

void MainWindow::on_extPatFrameRate_currentIndexChanged(const QString &arg1)
{
    int PatternRate;
    int PatsPerFrame    = ui->extPatPatperFrame->text().toInt();

    PatternRate     = PatsPerFrame*arg1.toInt();
    ui->extPatPatternRate->setText(QString::number(PatternRate));

    // See Table 2-78 External Video Sequences and Pattern Rates in DLPS023B for details
}

void MainWindow:: on_numPatterns_currentIndexChanged()
{
}

void MainWindow:: setTab(int modeNum)
{
    switch( modeNum ) {
    case 0:				/* Static Display or Test Pattern  */
    case 1:
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tabTestPtn));
        emit on_testPtnGetBut_clicked();
        break;
    case 2:				/* HDMI Video */
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tabVideo));
        emit on_videoModeGet_clicked();
        break;
    case 4:				/* Pattern Sequence */
        ui->tabWidget->setCurrentIndex(ui->tabWidget->indexOf(ui->tabPtnSeq));
        emit on_patternGetSetting_clicked();
        break;
    }
}

void MainWindow::on_PatternType_currentIndexChanged()
{
    emit on_bitDepth_currentIndexChanged(0);
}

void MainWindow::setHWPattern(void)
{
    int index = ui->patternNum->currentIndex();
    int ptnNum = ui->HWPtnSelect->currentIndex();
    if(index >= 0 && ptnNum >= 0)
    {
        HWPtns[index].number = ptnNum & 0xF;
        HWPtns[index].invert = ptnNum >> 4;
    }
}

/*!
 * \brief MainWindow::genHWPattern
 * \n Draw the HW pattern into the image
 */
void MainWindow::genHWPattern(QImage *img, int ptnNum, int inv)
{
    QPainter paint(img);

    ui->HWPtnSelect->setCurrentIndex(ptnNum + inv * 16);

    if(ptnNum <= 10)
    {
        int shift = 10 - ptnNum;
        for(int i = 0; i < PTN_WIDTH; i++)
        {
            int pattern = i + (1024 - PTN_WIDTH)/2;
            int prevBit = ((pattern >> (shift + 1)) & 1);
            int curBit = ((pattern >> shift) & 1);
            int asicFix = ptnNum >= 2;

            if((prevBit ^ curBit ^ inv ^ asicFix) != 0)
                paint.setPen(Qt::white);
            else
                paint.setPen(Qt::black);

            paint.drawLine(i, 0, i, PTN_HEIGHT);
        }
    }
    else
    {
        int width = ((16 - ptnNum) * PTN_WIDTH)/10;
        QColor color[2] = {Qt::black, Qt::white};

        paint.setBrush(QBrush(color[inv], Qt::SolidPattern));
        paint.setPen(color[inv]);
        paint.drawRect(0, 0, PTN_WIDTH, PTN_HEIGHT);
        paint.setPen(color[1 ^ inv]);
        paint.setBrush(QBrush(color[1 ^ inv], Qt::SolidPattern));
        paint.drawRect((PTN_WIDTH - width)/2, 0, width, PTN_HEIGHT);
    }
    paint.end();
}

void MainWindow::on_patternNum_currentIndexChanged(int index)
{
    QImage img(PTN_WIDTH, PTN_HEIGHT, QImage::Format_RGB32);
    bool imgSet = 0;
    QSize imgSize(ui->patternView->size());

    if(ui->PatternType->currentIndex() == 2)
    {

        if(index >= 0 && index < ARRAY_SIZE(HWPtns))
        {
            genHWPattern(&img, HWPtns[index].number, HWPtns[index].invert);
            imgSet = 1;
        }
    }
    else
    {
        if(index >= 0 && index < MAX_PATTERNS)
        {
            //ui->patternNum->setCurrentIndex(index);
            ui->patternFile->setText(patternFile[index].baseName());

            if(patternFile[index].isFile())
            {
                img.load(patternFile[index].absoluteFilePath());
                imgSet = 1;
            }
        }
    }
    if(imgSet)
    {
        ui->patternView->setPixmap(QPixmap::fromImage(img).scaled(imgSize));
    }
    else
    {
        ui->patternView->clear();
    }
}

void MainWindow::on_HWPtnSelect_currentIndexChanged()
{
    setHWPattern();
    emit on_patternNum_currentIndexChanged(ui->patternNum->currentIndex());
}

void MainWindow::on_setMBMCSeq_clicked()
{
    if(packetHandler.writeCommand(0x0A01))
        return;

    if(putTextIntData(ui->MBMCSeqStartVec->text(), 1))
        return;

    if(putTextIntData(ui->MBMCSeqNumVec->text(), 1))
        return;

    if(packetHandler.sendPacket())
        return;
}

/*!
 * \brief MainWindow::on_getMBMCSeq_clicked
 * \n Get multiple bit depth and multiple color sequence settings.
 * \n Command 0x0A 0x01
 */
void MainWindow::on_getMBMCSeq_clicked()
{
    if(packetHandler.readCommand(0x0A01))
        return;

    if(packetHandler.sendPacket())
        return;

    QString startVector = getTextIntData(1);
    QString numVectors = getTextIntData(1);

    ui->enableCustSeq->setChecked(((strToNum(startVector) == 0) && (strToNum(numVectors) == 0)) ? false : true);
    ui->MBMCSeqStartVec->setText(startVector);
    ui->MBMCSeqNumVec->setText(numVectors);
}

/*!
 * \brief MainWindow::on_browseMBMCSeq_clicked
 * \n Browse and select the sequence file.
 */
void MainWindow::on_browseMBMCSeq_clicked()
{
    QString fileName;

    fileName = QFileDialog::getOpenFileName(this,
                                            QString("Select Sequence File"),
                                            seqFilePath,
                                            "*.bin");


    if(!fileName.isEmpty())
    {
        ui->MBMCSeqFile->setText(fileName);
        seqFilePath = fileName;
        readmeFilePath = fileName;
        imagelistFilePath = fileName;
    }
}






void MainWindow::on_getMBMCSeqHelp_clicked()
{
    QString helpString = "This option allows the user to configure the LC2000 nHD kit to run MBMC (Multiple Bit Depth and Multiple Color Pattern) Sequence. Refer ";
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("LC2000 nHD MBMC Sequence...");
    msgBox.setTextFormat(Qt::RichText);   //this is what makes the links clickable
    msgBox.setText(helpString+QString("<a href=http://e2e.ti.com/support/dlp__mems_micro-electro-mechanical_systems/f/850/t/267784.aspx>E2E link</a> for more details."));
    msgBox.exec();
}

/*!
 * \brief MainWindow::on_getVersionInfoButton_clicked
 * \n Get the version information of LightCrafter control software and the time stamp of compilation.
 */
void MainWindow::on_getVersionInfoButton_clicked()
{
    QString date = __DATE__; //get compile date & time
    QString time = __TIME__;
    QString copyrightSymbol(QChar(169));
    char versionStr[128];
    sprintf(versionStr, "LC2000 nHD Control Software - %d.%d.%d %s", GUI_V_MAJOR, GUI_V_MINOR, GUI_V_BUILD, GUI_V_OPTIONAL);
    QMessageBox::information(this,versionStr," Copyright"+copyrightSymbol+" by Keynote Photonics\n Compiled On : "+date+" ["+time+"]       ");
}

/*!
 * \brief MainWindow::on_camPreview_clicked
 * \n Requests DM365 to grab a frame from Camera and Display it on the DMD.
 */
void MainWindow::on_camPreview_clicked()
{
    if(packetHandler.writeCommand(0x0501))
        return;

    if(packetHandler.sendPacket())
        return;
}

//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/// \brief MainWindow::  Classes for Firmware Modification (splash screen)
///
//////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////
/*!
 * \brief MainWindow::on_dlpc300Browse_clicked
 * \n Open User dialog for a file name/path for the firmware input file
 */
void MainWindow::on_dlpc300Browse_clicked()
{
    QString startPath = ui->splashFile->text();
    QString fileName;

    // Browse and Select DLPC300 firmware file for Input
    fileName = QFileDialog::getOpenFileName(0,
                                            QString("Select DLPC300 Firmware File to Modify"),
                                            startPath,
                                            "*.bin");
    // Update line edit widget with file and path located by browse dialog
    if(!fileName.isEmpty())
    {
        ui->splashFile->setText(fileName);
        splashLayoutError = false;                             // Assume layout is ok until it is checked after loading
        Load_InputFirmwareFile();
        ui->comboBox_SplashIndex->setCurrentIndex(0);           // Reset Index selection since we have a new file opened
        ui->splashView->clear();                                // No splash image selected so clear image preview on GUI
    }
}

void MainWindow::Load_InputFirmwareFile()
{
    QFile   firmWare(ui->splashFile->text());                   // Input firmware file
    SPLASH  *splash;                                            // Pointer into Firmware source file for start of Splash screen
    int     maxSplashImages;                                    // Max number of splash images that fit in the firmware file (based on check box settings)
    int     i;
    unsigned int ImageSize;
    unsigned int TempVal;
    uint PadSize;                           // Size of padding between splash images (based on image size check box)
    uint ResFlag;                           // Resolution flag for splash image

    if (ui->C300_BigFlash->isChecked())
    {
        if (ui->C300Flash_NativeSize->isChecked())
        {
            maxSplashImages = 4;                    // Max Native Res splash screens with BigFlash
            ImageSize = 0xCB100;
            PadSize = 0xF00;
            ResFlag = 0x23;
        } else {
            maxSplashImages = 8;                    // Max Normal Res splash screens with BigFlash
            ImageSize = 0x33000;
            PadSize = 0x000;
            ResFlag = 0x00;
        }
    } else {
        if (ui->C300Flash_NativeSize->isChecked())
        {
            showError("Selecting Native Resolution without the 32MB flash is not valid.");
        } else {
            maxSplashImages = 4;                    // Max Normal Res splash screens with Normal Flash
            ImageSize = 0x33000;
            PadSize = 0x000;
            ResFlag = 0x00;
        }
    }


    // Load input Firmware file into memory
    if (!firmWare.open(QIODevice::ReadOnly))                    // Open the firmware file
    {
        showError("Unable to open input firmware file.");
    } else {
        frmwareData = firmWare.readAll();                       // Read firmware data into memory
        firmWare.close();                                       // Close firmware file
    }

    if(!(ui->splashFile->text().isEmpty()))                     // If we have sucessfully entered a path then we have valid data in memory
    {
        // Check Firmware source file to make sure splash data is in correct locations
        splash = (SPLASH*)(frmwareData.data() + FIRMWARE_HEADER_OFFSET);
//        if (!(ui->C300_BigFlash->isChecked()))                  // Bypass check if 32 MB flash used
        {
            for(i = 0; (i < maxSplashImages) && (splashLayoutError == false); i++)
            {
                // Debug only
                TempVal = splash[i].Resolution;
                TempVal = splash[i].StartAddress;
                TempVal = (uint)(FIRMWARE_DATA_OFFSET + (ImageSize  + PadSize)* i);
                TempVal = splash[i].Size;
                TempVal = (uint)(ImageSize  + PadSize);
                //
                //TODO: The following checks only fail if they are all wrong
                if( splash[i].Resolution != (uint) ResFlag &&
                    splash[i].StartAddress != (uint)(FIRMWARE_DATA_OFFSET + (ImageSize  + PadSize)* i) &&
                    splash[i].Size != (uint)(ImageSize  + PadSize) )
                {
                    showError("Firmware file layout is not supported.");
                    splashLayoutError = true;
                }
            }
        }
    }
}


/*!
 * \brief MainWindow::on_dlpc300BrowseOut_clicked
 * \n OS Request dialog for a file name/path for the firmware output file
 */
void MainWindow::on_dlpc300BrowseOut_clicked()
{
    QString startPath = ui->splashOutFile->text();
    QString fileName;

    // Browse and Select DLPC300 firmware file for Output
    fileName = QFileDialog::getSaveFileName(0,
                                            QString("Select DLPC300 Firmware Output File"),
                                            startPath,
                                            "*.bin");
    // Update line edit widget with file and path located by browse dialog
    if(!fileName.isEmpty())
    {
        ui->splashOutFile->setText(fileName);
    }
}

/*!
 * \brief MainWindow::on_comboBox_SplashIndex_currentIndexChanged
 * \param index
 * \n Request a new splash index into the output C300 firmware file
 */
void MainWindow::on_comboBox_SplashIndex_currentIndexChanged(int index)
{
//    QImage          img(PTN_WIDTH, PTN_HEIGHT, QImage::Format_RGB32);           // Configure preview image
    QSize           imgSize(ui->splashView->size());
//    QString         startPath = ui->splashFile->text();                         // Input firmware file
//    QString         outPath = ui->splashOutFile->text();                        // Output firmware file
//    QString         fileName;                                                   // ??
//    QImage          splashImage(604,684,QImage::Format_RGB16);                      //* Configure preview image
//    QImage          splashImage(427,240,QImage::Format_RGB16);                      //* Configure preview image
//    QImage          splashNativeImage(PTN_WIDTH, PTN_HEIGHT,QImage::Format_RGB16);  // Configure preview image
//global    QFile           firmWare(ui->splashFile->text());                           //* Input firmware file
//    QFile           firmWareOut(ui->splashOutFile->text());                     //* Output firmware file
    SPLASH         *splash;                                                     //* Pointer into Firmware source file for start of Splash screen
    QRgb            pixelRGB;
    int             i,j,k;
    unsigned int    pixelR, pixelG, pixelB,pixel_1,pixel_0,pixel;
    unsigned int    SplashNumber = 0;                                           //* Currently selected splash image index (pointer into firmware file)
    int             maxSplashImages;                                            //* Max number of splash images that fit in the firmware file (based on check box settings)
    unsigned long ImageSize;
    uint PadSize;
    int splashImageHeight;
    int splashImageWidth;

    if (ui->C300_BigFlash->isChecked())
    {
        if (ui->C300Flash_NativeSize->isChecked())
        {
            maxSplashImages = 4;                    // Max Native Res splash screens with BigFlash
            ImageSize = 0xCB100;
            PadSize = 0xF00;
            splashImageHeight = 684;
            splashImageWidth = 604;
        } else {
            maxSplashImages = 8;                    // Max Normal Res splash screens with BigFlash
            ImageSize = 0x33000;
            PadSize = 0x000;
            splashImageHeight = 240;
            splashImageWidth = 427;
        }
    } else {
        if (ui->C300Flash_NativeSize->isChecked())
        {
            showError("Selecting Native Resolution without the 32MB flash is not valid.");
            maxSplashImages = 1;                    // Max Native Res splash screens with BigFlash
            ImageSize = 0xCB100;
            PadSize = 0xF00;
            splashImageHeight = 684;
            splashImageWidth = 604;
        } else {
            maxSplashImages = 4;                    // Max Normal Res splash screens with Normal Flash
            ImageSize = 0x33000;
            PadSize = 0x000;
            splashImageHeight = 240;
            splashImageWidth = 427;
        }
    }
    QImage  splashImage(splashImageWidth,splashImageHeight,QImage::Format_RGB16);   //* Configure preview image

    // Check which splash image to display
    if((index == 0) || (index > maxSplashImages))
    {
        ui->splashView->clear();                                                // No splash image selected, clear image on GUI
    }
    else
    {
        if( (!(ui->splashFile->text().isEmpty())) &&                            // If the input file field is filled out and
                (splashLayoutError == false) )                                  // the check when it was loaded is ok, then assume it data is loaded
        {
            // Display selected splash image in preview window
            SplashNumber = ui->comboBox_SplashIndex->currentIndex() - 1;        // Read in which Splash Index we need to show in the preview window
            // Check Firmware source file to make sure splash data is in correct locations
            splash = (SPLASH*)(frmwareData.data() + FIRMWARE_HEADER_OFFSET);

            // Load RGB16 raw data
            //splashImage = QImage(frmwareData.data()+0x63000+(0x33000*SplashNumber),427,240,QImage::Format_RGB16);

            // For every row
            k = 0;
            for(i = 0; i < splashImageHeight; i++)
            {
                // For every column
                for(j = 0; j < splashImageWidth; j++)
                {
                    // Read pixel data bytes from firmware
                    if (ui->C300_BigFlash->isChecked())
                    {
                        pixel_0 = (unsigned int) frmwareData.at(FIRMWARE_DATA_OFFSET+(ImageSize+PadSize)*SplashNumber+k);
                        pixel_1 = (unsigned int) frmwareData.at(FIRMWARE_DATA_OFFSET+(ImageSize+PadSize)*SplashNumber+k+1);
                    } else {
                        pixel_0 = (unsigned int) frmwareData.at((splash[SplashNumber].StartAddress)+k);
                        pixel_1 = (unsigned int) frmwareData.at((splash[SplashNumber].StartAddress)+k+1);
                    }
                    pixel   = ((pixel_1 << 8) & 0xFF00) | (pixel_0 & 0xFF);     // Combine bytes into 16 bit int

                    // Shift and mask bits to separate colors
                    pixelR = ((pixel >>  5) & 0x1F)<<3;
                    pixelG = ((pixel >> 10) & 0x3F)<<2;
                    pixelB = ((pixel >>  0) & 0x1F)<<3;

                    pixelRGB = qRgb(pixelR,pixelG,pixelB);                      // Create Pixel
                    splashImage.setPixel(j,i,pixelRGB);                         // Save pixel color to image
                    k = k + 2;                                                  // Increment counter since we are doing 2 pixels per loop
                }
                // Increment counter to account for 427 not being divisible by 2
                if (!ui->C300Flash_NativeSize->isChecked())
                    k  = k + 2;
                else k +=8;
            }
            // Display image in GUI
            ui->splashView->setPixmap(QPixmap::fromImage(splashImage).scaled(imgSize));
        }
    }
}



void MainWindow::on_pushButton_ChangeSplashImage_clicked()
{    
//    QString         outPath = ui->splashOutFile->text();
    QImage          bmpImage;
    QByteArray      bmpImageBits;
    QSize           imgSize(ui->splashView->size());
//    QFile           firmWare(ui->splashFile->text());
//    QByteArray      frmwareData;
//    SPLASH         *splash;
    int             i,j;
    QRgb            pixelRGB;
    int             pixelR, pixelG, pixelB, pixel;
    int bmpImageHeight;
    int bmpImageWidth;

    int maxSplashImages;                // Maximum number of splash images (based on flash size check box)
//    int ImageSizeMag;                   // Magnification of splash image size (based on flash size check box)
    unsigned long ImageSize;            // Number of bytes in the splash image (based on image size check box)
    uint PadSize;                       // Size of padding between splash images (based on image size check box)
    uint SplashNumber;                  // Currently selected Splash image index from GUI
    QString         fileName;           // BMP file name/path to be loaded into splash screen location of firmware file
    QString         startPath = ui->splashFile->text();


    if (ui->C300_BigFlash->isChecked())
    {
        if (ui->C300Flash_NativeSize->isChecked())
        {
            maxSplashImages = 4;                    // Max Native Res splash screens with BigFlash
            ImageSize = 0xCB100;
            PadSize = 0xF00;
  //          ImageSizeMag = 4;
            bmpImageHeight = 684;
            bmpImageWidth = 604;
        } else {
            maxSplashImages = 8;                    // Max Normal Res splash screens with BigFlash
            ImageSize = 0x33000;
            PadSize = 0x000;
 //           ImageSizeMag = 4;
            bmpImageHeight = 240;
            bmpImageWidth = 427;
        }
    } else {
        if (ui->C300Flash_NativeSize->isChecked())
        {
            showError("Selecting Native Resolution without the 32MB flash is not valid.");
        } else {
            maxSplashImages = 4;                    // Max Normal Res splash screens with Normal Flash
            ImageSize = 0x33000;
            PadSize = 0x000;
 //           ImageSizeMag = 1;
            bmpImageHeight = 240;
            bmpImageWidth = 427;
        }
    }

    // Check which splash image to display
    if(ui->comboBox_SplashIndex->currentIndex() == 0)
        showError("No splash index selected");                                  // Display error. No splash index selected
    else if(ui->comboBox_SplashIndex->currentIndex() > maxSplashImages)
        showError("Splash index out of range for currently selected flash size");// Display error. Over max number selected for non-32MB flash
    else {
        SplashNumber = ui->comboBox_SplashIndex->currentIndex() - 1;            // Load appropriate image for this index position and display for user

        // Prompt user to select BMP file
        fileName = QFileDialog::getOpenFileName(0,
                                                QString("Select BMP File"),
                                                startPath,
                                                "*.bmp");
        // Check filename
        if(!fileName.isEmpty())
        {
            // Load BMP into memory
            bmpImage = QImage(fileName, "BMP");

            // Check that BMP meets size and format requirements (error if Native size checked)
            if( ((bmpImage.width()   == bmpImageWidth)  && (bmpImage.height()  == bmpImageHeight)) ||
                    ui->C300Flash_NativeSize->isChecked())  //  &&
                //(bmpImage.format()  == QImage::Format_RGB888))

            {
                if(!(ui->splashFile->text().isEmpty()))                     // If we have sucessfully entered a path then we have valid data in memory
                {
                    //Add bitmap to firmware file in memory
                    if(splashLayoutError == false)
                    {
                        // Display image in GUI
                        ui->splashView->setPixmap(QPixmap::fromImage(bmpImage).scaled(imgSize));

                        // Convert image to RGB888
                        bmpImage = bmpImage.convertToFormat(QImage::Format_RGB32, Qt::AutoColor);

                        // Convert image format from RGB to GRB For every row
                        for(i = 0; i < bmpImageHeight; i++)
                        {
                            // For every column
                            for(j = 0; j < bmpImageWidth; j++)
                            {
                                // Read pixel color values
                                pixelRGB    = bmpImage.pixel(j, i);
                                pixelR      = qRed(pixelRGB);
                                pixelG      = qGreen(pixelRGB);
                                pixelB      = qBlue(pixelRGB);

                                // Mask bits to separate colors to R5 G6 B5
                                pixelR = (32 * pixelR / 256) & 0x1F;
                                pixelG = (64 * pixelG / 256) & 0x3F;
                                pixelB = (32 * pixelB / 256) & 0x1F;

                                // Combine colors into 16 bit pixel
                                pixel = 0;
                                pixel = (pixelG << 10) | (pixelR << 5) | (pixelB);

                                // Add pixel to data array
                                bmpImageBits.append((char) (pixel & 0xFF));
                                bmpImageBits.append((char)((pixel & 0xFF00)>>8));
                            }
                            if (ui->C300Flash_NativeSize->isChecked())
                            {
                                bmpImageBits.append((char)0);
                                bmpImageBits.append((char)0);
                                bmpImageBits.append((char)0);
                                bmpImageBits.append((char)0);
                                bmpImageBits.append((char)0);
                                bmpImageBits.append((char)0);
                                bmpImageBits.append((char)0);
                                bmpImageBits.append((char)0);
                            }
                            else
                            {
                                bmpImageBits.append((char)0);
                                bmpImageBits.append((char)0);
                            }
                        }
                        // Copy splash image from BMP into firmware memory
    //                    memcpy(frmwareData.data() + 0x63000 + ((ImageSize + PadSize) * SplashNumber),bmpImageBits.constData(),ImageSize*ImageSizeMag);// bitmapData, 0x33000)
                        memcpy(frmwareData.data() + FIRMWARE_DATA_OFFSET + ((ImageSize + PadSize) * SplashNumber),bmpImageBits.constData(),ImageSize);// bitmapData, 0x33000)
                    }
                } else {
                    showError("No source file loaded");
                }
            } else {
                showError("Invalid image size. Image must be 427x240 with standard flash size.");
            }
        } else {
            showError("Invalid filename");                  // Filename didn't exist, prompt user
        }
    }
}

void MainWindow::on_pushButton_WriteFirmwareFile_clicked()
{
    QFile   firmWareOut(ui->splashOutFile->text());

    // Rewrite the firmware file with the new splash image
    if(!firmWareOut.open(QIODevice::WriteOnly))
    {
        showError("Unable to open output file.");
    } else {
        firmWareOut.write(frmwareData,frmwareData.size());
        firmWareOut.close();

        QMessageBox msgBox(QMessageBox::Information,
                           "LC2000 nHD Message",
                           "Splash image successfully written.",
                           QMessageBox::NoButton,
                           this);
        msgBox.exec();                                          // Notify User
    }
}
/////////////////////////////////////////////////////////////////////////////////////
/// End of Splash screen utilities interface
///
///

void MainWindow::on_radioVideoMode_clicked()
{
    ui->groupBox_VideoStreaming->setEnabled(1);
    ui->groupBox_ExtPatSettings->setEnabled(0);
}

void MainWindow::on_radioExtPatternMode_clicked()
{
    ui->groupBox_ExtPatSettings->setEnabled(1);
    ui->groupBox_VideoStreaming->setEnabled(0);
}



void MainWindow::on_LEDCurRed_textChanged(const QString &arg1)
{
    int current = (arg1.toInt()*1.8)+140;
    ui->label_LEDRedCurrent->setText(QString::number(current)+" mA" );

    if( arg1.toInt() > LED_MAX_CURRENT_ACTIVE )
    {
        ui->LEDCurRed->setStyleSheet("QLineEdit#LEDCurRed{color:red}");
    }
    else
    {
        ui->LEDCurRed->setStyleSheet("QLineEdit#LEDCurRed{color:black}");
    }

    CheckLEDCurrent();
}

void MainWindow::on_LEDCurGreen_textChanged(const QString &arg1)
{
    int current = (arg1.toInt()*1.8)+140;
    ui->label_LEDGreenCurrent->setText(QString::number(current)+" mA" );

    if( arg1.toInt() > LED_MAX_CURRENT_ACTIVE )
    {
        ui->LEDCurGreen->setStyleSheet("QLineEdit#LEDCurGreen{color:red}");
    }
    else
    {
        ui->LEDCurGreen->setStyleSheet("QLineEdit#LEDCurGreen{color:black}");
    }

    CheckLEDCurrent();
}

void MainWindow::on_LEDCurBlue_textChanged(const QString &arg1)
{
    int current = (arg1.toInt()*1.8)+140;
    ui->label_LEDBlueCurrent->setText(QString::number(current)+" mA" );

    if( arg1.toInt() > LED_MAX_CURRENT_ACTIVE )
    {
        ui->LEDCurBlue->setStyleSheet("QLineEdit#LEDCurBlue{color:red}");
    }
    else
    {
        ui->LEDCurBlue->setStyleSheet("QLineEdit#LEDCurBlue{color:black}");
    }

    CheckLEDCurrent();
}

void MainWindow::CheckLEDCurrent()
{
    int currentRed   = ui->LEDCurRed->text().toInt();
    int currentGreen = ui->LEDCurGreen->text().toInt();
    int currentBlue  = ui->LEDCurBlue->text().toInt();

    if( currentRed   > LED_MAX_CURRENT_ACTIVE ||
        currentGreen > LED_MAX_CURRENT_ACTIVE ||
        currentBlue  > LED_MAX_CURRENT_ACTIVE )
    {
        ui->LEDSetting_Notes->setText("NOTE: Setting Not Recommended!");
        //ui->groupBox_LEDCurrent->setTitle("LED Current (Setting Not Recommended!)");
    }
    else if( currentRed   > LED_MAX_CURRENT_PASSIVE ||
             currentGreen > LED_MAX_CURRENT_PASSIVE ||
             currentBlue  > LED_MAX_CURRENT_PASSIVE)
    {
        ui->LEDSetting_Notes->setText("NOTE: Active Cooling Needed!");
        //ui->groupBox_LEDCurrent->setTitle("LED Current (Active Cooling Needed)");
    }
    else
    {
        ui->LEDSetting_Notes->setText("");
        //ui->groupBox_LEDCurrent->setTitle("LED Current:");
    }
}

/*!
 * \brief MainWindow::on_uploadMBMCSeq_clicked
 * \n Upload the selected MBMC sequence
 */
void MainWindow::on_uploadMBMCSeq_clicked()
{
    if(packetHandler.writeCommand(0x0A00))
        return;

    if(sendFile(ui->MBMCSeqFile->text()))
        return;
}

void MainWindow::on_btnMBMCImport_clicked()
{
    QString fileName;
    QString txtString;

    fileName = QFileDialog::getOpenFileName(this,
                                            QString("Select ReadMe File"),
                                            readmeFilePath,
                                            "*.txt");


    if(!fileName.isEmpty())
    {
        ui->MBMCReadmeFile->setText(fileName);
        seqFilePath = fileName;
        readmeFilePath = fileName;
        imagelistFilePath = fileName;

        TxtFile.ParseReadmeTextFile(fileName);

        // Load info in MBMC tab
        ui->MBMCReadme_Exposure->setText(TxtFile.Exposure);
        ui->MBMCReadme_MinTriggerPeriod->setText(TxtFile.MinTrigger);
        ui->MBMCSeqStartVec->setText(TxtFile.StartVector);
        ui->MBMCSeqNumVec->setText(TxtFile.NumOfVectors);
        ui->MBMCReadme_Patterns->setText(QString::number(TxtFile.pattern_count));
        ui->trigType->setCurrentIndex(1);   // Set trigger to auto

        // Load info in pattern settings tab
        ui->exposureTime->setText(TxtFile.Exposure);
        ui->trigPeriod->setText(TxtFile.MinTrigger);
        ui->numPatterns->setCurrentIndex(ui->numPatterns->findText(QString::number(TxtFile.pattern_seq_1bpp_planes)));

        if(TxtFile.error == MBMCREADMEPARSE_ERROR)
        {
            showError("Could not import Readme file");
            ui->MBMCReadmeFile->setText("");
            TxtFile.pattern_count = 0;
        }

        // Clear Image list text box and reset MBMC patterns groupbox items
        ui->MBMCImageList->setText("");
        ui->comboBox_MBMCPatterns->clear();
        ui->patternView_MBMC->clear();
    }
}

void MainWindow::on_enableCustSeq_clicked(bool checked)
{
    // Check state of checkbox
    if(checked)
    {
        // Checkbox is checked
        // Enable the MBMC settings and pattern groupboxes
        ui->groupBox_MBMCSettings->setEnabled(true);
        ui->groupBox_MBMCPatterns->setEnabled(true);

        // Set bit depth to 1
        ui->bitDepth->setCurrentIndex(0);

        // Set Pattern work
        ui->PatternType->setCurrentIndex(0);

        // Set LED Select
        ui->LEDSelect->setCurrentIndex(3);

        // Disable pattern sequence: bit depth, numPatterns, Pattern Type, and LED Select
        ui->bitDepth->setEnabled(false);
        ui->numPatterns->setEnabled(false);
        ui->PatternType->setEnabled(false);
        ui->LEDSelect->setEnabled(false);

    }
    else
    {
        // Checkbox is not checked
        // Disable the MBMC settings groupbox
        ui->groupBox_MBMCSettings->setEnabled(false);
        ui->groupBox_MBMCPatterns->setEnabled(false);

        // Enable pattern sequence: bit depth, numPatterns, Pattern Type, and LED Select
        ui->bitDepth->setEnabled(true);
        ui->numPatterns->setEnabled(true);
        ui->PatternType->setEnabled(true);
        ui->LEDSelect->setEnabled(true);
    }
}

void MainWindow::on_browseMBMCImageList_clicked()
{
    int         i,j;
    QString     fileName;
    QString     fileAbsDir;
    QString     fileAbsName;
    QString     txtString;
    QString     fileLine;
    QString     bitPlaneName;
    BMPParser   ImageParser;
    QStringList fileImages;


    int parseResult;

    if(TxtFile.pattern_count == 0)
    {
        showError("Please import valid Readme file first.");
        return;
    }

    fileName = QFileDialog::getOpenFileName(this,
                                            QString("Select ReadMe File"),
                                            imagelistFilePath,
                                            "*.txt");
    if(!fileName.isEmpty())
    {
        ui->MBMCImageList->setText(fileName);
        seqFilePath = fileName;
        readmeFilePath = fileName;
        imagelistFilePath = fileName;

        // Open the image list and read in the images
        QFile       fileImageList(fileName);
        QFileInfo   fileImageListInfo(fileName);

        if( fileImageList.open(QIODevice::ReadOnly) )
        {
            // Read in however many patterns were specificed in Readme file
            for(i=0;i<TxtFile.pattern_count;i++)
            {
                if(!fileImageList.atEnd())
                {
                    // Read image name from file
                    fileImages.append(fileImageList.readLine());
                }
                else
                {
                    // Not enough images listed in file
                    showError("The selected Image List file has fewer patterns than the Readme file.");
                    return;
                }
            }
        }
        else
        {
            showError("Could not open Image List file");
            ui->MBMCImageList->setText("");
            return;
        }


        // Check that Image List contains same number of patterns that the Readme Txt contained
        if(TxtFile.pattern_count == fileImages.count())
        {

            // Get image path directory
            fileAbsDir = fileImageListInfo.absolutePath();

            // Parse all of the pattern images
            for(i=0;i<TxtFile.pattern_count;i++)
            {
                // Load pattern image

                // Create full path filename
                fileAbsName.clear();
                fileAbsName.append(fileAbsDir);
                fileAbsName.append("/");
                fileAbsName.append(fileImages.at(i));

                // Remove new line character
                fileAbsName = fileAbsName.simplified();

                // Add name to MBMC image file name list
                patternFileMBMC[i].setFile(fileAbsName);


                // Check if bitplanes directory already exists and create it if not
                if(!QDir(fileAbsDir+"/bitplanes").exists()) QDir().mkdir(fileAbsDir+"/bitplanes");

                parseResult = ImageParser.LoadBMP(fileAbsName, TxtFile.pattern_bit_depth.at(i));
                if(parseResult == BMPPARSER_LOADED)
                {
                    // Parse the image into individual bitplanes
                    parseResult = ImageParser.Parse();
                    if(parseResult == BMPPARSER_SUCCESS)
                    {
                        // Go through the bitplanes from Readme file and save the parsed bitplanes
                        for(j = 0; j < TxtFile.pattern_seq_1bpp_planes; j++)
                        {
                            bitPlaneName.clear();
                            // Is bit plane for the sequence part of the parsed image?
                            if(TxtFile.pattern_seq_pattern.at(j) == i+1)
                            {
                                bitPlaneName.append(fileAbsDir+"/bitplanes/");
                                if(j<10)bitPlaneName.append("0");
                                bitPlaneName.append(QString::number(j)+"_PAT.bmp");
                                ImageParser.SaveBitPlane(bitPlaneName,TxtFile.pattern_seq_bitplane.at(j));

                                // Add bitplane image location to GUI
                                patternFile[j].setFile(bitPlaneName);

                            }
                        }
                    }
                    else
                    {
                        showError("Image pattern could not be successfully parsed.");
                        ui->MBMCImageList->setText("");
                        return;
                    }
                }
                else
                {
                    // BMP did not load properly. Give Error message
                    switch(parseResult)
                    {
                        case BMPPARSER_ERROR_SIZE:
                            showError("Image is not the correct resolution of 604x684.");
                            break;
                        case BMPPARSER_ERROR_FORMAT:
                            showError("Image is not the correct format.");
                            break;
                        case BMPPARSER_ERROR_FILE:
                            showError("Image file could not be opened or does not exist.");
                            break;
                    }
                    ui->MBMCImageList->setText("");
                    return;
                }
            }

            // Create the blank bitplane images
            for(j=0;j<TxtFile.pattern_seq_1bpp_planes;j++)
            {
                bitPlaneName.clear();
                if(TxtFile.pattern_seq_pattern.at(j) == ONE_BPP_BLACK_PATTERN_INDEX)
                {
                    bitPlaneName.append(fileAbsDir+"/bitplanes/");
                    if(j<10)bitPlaneName.append("0");
                    bitPlaneName.append(QString::number(j)+"_PAT.bmp");
                    ImageParser.blankImage(bitPlaneName);

                    // Add bitplane image location to GUI
                    patternFile[j].setFile(bitPlaneName);
                }
            }

            // Load first pattern into GUI on sequence setting
            fillNumPatterns();
            emit on_patternNum_currentIndexChanged(0);
            ptnImagePath = patternFile[0].absolutePath();

            // Load the comboBox_MBMCPatterns
            ui->comboBox_MBMCPatterns->clear();
            for(i=0;i<TxtFile.pattern_count;i++)
            {
                ui->comboBox_MBMCPatterns->addItem(QString::number(i+1));
            }
            ui->comboBox_MBMCPatterns->setCurrentIndex(0);
        }
        else
        {
            showError("Image list contains more images than the Readme file.");
            ui->MBMCImageList->setText("");
            return;
        }
    }
}

void MainWindow::on_btn_SendMBMCSettings_clicked()
{
    // Check that lightcrafter is in correct mode
    if(ui->modeList->currentIndex() != 3)
    {
        showError("Please set the LC2000 nHD's Display Mode to 'Internal Pattern Sequence'");
        return;
    }

    // Send vector information
    if(packetHandler.writeCommand(0x0A01))              return;
    if(putTextIntData(ui->MBMCSeqStartVec->text(), 1))  return;
    if(putTextIntData(ui->MBMCSeqNumVec->text(), 1))    return;
    if(packetHandler.sendPacket())return;

    // Send sequence file
    if(packetHandler.writeCommand(0x0A00))  return;
    if(sendFile(ui->MBMCSeqFile->text()))   return;
}

void MainWindow::on_radioVideoMode_toggled(bool checked)
{
    // If in HDMI Video Mode then disable the trigger
    if(checked && (ui->modeList->currentIndex() == 2))
    {
        ui->camTrigEnable->setChecked(false);
        ui->camTrigEnable->setEnabled(false);
    }
    else
    {
        ui->camTrigEnable->setEnabled(true);
    }
}

void MainWindow::on_modeList_currentIndexChanged(int index)
{
    if((index == 3) ||
      ((index == 2) && (ui->radioVideoMode->isChecked() == false)))
    {
        ui->camTrigEnable->setEnabled(true);
    }
    else
    {
        ui->camTrigEnable->setEnabled(false);
        ui->camTrigEnable->setChecked(false);
    }
}

void MainWindow::on_comboBox_MBMCPatterns_currentIndexChanged(int index)
{
    int x,y,pixel;
    QColor colorpixel;
    QImage img(PTN_WIDTH, PTN_HEIGHT, QImage::Format_RGB32);
    QImage imgMono(PTN_WIDTH,PTN_HEIGHT,QImage::Format_Indexed8);
    QSize imgSize(ui->patternView_MBMC->size());

    if(patternFileMBMC[index].isFile())
    {
        // Load the greyscale image
        imgMono.load(patternFileMBMC[index].absoluteFilePath());

        // Convert to color according to MBMC sequence settings
        for(x=0;x<PTN_WIDTH;x++)
        {
            for(y=0;y<PTN_HEIGHT;y++)
            {
                pixel = imgMono.pixelIndex(x,y);

                // Mask the pixel if not 1bpp already
                if(imgMono.format() == QImage::Format_Mono)
                {
                    switch(TxtFile.pattern_color.at(index))
                    {
                        case 'R':
                            colorpixel.setRgb(!pixel*255,0,0);
                            break;
                        case 'G':
                            colorpixel.setRgb(0,!pixel*255,0);
                            break;
                        case 'B':
                            colorpixel.setRgb(0,0,!pixel*255);
                            break;
                    }
                }
                else
                {
                    int adj = TxtFile.pattern_bit_depth.at(index);

                    pixel = 255 * pixel / (0x1 << (adj-1));

                    switch(TxtFile.pattern_color.at(index))
                    {
                        case 'R':
                            colorpixel.setRgb(pixel,0,0);
                            break;
                        case 'G':
                            colorpixel.setRgb(0,pixel,0);
                            break;
                        case 'B':
                            colorpixel.setRgb(0,0,pixel);
                            break;
                    }
                }
                img.setPixel(x,y, colorpixel.rgb());
            }
        }

        ui->patternView_MBMC->setPixmap(QPixmap::fromImage(img).scaled(imgSize));
    }
    else
    {
        ui->patternView_MBMC->clear();
    }
}

void MainWindow:: on_updatePowerMode_clicked()

{

    QString data;
    packetHandler.writeCommand(0x0110);

    if ( ui->PowerEnable->isChecked())
        packetHandler.putData( (quint32) 1 , 1);
    else
        packetHandler.putData( (quint32) 0 , 1);

    if(packetHandler.sendPacket())
        return;

}


void MainWindow:: on_setGamma_clicked()

{

    QString data;
    packetHandler.writeCommand(0x0130);


    packetHandler.putData( (quint32) ui->selectGamma->currentIndex() , 1);


    if(packetHandler.sendPacket())
        return;

}

