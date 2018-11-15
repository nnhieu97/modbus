#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QWidget>

#include "rtu_client.h"

class QGroupBox;
class QLineEdit;
class QSpinBox;
class QComboBox;
class QSettings;

class MainWindow : public QWidget
{
    Q_OBJECT

    private:
        QSpinBox* coilStart_;
        QSpinBox* coilQty_;
        QSpinBox* coilValue_;

        QSpinBox* dInStart_;
        QSpinBox* dInQty_;

        QSpinBox* inRegStart_;
        QSpinBox* inRegQty_;

        QSpinBox* holdRegStart_;
        QSpinBox* holdRegQty_;
        QSpinBox* holdRegValue_;

        QGroupBox* coilsGroupBox_;
        QGroupBox* discreteInputsGroupBox_;
        QGroupBox* inputRegistersGroupBox_;
        QGroupBox* holdingRegistersGroupBox_;

        QGroupBox* connectionSettingsGroupBox_;
        QComboBox *serialPortComboBox_;
        QSpinBox* unitId_;
        QComboBox* baudRateComboBox_;
        QComboBox* dataBitsComboBox_;
        QComboBox* stopBitsComboBox_;
        QComboBox* parityComboBox_;

        QGroupBox* userDefinedFunctionGroupBox_;
        QSpinBox* functionCode_;
        QSpinBox* subFunctionCode_;

        QSettings* settings_;

        modbus4qt::RtuClient* mbClient_;

        void createCoilsGroupBox_();
        void createDiscreteInputsGroupBox_();
        void createInputRegistersGroupBox_();
        void createHoldingRegistersGroupBox_();
        void createUserDefinedFunctionGroupBox_();

        void createConnectionSettingsGroupBox_();

        void restoreSettings_();

        void saveSettings_();

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void changePortName(int i);
        void changeBaudRate(int i);
        void changeDataBits(int i);
        void changeStopBits(int i);
        void changeParity(int i);

        void closeEvent(QCloseEvent *event);

        void errorMessage(const QString& msg);
        void infoMessage(const QString& msg);

        void readCoils();
        void readDiscreteInputs();
        void readInputRegistres();
        void readHoldingRegisters();

        void sendUserDefinedFunction();

        void setUnitId(int id);

        void writeCoils();
        void writeHoldingRegisters();
};

#endif // MAIN_WINDOW_H
