#ifndef SIGNINWINDOW_H
#define SIGNINWINDOW_H

#include <QWidget>

namespace Ui {
class SignInWindow;
}

class SignInWindow : public QWidget
{
    Q_OBJECT

public:
    explicit SignInWindow(QWidget *parent = nullptr);
    ~SignInWindow();

private slots:
    void on_loginLine_editingFinished();

    void on_passwordLine_editingFinished();

    void on_logInButton_clicked();

private:
    Ui::SignInWindow *ui;
};

#endif // SIGNINWINDOW_H
