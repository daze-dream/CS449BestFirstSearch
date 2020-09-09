// CS449BestFirstSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>
#include "node.h";

using namespace std;

// override priority queue comparison thing. Might be better in the class? Oh well.
bool operator<(const node& a, const node& b)
{
    return a.dist < b.dist;
}


// displays possible choices 
void openingScreen() 
{
    ifstream cityListy;
    cityListy.open("coordinates.txt");
    int count = 0;
    string city;
    cout << "Welcome to the Totally Accurate Travel Guide. Your cities are: " << endl;


    if (cityListy.fail())
    {
        cout << "this ain't it chief" << endl;
        exit;
    }

    while (!cityListy.eof())
    {
        if (count == 6)
        {
            cout << endl;
            count = 0;
        }
        cityListy >> city;
        cout << left << setw(15) << city;
        count++;
        cityListy.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// distance formula
float findDist(float cx, float cy, float gx, float gy) 
{
    float p1 = pow((gx - cx), 2);
    float p2 = pow((gy - cy), 2);
    float p3 = p1 + p2;
    return sqrt(p3);
}


int main()
{
    ifstream cityCoord;
    ifstream cityAdj;
    string startName, goalName;


    /* 
    here's what we do: we make a node for the start. Then we make new nodes from the adjacency and coordinates list, calculate the
        dist, and then push them onto the frontier queue. Then we select the best one and do the search again, so I guess
        it's recursive. hard part is going to be managing the files. Might be some overhead? meh for now.
    */
    
    

    // testing distance formula. 
    openingScreen();


   
    // Error checking needed later

    cout << endl << endl;
    cout << "Enter your starting city: ";
    cin >> startName;
    cout << endl;

    cout << "Enter your ending city: ";
    cin >> goalName;
    cout << endl;

    cout << "Beginning Search" << endl;
    node startNode;
    node goalNode;
    priority_queue<node> frontier;




    //Set up the nodes for the search function

    //Find the goal node
    cityCoord.open("coordinates.txt");
    while (!cityCoord.eof())
    {
        
        string temp;
        float x, y, final;
        cityCoord >> temp;
        if (temp != goalName)
        {
            cityCoord.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        cityCoord >> x >> y;
        goalNode.setData(temp, x, y, 0);
        break;
    }
    cityCoord.close();

    goalNode.displayState();

    cityCoord.open("coordinates.txt");
    //start node
    while (!cityCoord.eof())
    {
        string temp;
        float x, y, final;
        cityCoord >> temp;
        if (temp != startName)
        {
            cityCoord.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }

        cityCoord >> x >> y;
        startNode.setData(temp, x, y, findDist(x, y, goalNode.x, goalNode.y));
        break;
    }

    startNode.displayState();
    cityCoord.close();


    

    cout << endl;
    system("pause");
    return 0;
}


