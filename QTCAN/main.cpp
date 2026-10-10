
#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QMessageBox>
#include <QDateTime>

class CANWindow : public QWidget
{
public:
    CANWindow(QWidget *parent = nullptr)
        : QWidget(parent)
    {
        setWindowTitle("Qt CAN Bus POC");
        resize(550, 400);

        auto *layout = new QVBoxLayout(this);

        auto *connectBtn = new QPushButton("Connect to vcan0");
        auto *sendBtn = new QPushButton("Send CAN Frame");
        auto *disconnectBtn = new QPushButton("Disconnect");

        log = new QTextEdit();
        log->setReadOnly(true);

        layout->addWidget(new QLabel("CAN Bus Monitor"));
        layout->addWidget(connectBtn);
        layout->addWidget(sendBtn);
        layout->addWidget(disconnectBtn);
        layout->addWidget(log);

        connect(connectBtn, &QPushButton::clicked,
                this, [this]() { connectCAN(); });

        connect(sendBtn, &QPushButton::clicked,
                this, [this]() { sendCAN(); });

        connect(disconnectBtn, &QPushButton::clicked,
                this, [this]() {
                    if (device)
                        device->disconnectDevice();
                });
    }

private:
    QCanBusDevice *device = nullptr;
    QTextEdit *log = nullptr;

    void connectCAN()
    {
        if (device) {
            log->append("Device already created");
            return;
        }

        QString error;

        device = QCanBus::instance()->createDevice(
            "socketcan", "vcan0", &error);

        if (!device) {
            log->append("Error: " + error);
            return;
        }

        connect(device, &QCanBusDevice::framesReceived,
                this, [this]() { receiveCAN(); });

        connect(device, &QCanBusDevice::errorOccurred,
                this, [this](QCanBusDevice::CanBusError) {
                    log->append("CAN error: " +
                                device->errorString());
                });

        if (!device->connectDevice()) {
            log->append("Connection failed: " +
                        device->errorString());
            device->deleteLater();
            device = nullptr;
            return;
        }

        log->append("Connected to vcan0");
    }

    void sendCAN()
    {
        if (!device ||
            device->state() != QCanBusDevice::ConnectedState) {
            log->append("CAN not connected");
            return;
        }

        // Standard 11-bit CAN identifier
        const quint32 canId = 0x123;

        // Classic CAN: 8 data bytes
        QByteArray payload =
            QByteArray::fromHex("0102030405060708");

        QCanBusFrame frame(canId, payload);

        if (device->writeFrame(frame)) {
            log->append("TX ID: 0x123 Data: " +
                        QString(payload.toHex(' ')));
        } else {
            log->append("TX failed: " +
                        device->errorString());
        }
    }

    void receiveCAN()
    {
        while (device && device->framesAvailable()) {
            const QCanBusFrame frame = device->readFrame();

            QString id = QString::number(frame.frameId(), 16)
                             .toUpper();

            QString data =
                QString(frame.payload().toHex(' '));

            log->append(
                "RX ID: 0x" + id +
                " Data: " + data);
        }
    }
};

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    CANWindow window;
    window.show();

    return app.exec();
}
