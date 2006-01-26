// -*- c-basic-offset: 2; related-file-name: "element.C" -*-
/*
 * @(#)$Id$
 *
 * This file is distributed under the terms in the attached LICENSE file.
 * If you do not find this file, copies can be found by writing to:
 * Intel Research Berkeley, 2150 Shattuck Avenue, Suite 1300,
 * Berkeley, CA, 94704.  Attention:  Intel License Inquiry.
 * 
 * DESCRIPTION: Are inlines used?  Disabled for debugging
 */

#ifndef __INLINES_H__
#define __INLINES_H__

#define DEBUG_INLINE
#ifdef DEBUG_INLINE
#define REMOVABLE_INLINE
#else
#define REMOVABLE_INLINE inline
#endif

#endif
