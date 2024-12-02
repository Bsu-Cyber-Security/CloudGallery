#include "welcomepage.h"
#include "ui_welcomepage.h"
#include "signinwindow.h"
#include "signupwindow.h"

WelcomePage::WelcomePage(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WelcomePage)
{
    ui->setupUi(this);
}

WelcomePage::~WelcomePage()
{
    delete ui;
}

void WelcomePage::on_signInButton_clicked()
{
    SignInWindow* signInWindow = new SignInWindow();
    signInWindow->show();
    this->close();
}


void WelcomePage::on_signUpButton_clicked()
{
    SignUpWindow* signUpWindow = new SignUpWindow();
    signUpWindow->show();
    this->close();
}

