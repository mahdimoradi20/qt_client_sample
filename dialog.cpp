#include "dialog.h"
#include "./ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_btnLogin_clicked()
{
    QString validation =validate();
    if(validation == "none"){
        //Move to main dialog
        MainDialog *dialog = new MainDialog(nullptr , ui->textIPAddress->text() , ui->textUsername->text() , ui->textPassword->text());
        dialog->show();
        this->close();
    }else{
        QMessageBox::warning(this , "خطا" , validation);
    }
}

QString Dialog::validate()
{


    if(ui->textIPAddress->text() == "") return "آدرس سرور نمیتواند خالی باشد";
    if(ui->textUsername->text() == "") return "نام کاربری نمیتواند خالی باشد";
    if(ui->textPassword->text() == "") return "کلمه عبور نمیتواند خالی باشد";
    return "none";

}

