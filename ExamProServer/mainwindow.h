#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"QTcpServer"
#include"QTcpSocket"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onReadyRead();

    void onNewConnection();

    void on_btnStartServer_clicked();

    void on_btnStopServer_clicked();
private:
    QByteArray readData;
    QTcpSocket *tcpSocket;
    QTcpServer *tcpServer;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
