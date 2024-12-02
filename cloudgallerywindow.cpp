#include "cloudgallerywindow.h"
#include "ui_cloudgallerywindow.h"

CloudGalleryWindow::CloudGalleryWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::CloudGalleryWindow)
{
    ui->setupUi(this);
}

CloudGalleryWindow::~CloudGalleryWindow()
{
    delete ui;
}

void CloudGalleryWindow::on_addFileOption_triggered()
{

}


void CloudGalleryWindow::on_deleteFileOption_triggered()
{

}


void CloudGalleryWindow::on_sendToCloudOption_triggered()
{

}

