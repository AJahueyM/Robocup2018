#ifndef __UTILS_H_
#define __UTILS_H_
#include <Arduino.h>

#include "Absis.h"

#include "Coord.h"


using namespace std;

enum Direction {
	Up,
	Down,
	Left,
	Right
};

enum Color {
    White,
    Black,
    Silver
};

enum BumpLevel {
    Flat,
    Small,
    Medium,
    Max
};

float mapD(float x, float in_min, float in_max, float out_min, float out_max);

float shortestAngleTurn(float currentAngle, float target);

Absis<Coord> getClosestFrom(Absis <Coord>& vec, Coord start);

template<class T>
void removeFromVector(Absis<T> &vector, T element){
    Absis<T> newVector;
    for(int j = vector.size() - 1; j >= 0; --j){
        if(vector[j] == element){
            vector.erase(j);
        }
    }
}

template <class T>
int countsOnVector(Absis<T> & vector, T element){
    int counter = 0;
    for(int j = vector.size() - 1; j >= 0; --j){
        T& el = vector[j];
        if(element == el)
            counter++;
    }

    return counter;
}

#endif
