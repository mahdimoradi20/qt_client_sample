#include "maindialog.h"
#include "ui_maindialog.h"

MainDialog::MainDialog(QWidget *parent , QString host , QString username , QString password) :
    QDialog(parent),
    ui(new Ui::MainDialog)
{
    ui->setupUi(this);

    this->host = host;
    this->username = username;
    this->password = password;



    connect(&client , &Client::success , this , &MainDialog::success);
    connect(&client , &Client::data_recived , this , &MainDialog::data_recived);

    client.start(this->host,8080);

}

MainDialog::~MainDialog()
{
    delete ui;
}

void MainDialog::success()
{
    this->connected = true;

    //Send login data to server
    QVariantMap map;
    map.insert("action" , "login");
    map.insert("username" ,this->username);
    map.insert("password" ,this->password);

    client.send(map);

}

void MainDialog::data_recived(QJsonDocument data)
{


    if(data["action"].toString() == "login"){

        if(data["status"].toString() == "ok"){
            ui->txtHeader->setText(" کاربر " + data["full_name"].toString() + " خوش آمدید ");
            return;
        }

        if(data["status"].toString() == "error" && data["message"].toString().contains("wrong")){

            QMessageBox::information(this , "خطا" , "نام کاربری یا رمز عبور اشتباه است");

        }else{
            QMessageBox::information(this , "خطا" , "سرور پاسخگو نیست");

        }

        Dialog *dialog = new Dialog();
        dialog->show();
        this->close();
    }
}

void MainDialog::on_checkAgreement_stateChanged(int arg1)
{
    if(arg1)ui->btnSubmit->setEnabled(true);
    else ui->btnSubmit->setEnabled(false);

}


void MainDialog::on_spinHealth_textChanged(const QString &arg1)
{
    ui->progressHealth->setValue(arg1.toInt());
}


void MainDialog::on_btnColor_clicked()
{
    QColor color = QColorDialog::getColor(Qt::blue, this );
    if( color.isValid() )
    {
        ui->btnColor->setText(color.name());
        ui->btnColor->setStyleSheet("background-color: " + color.name());

    }else{
        QMessageBox::critical(this , "خطا" , "رنگ انتخاب شده معتبر نیست");
    }
}

