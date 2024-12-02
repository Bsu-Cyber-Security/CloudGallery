#include "signupwindow.h"
#include "ui_signupwindow.h"
#include "cloudgallerywindow.h"

SignUpWindow::SignUpWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignUpWindow)
{
    ui->setupUi(this);
    ui->signUpButton->setEnabled(false);
}

SignUpWindow::~SignUpWindow()
{
    delete ui;
}

bool loginInputted = false;
bool passwordInputted = false;
bool repetitivePasswordInputted = false;

bool readyToUnlock(bool loginIsInputted, bool passwordIsInputted, bool repetitivePasswordIsInputted){
    if (loginIsInputted&&passwordIsInputted&&repetitivePasswordIsInputted){
        return true;
    }
    return false;
}

void SignUpWindow::on_loginLine_editingFinished()
{
    loginInputted=true;
    if(readyToUnlock(loginInputted, passwordInputted, repetitivePasswordInputted)){
        ui->signUpButton->setEnabled(true);
    }
}


void SignUpWindow::on_passwordLine_editingFinished()
{
    passwordInputted=true;
    if(readyToUnlock(loginInputted, passwordInputted, repetitivePasswordInputted)){
        ui->signUpButton->setEnabled(true);
    }
}


void SignUpWindow::on_passwordRepeatLine_editingFinished()
{
    repetitivePasswordInputted=true;
    if(readyToUnlock(loginInputted, passwordInputted, repetitivePasswordInputted)){
        ui->signUpButton->setEnabled(true);
    }
}


void SignUpWindow::on_signUpButton_clicked()
{
    CloudGalleryWindow* cloudGalleryWindow = new CloudGalleryWindow();
    cloudGalleryWindow->show();
    this->close();
}

