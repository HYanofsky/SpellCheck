/*
Hadassah Yanofsky
DSA 2 - Fall 2020
Cooper Union 
Some of the code is gotten from https://www.geeksforgeeks.org/implementing-hash-table-open-addressing-linear-probing-cpp/
Hash function gotton from Data Structures And Algorithm Analysis In C++ by Mark Weiss 4th edition
hash.h written by Prof. Carl Sable
*/

#include "hash.h"
#include <iostream>
using namespace std;

// The hash function.
int hashTable::hash(const string& key) {
    //find the ascii sum of all characters in the key
     unsigned int hashVal = 0;
     for (char ch : key)
        hashVal = 37 * hashVal + ch;
    
    return hashVal % capacity; 
}

// The constructor initializes the hash table.
// Uses getPrime to choose a prime number at least as large as
// the specified size for the initial size of the hash table.
hashTable::hashTable(int size) {
    filled = 0;
	capacity = getPrime(size);
    data.reserve(capacity);
    for (unsigned int i = 0; i < capacity; ++i)
        data.push_back(hashItem());
}

// Insert the specified key into the hash table.
// If an optional pointer is provided,
// associate that pointer with the key.
// Returns 0 on success,
// 1 if key already exists in hash table,
// 2 if rehash fails.
//void* pv = nullptr 
int hashTable::insert(const string& key, void* pv) {
    //check if already in the hash table
    if (contains(key)) {
        //already there
        return 1;
    }
    else {

        // Apply hash function to find index for given key 
        int hashIndex = hash(key);
        int counter = 0;
        
        //find next free space  
        while (data[hashIndex].isOccupied && counter<= capacity)
        {
            counter++;
            hashIndex++;
            hashIndex %= capacity;
        }
        
        //rehash the hashTable if there are more nodes then there is capacity for 
        if ((filled*2) >= capacity) {
            rehash();
            return insert(key, pv);//insert into the new bigger hashtable
        }

        //sets all the values in the empty spot
        data[hashIndex].key=key;
        data[hashIndex].pv = pv;
        data[hashIndex].isOccupied = true;
        filled++;
        
        //check if it worked
        if (data[hashIndex].key == key) {
            //successful
            return 0;
        }
    }
    return 2;
}

// Check if the specified key is in the hash table.
// If so, return true; otherwise, return false.
bool hashTable::contains(const string& key) {
    int ans = findPos(key);
    if (ans == -1) {
        return false;
    }
    else {
        return true;
    }
}


// Search for an item with the specified key.
// Return the position if found, -1 otherwise.
int hashTable::findPos(const string& key) {
    // Apply hash function to find index for given key 
    int hashIndex = hash(key);
    int counter = 0;
    //finding the node with given key    
    while (data[hashIndex].isOccupied)
    {
        if (counter++ > capacity) {  //to avoid infinite loop 
            return -1;
        }
        //if node found return its pos
        if (data[hashIndex].key == key) {
            return hashIndex;
        }
        hashIndex++;
        hashIndex %= capacity;
    }
    
    //If not found return null 
    return -1;
}


// Return a prime number at least as large as size.
// Uses a precomputed sequence of selected prime numbers.
unsigned int hashTable::getPrime(int size) {
    int primes[] = {50021, 100043, 200087, 400187, 800399, 1600811};
    int count = 0;
    for (int i = 0; i < 7; i++) {
        if (primes[i] >= size) {
            return primes[i];
        }
    }
    return 0;
}

// The rehash function; makes the hash table bigger.
// Returns true on success, false if memory allocation fails.
bool hashTable::rehash() {
    //get new capacity
    int new_cap = getPrime(2 * capacity);
    hashTable new_hash(new_cap);
    
    //insert all data points from the old hashTable
    for (int i = 0; i < capacity; i++) {
        if (data[i].isOccupied) {
            new_hash.insert(data[i].key, data[i].pv);
        }
    }

    //change the hashTable to the new old
    swap(*this, new_hash);
    return true;
}