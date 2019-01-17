#ifndef FILETRANSFERTHREAD_H
#define FILETRANSFERTHREAD_H

#include <QWidget>
#include "QObject"
#include "QThread"
#include "QTcpSocket"

class fileTransferThread : public QThread
{
    Q_OBJECT
public:
    explicit fileTransferThread(QObject *parent, QString filePath);
protected:
    void run();
private:
    QTcpSocket *tcpClient;
    QString filePath;
    QFile *localFile;
    qint64 totalBytes;
    qint64 bytesWritten;
    qint64 bytesToWrite;
    qint64 loadSize;
    QString fileName;
    QByteArray outBlock;
private slots:
    void startTransfer();
    void updateClientProcess(qint64);
    void displayError(QAbstractSocket::SocketError);
    void openFile();
signals:
    void updateProgress(qint64 totalBytes, qint64 bytesWritten);
    void transferError(QString errorString);
};

#endif // FILETRANSFERTHREAD_H
