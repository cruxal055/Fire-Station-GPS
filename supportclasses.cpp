#include "supportclasses.h"
indexo::indexo()
{
    distance = -1.;
    whereto = -1;
}

indexo::indexo(int whereTo, double distance)
{
    this->distance = distance;
    this->whereto = whereTo;
}

indexo::indexo(const indexo &other)
{
    distance = other.distance;
    whereto = other.whereto;
}

indexo& indexo::operator=(const indexo &other)
{
    if(this != &other)
    {
        distance = other.distance;
        whereto = other.whereto;
    }
    return *this;
}

coordinates::coordinates()
{
    lattitude = longitude = "";
}

coordinates::coordinates(const QString &lat, const QString &lon)
{
    lattitude = lat;
    longitude = lon;
}

coordinates& coordinates::operator=(const coordinates &other)
{
    if(this != &other)
    {
        lattitude = other.lattitude;
        longitude = other.longitude;
    }
    return *this;
}

coordinates::coordinates(const coordinates &other)
{
    lattitude = other.lattitude;
    longitude = other.longitude;
}

bool operator<=(const coordinates &o1 ,const coordinates &o2)
{
    return o1.lattitude <= o2.lattitude;
}

bool operator>=(const coordinates &o1 ,const coordinates &o2)
{
    return o1.lattitude >= o2.lattitude;
}
bool operator>(const coordinates &o1 ,const coordinates &o2)
{
    return o1.lattitude > o2.lattitude;
}
bool operator<(const coordinates &o1 ,const coordinates &o2)
{
    return (o1.lattitude.toDouble() + o1.longitude.toDouble()) < (o2.lattitude.toDouble()+ o2.longitude.toDouble());
}
bool operator==(const coordinates &o1 ,const coordinates &o2)
{
    return o1.lattitude ==  o2.lattitude && o2.longitude == o1.longitude;
}

address::address()
{
    lattitude = longitude = "";
}
address::address(const QString &lat)
{
    lattitude = lat;
    longitude = "";
}

address::address(const QString &lat, const QString &lon)
{
    lattitude = lat;
    longitude = lon;
}

address& address::operator=(const address &other)
{
    if(this != &other)
    {
        lattitude = other.lattitude;
        longitude = other.longitude;
    }
    return *this;
}

address::address(const address &other)
{
    lattitude = other.lattitude;
    longitude = other.longitude;
}

bool operator<=(const address &o1 ,const address &o2)
{
    return o1.lattitude <= o2.lattitude;
}

bool operator>=(const address &o1 ,const address &o2)
{
    return o1.lattitude >= o2.lattitude;
}
bool operator>(const address &o1 ,const address &o2)
{
    return o1.lattitude > o2.lattitude;
}
bool operator<(const address &o1 ,const address &o2)
{
    return o1.lattitude < o2.lattitude;
}
bool operator==(const address &o1 ,const address &o2)
{
    return o1.lattitude ==  o2.lattitude;
}

vertex::vertex()
{
    rank = previous = -1;
    coordinate = coordinates();
    visited = false;
    shortestPath = numeric_limits<double>::max();
}

vertex::vertex(const vertex &other)
{
    rank = other.rank;
    coordinate = other.coordinate;
    visited = other.visited;
    shortestPath = other.shortestPath;
}
vertex& vertex::operator=(const vertex &other)
{
    if(this != &other)
    {
        rank = other.rank;
        coordinate = other.coordinate;
        visited = other.visited;
        shortestPath = other.shortestPath;
    }
    return *this;
}

bool operator<=(const vertex &one, const vertex &two)
{
    return one.shortestPath >= two.shortestPath;
}

bool operator>=(const vertex &one, const vertex &two)
{
    return one.shortestPath <= two.shortestPath;
}

bool operator<(const vertex &one, const vertex &two)
{
    return one.shortestPath > two.shortestPath;
}

bool operator>(const vertex &one, const vertex &two)
{
    return one.shortestPath < two.shortestPath;
}
