#include "dijkstra.h"
#include <QDebug>
#include <QDir>

dijkstra::dijkstra()
{
    graph = new vector<indexo>[80000];
    allVertexes = new vertex[80000];
    legend = map<coordinates,int>();
    shortest = QStack<coordinates>();
    readIn();
}

void dijkstra::readIn()
{
    QXmlStreamReader input;
    QFile toOpen;
    QDir cunt;
    toOpen.setFileName("/Users/blank/Desktop/Will-it-work\?/mapItExample/distances10INT.xml");
    toOpen.open(QFile::ReadOnly);
    input.setDevice(&toOpen);
    input.readNextStartElement();
    input.readNextStartElement();

    QString street;
    coordinates one, two;
    int rank1, rank2;
    double weight;

    while(!input.atEnd())
    {
        if(input.name() == "STREET")
        {
            address addressRange;


            input.readNextStartElement();
            street = input.readElementText();

            input.readNextStartElement();
            street += (" " + input.readElementText());

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

                    input.readNextStartElement();

                    two.lattitude = input.readElementText();
                    input.readNextStartElement();

                    two.longitude = input.readElementText();
                    input.readNextStartElement();

                    rank2 = input.readElementText().toInt();
                    input.readNextStartElement();

                    graph[rank1].push_back(indexo(rank2,weight));
                    graph[rank2].push_back(indexo(rank1,weight));
                    legend[one] = rank1;
                    legend[two] = rank2;

                    allVertexes[rank1].rank = rank1;
                    allVertexes[rank2].rank = rank2;
                    allVertexes[rank1].coordinate = one;
                    allVertexes[rank2].coordinate = two;


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
    int pos = legend[toSearch];
    allVertexes[pos].shortestPath = 0;
    priority_queue<vertex> pq;
    pq.push(allVertexes[pos]);
    while(!pq.empty())
    {
        vertex v = pq.top();
        pq.pop();
        if(!v.visited)
        {
            allVertexes[v.rank].visited = true;
            for(unsigned int i = 0; i < graph[v.rank].size(); ++i)
            {
                int curr = graph[v.rank][i].whereto;
                if(allVertexes[curr].shortestPath > (v.shortestPath + graph[v.rank][i].distance))
                {
                    allVertexes[curr].shortestPath = (v.shortestPath + graph[v.rank][i].distance);
                    allVertexes[curr].previous = v.rank;
                    pq.push(allVertexes[curr]);
                }
            }
        }
    }
}

coordinates dijkstra::closestOne(const coordinates &noFind, const QString &streetName, const QString &zipCode)
{
    auto toDecrement = closest[streetName.toUpper()].end();
    toDecrement--;
    qDebug() << "smallest one is: " << (closest[streetName.toUpper()].begin()->first.lattitude)<< endl;
    qDebug() << "largest one is: " << toDecrement->first.lattitude<< endl;
    qDebug() << "looking at: " << zipCode << endl;
    qDebug() << closest[streetName.toUpper()].size()  << endl;
    if(closest[streetName.toUpper()].size() == 1)
    {
        qDebug() << "yooooo size is 1 " << endl;
        qDebug() << "helloooo: " << closest[streetName.toUpper()].begin()->first.lattitude << endl;
        qDebug() << "helloooo: " << closest[streetName.toUpper()].begin()->first.longitude << endl;
        qDebug() <<  ((closest[streetName.toUpper()].begin()->first.lattitude.toInt()) > zipCode.toInt()) << endl;
        qDebug() <<   ((closest[streetName.toUpper()].begin()->first.longitude.toInt())  < zipCode.toInt()) << endl;

        bool one =  ((closest[streetName.toUpper()].begin()->first.lattitude.toInt()) > zipCode.toInt());
        bool two =  ((closest[streetName.toUpper()].begin()->first.longitude.toInt())  < zipCode.toInt());
        qDebug() << "one is: " << one << endl;
        qDebug() << " two is: " << two << endl;
        qDebug() << "OMFG STOP\n";
        qDebug() <<  "resulto: " << (one || two ) << endl;
        bool result = one || two;
        qDebug() << "result is: " << result << endl;
        if(result)
        {
            qDebug() << "bad\n";
            throw BAD_ADDRESS;
        }
    }
    else
    {
        if( (closest[streetName.toUpper()].begin()->first.lattitude.toInt()) > zipCode.toInt() || toDecrement->first.lattitude.toInt() < zipCode.toInt())
            throw BAD_ADDRESS;
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


double dijkstra::compileShortestPath(const QString &lat, const QString &lon,const QString &streetName, const QString &zipCode)
{
    shortest = QStack<coordinates>();
    coordinates wtf(lat,lon);
    double distance;
    if(!legend.count(wtf))
    {
        wtf = closestOne(wtf, streetName, zipCode);
    }
    int pos = legend[wtf];
    distance = allVertexes[pos].shortestPath;
    while(pos != -1)
    {
        shortest.push(allVertexes[pos].coordinate);
        pos = allVertexes[pos].previous;
    }
    return distance;
}

double dijkstra::justShortest(const QString &lat, const QString &lon,const QString &streetName, const QString &zipCode)
{
    shortest = QStack<coordinates>();
    coordinates wtf(lat,lon);
    double distance;
    if(!legend.count(wtf))
    {
        wtf = closestOne(wtf, streetName, zipCode);
    }
    int pos = legend[wtf];
    distance = allVertexes[pos].shortestPath;
    return distance;

}

void dijkstra::statusReport()
{
    cout << legend.size() << endl;
    cout << closest.size() << endl;

}
dijkstra::~dijkstra()
{
    delete []allVertexes;
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

