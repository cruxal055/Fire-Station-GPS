#include <QCoreApplication>
#include <iostream>
#include <QXmlStreamReader>
#include <map>
#include <QFile>
#include <set>
#include <algorithm>
#include <cmath>

using namespace std;

#define sSet set<street>

#define PI 4.0*atan(1.0)


struct coordinates
{
    public:
        QString lattitude, longitude;
        coordinates()
        {
            lattitude = " ";
            longitude = " ";
        }
        coordinates &operator=(const coordinates &other)
        {
            if(this != &other)
            {
                lattitude = other.lattitude;
                longitude = other.longitude;
            }
            return *this;
        }

        coordinates(const coordinates &other)
        {
            lattitude = other.lattitude;
            longitude = other.longitude;
        }

        friend bool operator<=(const coordinates &o1 ,const coordinates &o2)
        {
            return (o1.longitude.toDouble() + o1.lattitude.toDouble()) <= ((o2.longitude.toDouble() + o2.lattitude.toDouble()));
        }

        friend bool operator>=(const coordinates &o1 ,const coordinates &o2)
        {
            return (o1.longitude.toDouble() + o1.lattitude.toDouble()) >= ((o2.longitude.toDouble() + o2.lattitude.toDouble()));
        }
        friend bool operator>(const coordinates &o1 ,const coordinates &o2)
        {
            return (o1.longitude.toDouble() + o1.lattitude.toDouble()) > ((o2.longitude.toDouble() + o2.lattitude.toDouble()));
        }
        friend bool operator<(const coordinates &o1 ,const coordinates &o2)
        {
            return (o1.longitude.toDouble() + o1.lattitude.toDouble()) < ((o2.longitude.toDouble() + o2.lattitude.toDouble()));
        }
};


struct tempData
{
    double distance;
    coordinates place;
    int pos;
    int speedLimit;
    tempData()
    {
        distance = 0.;
        pos = 0;
        place = coordinates();
    }

    tempData& operator=(const tempData &other)
    {
        if(this != &other)
        {
            pos = other.pos;
            distance = other.distance;
            place = other.place;
        }
        return  *this;
    }

    tempData(const tempData &other)
    {
        pos = other.pos;
        distance = other.distance;
        place = other.place;
    }

    friend bool operator<=(const tempData &o1 ,const tempData &o2)
    {
        return o1.distance <= o2.distance;
    }


    friend bool operator>=(const tempData &o1 ,const tempData &o2)
    {
        return o1.distance >= o2.distance;
    }

    friend bool operator<(const tempData &o1 ,const tempData &o2)
    {
        return o1.distance < o2.distance;
    }


    friend bool operator>(const tempData &o1 ,const tempData &o2)
    {
        return o1.distance > o2.distance;
    }


};

class street
{
    public:

        QString fromAddress, toAddress, streetNum, zipCode;
        int speedLimit;
        QString streetName, streetSFX;
        QString direction;
        vector<coordinates> waypoints;
        set<coordinates> hasAlready;

        street()
        {
            fromAddress = toAddress = streetNum = zipCode = "";
            streetName = streetSFX = "";
            direction = "";
            speedLimit = 0;
        }

        street(const street &other)
        {
            fromAddress = other.fromAddress;
            toAddress = other.toAddress;
            streetNum = other.streetNum;
            zipCode = other.zipCode;
            streetName = other.streetName;
            streetSFX = other.streetSFX;
            direction = other.direction;
            waypoints = other.waypoints;
            hasAlready = other.hasAlready;
            speedLimit = other.speedLimit;
        }

        street& operator=(const street &other)
        {
            if(this != &other)
            {
                fromAddress = other.fromAddress;
                toAddress = other.toAddress;
                streetNum = other.streetNum;
                zipCode = other.zipCode;
                streetName = other.streetName;
                streetSFX = other.streetSFX;
                direction = other.direction;
                waypoints = other.waypoints;
               hasAlready = other.hasAlready;
               speedLimit = other.speedLimit;

            }
            return *this;
        }
        friend bool operator<=(const street &o1 ,const street &o2)
        {
            return o1.fromAddress.toInt() <= o2.fromAddress.toInt() ;
        }

