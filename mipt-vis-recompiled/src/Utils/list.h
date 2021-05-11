/**
 * File: list.h - list implementation.
 * Copyright (C) 2009  Boris Shurygin
 */
#ifndef LIST_H
#define LIST_H

/**
 * Types of direction in lists.
 * Lists are built from left to right by default.
 * That means if you take next in default direction - it will be element to the right.
 */
enum ListDir
{
    LIST_DIR_RIGHT = 0,
    /** Defult direction */
    LIST_DIR_DEFAULT = LIST_DIR_RIGHT,
    LIST_DIR_LEFT = 1,
    LIST_DIR_RDEFAULT = LIST_DIR_LEFT,
    /** Number of directions */
    LIST_DIR_NUM = 2
};

/**
 * Return direction that is reverse to given one
 */
inline ListDir
listRDir( ListDir dir)
{
    assert( LIST_DIR_NUM == 2);
    return ( dir == LIST_DIR_DEFAULT)? LIST_DIR_RDEFAULT: LIST_DIR_DEFAULT; 
}


/**
 * Class for objects that should have pointers to next/prev objects of their type( i.e. behave like list elements)
 * Implements headless list data structure
 */
template <class Data> class ListItem
{
    ListItem<Data> * peer[ LIST_DIR_NUM];
    Data *data_p;
public:
    
    /**get data */
    inline Data *data() const
    {
        return data_p;
    }
    /** set data */
    inline void setData( Data* d)
    {
        data_p = d;
    }
    
    /** get neighbour */
    inline ListItem<Data> * peerInDir( ListDir dir) const
    {
        return peer[ dir];
    }
    
    /** set neighbour */
    inline void setPeerInDir( ListItem<Data> *p, ListDir dir)
    {
        peer[ dir] = p;
    }
    /** Default peers gets */
    /** Return next peer in default direction */
    inline ListItem<Data> *next() const
    {
        return peerInDir( LIST_DIR_DEFAULT);
    }
    /** Return prev peer in default direction */
    inline ListItem<Data>* prev() const
    {
        return peerInDir( LIST_DIR_RDEFAULT);
    }
    /** Default peer sets */
    inline void setNext( ListItem<Data> *n)
    {
        setPeerInDir( n, LIST_DIR_DEFAULT);
    }
    inline void setPrev( ListItem<Data> *p)
    {
        setPeerInDir( p, LIST_DIR_RDEFAULT);
    }
    
    /** Attach this item to peer in given direction */
    inline void attachInDir( ListItem<Data>* p, ListDir dir)
    {
        ListDir rdir = listRDir( dir);
        setPeerInDir( p, dir);
        setPeerInDir( NULL, rdir);

        if ( isNotNullP( p))
        {
            ListItem<Data>* rdir_peer = p->peerInDir( rdir);
            if ( isNotNullP( rdir_peer))
            {
                rdir_peer->setPeerInDir( this, dir);
            }
            p->setPeerInDir( this, rdir);
            setPeerInDir( rdir_peer, rdir);
        }
    }
    
    /** Attach in default direction */
    inline void attach( ListItem<Data>* peer)
    {
        attachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Detach from neighbours */
    inline void detach()
    {
        /** Correct links in peers */
        if ( isNotNullP( peer[ LIST_DIR_DEFAULT]))
        {
            peer[ LIST_DIR_DEFAULT]->setPeerInDir( peer[ LIST_DIR_RDEFAULT], LIST_DIR_RDEFAULT);
        }
        if ( isNotNullP( peer[ LIST_DIR_RDEFAULT]))
        {
            peer[ LIST_DIR_RDEFAULT]->setPeerInDir( peer[ LIST_DIR_DEFAULT], LIST_DIR_DEFAULT);
        }
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    }

    /** Default constructor */
    ListItem()
    {
        setData( NULL);
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    };

    /** Constructor from data pointer */
    ListItem( Data* d)
    {
        setData( d);
        setPeerInDir( NULL, LIST_DIR_DEFAULT);
        setPeerInDir( NULL, LIST_DIR_RDEFAULT);
    };

    /** Insert element before the given one */
    ListItem( ListItem<Data> *peer, Data* d)
    {
        setData( d);
        attachInDir( peer, LIST_DIR_DEFAULT);
    }

    /** Insert element in given direction */
    ListItem( ListItem<Data> *peer, ListDir dir, Data *d)
    {
        setData( d);
        attachInDir( peer, dir);
    }

    /** Destructor */
    ~ListItem()
    {
        detach();
    }
};

template <class Data> class List
{
    ListItem<Data>* l_head;
    ListItem<Data>* l_tail;
    int l_size;

private:
    /** Function to move element from one list &p1 to another &p2 */
    inline void moveItem( ListItem<Data> **p1, ListItem<Data> **p2)
    {
        (*p1)->setPrev( *p2);
        (*p2)->setNext( *p1);
        (*p2) = (*p1);
        (*p1) = (*p1)->next();
    }

    /** Internal constructor */
    List( ListItem<Data>* p_head, ListItem<Data>* p_tail)
    {
        l_head = p_head;
        l_tail = p_tail;
        if (l_head == NULL) l_size = 0;
        else
        {
            l_size = 1;
            ListItem<Data>* p = l_head;
            while ( p != l_tail)
            {
                l_size++;
                p = p->next();
            }
        }
    }

    /**
     * Internal list merge function.
     * A part of Sort function.
     */

    void merge ( ListItem<Data>* *first_p, ListItem<Data>* middle,
        ListItem<Data>* *last_p, int (*comp)(const void *, const void *))
    {
        // Define internal variables
        ListItem<Data>* first = *first_p;
        ListItem<Data>* last = *last_p;
        ListItem<Data> *p, *p1, *p2, *last_ext = last->next(), *first_ext = first->prev();
        p1 = first;
        p2 = middle;
        // Find first element
        if ( comp( first->data(), middle->data()) <= 0)
        {
            p = first;
            p1  = p1->next();
        }
        else
        {
            p = middle;
            p2 = p2->next();
        }
        // Change external pointer to the first element
        *first_p = p;
        p->setPrev( first_ext);
        if ( first_ext != NULL) first_ext->setNext( p); 
        // Work with elements from the first list
        while ( p1 != middle)
        {
            // If there are no elements in the second list, attach elements only from the first
            if ( p2 == last_ext) { moveItem( &p1, &p);}
            else
            {
                // If there are such, compare
                if ( comp( p1->data(), p2->data()) <= 0) { moveItem( &p1, &p);}
                else { moveItem( &p2, &p);}
            }
        }
        // Work with elements from the second list
        // If there are such, attach them all
        while ( p2 != last_ext) { moveItem( &p2, &p);}
        // Change external pointer to the last element
        p->setNext( last_ext);
        if ( last_ext != NULL) last_ext->setPrev( p); 
        *last_p = p;
    };

public:
    /** get head */
    inline ListItem<Data>* head() const
    {
        return l_head;
    }

    /** get tail */
    inline ListItem<Data>* tail() const
    {
        return l_tail;
    }

    /** get size of the list */
    inline int size() const
    {
        return l_size;
    }

    /** set head */
    inline void setHead( ListItem<Data>* p)
    {
        l_head = p;
    }

    /** set tail */
    inline void setTail( ListItem<Data>* p)
    {
        l_tail = p;
    }

    /** Add an element to the list in the tail */
    inline void addItem( Data* p)
    {
        ListItem<Data>* list_p = new ListItem<Data>( p);
        list_p->attachInDir( l_tail, LIST_DIR_LEFT);
        if ( l_head == NULL) l_head = list_p;
        l_tail = list_p;
        l_size++;
    }

    /** Delete an element from the list */
    inline void deleteItem( ListItem<Data>* p)
    {
        if (p == l_head) l_head = p->next();
        if (p == l_tail) l_tail = p->prev();
        delete p;
        l_size--;
    }
    
    /** Check correctness of ListItem pointers and size of the list */
    inline bool isCorrect()
    {
        int s=0;
        ListItem<Data>* p;
        p = l_head;
        while ( p != l_tail->next())
        {
            //check previous element
            if ( p->prev() == NULL){ if ( p != l_head) return false;}
            else if ( p->prev()->next() != p) return false;
            //check next element
            if ( p->next() == NULL){ if ( p != l_tail) return false;}
            else if ( p->next()->prev() != p) return false;
            //count elements
            s++;
            p = p->next();
        }
        if ( s != l_size) return false;
        return true;
    }

    /**
     * Sort function
     * Implements merge sorting for the list
     */
    void sort( int (*comp)(const void *, const void *))
    {
        if ( l_size != 1)
        {
            // Initialisation
            ListItem<Data>* first = l_head;
            ListItem<Data>* middle = first;
            ListItem<Data>* last = l_tail;
            // Find the middle of the list
            ListItem<Data>* p;
            p = l_head;
            while ( p != last->next())
            {
                p = p->next();
            }
            for( int i = 1; i < ( l_size / 2 + 1); i++)
            {
                middle = middle->next();
            }
            // Divide list into two parts
            List<Data> list1( first, middle->prev());
            List<Data> list2( middle, last);
            // Sorts them separatly
            list1.sort( comp);
            list2.sort( comp);
            // Evaluate new ranges
            first = list1.head();
            middle = list2.head();
            last = list2.tail();
            middle->setPrev( list1.tail());
            list1.tail()->setNext( middle);
            // Merge two sorted parts
            merge( &first, middle, &last, comp);
            // Evaluate new ranges for list
            l_head = first;
            l_tail = last;
            p = l_head;
            while ( p != l_tail->next())
            {
                p = p->next();
            }
            //Destroy temporary lists
            list1.setHead( NULL);
            list1.setTail( NULL);
            list2.setHead( NULL);
            list2.setTail( NULL);
        }
    }

    /** Default constructor */
    List()
    {
        l_head = NULL;
        l_tail = NULL;
        l_size = 0;
    };

    /** Constructor from data pointer */
    List( Data* d)
    {
        head = new ListItem<Data>( d);
        tail = head;
        size = 1;
    };

    /** Default destructor */
    ~List()
    {
        while( l_head != NULL)
        {
            deleteItem( l_head);
        }
    }
};

#endif
