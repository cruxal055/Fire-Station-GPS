#ifndef SUPPORTCLASSES_H
#define SUPPORTCLASSES_H


#include <QString>
#include <climits>

using namespace std;

struct indexo
{
    double distance;
    int whereto;

    indexo();
    indexo(int whereTo, double distance);
    indexo(const indexo &other);
    indexo& operator=(const indexo &other);
};


struct coordinates
{
    public:
        QString lattitude, longitude;
        coordinates();
        coordinates(const QString &lat, const QString &lon);
        coordinates &operator=(const coordinates &other);
        coordinates(const coordinates &other);

        friend bool operator<=(const coordinates &o1 ,const coordinates &o2);
        friend bool operator>=(const coordinates &o1 ,const coordinates &o2);
        friend bool operator>(const coordinates &o1 ,const coordinates &o2);
        friend bool operator<(const coordinates &o1 ,const coordinates &o2);
        friend bool operator==(const coordinates &o1 ,const coordinates &o2);
};



struct address
{
    public:
        QString lattitude, longitude;
        address();
        address(const QString &lat);

        address(const QString &lat, const QString &lon);

        address &operator=(const address &other);

        address(const address &other);

        friend bool operator<=(const address &o1 ,const address &o2);
        friend bool operator>=(const address &o1 ,const address &o2);
        friend bool operator>(const address &o1 ,const address &o2);
        friend bool operator<(const address &o1 ,const address &o2);
        friend bool operator==(const address &o1 ,const address &o2);
};
struct vertex
{
    int rank, previous;
    coordinates coordinate;
    bool visited;
    double shortestPath;

    vertex();
    vertex(const vertex &other);
    vertex& operator=(const vertex &other);

    friend bool operator<=(const vertex &one, const vertex &two);
    friend bool operator>=(const vertex &one, const vertex &two);
    friend bool operator<(const vertex &one, const vertex &two);
    friend bool operator>(const vertex &one, const vertex &two);


};


#endif // SUPPORTCLASSES_H
