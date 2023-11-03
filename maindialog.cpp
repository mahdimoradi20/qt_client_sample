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

    init_set_places_model();

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


    qInfo() << data;

    if(data["action"].toString() == "login"){

        if(data["status"].toString() == "ok"){
            ui->txtHeader->setText(" کاربر " + data["full_name"].toString() + " خوش آمدید ");
            QSettings qsettings("config.cfg");
            qsettings.setValue("access_token" ,data["access_token"].toString());
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
    }else if(data["action"].toString() == "submit_form"){
        if(data["status"].toString() == "ok"){
            clearInputs();

            QMessageBox::information(this , "موفق" , "اطلاعات با موفقیت ثبت شد");
            return;
        }else{
            QMessageBox::critical(this , "خطا" , "در ثبت اطلاعات خطایی پیش آمد" );
        }


    }else if(data["action"].toString() == "logout"){
        if(data["status"].toString() == "ok"){
            this->close();
            return;
        }else{
            QMessageBox::critical(this , "خطا" , "خطایی پیش آمد" );
        }

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
        this->color = color.name();

    }else{
        QMessageBox::critical(this , "خطا" , "رنگ انتخاب شده معتبر نیست");
    }
}

void MainDialog::init_set_places_model()
{
    QStandardItem *root = model.invisibleRootItem();
    QStringList headers;
    headers << "استان و شهر ها";
    model.setHorizontalHeaderLabels(headers);

    QStandardItem *esfahan = new QStandardItem("اصفهان");
    QStandardItem *shareza = new QStandardItem("شهرضا");
    QStandardItem *golpaygan = new QStandardItem("گلپایگان");


    QStandardItem *tehran = new QStandardItem("تهران");
    QStandardItem *firozkoh = new QStandardItem("فیروز کوه");
    QStandardItem *damavand = new QStandardItem("دماوند");


    root->appendRow(esfahan);
    root->appendRow(tehran);
    esfahan->appendRow(shareza);
    esfahan->appendRow(golpaygan);
    tehran->appendRow(firozkoh);
    tehran->appendRow(damavand);


    ui->treePlace->setModel(&model);
}

void MainDialog::on_btnSubmit_clicked()
{
    QString validation = validate();
    if(validation == "none"){
        QVariantMap map;
        QSettings qsettings("config.cfg");
        QString gender = "male";
        if(ui->radioFemale->isChecked()) gender = "female";

        map.insert("action" , "submit_form");
        map.insert("access_token" ,qsettings.value("access_token" ,"none"));
        map.insert("full_name" ,ui->textFullName->text());
        map.insert("gender" ,gender);
        map.insert("name" ,ui->comboCarName->currentText());
        map.insert("color" ,color);
        map.insert("place" ,place);
        map.insert("national_code" ,ui->textNationalCode->text());
        map.insert("address" ,ui->textAddress->toPlainText());
        map.insert("passenger_count" ,ui->spinCount->value());
        map.insert("year" ,ui->textYear->text());
        map.insert("health" ,ui->spinHealth->value());


        client.send(map);

    }else{
        QMessageBox::critical(this , "خطا" , validation);
    }
}

QString MainDialog::validate()
{
    if(ui->textFullName->text() == "") return "نام و نام خانوادگی را نمیتوانید خالی بگذارید";
    if(ui->textNationalCode->text() == "") return "کد ملی را نمیتوانید خالی بگذارید";
    if(ui->textAddress->toPlainText() == "") return "آدرس را نمیتوانید خالی بگذارید";
    if(ui->textYear->text() == "") return "مدل را نمیتوانید خالی بگذارید";

    place = ui->treePlace->model()->itemData(ui->treePlace->currentIndex()).value(0).toString();
    if(place == "") return "لطفا یک شهر را انتخاب کنید";

    return "none";

}

void MainDialog::clearInputs()
{
    ui->textFullName->setText("");
    ui->textNationalCode->setText("");
    ui->textAddress->setPlainText("");
    ui->textYear->setText("");
    ui->radioMale->setCheckable(true);
    ui->checkAgreement->setChecked(false);
    ui->spinCount->setValue(1);
    ui->spinHealth->setValue(0);
    ui->treePlace->clearSelection();
    ui->treePlace->collapseAll();

    place = "none";
    color = "none";

}

void MainDialog::logout()
{
    QVariantMap map;
    QSettings qsettings("config.cfg");

    map.insert("action" , "logout");
    map.insert("access_token" ,qsettings.value("access_token"));

    client.send(map);

}


void MainDialog::on_btnExit_clicked()
{
    QMessageBox::StandardButton reply = QMessageBox::question(this,"خروج" , "آیا از خروج مطمئن هستید؟");
    if (reply == QMessageBox::Yes) {
        logout();
    }
}



void MainDialog::closeEvent(QCloseEvent *event)
{
    logout();
}
