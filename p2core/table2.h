/*
 * @(#)$Id$
 *
 * Copyright (c) 2005 Intel Corporation. All rights reserved.
 *
 * This file is distributed under the terms in the attached INTEL-LICENSE file.
 * If you do not find these files, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * 
 * DESCRIPTION: Second-generation in-memory table implementation.
 *
 * It has a single primary key (single or multiple-field index).
 *
 * It may have any number of secondary indices.
 *
 * It has a maximum lifetime of tuples and a maximum size.
 *
 * Insertions of existing tuples are no-ops.
 *
 * Deletions are always with regards to the primary key.
 *
 * Insertions and deletions do not require a copy of fields into/out of
 * tuples but can be performed leaving appropriate fields in place.
 *
 */

#ifndef __TABLE2_H__
#define __TABLE2_H__

#include "value.h"
#include "tuple.h"
#include <set>
#include "boost/date_time/posix_time/posix_time.hpp"
#include <deque>


class Table2 {
public:
  ////////////////////////////////////////////////////////////
  // Tuple wrapper
  ////////////////////////////////////////////////////////////

  /** A tuple wrapper contains a tuple and its time of insertion */
  struct Entry {
    TuplePtr tuple;
    
    boost::posix_time::ptime time;
    
    Entry(TuplePtr tp);
  };
  



  ////////////////////////////////////////////////////////////
  // special vectors
  ////////////////////////////////////////////////////////////

  /** A key is a vector of unsigned field numbers */
  typedef std::vector< unsigned > Key;


  /** Some default keys */
  static Key KEYID;
  static Key KEY0;
  static Key KEY1;
  static Key KEY2;
  static Key KEY3;
  static Key KEY01;
  static Key KEY12;
  static Key KEY23;
  static Key KEY13;
  static Key KEY123;




  ////////////////////////////////////////////////////////////
  // Comparators
  ////////////////////////////////////////////////////////////

  /** A comparator object for key specs */
  struct KeyComparator
  {
    bool
    operator()(const Key* first,
               const Key* second) const;
  };


  /** A comparator of value ptr vectors by a given key */
  struct KeyedEntryComparator
  {
    /** What's my key spec? */
    const Key& _key;
    
    /** Construct me */
    KeyedEntryComparator(const Key& key);
    
    
    /** My comparison operator */
    bool
    operator()(const Entry *,
               const Entry *) const;
  };




  ////////////////////////////////////////////////////////////
  // Indices
  ////////////////////////////////////////////////////////////
  
  /** A primary index is a set of Entries sorted by those tuple values
      indicated by a key designation. The key designation will appear
      within the particular KeyedComparator object passed during
      construction */
  typedef std::set< Entry *, KeyedEntryComparator > PrimaryIndex;


  /** A secondary index is a multiset of Entries sorted by those tuple
      values indicated by a key designation. The key designation will
      appear within the particular KeyedComparator object passed during
      construction */
  typedef std::multiset< Entry *, KeyedEntryComparator > SecondaryIndex;


  /** An index of indices secondary indices is a map from key
      designations to secondary indices. */
  typedef std::map< Key*, SecondaryIndex*, KeyComparator > SecondaryIndexIndex;




  ////////////////////////////////////////////////////////////
  // Secondary indices
  ////////////////////////////////////////////////////////////

  /** Create a secondary index on the given key spec. The key spec must
      not be empty. If such an index exists already nothing is done and
      false is returned. Otherwise, true is returned. */
  bool
  secondaryIndex(Key& key);




  


  ////////////////////////////////////////////////////////////
  // Constructors
  ////////////////////////////////////////////////////////////

  /**  Create a new table.  name is an identifying string.  key is a
       vector containing a sequence of field numbers, which make up the
       primary key of the table; an empty key vector means the implicit
       tuple ID is the primary key.  maxSize is how many tuples it will
       hold before discarding (FIFO) and must be non-negative.  Max size
       of 0 means unlimited table size.  lifetime is how long to keep
       tuples for before discarding and must be a positive time
       duration; a lifetime may be positive infinite, indicating no
       expiration. */
  Table2(string tableName,
         Key& key,
         size_t maxSize,
         boost::posix_time::time_duration& lifetime);

  /** A convenience constructor that allows the use of string
      representations for maximum tuple lifetime. */
  Table2(string tableName,
         Key& key,
         size_t maxSize,
         string lifetime);
  

  /** A convenience constructor that does not expire tuples. */
  Table2(string tableName,
         Key& key,
         size_t maxSize);
  

  /** A convenience constructor with no size or time limits. */
  Table2(string tableName,
         Key& key);
  

  /** A destructor. It empties out the table and then destroys it. */
  ~Table2();




  ////////////////////////////////////////////////////////////
  // Metadata checks
  ////////////////////////////////////////////////////////////

  /** Table size. It returns the number of tuples within the table
      (excluding those that may have been logically deleted but not yet
      physically removed). */
  size_t
  size();
  
  
  
  
  ////////////////////////////////////////////////////////////
  // Lookup Iterator
  ////////////////////////////////////////////////////////////

