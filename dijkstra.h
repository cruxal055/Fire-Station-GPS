#ifndef DIJKSTRA_H
#define DIJKSTRA_H


#include <QString>
#include <QXmlStreamReader>
#include <climits>
#include "supportclasses.h"
#include <QFile>
#include <map>
#include <iostream>
#include <queue>
#include <cmath>
#include <limits>
#include <climits>
#include <stack>
#include <QStack>
#include <QDir>

#define PI 4.0*atan(1.0)

enum errors{ BAD_ADDRESS  };


using namespace std;



class dijkstra
{


    public:
        dijkstra();
        QStack<coordinates> shortest;
        ~dijkstra();

    private:
    vector<indexo> *graph;
    vertex *allVertexes;
    map<coordinates, int> legend;
    map<QString,map<address, vector<coordinates> >  > closest;


    coordinates focus;



    void readIn();
    coordinates closestOne(const coordinates &noFind, const QString &streetName, const QString &zipCode);
    double performHaversine(const coordinates &one, const coordinates &two);

    public:
    double getShortestPath(const QString &lat, const QString &lon,const QString &streetName, const QString &zipCode);
    void perform(const QString &lat, const QString &lon, const QString &streetName);
    void statusReport();
    void toString();
};


#endif // DIJKSTRA_H
