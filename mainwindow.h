
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWebEngineView>
#include <QWebChannel>
#include <QMessageBox>
#include <QString>
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QJsonObject>
#include <QStack>
#include <QJsonArray>
#include <QVariantList>
#include "supportclasses.h"
#include "dijkstra.h"

namespace Ui
{
    class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT
    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow();


    public slots:
        void updateLatLong(const QString &latLng);
        void testFunc();
        void addresso();


    signals:
        void testing(QJsonArray items);
        void drawMap();
        void getCoordinate(QJsonArray where);

    private:
        Ui::MainWindow *ui;
        QWebEngineView *view;
        QWebEnginePage *page;
        QWebChannel *channel;
        QString httpString;


        void setupWebEngine();
        void setupSignalsAndSlots();
        void displayMap();
        void getShorto(const QString &one, const QString &two);
        dijkstra *master;

    private slots:
        void loadMap();
};

#endif // MAINWINDOW_H
