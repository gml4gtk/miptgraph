/**
 * File: utils_utest.cpp - Implementation of testing of Utils library in MiptVis tool.
 * Copyright (C) 2009 MIPTVIS
 */

#include "utest_impl.h"
#include <time.h>

/**
 * Function that compares two list items with integer data types
 */
int CompInt( const void *x, const void *y)
{
    int res;
    if ( (*(int * const*)x) < (*(int * const*)y)) res =  -1;
    if ( (*(int * const*)x) > (*(int * const*)y)) res = 1;
    if ( (*(int * const*)x) == (*(int * const*)y)) res = 0;
    return res;

}

/**
 * Print function.
 * Print the integer list to the standart output
 */

void printListInt( List<int>* list)
{
    ListItem<int>* p = list->head();
    while (p != NULL)
    {
        out("%d", *(p->data()));
        p = p->next();
    }
}

bool uTestList()
{
    // Print welcome message
    out( "List test starts...");

    srand( (unsigned)time( NULL ));

    //Initialize test list
    List<int> testList;
    for (int i = 0; i<10; i++)
    {
        testList.addItem( new int( rand()%100));
    }
    
    // Print test list
    out( "Test list before sorting: ");
    printListInt( &testList);
    if ( !testList.isCorrect())
    {
        out("Pointers test has failed after creation");
        return false;
    }
    
    // Delete and add tests
    out( "Delete first element: ");
    testList.deleteItem( testList.head());
    printListInt( &testList);
    if ( !testList.isCorrect())
    {
        out("Pointers test has failed after deleting");
        return false;
    }

    out( "Add an element to the end: ");
    testList.addItem( new int( rand()%100));
    printListInt( &testList);
    if ( !testList.isCorrect())
    {
        out("Pointers test has failed after adding");
        return false;
    }

    // Sort test
    testList.sort( CompInt);
    
    out( "After sorting: ");
    printListInt( &testList);
    if ( !testList.isCorrect())
    {
        out("Pointers test has failed after sorting");
        return false;
    }
    out("All list tests has passed successfully");
    return true;
}