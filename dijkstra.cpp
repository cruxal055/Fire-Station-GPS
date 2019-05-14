#include "dijkstra.h"
#include <QDebug>
#include <QDir>

dijkstra::dijkstra()
{
    graph = new vector<indexo>[80000]();
    speedLimitGraph = new vector<indexo>[80000]();
    allVertexes = new vertex[80000]();
    allSpeedVertexes = new vertex[80000]();
    legend = map<coordinates,int>();
    shortest = QStack<coordinates>();
    readIn();
}

void dijkstra::readIn()
{
    QXmlStreamReader input;
    QFile toOpen;
    QDir cunt;
    toOpen.setFileName("/Users/blank/Documents/Honors_Project/build-reparser3-Desktop_Qt_5_11_0_clang_64bit-Debug/distances12INT.xml");
    toOpen.open(QFile::ReadOnly);
    input.setDevice(&toOpen);
    input.readNextStartElement();
    input.readNextStartElement();

    QString street;
    coordinates one, two;
    int rank1, rank2;
    double weight;
    double time;

    while(!input.atEnd())
    {
        if(input.name() == "STREET")
        {
            address addressRange;


            input.readNextStartElement();
            street = input.readElementText();

            input.readNextStartElement();
            street += (" " + input.readElementText());
            if(street[street.size()-1] == " ")
                street.remove(street.size()-1,1);

            input.readNextStartElement();

            while(input.name().toString().toStdString() == "FROM_ADDRESSES")
            {
                addressRange.lattitude = input.readElementText();

                input.readNextStartElement();
                addressRange.longitude = input.readElementText();

                input.readNextStartElement();
                while(input.name() == "EDGE")
                {

                    input.readNextStartElement();
                    input.readNextStartElement();

                    one.lattitude = input.readElementText();
                    input.readNextStartElement();

                    one.longitude = input.readElementText();
                    input.readNextStartElement();

                    rank1 = input.readElementText().toInt();
                     input.readNextStartElement(); //from vertex

                     input.readNextStartElement();

                    weight = input.readElementText().toDouble();
                    input.readNextStartElement();

                    time = input.readElementText().toDouble();
                    input.readNextStartElement();

                    input.readNextStartElement();

                    two.lattitude = input.readElementText();
                    input.readNextStartElement();

                    two.longitude = input.readElementText();
                    input.readNextStartElement();

                    rank2 = input.readElementText().toInt();
                    input.readNextStartElement();

                    graph[rank1].push_back(indexo(rank2,weight));
                    graph[rank2].push_back(indexo(rank1,weight));

                    speedLimitGraph[rank1].push_back(indexo(rank2,time));
                    speedLimitGraph[rank2].push_back(indexo(rank1, time));

                    legend[one] = rank1;
                    legend[two] = rank2;

                    allVertexes[rank1].rank = rank1;
                    allVertexes[rank2].rank = rank2;
                    allVertexes[rank1].coordinate = one;
                    allVertexes[rank2].coordinate = two;

                    allSpeedVertexes[rank1].rank = rank1;
                    allSpeedVertexes[rank2].rank = rank2;
                    allSpeedVertexes[rank1].coordinate = one;
                    allSpeedVertexes[rank2].coordinate = two;

                    if(!closest[street].count(addressRange))
                        closest[street][addressRange] = vector<coordinates>();
                    closest[street][addressRange].push_back(one);
                    closest[street][addressRange].push_back(two);
                    input.readNextStartElement();
                    input.readNextStartElement();
//                    cout << "ended with: " << input.name().toString().toStdString() << endl;
                    if(input.name() == "FROM_ADDRESSES")
                        break;
                }
            }
        }
        input.readNextStartElement();
    }
    statusReport();
}

void dijkstra::perform(const QString &lat, const QString &lon)
{
    coordinates toSearch(lat,lon);
    performAlgo(toSearch, speedLimitGraph, allSpeedVertexes);
    performAlgo(toSearch, graph, allVertexes);
}