  /** An opaque container for the iterator logic to be used with
      indices.  It hides details about the underlying STL index, i.e.,
      whether it's a set or a multiset.  As far as the interface is
      concerned, an index is an index. */
  class Iterator {
  public:
    /** Fetch the next tuple pointer, or null if no next tuple exists */
    TuplePtr next();
    
    
    /** Is the iterator done? */
    bool done();
    
    
    /** The constructor just initializes the iterator */
    Iterator(std::deque< TuplePtr >* spool);


    /** The destructor kills the spool queue */
    ~Iterator();
    
    
    

  private:
    /** The iterator's data. This is spooled, instead of read on-line
        from the table, to deal with the following STL interface
        concurrency shortcoming: if an iterator still has elements to
        traverse while the underlying container is concurrently modified
        (e.g., shrunk), there's no way to tell that the iterator is
        viewing an inconsistent view of the container (e.g., pointing at
        a removed element) but instead segfaults. */
    std::deque< TuplePtr >* _spool;
  };


  /** A pointer to lookup iterators */
  typedef boost::shared_ptr< Iterator > IteratorPtr;
  
  
  
  
  ////////////////////////////////////////////////////////////
  // Lookups
  ////////////////////////////////////////////////////////////

  /** Returns a pointer to a lookup iterator on all elements matching
      the given tuple on the index specified by the key spec. If no such
      index exists, a null pointer is returned. */
  IteratorPtr
  lookup(Key& key, TuplePtr t);
  




  ////////////////////////////////////////////////////////////
  // Updates
  ////////////////////////////////////////////////////////////

  /** Insert a tuple into the table. Return true if the insertion
      modified the table, by growing by a tuple or replacing an existing
      tuple. Return false if the insertion did not affect the table at
      all, e.g., it corresponded to a tuple equal (as per tuple
      comparison) to an existing tuple. Insertion time, for the purposes
      of tuple expiration, indicates the first time a particular tuple
      was inserted; any attempts to reinsert that tuple do not update
      the original insertion time.

      The semantics is as follows: find the tuple by the same primary
      key. If it's equal (as per compareTo) do nothing. If it's
      different, replace it. If it doesn't exist, insert the new one
      in. */
  bool
  insert(TuplePtr t);

  
  /** Remove a tuple from the table. Return true if the removal modified
      the table by shrinking it by one tuple. Return false if the
      removal did not affect the table, e.g., because the given tuple
      was not there to be removed.  Removal happens with regards to the
      primary key of the table.  Therefore, the tuple given the remove
      method and the tuple actually removed from the table may not be
      the same or equal in terms of Tuple::compareTo. */
  bool
  remove(TuplePtr t);




private:
  /** My name (human readable). */
  std::string _name;


  /** My primary key. If empty, use the tuple ID. */
  Key& _key;


  /** My maximum size in tuples. If 0, size is unlimited. */
  size_t _maxSize;

  
  /** My maximum lifetime. It must be positive, and may be positive
      infinity, meaning table entries do not expire. */
  boost::posix_time::time_duration _maxLifetime;


  /** My secondary indices, sorted by key spec. */
  SecondaryIndexIndex _indices;


  /** A queue holding all secondary index comparator objects for
      elimination during destruction */
  std::deque< KeyedEntryComparator* > _keyedComparators;

  
  /** My primary index */
  PrimaryIndex _primaryIndex;


  /** The time-order queue of entries, for fast garbage collection. This
      is only initialized if the table has a finite expiration time */
  std::deque< Entry * > _queue;


  /** Are we garbage collecting?  We are only garbage collecting if
      entries can be auto-flushed, either due to expiration or due to
      table size limitations. */
  bool _flushing;




  ////////////////////////////////////////////////////////////
  // Static Initializer
  ////////////////////////////////////////////////////////////

  /** A static initializer object to initialize static class objects */
  class Initializer {
  public:
    Initializer();
  };
  static Initializer
  _INITIALIZER;
  



  ////////////////////////////////////////////////////////////
  // Queue management
  ////////////////////////////////////////////////////////////

  /** Flush expired or supernumerary entries */
  void
  flush();




  ////////////////////////////////////////////////////////////
  // Convenience Functions
  ////////////////////////////////////////////////////////////

  /** The common search entry to be used with lookups */
  static Entry
  _searchEntry;

  /** Check if the given tuple appears in the table currently. */
  bool
  tupleInTable(TuplePtr t);


  /** Remove an existing tuple from the database including all
      indices. This tuple always causes a tuple to be removed from the
      table and therefore always calls any deletion listeners. */
  void
  removeTuple(TuplePtr t);


  /** Insert a brand new tuple into the database including all
      indices. This method *always* causes a new tuple to appear within
      the table and, therefore, always calls any insertion listeners. */
  void
  insertTuple(TuplePtr t);
};


/** A pointer to tables */
typedef boost::shared_ptr< Table2 > Table2Ptr;

#endif
