#ifndef WELCOMEPAGE_H
#define WELCOMEPAGE_H

#include <QWidget>

namespace Ui {
class WelcomePage;
}

class WelcomePage : public QWidget
{
    Q_OBJECT

public:
    explicit WelcomePage(QWidget *parent = nullptr);
    ~WelcomePage();

private slots:
    void on_signInButton_clicked();

    void on_signUpButton_clicked();

private:
    Ui::WelcomePage *ui;
};

#endif // WELCOMEPAGE_H
