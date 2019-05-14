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
    vector<indexo> *speedLimitGraph;
    vertex *allVertexes;
    vertex *allSpeedVertexes;
    map<coordinates, int> legend;
    map<QString,map<address, vector<coordinates> >  > closest;
    coordinates focus;



    void readIn();
    coordinates closestOne(const coordinates &noFind, const QString &streetName, const QString &zipCode);
    double performHaversine(const coordinates &one, const coordinates &two);
    void performAlgo(const coordinates &toSearch, vector<indexo> *graphToUse, vertex *vertexList);
    double compile(coordinates &toSearch, const QString &streetName, const QString &zipCode, const vertex *vertexList);
    double compile2(const coordinates &toSearch, const QString &streetName, const QString &zipCode, const vertex *vertexList);
    coordinates figureOutClosest(const QString &streetName, const coordinates &startingPos, vertex *graphToUse);
    public:

    double compileShortestPath(const QString &lat, const QString &lon, const QString &streetName, const QString &zipCode, bool speed);
    double compileShortestPath2(const coordinates &toSearch, const QString &streetName, const QString &zipCode, bool speed);
    double justShortest(const QString &lat, const QString &lon, const QString &streetName, const QString &zipCode, bool speed);
    double justShortestVer2(coordinates &toSeek, const QString &streetName, const QString &zipCode, bool speed);


    void perform(const QString &lat, const QString &lon);
    void statusReport();
    void toString();
    bool withinBounds(const QString &streetName);
    bool legitStreet(const QString &streetName);
};


#endif // DIJKSTRA_H
