#include "mywidget.h"
#include "ui_mywidget.h"

myWidget::myWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::myWidget)
{
    ui->setupUi(this);

    connect(ui->closeButton, &QPushButton::clicked, this, &QWidget::close);
}

myWidget::~myWidget()
{
    delete ui;
}