void dijkstra::performAlgo(const coordinates &toSearch, vector<indexo> *graphToUse, vertex *vertexList)
{
    int pos = legend[toSearch];
    vertexList[pos].shortestPath = 0;
    priority_queue<vertex> pq;
    pq.push(vertexList[pos]);
    while(!pq.empty())
    {
        vertex v = pq.top();
        pq.pop();
        if(!v.visited)
        {
            vertexList[v.rank].visited = true;
            for(unsigned int i = 0; i < graphToUse[v.rank].size(); ++i)
            {
                int curr = graphToUse[v.rank][i].whereto;
                if(vertexList[curr].shortestPath > (v.shortestPath + graphToUse[v.rank][i].distance))
                {
                    vertexList[curr].shortestPath = (v.shortestPath + graphToUse[v.rank][i].distance);
                    vertexList[curr].previous = v.rank;
                    pq.push(vertexList[curr]);
                }
            }
        }
    }
}

coordinates dijkstra::closestOne(const coordinates &noFind, const QString &streetName, const QString &zipCode)
{
    auto toDecrement = closest[streetName.toUpper()].end();
    toDecrement--;
    if(closest[streetName.toUpper()].size() == 1)
    {
        bool one =  ((closest[streetName.toUpper()].begin()->first.lattitude.toInt()) > zipCode.toInt());
        bool two =  ((closest[streetName.toUpper()].begin()->first.longitude.toInt())  < zipCode.toInt());
        bool result = one || two;
        if(result)
        {
            throw BAD_ADDRESS;
        }
    }
    else
    {
        if( (closest[streetName.toUpper()].begin()->first.lattitude.toInt()) > zipCode.toInt() || toDecrement->first.lattitude.toInt() < zipCode.toInt())
        {
            if( toDecrement->first.lattitude.toInt() < zipCode.toInt() && toDecrement->first.longitude  .toInt() < zipCode.toInt())
            throw BAD_ADDRESS;
        }
    }

    address search(zipCode);
    closest[streetName.toUpper()][search] = vector<coordinates>();
    coordinates toReturn;
    auto pos = closest[streetName.toUpper()].find(search);
    --pos;
    unsigned int shortest = 0;
    double shortestPath = numeric_limits<double>::max(), hold;
    for(unsigned int i = 0; i < pos->second.size(); ++i)
    {
        if((hold = performHaversine(pos->second[i], noFind)) < shortestPath)
        {
            shortestPath = hold;
            shortest = i;
        }
    }
    toReturn = pos->second[shortest];
    closest[streetName.toUpper()].erase(++pos);
    return toReturn;


}


double dijkstra::compileShortestPath(const QString &lat, const QString &lon,const QString &streetName, const QString &zipCode, bool speed)
{
    shortest = QStack<coordinates>();
    coordinates toSearch(lat,lon);
    if(speed)
    {
        qDebug() << "Compiling based on speed...\n";
        return compile(toSearch, streetName, zipCode, allSpeedVertexes);
    }
    else
        return compile(toSearch, streetName, zipCode, allVertexes);
}

double dijkstra::compileShortestPath2(const coordinates &toSearch, const QString &streetName, const QString &zipCode, bool speed)
{
    shortest = QStack<coordinates>();
    if(speed)
    {
        qDebug() << "Compiling based on speed...\n";
        return compile2(toSearch, streetName, zipCode, allSpeedVertexes);
    }
    else
        return compile2(toSearch, streetName, zipCode, allVertexes);
}

double dijkstra::compile(coordinates &toSearch, const QString &streetName, const QString &zipCode, const vertex *vertexList)
{
    double distance;
    if(!legend.count(toSearch))
    {
        qDebug() << "compiling...\n";
        toSearch = (zipCode!= " ")
        ? closestOne(toSearch, streetName, zipCode) : closest[streetName.toUpper()].begin()->second[0]; // old
    }
    int pos = legend[toSearch];
    distance = vertexList[pos].shortestPath;
    while(pos != -1)
    {
        shortest.push(vertexList[pos].coordinate);
        pos = vertexList[pos].previous;
    }
    return distance;
}


