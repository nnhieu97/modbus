#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>

#include <modbus4qt/tcp_client.h>

class QGroupBox;
class QLineEdit;
class QSpinBox;
class QMenu;

class MainWindow : public QMainWindow
{
    Q_OBJECT

    private:
        QAction* exitAction_;
        QAction* aboutAction_;
        QAction* aboutQtAction_;

        QMenu* fileMenu_;
        QMenu* helpMenu_;

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

        QGroupBox* serverGroupBox_;

        QLineEdit* serverAddress_;

        QSpinBox* serverPort_;

        modbus4qt::TcpClient* tcpClient;

    public:
        MainWindow(QWidget *parent = 0);
        ~MainWindow();

    public slots:
        void about();

        void errorMessage(const QString& msg);
        void infoMessage(const QString& msg);

        void readCoils();
        void readDiscreteInputs();
        void readInputRegistres();
        void readHoldingRegisters();

        void writeCoils();
        void writeHoldingRegisters();

    private:
        void createActions_();

        void createCoilsGroupBox_();

        void createDiscreteInputsGroupBox_();

        void createInputRegistersGroupBox_();

        void createHoldingRegistersGroupBox_();

        void createMenus_();

        void createServerGroupBox_();
};

#endif // MAIN_WINDOW_H
