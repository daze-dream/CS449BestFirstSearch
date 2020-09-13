// CS449BestFirstSearch.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/* Best First Search
Takes the names of two cities and utilizing Best First Search with the heuristic of smallest distance to goal, finds a path.
This is a heavy I/O approach: instead of making the graph data into a data struct, this opens the files frequently and creates
    the nodes as necessary.
Improvements: probably could make the node creation a function.
              change file names so that it could work with any file.
*/

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
    ifstream cityCoord; //coordinates file
    ifstream cityAdj; //adjacencies file
    string startName, goalName; //names for input
    set<string> validNames; // a set to keep track of valid cities to input
    char looperino = ' '; // for starting the loop 
    int validInputCount = 0; // counter for while loops. Yes it is jank.

    //make sure the files we need are there

    cityCoord.open("CoordinatesNew.txt");
    cityAdj.open("AdjacenciesNew.txt");

    if (cityAdj.fail() || cityCoord.fail())
    {
        cout << "Necesary files were not found. Make sure that the files CoordinatesNew.txt and AdjacenciesNew.txt are in the directory. " << endl;
        return 0;
    }

    cityCoord.close();
    cityAdj.close();

    cityCoord.open("CoordinatesNew.txt");

 

    cin.clear();

    //set up valid names for search
    while (!cityCoord.eof())
    {
        string cityValid; //temp store names in coordinates
        cityCoord >> cityValid;
        validNames.insert(cityValid);
        cityCoord.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    cityCoord.close();
    while (looperino != 'Q') // This whole driver program is in a loop so you can find as many paths as you want.
    {
        //display the possible cities 
        openingScreen();


        cout << endl << endl;

        //begin loop for valid input
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

        

        validInputCount = 0; // reset variable

        //same as above
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

        validInputCount = 0; //reset

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
        cout << "START: " << endl;
        cout << left << setw(15) << startNode.name << " " << "| Distance remaining: " << startNode.dist << " " << "->" << endl;

        BestFS(startNode, goalNode);

        cout << "Find another route? Type and enter 'Q' to quit, or any other key to continue: ";
        cin >> looperino;
        looperino = toupper(looperino);
        //cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

    }

    cout << "GOODBYE" << endl;
    system("pause");
    return 0;
}

void BestFS(node current, node goal)
{
    priority_queue<node> frontier; //cities we have not visited yet, but can see
    queue<node> path; // trace the path
    int count = 0; // generic count 
    int dangerLoop = 0; // this is for when we get into an impossible loop
    node megaTemp; // even more temporary variables because yes

    set<node> visited; // keep track of what we have visited to keep out of queue
    

    ifstream cityCoord;
    ifstream cityAdj;

    node temp = current; //capture the node
    path.push(temp); // begin the path
    visited.insert(temp); // we have visited the start city.
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
                    count++; // this has been deprecated, but was useful for debugging.
                    cityCoord.open("CoordinatesNew.txt"); //open the coordinates file
                    string tempCoordN; // temporoary name storage for file processing
                    float tx, ty; //temp x y storage
                    while (!cityCoord.eof()) // going through the file..
                    {
                        cityCoord >> tempCoordN; //load in the city name
                        if (tempCoordN == tempName) // if we find a match 
                        {

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

        // Now...if we find ourselves at a loop where all the adjacent cities are visited, we go back to the old city and select something different
        if (cityAdj.peek() == '\n' && /*count <= 1 &&*/ frontier.size() == 0)
        {
            cout << temp.name << " " << "Distance remaining: " << temp.dist << " " <<  " ->" << endl; //output the city 
            visited.insert(temp); //put that city in the visited so we avoid it when going back to the frontier. This is probably unnecessary
                                   // since the city is already in the visited thing, but just to make sure IT DOESN'T HAPPEN
            temp = megaTemp; // reset temp to the previous city, which megaTemp stores
            dangerLoop++;
            priority_queue<node>  empty; // create an empty queue
            swap(frontier, empty); // clear the queue by swapping it with an empty version
            count = 0; //reset the counter
            cityAdj.close(); // close the file
            if (dangerLoop >= 10) // When the heuristic fails and the pathfinding is stuck, aka all the cities around it are visited...
            {
                cout << "FATAL ERROR: Too many loops detected. The Best First Search has failed to find a path :/ . " << endl;
                break;
            }
            cout << "Encountered a loop. Returning to previous city to select a new city..." << endl;
            continue;
        }

        cityAdj.close();


        megaTemp = temp; //this is to preseve the last node just in case the next search returns a loop. 
        cout << left << setw(15) << frontier.top().name << " " << "| Distance remaining: " << frontier.top().dist << " " << "->" << endl;
        visited.insert(frontier.top()); // put the node in the visited container
        path.push(frontier.top()); // push that into our path.
        temp = frontier.top(); // we change our temp to the "top" (lowest) value and start the loop again.
        priority_queue<node>  empty;
        swap(frontier, empty); // clear the queue by swapping it with an empty version
        count = 0;
    }

    // if a valid path was found, output the path we took. 
    if (temp.name == goal.name)
    {
        cout << "We've reached the goal! The path we took was: " << endl;
        while (path.size() != 0)
        {
            cout << path.front().name;
            if (path.size() != 1)
            {
                cout << " -> ";
            }
            path.pop();
        }
        cout << endl;

    }
    cout << "Pathfind completed" << endl;
}