double dijkstra::compile2(const coordinates &toSearch, const QString &streetName, const QString &zipCode, const vertex *vertexList)
{
    double distance;
    int pos = legend[toSearch];
    distance = vertexList[pos].shortestPath;
    while(pos != -1)
    {
        shortest.push(vertexList[pos].coordinate);
        pos = vertexList[pos].previous;
    }
    return distance;
}

double dijkstra::justShortest(const QString &lat, const QString &lon,const QString &streetName, const QString &zipCode, bool speed)
{
    shortest = QStack<coordinates>();
    coordinates coord(lat,lon);
    if(!legend.count(coord))
    {
        if(zipCode!= " ")
            coord = closestOne(coord, streetName, zipCode);
    }
    int pos = legend[coord];
    return speed ? allSpeedVertexes[pos].shortestPath :  allVertexes[pos].shortestPath;
}

double dijkstra::justShortestVer2(coordinates &toSeek, const QString &streetName, const QString &zipCode, bool speed)
{
    shortest = QStack<coordinates>();

    if(!legend.count(toSeek))
    {//    map<QString,map<address, vector<coordinates> >  > closest;
        if(zipCode!= " ")
            toSeek = closestOne(toSeek, streetName, zipCode);
        else
        {
            if(speed)
                toSeek = figureOutClosest(streetName.toUpper(), toSeek, allSpeedVertexes);
            else
                toSeek = figureOutClosest(streetName.toUpper(), toSeek, allVertexes);
        }
    }
    int pos = legend[toSeek];
    return speed ? allSpeedVertexes[pos].shortestPath :  allVertexes[pos].shortestPath;

}




void dijkstra::statusReport()
{
    cout << legend.size() << endl;
    cout << closest.size() << endl;

}
dijkstra::~dijkstra()
{
    delete []allVertexes;
    delete []allSpeedVertexes;
    delete []speedLimitGraph;
    delete []graph;
}

void dijkstra::toString()
{
    while(!shortest.empty())
    {
        cout << shortest.top().lattitude.toStdString() << " , "  << shortest.top().longitude.toStdString() << endl;
        shortest.pop();
    }
}

bool dijkstra::withinBounds(const QString &streetName)
{
    return (closest.count(streetName.toUpper()));
}


double dijkstra::performHaversine(const coordinates &one, const coordinates &two)
{
   double lat1 = one.lattitude.toDouble() * (PI/180.), long1 = one.longitude.toDouble() * (PI/180.);
   double lat2 = two.lattitude.toDouble() * (PI/180.), long2 = two.longitude.toDouble()* (PI/180.),
           dLat = lat2 - lat1, dLong = long2 - long1;
   double computation = asin(sqrt(sin(dLat / 2.) * sin(dLat / 2.) + cos(lat1) * cos(lat2) * sin(dLong / 2.) * sin(dLong / 2.)));
   double d = 3959.9 * 2 * computation;
   return d;
}

coordinates dijkstra::figureOutClosest(const QString &streetName, const coordinates &startingPos, vertex *graphToUse)
{
    int shortest = legend[closest[streetName].begin()->second[0]];
    double shortestSoFar = numeric_limits<double>::max();

    for(auto i = closest[streetName].begin(); i != closest[streetName].end(); ++i)
    {
        for(unsigned int j = 0; j < i->second.size(); ++j)
        {
            double temp;
            if(shortestSoFar > (temp = graphToUse[legend[i->second[j]]].shortestPath))
            {
                shortestSoFar = temp;
                shortest = legend[i->second[j]];
            }
        }
    }
    return graphToUse[shortest].coordinate;
}

bool dijkstra::legitStreet(const QString &streetName)
{
    return closest.count(streetName.toUpper());
}

