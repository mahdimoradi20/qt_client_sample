#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "client.h"
#include<QMessageBox>
#include<QJsonDocument>
#include "dialog.h"

namespace Ui {
class MainDialog;
}

class MainDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MainDialog(QWidget *parent = nullptr , QString host = "127.0.0.1" , QString username = "" , QString password = "");
    ~MainDialog();

private slots:
    void success();
    void data_recived(QJsonDocument data);

private:
    Ui::MainDialog *ui;

    QString host;
    QString username;
    QString password;

    Client client;

    bool connected = false;
};

#endif // MAINDIALOG_H