        friend bool operator>=(const street &o1 ,const street &o2)
        {
            return o1.fromAddress.toInt() >= o2.fromAddress.toInt() ;
        }
        friend bool operator>(const street &o1 ,const street &o2)
        {
            return o1.fromAddress.toInt() > o2.fromAddress.toInt();
        }
        friend bool operator<(const street &o1 ,const street &o2)
        {
            return o1.fromAddress.toInt() < o2.fromAddress.toInt() ;
        }
};

double performHaversine(const coordinates &one, const coordinates &two)
{

       double lat1 = one.lattitude.toDouble() * (PI/180.), long1 = one.longitude.toDouble() * (PI/180.);
       double lat2 = two.lattitude.toDouble() * (PI/180.), long2 = two.longitude.toDouble()* (PI/180.),
               dLat = lat2 - lat1, dLong = long2 - long1;
        double computation = asin(sqrt(sin(dLat / 2.) * sin(dLat / 2.) + cos(lat1) * cos(lat2) * sin(dLong / 2.) * sin(dLong / 2.)));
        double d = 3959.9 * 2 * computation;
        return d;
}

void writeOut(map<QString, sSet> &toOutput)
{
    QFile out;
    QXmlStreamWriter output;
    out.setFileName("santized_4_8.xml");
    out.open(QIODevice::WriteOnly);
    output.setDevice(&out);
    output.setAutoFormatting(true);
    output.writeStartDocument();
    output.writeStartElement("EAGLE_ROCK");
    for(auto i = toOutput.begin(); i != toOutput.end(); ++i)
    {
        auto temp = i->second.begin();
        output.writeStartElement("STREET");
        output.writeTextElement("STREET_NAME", temp->streetName);
        output.writeTextElement("STREET_NUM", temp->streetNum);
        output.writeTextElement("STREET_SFX", temp->streetSFX);
        output.writeTextElement("TDIR", temp->direction);
        output.writeTextElement("ZIP", temp->zipCode);
        output.writeTextElement("SPEED_LIMIT", QString::fromStdString(to_string(temp->speedLimit)));

       for(auto j = i->second.begin(); j != i->second.end(); ++j)
       {
           output.writeTextElement("FROM_ADDRESS", j->fromAddress
                                   );
           output.writeTextElement("TO_ADDRESS", j->toAddress);
       }
       for(auto j = i->second.begin(); j != i->second.end(); ++j)
       {
           for(unsigned int k = 0; k < j->waypoints.size(); ++k)
           {
               output.writeTextElement("LATTITUDE" , j->waypoints[k].lattitude);
               output.writeTextElement("LONGITUDE" ,  j->waypoints[k].longitude);
           }
           auto temp = j;
           if( ++temp != i->second.end())
            output.writeTextElement("INTERSECTION" ,  "-------------------------------");

       }
       output.writeEndElement();
    }
    output.writeEndElement();
    output.writeEndDocument();

}

