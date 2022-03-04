#include <string>
#include <iostream>
#include "SetType.h"

using namespace std;

template<class T>
SetType<T>::SetType() {
    // Create an array of forward_lists and initially set to an empty forward_list
    buckets = new forward_list<T>[DEFAULT_BUCKETS]; //initializing the buckets array to default size
    numBuckets = DEFAULT_BUCKETS;   //setting all variables to 0 such as the iterators and position of bucket
    currBucket = 0;
    numElems = 0;
    bucketIter = buckets[0].begin();    //setting bucket iterator to the very first bucket
    iterCount = 0;
    maxLoad = 1;        // max load initialized to 1
}

template<class T>
SetType<T>::SetType(SetType &otherSet) {
    // This function is complete. No modification required.
    // This assumes the operator= is implemented.
    *this = otherSet;
}

template<class T>
SetType<T>::~SetType() {
    MakeEmpty();    //making all lists empty before deleteing array
    delete [] buckets;  //deleting the array of buckets
}


template<class T>
SetType<T> SetType<T>::operator+(T elem) {      //operator + to add elements to the existing list
    SetType<T> result;          //set result to the existing list

    result = *this;             //adding the passed element to the list
    result.Add(elem);

    return result;
}

template<class T>
SetType<T> SetType<T>::operator-(T elem) {      //operator - to remove elements to the existing list
    SetType<T> result;          //set results to the existing list

    result = *this;         //removing the passed element from the list
    result.Remove(elem);

    return result;          //return the list
}

template<class T>
SetType<T> SetType<T>::operator+(SetType & otherSet) {
    SetType<T> result;          //setting the current list to result
    result = *this;                 //adding the other list passed through parameter to the result with a double for loop
    for(int i=0; i<otherSet.numBuckets;i++){
        for (auto bIter = otherSet.buckets[i].begin();bIter != otherSet.buckets[i].end();++bIter){
            result.Add(*bIter);         //using the add function to add the current elemnt of the list to result
        }
    }

    return result;      //result ends up with a unify of both lists
}

template<class T>
SetType<T> SetType<T>::operator*(SetType & otherSet) {
    SetType<T> result;          //setting the current list to result

    result = *this;         //using for loop to iterate through the passed list and eliminating the elements that the
    for(int i=0; i<otherSet.numBuckets;i++) {       //current list and other set doesnt
        for (auto bIter = otherSet.buckets[i].begin();bIter != otherSet.buckets[i].end(); ++bIter) {
            if (result.Contains(*bIter) && !otherSet.Contains(*bIter))  //using the remove function to get the intersection of both lists
                result.Remove(*bIter);
        }
    }

    return result;      //return a list of just elements that exist in currlist and not the other set. resulting in an intersection
}

template<class T>
SetType<T> SetType<T>::operator-(SetType& otherSet) {
    SetType<T> result;

    result = *this;         //setting the current list to result
    for(int i=0; i<otherSet.numBuckets;i++) {       //using a for loop to iterate through the passed list
        for (auto bIter = otherSet.buckets[i].begin();bIter != otherSet.buckets[i].end(); ++bIter) {
            if (result.Contains(*bIter) && otherSet.Contains(*bIter))       //keeping only elements that are unique
                result.Remove(*bIter);      //to curr list resulting in a list that contains elemtents that
        }                                   //other set doesnt
    }

    return result;          //returning the list created throught the doubled for loop
}

template<class T>
T SetType<T>::GetNextItem() {
    // Returns the current item and then move to the next item
    T item;
    if (iterCount == numElems)          //if the iterator goes over the total number of elements. through a error
        throw IteratorOutOfBounds();
    while (bucketIter == buckets[currBucket].end()) {       //if bucketiter is at the end of a list. move it the next list
        currBucket++;           //with values
        bucketIter = buckets[currBucket].begin();       //set it to the beginning of that particular list
    }
    item = *bucketIter;         //setting item to the value that will be returned
    bucketIter++;       //increment the iterator
    iterCount++;

    return item;
}

template<class T>
int SetType<T>::GetHashIndex(const T& key) {
    // This is done... No touching!!!
    unordered_map<T,T> mapper;
    typename unordered_map<T,T>::hasher hashFunction = mapper.hash_function();
    return static_cast<int>(hashFunction(key) % numBuckets);
}


template<class T>
void SetType<T>::SetMaxLoad(double max) {
    // This function is done
    if (maxLoad < 0.1)
        maxLoad = 0.1;

    maxLoad = max;
}

template<class T>
SetType<T>& SetType<T>::operator=(SetType const &other) {
    MakeEmpty();
    if(numBuckets != other.numBuckets) {    //if the lists size dont match. delete the curr list and rrebuild it
        delete[] buckets;           //with the passed list
        buckets = new forward_list<T>[other.numBuckets];
    }
    numBuckets = other.numBuckets;  //copy the variables from other list
    maxLoad = other.maxLoad;
    currBucket = other.currBucket;
    iterCount = other.iterCount;
    bucketIter = other.bucketIter;


    for(int i=0; i<numBuckets;i++){     //using a double for loop copy the values from other list to this
        for (auto bIter = other.buckets[i].begin();bIter != other.buckets[i].end();++bIter){
            this->Add(*bIter);
        }
    }
    return *this;       //return the newly added list
}


template<class T>
void SetType<T>::Rehash(int newNumBuckets) {
    SetType<T> rehashedSet(newNumBuckets);      //rehashed function with double the size of previous list

    rehashedSet.numBuckets = newNumBuckets;
    for(int i=0; i<numBuckets;i++){
        for (auto bIter = buckets[i].begin();bIter != buckets[i].end();++bIter){    //using add funtion add all the valies from previous to current list
            rehashedSet.Add(*bIter);
        }
    }
     *this = rehashedSet;   // this is equal to newly madelist
}

template<class T>
SetType<T>::SetType(int numBucks) {
    buckets = new forward_list<T>[numBucks];        //contstuctor to initialize all the variables
    numBuckets = numBucks;
    currBucket = 0;
    numElems = 0;
    bucketIter = buckets[0].begin();
    iterCount = 0;
    maxLoad = 1;

}

template<class T>
void SetType<T>::Add(T elem) {
    if(Contains(elem)){
        return;     //if the element already exist in list return
    }else{
        int bucket = GetHashIndex(elem);    //else add the element in the given index
        buckets[bucket].push_front(elem);
        numElems++;

        double LF = LoadFactor();       //if load factor is bigger than maxload. rehash with doubled size
        if(maxLoad<LF)
            Rehash(numBuckets*2);
    }
}

template<class T>
void SetType<T>::Remove(T elem) {
    if(!Contains(elem)){        //remove the certain elemnt from the list
        return;
    }else{
        int bucket = GetHashIndex(elem);
        buckets[bucket].remove(elem);
        numElems--;
    }
}

template<class T>
bool SetType<T>::Contains(T elem) {
    int bucket = GetHashIndex(elem);        // iterator throught the certain index to see if the element si already in list
    for(auto bIter = buckets[bucket].begin();bIter != buckets[bucket].end(); ++bIter){
        if(elem == *bIter){
            return true;
        }
    }
    return false;
}

template<class T>
void SetType<T>::MakeEmpty() {
    for(int i=0; i<numBuckets; i++){        //clear all the lists in alll indexes
        buckets[i].clear();
    }
    numElems = 0;
    currBucket = 0;
    iterCount = 0;
}

template<class T>
void SetType<T>::ResetIterator() {      //set all iterator veriables to the beginning of the bucket list
    bucketIter = buckets[0].begin();
    iterCount = 0;
    currBucket = 0;
}
