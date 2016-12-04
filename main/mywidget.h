#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QWidget>
#include <QtGui>
#include <QTcpServer>
#include <QTcpSocket>

namespace Ui {
class myWidget;
}

class myWidget : public QWidget
{
    Q_OBJECT

public:
    explicit myWidget(QWidget *parent = 0);
    ~myWidget();

    void slotsendmsgbutton();

public slots:
    void slotstartTcpserver();
    void slotnewconnet();
    void slotreadmessage();
    void slotconnetserver();

private:
    Ui::myWidget *ui;
    QPalette pe;

    QTcpServer * m_server;
    QTcpSocket * m_socket;
    QTcpSocket * m_csocket;

};

#endif // MYWIDGET_H
