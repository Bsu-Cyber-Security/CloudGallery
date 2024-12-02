#ifndef CLOUDGALLERYWINDOW_H
#define CLOUDGALLERYWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>

namespace Ui {
class CloudGalleryWindow;
}

class CloudGalleryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit CloudGalleryWindow(QWidget *parent = nullptr);
    ~CloudGalleryWindow();

private slots:
    void on_addFileOption_triggered();

    void on_deleteFileOption_triggered();

    void on_sendToCloudOption_triggered();

private:
    Ui::CloudGalleryWindow *ui;
};

#endif // CLOUDGALLERYWINDOW_H
