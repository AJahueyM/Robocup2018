#ifndef __ABSIS_H_
#define __ABSIS_H_

template<class T>
class Absis {
public:
    Absis() : array{ new T[currentSize] }{

    }
    Absis(const Absis& a){
        array = new T[a.size()];
        for(int i = 0; i < a.size(); ++i){
            array[i] = a[i];
        }
        currentSize = a.size();
        

    }
    Absis& operator=( const Absis& a){
        if(this != &a){
            delete[] array;
            array = new T[a.size()];
            for(int i = 0; i < a.size(); ++i){
                array[i] = a[i];
            }
            currentSize = a.size();
        }
    }
    void push_back(T t){
        T *newArray = new T[currentSize + 1];

        for(int i = 0; i < currentSize; ++i){
            newArray[i] = array[i];
        }
        newArray[currentSize] = t;
        
        delete [] array;
        this->array = newArray;
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
        this->array = newArray;

        currentSize++;
       
    }

    void erase(unsigned short int index){
        if(index >= 0 && index < currentSize){
            T *newArray = new T[currentSize - 1];

            for(int i = 0; i < currentSize; ++i){
                if(i != index){
                    if(i > index){
                        newArray[i-1] = array[i];
                    }else{
                        newArray[i] = array[i];
                        
                    }
                }
            }
            
            delete [] array;
            this->array = newArray;

            currentSize--;          
        }
    } 

    unsigned short int size() const{
        return currentSize ;
    }

    T& operator[](unsigned short int index) const{
        if(index >= 0 && index < currentSize)
            return array[index];
    }

    ~Absis() {
       delete[] array;
    }
private:
    unsigned short int currentSize = 0;
    T *array;
};

#endif