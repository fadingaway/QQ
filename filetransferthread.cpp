#include "filetransferthread.h"
#include "QTcpSocket"
#include "QFile"
#include "QTextStream"
#include "QByteArray"
#include "QDataStream"
#include "QFileDialog"

fileTransferThread::fileTransferThread(QObject *parent, QString filePath):QThread(parent)
{
    this->filePath = filePath;
    loadSize = 4*1024;
    totalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    tcpClient = new QTcpSocket(this);
    //当连接服务器成功时，发出connected()信号，我们开始传送文件
    connect(tcpClient,SIGNAL(connected()),this,SLOT(startTransfer()));
    //当有数据发送成功时，我们更新进度条
    connect(tcpClient,SIGNAL(bytesWritten(qint64)),this,
           SLOT(updateClientProgress(qint64)));
    connect(tcpClient,SIGNAL(error(QAbstractSocket::SocketError)),this,
           SLOT(displayError(QAbstractSocket::SocketError)));
}

void fileTransferThread::run()
{
    bytesWritten = 0;
    tcpClient->connectToHost("127.0.0.1", "22");
}

void fileTransferThread::startTransfer()
{
    localFile = new QFile(filePath);
    if(!localFile->open(QFile::ReadOnly))
    {
        qDebug()<<"Open File Error!";
        emit transferError("Open File Error!");
        return;
    }

    totalBytes = localFile->size();
    QDataStream sendOut(&outBlock, QIODevice::WriteOnly);
    sendOut.setVersion(QDataStream::Qt_5_7);

    QString currentFileName = filePath.right(filePath.size() - filePath.lastIndexOf('/')-1);
    sendOut<<qint64(0)<<qint64(0)<<currentFileName;
    totalBytes+= outBlock.size();
    sendOut.device()->seek(0);
    sendOut<<totalBytes<<qint64(outBlock.size()-sizeof(qint64)*2);
    bytesToWrite = totalBytes - tcpClient->write(outBlock);
    outBlock.resize(0);
}

void fileTransferThread::updateClientProcess(qint64 numBytes)
{
    bytesWritten += (int)numBytes;
    if(bytesWritten >0)
    {
        outBlock = localFile->read(qMin(bytesToWrite, loadSize));
        bytesToWrite -= (int)tcpClient->write(outBlock);
        outBlock.resize(0);
    }
    else
    {
        localFile->close();
    }
    emit updateProgress(totalBytes, bytesWritten);
    if(bytesWritten == totalBytes)
    {
        localFile->close();
        tcpClient->close();
    }
}

void fileTransferThread::displayError(QAbstractSocket::SocketError) //显示错误
{
    qDebug() << tcpClient->errorString();
    tcpClient->close();
    emit transferError(tcpClient->errorString());
}
