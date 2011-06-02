/* $RCSfile: idlist.cxx,v $
 * $Revision: 1.1 $ $Date: 2006/08/02 21:54:13 $
 * Auth: David Loffredo (loffredo@steptools.com)
 * 
 * Copyright (c) 1991-2006 by STEP Tools Inc. 
 * All Rights Reserved.
 * 
 * This file may be distributed and/or modified under the terms of 
 * the GNU General Public License version 2 as published by the Free
 * Software Foundation and appearing in the file LICENSE.GPL included
 * with this file.
 * 
 * THIS FILE IS PROVIDED "AS IS" WITH NO WARRANTY OF ANY KIND,
 * INCLUDING THE WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE.
 * 
 * 		----------------------------------------
 */

#include "STEPNCWrite.h"

/* Since we allocate data, we need to define proper copy and
 * assignment operations.  These will duplicate the data buffers, so
 * use with care!
 */

STEPNCIDList::STEPNCIDList (const STEPNCIDList &other)
{
    unsigned newcap = other.f_capacity;
    register unsigned* newbuf = (newcap? (new unsigned [newcap]): 0);
    register unsigned* oldbuf = other.f_data;

    f_capacity = newcap;
    f_data     = newbuf;
    f_size     = other.f_size;

    register unsigned i = f_size;
    while (i--) *newbuf++ = *oldbuf++;
}

STEPNCIDList& STEPNCIDList::operator= (const STEPNCIDList &other)
{
    /* expand capacity if needed.  Use size instead? */
    if (f_capacity < other.f_capacity) {
	f_capacity = other.f_capacity;
	if (f_data) delete [] f_data;
	f_data = new unsigned [f_capacity];
    }

    /* copy the contents */
    register unsigned* newbuf = f_data;
    register unsigned* oldbuf = other.f_data;

    f_size = other.f_size;

    register unsigned i = f_size;
    while (i--) *newbuf++ = *oldbuf++;
    return *this;
}


STEPNCIDList::~STEPNCIDList() 
{
    if (f_data) delete [] f_data;
}


void STEPNCIDList::append (unsigned data) 
{
    while (f_size >= f_capacity) expand();
    f_data [f_size++] = data;
}



void STEPNCIDList::expand (unsigned min_extra)
{
    // make sure it is at least 10 and double previous size
    unsigned newcap = f_capacity + min_extra;
    if (newcap < 10) 		newcap = 10;
    if (newcap < 2*f_capacity)	newcap = 2*f_capacity;

    capacity (newcap);
}



void STEPNCIDList::capacity (unsigned newcap)
{
    /* empty on zero capacity */
    if (!newcap) {
	if (f_data) delete [] f_data;
	f_capacity = 0;
	f_size = 0;
	f_data = 0;
	return;
    }

    /* ignore other lesser capacities. */
    if (newcap <= f_capacity) return;

    /* allocate new buffer and copy */
    register unsigned* newbuf = new unsigned [newcap];
    register unsigned* oldbuf = f_data;
    unsigned* freeme = oldbuf;

    f_data     = newbuf;
    f_capacity = newcap;

    register unsigned i = f_size;
    while (i--) *newbuf++ = *oldbuf++;
    if (freeme) delete [] freeme;
}




unsigned STEPNCIDList::pop()
{
    return (f_size? f_data [--f_size]: 0);
}


unsigned STEPNCIDList::top()
{
    return (f_size? f_data [f_size-1]: 0);
}

