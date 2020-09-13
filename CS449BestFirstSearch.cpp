// CS449BestFirstSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include <queue>
#include <iomanip>
#include <stack>
#include <set>

#include "node.h"

using namespace std;

//the main search function. Jank.
void BestFS(node current, node goal);

// override priority queue comparison thing. Might be better in the node class? Oh well.
bool operator<(const node& a, const node& b)
{
    return a.dist > b.dist;
}


// displays possible choices 
void openingScreen() 
{
    ifstream cityListy;
    cityListy.open("CoordinatesNew.txt");
    int count = 0;
    string city;
    cout << "Welcome to the Totally Accurate Travel Guide. Your cities are: " << endl;


    if (cityListy.fail())
    {
        cout << "this ain't it chief" << endl;
        exit(1);
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

//driver
int main()
{
    ifstream cityCoord;
    ifstream cityAdj;
    string startName, goalName;
    set<string> validNames;
    char looperino = ' '; // for starting the loop 
    int validInputCount = 0;


    //set up valid names for search
    cityCoord.open("CoordinatesNew.txt");

    string cityValid;

    cin.clear();
    while (!cityCoord.eof())
    {

        cityCoord >> cityValid;
        validNames.insert(cityValid);
        cityCoord.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    cityCoord.close();
    while (looperino != 'Q')
    {
        openingScreen();

        // Error checking needed later for valid cities

        cout << endl << endl;

        while (validInputCount == 0)
        {
            cout << "Enter your starting city: ";
            cin >> startName;
            if (validNames.count(startName) == 1)

            {
                validInputCount++;
            }
            else
            {
                cout << "No such city found. Please enter a valid city from the choices above" << endl;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cout << endl;

        

        validInputCount = 0;

        while (validInputCount == 0)
        {
            cout << "Enter your goal city: ";
            cin >> goalName;
            if (validNames.count(goalName) == 1)

            {
                validInputCount++;
            }
            else
            {
                cout << "No such city found. Please enter a valid city from the choices above" << endl;
            }
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

        }
        cout << endl;

        cin.clear();
 

        node startNode;
        node goalNode;


        //Set up the nodes for the search function

        //Find the goal node
        cityCoord.open("CoordinatesNew.txt");
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

        //debug
        //goalNode.displayState();

        cityCoord.open("CoordinatesNew.txt");
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

        //debug
        //startNode.displayState();
        cityCoord.close();

        cout << "Beginning Search Function" << endl;
        cout << "Starting at: " << endl;
        cout << startNode.name << " " << startNode.dist << "->" << endl << endl;

        BestFS(startNode, goalNode);

        cout << "Find another route? Type and enter 'Q' to quit, or any other key to continue: ";
        cin >> looperino;
        looperino = toupper(looperino);
        cin.clear();


    }

    cout << "GOODBYE" << endl;
    system("pause");
    return 0;
}

void BestFS(node current, node goal)
{
    priority_queue<node> frontier; //cities we have no visited yet, but can see
    queue<node> path; // trace the path
    int count = 0; // generic count 
    int dangerLoop = 0;
    node megaTemp; // even more temporary variables because yes

    set<node> visited; // keep track of what we have visited to keep out of queue
    

    ifstream cityCoord;
    ifstream cityAdj;

    node temp = current;

    visited.insert(temp);
    while (temp.name != goal.name) // check if at goal
    {
        string tempName; // temp variable
        cityAdj.open("AdjacenciesNew.txt"); // open adjacenies to start loading in nodes
        while (!cityAdj.eof()) // go through the adj file
        {
            cityAdj >> tempName; // start checking for the adjacency list
            if (tempName == temp.name) // if the tempAdj name matches the current node's name
            {
                while (cityAdj.peek() != '\n') // while we do not reach a newline character in adj
                {
                    cityAdj >> tempName; // load in the next name. We're building the "adjacent nodes"
                    count++;
                    cityCoord.open("CoordinatesNew.txt"); //open the coordinates file
                    string tempCoordN; // temporoary name storage for file processing
                    float tx, ty; //temp x y storage
                    while (!cityCoord.eof()) // going through the file..
                    {
                        cityCoord >> tempCoordN; //load in the city name
                        if (tempCoordN == tempName) // if we find a match 
                        {
                            // might have to check the set for duplicate names later, aka what we've alreayd visited
                            //idea: when you reach a new node and it will loop, check the closest child, and if it is in the visited,
                            //break out, pop that value out of the visited, and go back to the frontier city.
                            cityCoord >> tx >> ty; // load in the x y coords for the current city
                            node* p = new node; // make a new pointer
                            p->setData(tempCoordN, tx, ty, findDist(tx, ty, goal.x, goal.y)); // "construct" the node
                            if (visited.count(*p) >= 1) // in theory, if this node already exists within the visited struct, ignore it
                            {
                                //IT WORKS but have to make sure to fix the cities with only one adjacency looping back
                                //The idea to fix is to check if the name exists in Visited and if 
                                //  peek('\n') immediately returns after one time. So might need a counter
                                
                                cityCoord.close(); //closes the file
                                break; // or continue? Only testing will tell
                            }
                            frontier.push(*p); // push the actual value on to the frontier
                            cityCoord.close(); // close file
                            break; // break from loop, and go on til the next name in the adjacency file
                        }
                        else // if the names do not match any coordinates...
                        {
                            cityCoord.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // ignore the rest and go to next line
                        }
                    }
                }
            }

            else // if the names do not match in the adjacency file...
            {
                cityAdj.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // go to next line to try and find adjacency
                continue;
            }

            break;// when the file is done with, end the loop. 
        }

        //need to reverse this to get the "minimum". Might want to check the bool override.

         // close the file
        if (cityAdj.peek() == '\n' && /*count <= 1 &&*/ frontier.size() == 0)
        {
            cout << temp.name << " " << temp.dist << endl; //output the city 
            visited.insert(temp); //put that city in the visited so we avoid it when going back to the frontier. This is probably unnecessary
                                   // since the city is already in the visited thing, but just to make sure IT DOESN'T HAPPEN
            temp = megaTemp; // reset temp to the previous city, which megaTemp stores
            dangerLoop++;
            priority_queue<node>  empty; // create an empty queue
            swap(frontier, empty); // clear the queue by swapping it with an empty version
            count = 0; //reset the counter
            cityAdj.close(); // close the file
            if (dangerLoop >= 10)
            {
                cout << "Too many loops detected. The Best First Search has failed to find a path :/ . " << endl;
                break;
            }
            cout << "Encountered a loop. Returning to select a new city..." << endl;
            continue;
        }

        cityAdj.close();


        megaTemp = temp; //this is to preseve the last node just in case the next search returns a loop. 
        cout << frontier.top().name << " " << frontier.top().dist << endl;
        visited.insert(frontier.top()); // put the node in the visited container
        path.push(frontier.top()); // push that into our path.
        temp = frontier.top(); // we change our temp to the "top" (lowest) value and start the loop again.
        priority_queue<node>  empty;
        swap(frontier, empty); // clear the queue by swapping it with an empty version
        count = 0;
    }

}
