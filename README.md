# eagle_rock_firefighter
Welcome to the GPS shortest route calculator for Eagle Rock. This project was designed as a side project with my professor 
as my mentor and supervisor for it (Dr. John Paul Wilkinson of Pasadena City College). 

# Summary:
- This program essentially takes an input in the form of an address, or street and yields a path from the fire station that is 
closest to it.

### Features:
- In order to search up an address use the hourglass on the upper left hand side. This is a geocoder from Ersi which yields the coordinates of whatever address the user searches up. The geocoder and the code related to it are on the front-end portion of the program. This portion was done entirely via JS.
![geocoder](https://github.com/cruxal055/eagle_rock_firefighter/blob/master/Screen%20Shot%202019-05-26%20at%201.55.59%20AM.png)
#### Link to Ersi's geocoder: [Geocoder](https://developers.arcgis.com/javascript/3/jshelp/tutorial_geocoder.html)

###
- shortly after searching up an address, the closest fire station to the address will be calculated and a shortest path will be displayed.
![shortestPath](https://github.com/cruxal055/eagle_rock_firefighter/blob/master/Screen%20Shot%202019-05-26%20at%202.03.25%20AM.png)

- the program also allows you to calculate the shortest path based on the speed limit, and there is a small check box to allow for this feature
![shortestTime](https://github.com/cruxal055/eagle_rock_firefighter/blob/master/Screen%20Shot%202019-05-26%20at%202.04.51%20AM.png)

- also, if you search up a street rather than an address, the shortest route to the street will be used instead, however, it will provide a warning 

## How was the data retrieved?
- Using the City of Los Angeles's data on streets, which was formatted for arcGIS programs, I parsed through the "centerline.kml" data, filtering and writing out data pertaining to Eagle Rock and its streets to an XML file.
#### Link to data: [data](http://geohub.lacity.org/datasets/d3cd48afaacd4913b923fd98c6591276_36?geometry=-120.389%2C33.625%2C-116.434%2C34.422)
- Then, I filtered the data once more, using the Haversine formula to calculate distances between the streets, and the formula for time to calculate speed limit costs between the streets. The data was formatted two coordinates on the map as the "vertecies" for dijkstra's algorithm, and the distance + speed limit costs as the "weights" of the edges.
- the program uses Dijkstra's algorithm to calculate the shortest path between the fire stations and every single coordinate
- ## Some tidbits that I thought was super clever of me:
- in order to calculate the shortest path to a specific street, I utilized the underlying data structure that a map is based off of, a Binary Search Tree(BST) 
- I inputted the coordinate into the map, letting it perform an insertion 
- Then I subtracted the iterator that it returned to obtain the "root" of that new insertion, which represented the "shortest" path to that street.

## Libraries Used:
#### QT: [https://doc.qt.io/qt-5.9/qtgui-index.html](http://geohub.lacity.org/datasets/d3cd48afaacd4913b923fd98c6591276_36?geometry=-120.389%2C33.625%2C-116.434%2C34.422)