void justDisplay(map<QString, sSet> &toOutput)
{
    for(auto i = toOutput.begin(); i != toOutput.end() ; ++i)
    {
        cout << "for street: " << i->first.toStdString() << endl;
        for(auto j = i->second.begin(); j != i->second.end(); ++j)
        {
            cout << "FROM_ADDRESS: " << j->fromAddress.toStdString() << endl;
            cout << "TO_ADDRESS: " << j->toAddress.toStdString() << endl;;
        }
        cout << endl;
    }
}
int main(int argc, char *argv[])
{
    QFile stream;
    stream.setFileName("/Users/blank/Documents/Honors_Project/build-reparserTWO-Desktop_Qt_5_11_0_clang_64bit-Debug/streets10_2.xml");
    stream.open(QFile::ReadOnly | QFile::Text);
    map<QString,sSet> master;
    QXmlStreamReader streamer;
    streamer.setDevice(&stream);
    streamer.readNextStartElement();
    streamer.readNextStartElement();
    bool skp = false;

    while(!streamer.atEnd())
    {
        string item;
        street temp;
        coordinates grid;

        if(streamer.name() == "STREET" )
        {
            streamer.readNextStartElement();

            temp.streetName = streamer.readElementText();


            streamer.readNextStartElement();
            temp.streetNum = streamer.readElementText();

            streamer.readNextStartElement();
            if(streamer.name() == "STSFX")
            {
                temp.streetSFX = streamer.readElementText();
                streamer.readNextStartElement();
            }

            temp.speedLimit = streamer.readElementText().toInt();
            streamer.readNextStartElement();


            temp.direction = streamer.readElementText();
            streamer.readNextStartElement();

            temp.zipCode = streamer.readElementText();
            streamer.readNextStartElement();

            if(streamer.name() =="SKIP")
            {
                streamer.readElementText();
                streamer.readNextStartElement();
                streamer.readNextStartElement();
                skp = true;
                continue;
            }

            skp = false;
            temp.fromAddress = streamer.readElementText();
            streamer.readNextStartElement();


            temp.toAddress = streamer.readElementText();
            streamer.readNextStartElement();



            while(streamer.name() == "LATITUDE")
            {
                grid.lattitude = streamer.readElementText();


                streamer.readNextStartElement();
                grid.longitude = streamer.readElementText();

                streamer.readNextStartElement();

                temp.hasAlready.insert(grid);
                temp.waypoints.push_back(grid);

            }
            streamer.readNextStartElement();
        }
        else
        {
            cout << "finished parsing\n";
            break;
        }
        if(streamer.hasError())
        {
            cout << streamer.errorString().toStdString() << endl;
        }
        //figueroa
        if(temp.streetName == "FIGUEROA")
        {
            if(5921 >= temp.fromAddress.toInt() && 5921 <= temp.toAddress.toInt())
            {
                coordinates tempC;
                vector<tempData> stuff;
                tempC.lattitude = "34.11091760266492";
                tempC.longitude = "-118.19074248113876";
                for(unsigned int i = 0; i < temp.waypoints.size(); ++i)
                {
                    tempData ono;
                    ono.place = temp.waypoints[i];
                    ono.pos = i;
                    ono.distance = performHaversine( temp.waypoints[i], tempC);
                    stuff.push_back(ono);
                }
                sort(stuff.begin(), stuff.end());
                temp.waypoints.begin() + stuff[0].pos;
                temp.waypoints.insert(temp.waypoints.begin() + stuff[0].pos,  tempC);
            }
        }
        else
        {
            if(temp.streetName == "YORK")
            {
                if(4455 >= temp.fromAddress.toInt() && 4455 <= temp.toAddress.toInt())
                {

                    coordinates tempC;
                    vector<tempData> stuff;
                    tempC.lattitude = "34.12416049793684";
                    tempC.longitude = "-118.21936667211293";
                    for(unsigned int i = 0; i < temp.waypoints.size(); ++i)
                    {
                        tempData ono;
                        ono.place = temp.waypoints[i];
                        ono.pos = i;
                        ono.distance = performHaversine( temp.waypoints[i], tempC);
                        stuff.push_back(ono);
                    }
                    sort(stuff.begin(), stuff.end());
                    temp.waypoints.begin() + stuff[0].pos;
                    temp.waypoints.insert(temp.waypoints.begin() + stuff[0].pos,  tempC);
                }
            }
            else
            {
                if(temp.streetName == "COLORADO")
                {
                    if(2021 >= temp.fromAddress.toInt() && 2021 <= temp.toAddress.toInt())
                    {
                        cout << "from: " << temp.fromAddress.toStdString() << " to: " << temp.toAddress.toStdString() << endl;
                        coordinates tempC;
                        vector<tempData> stuff;
                        tempC.lattitude = "34.13948426993484";
                        tempC.longitude = "-118.21073194836286";
                        for(unsigned int i = 0; i < temp.waypoints.size(); ++i)
                        {
                            tempData ono;
                            ono.place = temp.waypoints[i];
                            ono.pos = i;
                            ono.distance = performHaversine( temp.waypoints[i], tempC);
                            stuff.push_back(ono);
                        }
                        sort(stuff.begin(), stuff.end());
                        temp.waypoints.begin() + stuff[0].pos;
                        temp.waypoints.insert(temp.waypoints.begin() + stuff[0].pos,  tempC);
                    }
                }
            }
        }
        master[temp.streetName + " " + temp.streetSFX].insert(temp);
    }
    writeOut(master);
    return 0;
}




