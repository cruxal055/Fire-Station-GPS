#include <QCoreApplication>
#include <QXmlStreamReader>
#include <QString>
#include <QXmlStreamWriter>
#include <QFile>
#include <iostream>
#include <cmath>
#include <QDebug>
#include <set>

#define PI 4.0*atan(1.0)

using namespace std;


struct coordinate
{
    QString lon, lat;

    coordinate()
    {
        lat = lon = "";
    }

    coordinate(const QString &lat, const QString &lon)
    {
        this->lat = lat;
        this->lon = lon;
    }

    coordinate(const coordinate &other)
    {
        copy(other);
    }

    coordinate& operator=(const coordinate &other)
    {
        if(this != &other)
            copy(other);
        return *this;
    }

    void copy(const coordinate &other)
    {
        lon = other.lon;
        lat = other.lat;
    }

    friend bool operator==(const coordinate &one, const coordinate &two)
    {
        return (one.lat == two.lat) && (one.lon == two.lon);
    }

    friend bool operator<=(const coordinate &one, const coordinate &two)
    {
        return (one.lat <= two.lat);
    }

    friend bool operator>=(const coordinate &one, const coordinate &two)
    {
        return (one.lat >= two.lat);
    }

    friend bool operator<(const coordinate &one, const coordinate &two)
    {
        return (one.lat < two.lat);
    }

    friend bool operator>(const coordinate &one, const coordinate &two)
    {
        return (one.lat > two.lat);
    }



    ~coordinate()
    {
        lon = lat = "";
    }


};

void open(QXmlStreamReader &toInput, QXmlStreamWriter &toOutput,  QFile &in, QFile &out);
void performReadAndWrite(QXmlStreamReader &toInput, QXmlStreamWriter &toOutput);
QString performHaversine(const coordinate &one, const coordinate &two);


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QFile in, out;
    QXmlStreamReader input;
    QXmlStreamWriter output;
    open(input,output, in, out);
    performReadAndWrite(input,output);
    cout << "done\n";


//    return a.exec();
    return 0;
}


void open(QXmlStreamReader &toInput, QXmlStreamWriter &toOutput, QFile &in, QFile &out)
{
    out.setFileName("distances12INT.xml");
    in.setFileName("/Users/blank/Documents/Honors_Project/build-reparser3-Desktop_Qt_5_11_0_clang_64bit-Debug/santized_4_8.xml");
//    if(!in.open(QFile::ReadOnly | QFile::Text))
//    {
//       qDebug() << "Cannot read file" << in.errorString();
//       exit(0);
//    }
    in.open(QIODevice::ReadOnly);
    toInput.setDevice(&in);
    out.open(QIODevice::WriteOnly);
    toOutput.setDevice(&out);
    toOutput.setAutoFormatting(true);
    toOutput.writeStartDocument();
}

