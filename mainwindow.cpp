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
}


void MainWindow::displayMap()
{
    QDir dir("../Resources/map.html");
    page->load(QUrl(QUrl::fromLocalFile(dir.canonicalPath())));
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
        int whereToBegin = 3;
        ui->currLatLng->setText("Current position: " + latLng);
        if(latLng[latLng.size()-1] == ' ')
        {
            QMessageBox::information(
            this,
            tr("Error!"),
            tr("Recieved name of a place instead of an actual address"));
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
        if(regex[0] == "34.13927000000007" && regex[1] == "-118.21086999999994")
        {
            QMessageBox::information(
            this,
            tr("Already at eagle rock"),
            tr("Already at eagle rock."
               ""));
            emit resetNeeded();
            return;
        }
        if(!(regex[regex.size()-1] == "90041" || regex[regex.size()-1] == "90042" || regex[regex.size()-1] == "90065"))
        {
            QMessageBox::information(
            this,
            tr("Error!"),
            tr("Not within the bounds of eagle rock!") );
            emit resetNeeded();
            return;
        }
        regex.erase(regex.end()-1);
        if(regex[regex.size()-1] == "Rd")
            regex[regex.size()-1] = "ROAD";
        if(regex.size() >= 4)
        {
            if(regex[3].size() == 1)
            {
                regex[3] = regex[4];
                regex.erase(regex.begin()+4);
            }
            for(int i = 2; i < regex.size()-1; ++i )
            {
                QString toLook = regex[i];
                for(int j = i+1; j < regex.size(); ++j)
                {
                    if(regex[j] == "Ave")
                        regex[j] = "AVENUE";
                    toLook += (" " + regex[j]);
                }
                if(master[0].withinBounds(toLook))
                {
                    whereToBegin = i;
                }
                else
                {
                    if(regex[regex.size()-1] == "AVENUE")
                    {
                        toLook.chop(6);
                        toLook+=("AVE");
                        if(master[0].withinBounds(toLook))
                        {
                            whereToBegin = i;
                            regex[regex.size()-1] = "AVE";
                            break;
                        }
                    }
                    else
                    {
                        if(regex[regex.size()-1] == "AVE")
                        {
                            toLook.chop(3);
                            toLook+=("AVENUE");
                            if(master[0].withinBounds(toLook))
                            {
                                regex[regex.size()-1] = "AVENUE";
                                whereToBegin = i;
                                break;
                            }
                        }
                    }
                }
            }
            for(int i = whereToBegin+1; i < regex.size(); ++i)
                regex[whereToBegin]+= " " + regex[i];
        }
        else
            regex[3] += " " + regex[4];

        if(!master[0].withinBounds(regex[whereToBegin]))
        {
            QMessageBox::information(
            this,
            tr("Error!"),
            tr("Not within the bounds of eagle rock!") );
            emit resetNeeded();

            return;
        }
        QJsonArray jArray;

        int smallestPos = 0;
        QString useThis = " ";
        if(whereToBegin != 2)
        {
            if(regex[2].size() != 1)
                useThis = regex[2];
            else
            {
                QMessageBox::information(
                this,
                tr("GENERAL STREET WARNING!!!!!!"),
                tr("You have not given a specific address, but a general street, the program will direct you to the shortest path to the street"
                   "be careful of lopsided streets!!!!!!!!!") );
                emit resetNeeded();
            }
        }
        if(whereToBegin == 2)
        {
            QMessageBox::information(
            this,
            tr("Warning!!!!!"),
            tr("You have not given a specific address, but a general street, the program will direct you to the shortest path to the street") );
            emit resetNeeded();

        }
        coordinates whereTo(regex[0], regex[1]);
        coordinates whereTo2(regex[0], regex[1]);
        coordinates whereTo3(regex[0], regex[1]);
        coordinates toUse;

        double smallest = master[0].justShortestVer2(whereTo, regex[whereToBegin], useThis, ui->speedLimitVer->isChecked());
        toUse = whereTo;
        double one =  master[1].justShortestVer2(whereTo2, regex[whereToBegin], useThis, ui->speedLimitVer->isChecked()),
               two =  master[2].justShortestVer2(whereTo3,regex[whereToBegin], useThis, ui->speedLimitVer->isChecked());

        if (smallest > one)
        {
            smallest = one;
            smallestPos = 1;
            toUse = whereTo2;
        }
        else
        {
            if (smallest > two)
            {
                toUse = whereTo3;
                smallest = two;
                smallestPos = 2;
            }
        }

        master[smallestPos].compileShortestPath2(toUse,regex[whereToBegin], useThis, ui->speedLimitVer->isChecked());

        while(!master[smallestPos].shortest.empty())
        {
            coordinates temp = master[smallestPos].shortest.top();
            master[smallestPos].shortest.pop();
            jArray.push_back(QJsonValue(temp.lattitude + ", " + temp.longitude));
        }

        emit testing(jArray);
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



