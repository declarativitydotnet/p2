// -*- c-basic-offset: 2;
/*
 * @(#)$Id$
 *
 * A shell of the Click synchronization primitives by Eddie Kohler
 * 
 * Copyright (c) 1999-2000 Massachusetts Institute of Technology
 * Copyright (c) 2004 Regents of the University of California
 * Copyright (c) 2004 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software")
 * to deal in the Software without restriction, subject to the conditions
 * listed in the Click LICENSE file. These conditions include: you must
 * preserve this copyright notice, and you cannot mention the copyright
 * holders in advertising related to the Software without their permission.
 * The Software is provided WITHOUT ANY WARRANTY, EXPRESS OR IMPLIED. This
 * notice is a summary of the Click LICENSE file; the license in that file is
 * legally binding.
 * 
 * This file is distributed under the terms in the attached INTEL-LICENSE file.
 * If you do not find these files, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * 
 * DESCRIPTION: Synchronization primitives.  Mostly no-ops for the
 * user-level runtime.
 */
#ifndef __SYNC_H__
#define __SYNC_H__

/** An empty Spinlock implementation */
class Spinlock { public:

  Spinlock()					{ }

  REMOVABLE_INLINE void acquire()		{ }
  REMOVABLE_INLINE void release()		{ }
  REMOVABLE_INLINE bool attempt()		{ return true; }
  REMOVABLE_INLINE bool nested() const		{ return false; }
  
};

#endif
