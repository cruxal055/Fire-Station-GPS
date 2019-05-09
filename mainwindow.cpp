#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtDebug>
#include <QDebug>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MainWindow)
{
    master = new dijkstra[3];
    ui->setupUi(this);

    this->setWindowTitle("Eagle Rock Fire Stations");
    setupWebEngine();
    setupSignalsAndSlots();
    displayMap();

    master[0].perform("34.12416049793684", "-118.21936667211293"); //york
    master[1].perform("34.13948426993484", "-118.21073194836286"); //colorado
    master[2].perform("34.11091760266492", "-118.19074248113876"); //Fug.


}

MainWindow::~MainWindow()
{
    delete []master;
    delete view;
    //delete channel;
    //delete page;
}

void MainWindow::setupSignalsAndSlots()
{
    connect(ui->quitButton,SIGNAL(clicked(bool)),this,SLOT(close()));
    connect(ui->tester, SIGNAL(clicked(bool)), this, SLOT(testFunc()));
}


void MainWindow::displayMap()
{
    QDir dir("../Resources/map.html");
    QFileInfo exists("../Resources/map.html");
    if(exists.exists())
    {
        qDebug() << exists.absoluteFilePath() << endl;
    }


    page->load(QUrl(QUrl::fromLocalFile(dir.canonicalPath())));
}


void MainWindow::testFunc()
{
    QStack<QString> stuff;
    QString oof = "oof";
    QVariantList temp;
    QJsonObject wtf1, wtf2;
    QJsonArray oofOwie;
//    master[0].getShortestPath("34.136015977443542", "-118.221345633423269");


    while(!master[0].shortest.empty())
    {
        coordinates temp = master[0].shortest.top();
        master[0].shortest.pop();
        oofOwie.push_back(QJsonValue(temp.lattitude + ", " + temp.longitude));
    }
    emit testing(oofOwie);
}


void MainWindow::getShorto(const QString &one, const QString &two)
{
    QStack<QString> stuff;
    QString oof = "oof";
    QVariantList temp;
    QJsonObject wtf1, wtf2;
    QJsonArray oofOwie;
//    master[0].getShortestPath(one, two);
    if(master[0].shortest.empty())
        qDebug() << "it's empty\n";
    while(!master[0].shortest.empty())
    {
        coordinates temp = master[0].shortest.top();
        master[0].shortest.pop();
        oofOwie.push_back(QJsonValue(temp.lattitude + ", " + temp.longitude));
    }
    emit testing(oofOwie);

}
void MainWindow::setupWebEngine()
{
    qDebug()<<"starting";
    view = new QWebEngineView;
    view->setParent(ui->centralWidget);
    view->setGeometry(20,20,800,600); //20,20,800,600
    channel = new QWebChannel(view->page());
    //attach it to the QWebEnginePage
    view->page()->setWebChannel(channel);
    //register a QObject to be exposed to JavaScript
    channel->registerObject(QStringLiteral("mainwindow"), this);
    page = view->page();
}

void MainWindow::updateLatLong(const QString &latLng)
{
    try
    {
        qDebug() << "recieved: " << latLng << endl;
        ui->currLatLng->setText("Current position: " + latLng);
        if(latLng[latLng.size()-1] == ' ')
        {
            QMessageBox::information(
            this,
            tr("Error!"),
            tr("Recieved store name instead of an actual address"));
            emit resetNeeded();
            return;
        }
        QStringList regex = latLng.split(" ");
        if(regex.size() < 4)
        {
            QMessageBox::information(
            this,
            tr("Error!"),
            tr("Recieved bad data!"));
            emit resetNeeded();
            return;
        }
        qDebug() << "why are you doing this\n";
        qDebug() << " size of regex is: " <<  regex.size() << endl;
        if(!(regex[regex.size()-1] == "90041" || regex[regex.size()-1] == "90042" || regex[regex.size()-1] == "90065"))
        {
            QMessageBox::information(
            this,
            tr("Error!"),
            tr("Not within the bounds of eagle rock!") );
            emit resetNeeded();
            return;
        }
        if(regex[4] == "Rd")
            regex[4] = "ROAD";
        if(regex.size() > 5)
        {
            if(regex[3].size() == 1)
            {
                regex[3] = regex[4];
                regex.erase(regex.begin()+4);
            }
            for(int i = 4; i < regex.size()-1; ++i)
                regex[3]+= " " + regex[i];
        }
        else
            regex[3] += " " + regex[4];

        for(int i = 0; i < regex.size(); ++i)
        {
            qDebug() << regex[i] << " ";
        }
        qDebug() << endl;
        if(!master[0].withinBounds(regex[3]))
        {
            QMessageBox::information(
            this,
            tr("Error!"),
            tr("Not within the bounds of eagle rock!") );
            emit resetNeeded();

            return;
        }
        QStack<QString> stuff;
        QString oof = "oof";
        QVariantList temp;
        QJsonObject wtf1, wtf2;
        QJsonArray oofOwie;

        int smallestPos = 0;
        double smallest = master[0].justShortest(regex[0], regex[1], regex[3], regex[2], ui->speedLimitVer->isChecked());
        double one =  master[1].justShortest(regex[0], regex[1], regex[3], regex[2], ui->speedLimitVer->isChecked()),
               two =  master[2].justShortest(regex[0], regex[1], regex[3], regex[2], ui->speedLimitVer->isChecked());
        if (smallest > one)
        {
            smallest = one;
            smallestPos = 1;
        }
        else
        {
            if (smallest > two)
            {
                smallest = two;
                smallestPos = 2;
            }
        }
        master[smallestPos].compileShortestPath(regex[0], regex[1], regex[3], regex[2], ui->speedLimitVer->isChecked());

        while(!master[smallestPos].shortest.empty())
        {
            coordinates temp = master[smallestPos].shortest.top();
            master[smallestPos].shortest.pop();
            oofOwie.push_back(QJsonValue(temp.lattitude + ", " + temp.longitude));
        }

        emit testing(oofOwie);
    }catch(errors e)
    {
        if(e == BAD_ADDRESS)
        {
            QMessageBox::information(
            this,
            tr("Error!"),
            tr("Recieved bad address!!"));
            emit resetNeeded();

        }
    }
}



