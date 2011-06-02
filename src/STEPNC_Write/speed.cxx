/* $RCSfile: speed.cxx,v $
 * $Revision: 1.4 $ $Date: 2006/08/21 14:15:51 $
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

void STEPNCWrite::Feedrate(
    double feed
    )
{
    if (!file) return;
    if (STEPNC_FLOAT_EQUAL (current_feed, feed) && !is_rapid)
	return;

    end_move();	    // Feed change forces a new toolpath

    current_feed = feed;
    is_rapid = 0;
}


void STEPNCWrite::Rapid()
{
    if (!file) return;
    if (is_rapid) return;

    end_move();	    // Feed change forces a new toolpath

    is_rapid = 1;
}


void STEPNCWrite::SpindleSpeed(
    double speed
    )
{
    if (!file) return;
    if (STEPNC_FLOAT_EQUAL (current_speed, speed))
	return;

    // As a matter of policy, You can have a spindle change either
    // start a new toolpath or a new workinngstep
    //
    // end_move();	    // Spindle change forces a new toolpath
    end_workingstep();	    // Spindle change forces a new workingstep

    current_speed = speed;
}


// The speeds and feeds are handled as properties on a
// machining_technology object.  Rapid is handled as a speed profile
// modifier on the toolpath.  Other process parameters can go on the
// technology too, but we do not bother with them yet.
// 
unsigned STEPNCWrite::make_technology()
{
    // No change in feeds/speeds?
    if (current_tech &&
	STEPNC_FLOAT_EQUAL (current_feed,  current_tech-> feed) &&
	STEPNC_FLOAT_EQUAL (current_speed, current_tech-> speed))
    {
	return current_tech-> tech_id;
    }
    
    // Look for an existing technology record for this particular
    // feed/speed combination.
    techdef * t = all_techs;
    while (t) {
	if (STEPNC_FLOAT_EQUAL (current_feed,  t-> feed) &&
	    STEPNC_FLOAT_EQUAL (current_speed, t-> speed))
	{
	    current_tech = t;
	    return t-> tech_id;
	}
	t = t-> next_tech;
    }

    // No match, create a new technology for this feed/speed
    t = new techdef;
    t-> feed  = current_feed;
    t-> speed = current_speed;
    t-> tech_id = next_id();
    t-> next_tech = all_techs;
    all_techs = t;
    current_tech = t;

    fprintf (file, "\n<!-- TECHNOLOGY FOR FEED %g, SPINDLE %g -->\n", 
	     current_feed, current_speed
	);

    fprintf (file, "<Machining_technology id=\"id%u\" "
	     "Name=\"\" Description=\"milling\" "
	     "Consequence=\"\" Purpose=\"\"/>\n",
	     t-> tech_id
	);


    /* Create properties for the spindle speed and feed values */
    unsigned val, rep;
    unsigned ctx = make_simple_context();

    val = make_spindle_measure ("rotational speed", current_speed);
    rep  = next_id();

    fprintf (file, "<Machining_spindle_speed_representation id=\"id%u\" "
	     "Name=\"spindle speed\" Items=\"id%u\" "
	     "Context_of_items=\"id%u\"/>\n",
	     rep, val, ctx
	);
    make_action_property (
	"spindle", t-> tech_id, rep, "Machining_technology"
	);


    val = make_feed_measure ("feed speed", current_feed);
    rep = next_id();

    fprintf (file, "<Machining_feed_speed_representation id=\"id%u\" "
	     "Name=\"feed speed\" Items=\"id%u\" "
	     "Context_of_items=\"id%u\"/>\n",
	     rep, val, ctx
	);
    make_action_property (
	"feedrate", t-> tech_id, rep, "Machining_technology"
	);

    return t-> tech_id;
}

