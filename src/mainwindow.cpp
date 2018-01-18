#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->mygl->setFocus();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::on_actionLoad_Mesh_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load Mesh", ":/", "*.obj");
    ui->mygl->createMesh(fileName);
}

void MainWindow::on_actionExport_Mesh_triggered()
{
    ui->mygl->exportOBJ();
}

void MainWindow::on_actionLoad_Skeleton_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Load Skeleton", ":/", "*.json");
    ui->mygl->createSkeleton(fileName);
}
