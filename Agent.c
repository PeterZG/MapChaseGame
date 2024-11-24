// Implementation of the Agent ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Agent.h"
#include "Map.h"

#define INFINITY_LARGE 9999
#define NEGATIVE_INFINITY -1
#define ZERO 0

// This struct stores information about an individual agent and can be
// used to store information that the agent needs to remember.
struct agent {
    char *name;
    int startLocation;
    int location;
    int maxStamina; // max stamina
    int stamina;    // current stamina
    int strategy;
    Map map;

    // Cheapest-Least-Visited strategy Part
    int *visted;   // array of visited cities for CLV strategy

    // Depth-First-Search strategy Part
    int *visted_d;  // array of visited cities for DFS strategy
    int *path;  // path to current location
    int start;  // start of the path
    int total;  // total distance
    int count;  // count of the path
};

static Move chooseRandomMove(Agent agent, Map m);
static int filterRoads(Agent agent, Road roads[], int numRoads,
                       Road legalRoads[]);

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// ADDED FUNCTION //////////////////////////////////

static Move CLVMove(Agent agent, Map m);
static Move DFSMove(Agent agent, Map m);
static void ForDFSMove(Agent agent, Map m, int visited);
static int LeastVisted(Agent agent, int numLegalRoads, Road legalRoads[]);
static int compareRoads(const void *a, const void *b);

////////////////////////////////// ADDED FUNCTION //////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Creates a new agent
 */
Agent AgentNew(int start, int stamina, int strategy, Map m, char *name) {
    if (start >= MapNumCities(m)) {
        fprintf(stderr, "error: starting city (%d) is invalid\n", start);
        exit(EXIT_FAILURE);
    }

    Agent agent = malloc(sizeof(struct agent));
    if (agent == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    agent->startLocation = start;
    agent->location = start;
    agent->maxStamina = stamina;
    agent->stamina = stamina;
    agent->strategy = strategy;
    agent->map = m;
    agent->name = strdup(name);

    // TODO: You may need to add to this

    // initalise visted count array(CLV)
    int array = MapNumCities(agent->map);
    agent->visted = malloc(array * sizeof(int));
    if (agent->visted == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < array; i++) {
        agent->visted[i] = ZERO;
    }
    agent->visted[start] = 1;
    // for memory management of DFS
    agent->visted_d = malloc(array * sizeof(int));
    agent->count = ZERO;
    agent->start = NEGATIVE_INFINITY;
    int double_a = 2 * array;
    agent->path = calloc(double_a, sizeof(int));

    for (int i = 0; i < double_a; i++) {
        // loop the array, default value
        agent->path[i] = NEGATIVE_INFINITY;
    }
    // TODO: You may need to add to this
    return agent;
}

/**
 * Frees all memory associated with the agent
 * NOTE: You should not free the map because the map is owned by the
 *       main program, and the main program will free it
    */
void AgentFree(Agent agent) {
    // CLV free part
    free(agent->visted);
    free(agent->name);
    // DFS free part
    free(agent->visted_d);
    free(agent->path);
    free(agent);
}

////////////////////////////////////////////////////////////////////////
// Making moves

/**
 * Calculates the agent's next move
 */
Move AgentGetNextMove(Agent agent, Map m) {
    switch (agent->strategy) {
        case STATIONARY: return (Move){agent->location, 0};
        case RANDOM:     return chooseRandomMove(agent, m);
        // more strategies here
        case CHEAPEST_LEAST_VISITED: return CLVMove(agent, m);
        case DFS:
            if (agent->start == NEGATIVE_INFINITY) {
                agent->start = agent->location;
                ForDFSMove(agent, m, agent->start);
                agent->count = ZERO;
            }
            else if (agent->count == agent->total) {
                    agent->count = ZERO;
                    agent->total = ZERO;
                    agent->start = NEGATIVE_INFINITY;
            }
            return DFSMove(agent, m);
        // more strategies here
        default:
            printf("error: strategy not implemented yet\n");
            exit(EXIT_FAILURE);
    }
}

static Move chooseRandomMove(Agent agent, Map m) {
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));

    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);

    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    Move move;
    if (numLegalRoads > 0) {
        // Sort the roads using insertion sort
        for (int i = 1; i < numLegalRoads; i++) {
            Road r = legalRoads[i];
            int j = i;
            while (j > 0 && r.to < legalRoads[j - 1].to) {
                legalRoads[j] = legalRoads[j - 1];
                j--;
            }
            legalRoads[j] = r;
        }

        // nextMove is randomly chosen from the legal roads
        int k = rand() % numLegalRoads;
        move = (Move){legalRoads[k].to, legalRoads[k].length};
    } else {
        // The agent must stay in the same location
        move = (Move){agent->location, 0};
    }

    free(legalRoads);
    free(roads);
    return move;
}

// Takes an array with all the possible roads and puts the ones the agent
// has enough stamina for into the legalRoads array
// Returns the number of legal roads
static int filterRoads(Agent agent, Road roads[], int numRoads,
                       Road legalRoads[]) {
    int numLegalRoads = ZERO;
    for (int i = 0; i < numRoads; i++) {
        if (roads[i].length <= agent->stamina) {
            legalRoads[numLegalRoads++] = roads[i];
        }
    }
    return numLegalRoads;
}

/**
 * Executes a given move by updating the agent's internal state
 */