void performReadAndWrite(QXmlStreamReader &toInput, QXmlStreamWriter &toOutput)
{

    toOutput.writeStartElement("EAGLE_ROCK");
    vector<coordinate> addresses;
    QString speedLimit;
    map<coordinate, QString> existsAlready;
    unsigned int rank = 0;
    toInput.readNextStartElement();
    if(toInput.hasError())
        cout << "no bueno\n";
    while(!toInput.atEnd())
    {
        unsigned int usePos = 0;

        toInput.readNextStartElement();

        if(toInput.name() == "STREET")
        {
            toInput.readNextStartElement();
            toOutput.writeStartElement("STREET");
            toOutput.writeTextElement("STREET_NAME", toInput.readElementText());

            toInput.readNextStartElement();
            toInput.readElementText();
            //toOutput.writeTextElement("STREET_NUM", toInput.readElementText());

            toInput.readNextStartElement();
            toOutput.writeTextElement("STREET_SFX", toInput.readElementText());

            toInput.readNextStartElement();
            toInput.readElementText();
            //toOutput.writeTextElement("TDIR", toInput.readElementText());
            toInput.readNextStartElement();

            //toOutput.writeTextElement("ZIP", toInput.readElementText());
            speedLimit = toInput.readElementText();
            toInput.readNextStartElement();

            speedLimit = toInput.readElementText();
            toInput.readNextStartElement();
            cout << "speed limit is:  " << speedLimit.toStdString() << endl;

            while(toInput.name() == "FROM_ADDRESS")
            {
                coordinate temp;
                temp.lat = toInput.readElementText();
                toInput.readNextStartElement();
                temp.lon = toInput.readElementText();
                addresses.push_back(temp);
                toInput.readNextStartElement();
            }
            cout << "ooooo my\n";
            toOutput.writeTextElement("FROM_ADDRESSES", addresses[0].lat);
            toOutput.writeTextElement("TO_ADDRESSES", addresses[0].lon);
            ++usePos;

            unsigned int counter = 0;
            vector<coordinate> temp1;
            bool intersection = false;

            while(toInput.name() == "LATTITUDE")
            {
                coordinate temp;
                temp.lat = toInput.readElementText();
                toInput.readNextStartElement();
                temp.lon = toInput.readElementText();
                toInput.readNextStartElement();
                if(!existsAlready.count(temp))
                    existsAlready[temp] = QString::fromStdString(to_string(rank++));
                ++counter;
                if(temp1.size())
                {
                    if(intersection)
                    {
                        temp1.clear();
                        temp1.push_back(temp);
                    }
                    else
                    {
                        toOutput.writeStartElement("EDGE");

                        toOutput.writeStartElement("FFROM_VERTEX");
                        toOutput.writeTextElement("LATTITUDE", temp1[0].lat);
                        toOutput.writeTextElement("LONGITUDE", temp1[0].lon);
                        toOutput.writeTextElement("rank", existsAlready[temp1[0]]);
                        toOutput.writeEndElement();


                        toOutput.writeTextElement("WEIGHT", performHaversine(temp1[0], temp));
                        toOutput.writeTextElement("TIME", QString::fromStdString(to_string(performHaversine(temp1[0], temp).toDouble()/speedLimit.toDouble())))
                                ;


                        toOutput.writeStartElement("TO_VERTEX");
                        toOutput.writeTextElement("LATTITUDE", temp.lat);
                        toOutput.writeTextElement("LONGITUDE", temp.lon);
                        toOutput.writeTextElement("rank", existsAlready[temp]);
                        toOutput.writeEndElement();

                        toOutput.writeEndElement();
                        counter = 0;
                        temp1.clear();
                        temp1.push_back(temp);
                    }
                    intersection = false;
                }
                else
                    temp1.push_back(temp);

                if(toInput.name() == "INTERSECTION")
                {
                    toOutput.writeTextElement("FROM_ADDRESSES", addresses[usePos].lat);
                    toOutput.writeTextElement("TO_ADDRESSES", addresses[usePos].lon);
                    ++usePos;
                    temp.lat = toInput.readElementText();
                    toInput.readNextStartElement();
                    intersection = true;
                }
            }
            toOutput.writeEndElement();
//            cout << "currently looking at: " << toInput.name().toString().toStdString() << endl;
        }
        addresses.clear(); //added clear
        if(toInput.hasError())
        {
            cout << "error: " << toInput.errorString().toStdString() << endl;
        }
    }
    toOutput.writeEndElement();
    toOutput.writeEndDocument();
}

QString performHaversine(const coordinate &one, const coordinate &two)
{
   double lat1 = one.lat.toDouble() * (PI/180.), long1 = one.lon.toDouble() * (PI/180.);
   double lat2 = two.lat.toDouble() * (PI/180.), long2 = two.lon.toDouble()* (PI/180.),
           dLat = lat2 - lat1, dLong = long2 - long1;
    double computation = asin(sqrt(sin(dLat / 2.) * sin(dLat / 2.) + cos(lat1) * cos(lat2) * sin(dLong / 2.) * sin(dLong / 2.)));
    double d = 3959.9 * 2 * computation;
    return QString::fromStdString(to_string(d));
}

