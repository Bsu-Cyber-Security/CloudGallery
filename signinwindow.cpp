#include "signinwindow.h"
#include "ui_signinwindow.h"
#include "cloudgallerywindow.h"

SignInWindow::SignInWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SignInWindow)
{
    ui->setupUi(this);
    ui->logInButton->setEnabled(false);
}

SignInWindow::~SignInWindow()
{
    delete ui;
}

bool loginIsInputted = false;
bool passwordIsInputted = false;

bool readyToUnlock(bool loginIsInputted, bool passwordIsInputted){
    if (loginIsInputted&&passwordIsInputted){
        return true;
    }
    return false;
}

void SignInWindow::on_loginLine_editingFinished()
{
    loginIsInputted=true;
    if (readyToUnlock(loginIsInputted, passwordIsInputted)){
        ui->logInButton->setEnabled(true);
    }
}


void SignInWindow::on_passwordLine_editingFinished()
{
    passwordIsInputted=true;
    if (readyToUnlock(loginIsInputted, passwordIsInputted)){
        ui->logInButton->setEnabled(true);
    }
}


void SignInWindow::on_logInButton_clicked()
{
    CloudGalleryWindow* cloudGalleryWindow = new CloudGalleryWindow();
    cloudGalleryWindow->show();
    this->close();
}