void AgentMakeNextMove(Agent agent, Move move) {
    if (move.to == agent->location) {
        agent->stamina = agent->maxStamina;
    } else {
        agent->stamina -= move.staminaCost;
    }
    agent->location = move.to;
    // TODO: You may need to add to this to handle different strategies
}

////////////////////////////////////////////////////////////////////////
// Gets information about the agent
// NOTE: It is expected that these functions do not need to be modified

/**
 * Gets the name of the agent
 */
char *AgentName(Agent agent) {
    return agent->name;
}

/**
 * Gets the current location of the agent
 */
int AgentLocation(Agent agent) {
    return agent->location;
}

/**
 * Gets the amount of stamina the agent currently has
 */
int AgentStamina(Agent agent) {
    return agent->stamina;
}

////////////////////////////////////////////////////////////////////////
// Learning information

/**
 * Tells the agent where the thief is
 */
void AgentGainInfo(Agent agent, int thiefLocation) {
    // is the location of the thief
    agent->visted[thiefLocation] = 1;
}


////////////////////////////////////////////////////////////////////////
// Displaying state

/**
 * Prints information about the agent (for debugging purposes)
 */
void AgentShow(Agent agent) {
    int city = agent->location;
    printf("%s is in %s with %d stamina\n", agent->name,
           MapGetName(agent->map, city), agent->stamina);
    if (strcmp(agent->name, "Thief") == ZERO) {
        printf("Thief is in %s\n", MapGetName(agent->map, city));
    }
    putchar('\n');
}

////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////// ADDED FUNCTION //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////

// compare two roads and return the one with the shortest length
static int compareRoads(const void *a, const void *b) {
    Road r1 = *(Road *)a;
    Road r2 = *(Road *)b;
    if (r1.length == r2.length) {
        return r1.to - r2.to;
    }
    else {
        return r1.length - r2.length;
    }
}

// returns the next move for the cheapest least visited strategy
static Move CLVMove(Agent agent, Map m) {
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));
    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, agent->location, roads);
    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);

    Move move;
    // Sort the roads using insertion sort
    if (numLegalRoads > 0) {
        // To find the least visted city
        int Road_visit = LeastVisted(agent, numLegalRoads, legalRoads);
        if (Road_visit > 1) {
                // if only one road, then go there
            qsort(legalRoads, Road_visit, sizeof(legalRoads[0]), compareRoads);
            move = (Move){legalRoads[0].to, legalRoads[0].length};
            agent->visted[agent->location]++;
        }
        else if (Road_visit == 1) {
            move = (Move){legalRoads[0].to, legalRoads[0].length};
            agent->visted[agent->location]++;
        }
    }
    // The agent must stay in the same location
    else {
        move = (Move){agent->location, 0};
        agent->visted[agent->location]++;
    }
    free(legalRoads);
    free(roads);
    return move;
}

// for the cheapest least visited strategy
static int LeastVisted(Agent agent, int numLegalRoads, Road legalRoads[]) {
    // find least visted cities
    // the larger number is better
    int LRecord = INFINITY_LARGE;
    // assemble array of least cities
    int roads = ZERO;
    // find the least visted city
    for (int i = 0; i < numLegalRoads; i++) {
        if (agent->visted[legalRoads[i].to] < LRecord) {
            LRecord = agent->visted[legalRoads[i].to];
        }
    }
    // find the roads that have the least visted city
    for (int i = 0; i < numLegalRoads; i++) {
        if (agent->visted[legalRoads[i].to] == LRecord) {
            legalRoads[roads++] = legalRoads[i];
        }
    }
    return roads;
}

// for the Depth-First-Search strategy: find the path
static void ForDFSMove(Agent agent, Map m, int visited) {
    Road *roads = malloc(MapNumCities(m) * sizeof(Road));
    Road *legalRoads = malloc(MapNumCities(m) * sizeof(Road));
    // Get all roads to adjacent cities
    int numRoads = MapGetRoadsFrom(m, visited, roads);
    // Filter out roads that the agent does not have enough stamina for
    int numLegalRoads = filterRoads(agent, roads, numRoads, legalRoads);
    agent->visted_d[visited] = 1;

    if (numLegalRoads > 0) {
        // Sort the roads using insertion sort
        for (int i = 1; i < numLegalRoads; i++) {
            Road temp = legalRoads[i];
            int j = i;
            for (int j = i; j > 0 && temp.to < legalRoads[j - 1].to; j--) {
                legalRoads[j] = legalRoads[j - 1];
            }
            legalRoads[j] = temp;
        }
        // for traveling every single roads
        for (int i = 0; i < numLegalRoads; i++) {
            Road temp = legalRoads[i];
            // check the next city is not visited
            if (agent->visted_d[temp.to] == 0) {
                agent->path[agent->total] = temp.to;
                agent->total++;
                ForDFSMove(agent, m, temp.to);
                agent->path[agent->total] = visited;
                agent->total++;
            }
        }
    }
    free(legalRoads);
    free(roads);
}

// Depth-First-Search strategy
static Move DFSMove(Agent agent, Map m) {
    // move according to the array of path
    int stage = agent->path[agent->count];
    int contained_road = MapContainsRoad(m, agent->location, stage);

    Move move;
    // if the agent can travel to the next city
    if (agent->stamina < contained_road) {
        move = (Move){agent->location, 0};
    }
    else if (agent->stamina >= contained_road) {
        move = (Move){stage, contained_road};
        agent->count++;
    }
    return move;
}

////////////////////////////////// ADDED FUNCTION //////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
