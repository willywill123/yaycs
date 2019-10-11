/*
 Student Name: William Zhu
 Date: 3/18/2019

=======================
ECE 2035 Project 2-1:
=======================
This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */
struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;

  /** The hash function pointer */
  HashFunction hash;

  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;

  /** The value associated with this hash table entry */
  void* value;

  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/

static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {
    //make space for the entry
    HashTableEntry* hashTableEntry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
    //initialize key, value, and next pointer as NULL
    hashTableEntry -> key = key;
    hashTableEntry -> value = value;
    hashTableEntry -> next = NULL;
    
  //return the pointer to this hash table entry
    return hashTableEntry;
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {
    //hash table lookup, returning the located link if it exists
    //otherwise, it returns NULL if it does not find the link.
    //make an index variable
    int index = hashTable -> hash(key);
  //pointer to the head of the linkedlist in hash table
    HashTableEntry* thisLink = hashTable -> buckets[index];
  //while it is not null nor the current link, search;
    while (thisLink != NULL && thisLink -> key != key) {
        //move on to the next entry
        thisLink = thisLink -> next;
    }
    return thisLink;
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }

  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));

  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));

  // As the new buckets contain indeterminant values, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }

  // Return the new HashTable struct.
  return newTable;
}
/**
* destroyHashTable
*
* destroys the HashTable (loops from head to tail)
*
* @param hashTable The pointer to the hash table.
*/
void destroyHashTable(HashTable* hashTable) {
    //initialize pointers currentLink an nextLink
    //and loop through all the buckets
  for (int i = 0; i < (hashTable -> num_buckets); i++) {
    //Get the head of the bucket
    HashTableEntry* thisLink = hashTable -> buckets[i];
    //EDGE CASE: check to see if the first link is null
    //loop through each bucket and free them if not null
    if (thisLink != NULL) {
        //hold the next node
      HashTableEntry* nextLink = thisLink -> next;
      //while the next nodes are not NULL
      while (nextLink) {
        //free the value and pointer
        free(thisLink -> value);
        free(thisLink);
        //move on to the next pointer and the value of it
        thisLink = nextLink;
        nextLink = nextLink -> next;
      } 
      free(thisLink -> value); //CAUSING MEMORY LEAKS!!free the value even if it was not found
      free(thisLink); //free the pointer as well
    }
  }
  //Free the buckets array from the heap
  free(hashTable -> buckets);
  //Free the whole hash table now
  free(hashTable);
  return;
}


void* insertItem(HashTable* hashTable, unsigned int key, void* value) {  
  //use helper function findItem to find and get the node with the key
  HashTableEntry* currentLink = findItem(hashTable, key);
  //if not NULL, then the node with key exists
  if (currentLink != NULL) {
      //make a temp variable to store 
      void* temp = currentLink -> value;
      //replace the value and return the temp value
      currentLink -> value = value;
      //return temp
      return temp;
}else {
    unsigned int index = hashTable -> hash(key); //WORK ON THIS METHOD
    //if it did not already exist, allocating new space for it
    HashTableEntry* newLink = createHashTableEntry(key, value);
    //add this newLink to the head
    HashTableEntry* head = hashTable -> buckets[index];
    //set the next pointer of new entry to the head
    newLink -> next = head;
    //setting head to the new entry
    hashTable -> buckets[index] = newLink;
    return NULL;
    }
}

void* getItem(HashTable* hashTable, unsigned int key) {
//first check if the link exists by calling private function findItem()
  HashTableEntry* nodeExist = findItem(hashTable, key);
  //if it does exist, return the value- else return null
  if(nodeExist != NULL) {
    return nodeExist -> value;
  } else {
  return NULL;
}
}

void* removeItem(HashTable* hashTable, unsigned int key) {
//make a new index variable
    int index = hashTable -> hash(key);
    HashTableEntry* currentLink = hashTable -> buckets[index];  
    // lets remove the first 
    //edge case!
    if(findItem(hashTable, key) == NULL) {
        return NULL;
    }
    if(currentLink != NULL && currentLink -> key == key) {
        //make a temp variable to store the value that will be removed
        void* temp = currentLink -> value;
        //edge case to see if next is NULL given that the key is found
        if (currentLink -> next == NULL) {
            //set buckets to NULL
            hashTable -> buckets[index] = NULL;
        }else{
            //move on to the next link
            hashTable -> buckets[index] = currentLink -> next;
        }
        //now we can free the pointer
        free(currentLink);
        return temp;
    } 


    //removing each of the link instead of the first one 
    //while the first one is not null and the next is not null
    while(currentLink != NULL && currentLink -> next) {
        //if the key matches the value, then store into a temporary variable
        //for removal
        if (currentLink -> next -> key == key) {
            HashTableEntry* temp = currentLink -> next;
            void *tempValue = temp -> value;
            //point to the delete-able entry // use temp -> next
            currentLink -> next = temp -> next;
            //now we can free the pointer
            free(temp);
            return tempValue;
            }
            //move onto the next entries
            currentLink = currentLink -> next;
        }
        return NULL;

}
            

void deleteItem(HashTable* hashTable, unsigned int key) {
    //make a new index variable 
    int index = hashTable -> hash(key);
    //get the correct pointer to the bucket
    HashTableEntry* currentLink = hashTable -> buckets[index];
    //edge case!!: check if null
    if(findItem(hashTable, key) == NULL) {
        return;
    }
    //lets remove the first link again
    if (currentLink != NULL && currentLink -> key == key) {
        
        //edge case to see if next is NULL given that the key exists
        if (currentLink -> next == NULL) {
            //SET THE BUCKETS TO NULL
            hashTable -> buckets[index]= NULL;
            }else{
                //else, move on to the next link
                hashTable -> buckets[index] = currentLink -> next;
            }
        //now we can free the value and the pointer
        free(currentLink -> value);
        free(currentLink);
        return;
        }
    //lets remove the internal links now
    while(currentLink && currentLink -> next) {
    //check if next one is delete-able
        if(currentLink -> next -> key == key) {
            //make a temp variable to save the value that will be deleted
            HashTableEntry* temp = currentLink -> next;
            //point to it
            currentLink -> next = temp -> next;
            //now we can free the value and the pointer;
            free(temp -> value);
            free(temp);
            return;     
        }
    //keep going through the internal links until we reach the end
        currentLink = currentLink -> next;
    }
}
