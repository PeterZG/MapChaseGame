// Implementation of the Map ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Map.h"


struct map {
    int cities;     // The number of cities
    int nE;         // The number of edges
    int **edges;    // pointers to edges
    char **name;    // pointers to names
};


/**
 * Creates a new map with the given number of cities
 * Assumes that numCities is positive
 */
Map MapNew(int numCities) {
    assert(numCities >= 0);

    // Allocate memory for the map
    Map map = malloc(sizeof(*map));
    if (map == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    assert(map != NULL);
    map->cities = numCities;
    map->nE = 0;

    // Allocate memory for the edges
    map->edges = malloc(numCities * sizeof(int *));
    if (map->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < numCities; i++) {
        map->edges[i] = calloc(numCities, sizeof(int));
        if (map->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    // Allocate memory for the names
    map->name = malloc(numCities * sizeof(char *));
    for (int i = 0; i < numCities; i++) {
        // from game.c MAX_CITY_NAME is 30
        // 31 OR 30 + 1 (1 = '\0')
        map->name[i] = calloc((30 + '\0'), sizeof(char));
    }
    return map;
}
/**
 * Frees all memory associated with the given map
 */
void MapFree(Map m) {
    assert(m != NULL);

    // Free the edges
    for (int i = 0; i < m->cities; i++) {
        free(m->edges[i]);
    }
    free(m->edges);

    // Free the names
    for (int i = 0; i < m->cities; i++) {
        free(m->name[i]);
    }
    free(m->name);

    // Free the map
    free(m);
}

/**
 * Returns the number of cities on the given map
 */
int MapNumCities(Map m) {
    if (m == NULL) {
        fprintf(stderr, "error: MapNumCities is null\n");
        exit(EXIT_FAILURE);
    }
    return m->cities;
}

/**
 * Returns the number of roads on the given map
 */
int MapNumRoads(Map m) {
    if (m == NULL) {
        fprintf(stderr, "error: MapNumRoads is null\n");
        exit(EXIT_FAILURE);
    }
    return m->nE;
}

/**
 * Inserts a road between two cities with the given length
 * Does nothing if there is already a road between the two cities
 * Assumes that the cities are valid and are not the same
 * Assumes that the length of the road is positive
 */
void MapInsertRoad(Map m, int city1, int city2, int length) {
    Road *new = malloc(sizeof(Road));
    if (new == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    new->from = city1;
    new->to = city2;
    new->length = length;

    if (m->edges[city1][city2] == 0) {
        m->edges[city1][city2] = length;
        m->edges[city2][city1] = length;
        m->nE++;
    }
    else {
        return;
    }
    free(new);
}


/**
 * Sets the name of the given city
 */
void MapSetName(Map m, int city, char *name) {
    m->name[city] = realloc(m->name[city], sizeof(char) * strlen(name) + 1);
    strcpy(m->name[city], name);
}

/**
 * Returns the name of the given city
 */
char *MapGetName(Map m, int city) {
    return m->name[city];
}

/**
 * Checks if there is a road between the two given cities
 * Returns the length of the road if there is a road, and 0 otherwise
 */
int MapContainsRoad(Map m, int city1, int city2) {
    if (m == NULL) {
        fprintf(stderr, "error: MapContainsRoad is null\n");
        exit(EXIT_FAILURE);
    }
    if (m->edges[city1][city2] == 0) {
        return 0;
    }
    else {
        int length =  m->edges[city1][city2];
        return length;
    }
}

/**
 * Returns the number of roads connected to the given city and stores
 * them in the given roads array. The `from` field should be equal to
 * `city` for all the roads in the array.
 * Assumes that the roads array is large enough to store all the roads
 */
int MapGetRoadsFrom(Map m, int city, Road roads[]) {
    int num = 0;
    // for each city
    for(int i = 0; i < m->cities; i++) {
        if(MapContainsRoad(m, city, i)) {
            roads[num].from = city;
            roads[num].to = i;
            roads[num].length = MapContainsRoad(m, city, i);
            num++;
        }
    }
    return num;
}

/**
 * Displays the map
 * !!! DO NOT EDIT THIS FUNCTION !!!
 * This function will work once the other functions are working
 */
void MapShow(Map m) {
    printf("Number of cities: %d\n", MapNumCities(m));
    printf("Number of roads: %d\n", MapNumRoads(m));
    Road *roads = malloc(MapNumRoads(m) * sizeof(Road));
    if (roads == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < MapNumCities(m); i++) {
        printf("[%d] %s has roads to:", i, MapGetName(m, i));
        int numRoads = MapGetRoadsFrom(m, i, roads);
        for (int j = 0; j < numRoads; j++) {
            if (j > 0) {
                printf(",");
            }
            printf(" [%d] %s (%d)", roads[j].to, MapGetName(m, roads[j].to),
                   roads[j].length);
        }
        printf("\n");
    }
    free(roads);
}
