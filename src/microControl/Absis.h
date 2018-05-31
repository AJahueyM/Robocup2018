#ifndef __ABSIS_H_
#define __ABSIS_H_
#include <ArduinoSTL.h>
using namespace std;

template<class T>
class Absis {
public:
    Absis(){
        array = new T[currentSize];
    }
    void push_back(T t){
        T *newArray = new T[currentSize + 1];

        for(int i = 0; i < currentSize; ++i){
            newArray[i] = array[i];
        }
        newArray[currentSize] = t;
        delete [] array;
        array = newArray;

        currentSize++;
    }

    void emplace_back(T t){
        push_back(t);
    }

    void addStart(T t){
        T *newArray = new T[currentSize + 1];

        for(int i = 0; i < currentSize; ++i){
            newArray[i + 1] = array[i];
        }
        newArray[0] = t;
        delete [] array;
        array = newArray;

        currentSize++;
       
    }
    unsigned short int size(){
        return currentSize ;
    }
    T& operator[](unsigned short int index){
        if(index >= 0 && index < currentSize)
            return array[index];
    }

private:
    unsigned short int currentSize = 0;
    T *array;
};

#endif