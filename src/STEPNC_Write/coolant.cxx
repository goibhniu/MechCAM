/* $RCSfile: coolant.cxx,v $
 * $Revision: 1.3 $ $Date: 2006/08/21 14:15:51 $
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

void STEPNCWrite::CoolantOn() 
{
    if (!file) return;
    if (!current_coolant) return;

    end_move();	    // Coolant change forces a new toolpath

    current_coolant = 1; 
}

void STEPNCWrite::CoolantOff()	
{ 
    if (!file) return;
    if (!current_coolant && !current_mist) return;

    end_move();	    // Coolant change forces a new toolpath

    current_coolant = 0; 
    current_mist = 0; 
}

void STEPNCWrite::CoolantMist()	
{ 
    if (!file) return;
    if (!current_mist) return;

    end_move();	    // Coolant change forces a new toolpath

    current_mist = 1; 
}


// Coolant state is handled as properties on a machining_functions
// object.  Other machine state properties that can go on this too,
// but we do not bother with them yet.
// 
unsigned STEPNCWrite::make_mfunc()
{
    // No change in feeds/speeds?
    if (current_mfunc &&
	current_coolant == current_mfunc-> is_coolant &&
	current_mist    == current_mfunc-> is_mist)
    {
	return current_mfunc-> mfunc_id;
    }
    
    // Look for an existing mfunc record for this particular
    // feed/speed combination.
    mfuncdef * t = all_mfuncs;
    while (t) {
	if ((current_coolant == t-> is_coolant) &&
	    (current_mist    == t-> is_mist))
	{
	    current_mfunc = t;
	    return t-> mfunc_id;
	}
	t = t-> next_mfunc;
    }

    // No match, create a new mfunc for this feed/speed
    t = new mfuncdef;
    t-> is_coolant = current_coolant;
    t-> is_mist    = current_mist;
    t-> mfunc_id   = next_id();
    t-> next_mfunc = all_mfuncs;
    all_mfuncs = t;
    current_mfunc = t;

    fprintf (file, "\n<!-- MACHINE FUNCTIONS FOR COOLANT %s, MIST %s -->\n", 
	     (current_coolant? "ON":"OFF"), 
	     (current_mist? "ON":"OFF")
	);
	     
    fprintf (file, "<Machining_functions id=\"id%u\" "
	     "Name=\"\" Description=\"milling\" "
	     "Consequence=\"\" Purpose=\"\"/>\n",
	     t-> mfunc_id
	);

    // We set a number of boolean properties, but we share the
    // representation and descriptive rep items for each value.
    // 
    STEPNCSharedID val;

    // Coolant 
    val = current_coolant? id_coolant_on: id_coolant_off;
    if (!shared[val]) {
	shared[val] = make_string_rep (
	    current_coolant? "coolant on": "coolant off"
	    );
    }
    make_action_property (
	"coolant", t-> mfunc_id, shared[val], "Machining_functions"
	);

    // Mist 
    val = current_mist? id_mist_on: id_mist_off;
    if (!shared[val]) {
	shared[val] = make_string_rep (
	    current_mist? "mist on": "mist off"
	    );
    }
    make_action_property (
	"mist", t-> mfunc_id, shared[val], "Machining_functions"
	);

    // Through Spindle -- off for now
    val = id_through_spindle_off;
    if (!shared[val]) {
	shared[val] = make_string_rep (
	    // "through spindle coolant on"
	    "through spindle coolant off"
	    );
    }
    make_action_property (
	"through spindle coolant", t-> mfunc_id, shared[val], 
	"Machining_functions"
	);

    // Chip Removal -- off for now
    val = id_chip_removal_off;
    if (!shared[val]) {
	shared[val] = make_string_rep (
	    // "chip removal on"
	    "chip removal off"
	    );
    }
    make_action_property (
	"chip removal", t-> mfunc_id, shared[val], "Machining_functions"
	);

    return t-> mfunc_id;
}

