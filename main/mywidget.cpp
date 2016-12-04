#include "mywidget.h"
#include "ui_mywidget.h"
#include "QDebug"
#include <QMessageBox>
#include <QPalette>

myWidget::myWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myWidget)
{
    ui->setupUi(this);

    pe.setColor(QPalette::WindowText, Qt::red);
    ui->statetext->setPalette(pe);
    ui->statetext->setText("未连接");
    setFixedSize(440, 395);

    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
    connect(ui->sendmsgbutton, &QPushButton::clicked, this, &myWidget::slotsendmsgbutton);
    connect(ui->serverbutton, &QPushButton::clicked, this, &myWidget::slotstartTcpserver);
    connect(ui->connectip, &QPushButton::clicked, this, &myWidget::slotconnetserver);
}

myWidget::~myWidget()
{
    delete ui;
}

void myWidget::slotsendmsgbutton()
{
    const QString msgStr = ui->inputEdit->toPlainText();
    ui->outputEdit->setAlignment(Qt::AlignRight);
    ui->outputEdit->insertPlainText(msgStr + "\n");
    m_socket->write(msgStr.toStdString().c_str(), strlen(msgStr.toStdString().c_str()));

    ui->inputEdit->clear();
}

void myWidget::slotstartTcpserver()
{
    m_server = new QTcpServer(this);
    if (!m_server->isListening())
    {
        if(m_server->listen(QHostAddress::Any, 33333)) //监听任何脸上66666端口的
        {
            QMessageBox::about(NULL, "提示", "打开监听端口成功");
        }else
        {
            QMessageBox::about(NULL, "提示", "打开监听端口失败");
        }
    }else
    {
        QMessageBox::about(NULL, "提示", "正在监听中。。。");
    }
    connect(m_server, &QTcpServer::newConnection, this, &myWidget::slotnewconnet);
}

void myWidget::slotnewconnet()
{
    pe.setColor(QPalette::WindowText, Qt::blue);
    ui->statetext->setPalette(pe);
    ui->statetext->setText("已连接");
    QMessageBox::about(NULL, "提示", "连接成功，可以开始使用了");

    m_socket = m_server->nextPendingConnection(); //得到连接进来的socket
    connect(m_socket, &QTcpSocket::disconnected, m_socket, &QTcpSocket::deleteLater);
    connect(m_socket, &QTcpSocket::readyRead, this, &myWidget::slotreadmessage);
}

void myWidget::slotreadmessage()
{
    QByteArray amsg = m_socket->readAll(); //去读数据
    QString smsg = QVariant(amsg).toString();
    ui->outputEdit->setAlignment(Qt::AlignLeft);
    ui->outputEdit->insertPlainText(smsg + "\n");
}

void myWidget::slotconnetserver()
{
    QHostAddress m_address;
    m_address.setAddress(QObject::tr(ui->iplineedit->text().toStdString().c_str()));
    m_socket = new QTcpSocket(this);
    m_socket->abort();
    m_socket->connectToHost(m_address, 33333);
    if (m_socket->waitForConnected(1000))
    {
        pe.setColor(QPalette::WindowText, Qt::blue);
        ui->statetext->setPalette(pe);
        ui->statetext->setText("已连接");
        QMessageBox::about(NULL, "提示", "连接成功");
    }else
    {
        QMessageBox::about(NULL, "提示", "连接超时");
    }
    connect(m_socket, &QTcpSocket::readyRead, this, &myWidget::slotreadmessage);
}
