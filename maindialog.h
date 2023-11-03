#ifndef MAINDIALOG_H
#define MAINDIALOG_H

#include <QDialog>
#include "client.h"
#include<QMessageBox>
#include<QJsonDocument>
#include<QColorDialog>
#include<QStandardItem>
#include<QStandardItemModel>
#include<QSettings>

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

    void on_checkAgreement_stateChanged(int arg1);

    void on_spinHealth_textChanged(const QString &arg1);

    void on_btnColor_clicked();

    void init_set_places_model();

    void on_btnSubmit_clicked();


    void on_btnExit_clicked();

private:
    Ui::MainDialog *ui;

    QString host;
    QString username;
    QString password;
    QString color = "none";
    QString place = "none";

    Client client;

    QStandardItemModel model;

    bool connected = false;

    QString validate();
    void clearInputs();
    void logout();


    // QWidget interface
protected:
    void closeEvent(QCloseEvent *event);
};


#endif // MAINDIALOG_H
