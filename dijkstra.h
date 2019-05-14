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
        QStack<coordinates> shortest;
        dijkstra();
        ~dijkstra();
        dijkstra(const dijkstra &other);
        dijkstra& operator=(const dijkstra &other);
        double compileShortestPath2(const coordinates &toSearch, const QString &streetName, const QString &zipCode, bool speed);
        double justShortestVer2(coordinates &toSeek, const QString &streetName, const QString &zipCode, bool speed);
        void perform(const QString &lat, const QString &lon);
        bool withinBounds(const QString &streetName);

    private:
        vector<indexo> *graph;
        vector<indexo> *speedLimitGraph;
        vertex *allVertexes;
        vertex *allSpeedVertexes;
        map<coordinates, int> legend;
        map<QString,map<address, vector<coordinates> >  > closest;
        coordinates focus;

        void copyOther(const dijkstra &other);

        void readIn();
        coordinates closestOne(const coordinates &noFind, const QString &streetName, const QString &zipCode);
        double performHaversine(const coordinates &one, const coordinates &two);
        void performAlgo(const coordinates &toSearch, vector<indexo> *graphToUse, vertex *vertexList);
        double compile2(const coordinates &toSearch, const QString &streetName, const QString &zipCode, const vertex *vertexList);
        coordinates figureOutClosest(const QString &streetName, const coordinates &startingPos, vertex *graphToUse);


};


#endif // DIJKSTRA_H
