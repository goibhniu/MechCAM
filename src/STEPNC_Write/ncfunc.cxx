/* $RCSfile: ncfunc.cxx,v $
 * $Revision: 1.2 $ $Date: 2006/08/21 14:15:51 $
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

void STEPNCWrite::PPrint(
	const char * label, 
	const char * message
	)
{
    if (!file) return;

    end_workingstep();

    unsigned id = next_id();
    fprintf (file, "\n<!-- DISPLAY MESSAGE -->\n");

    fprintf (file, "<Machining_nc_function id=\"id%u\" Name=\"", id);
    print_string (label? label: "");
    fprintf (file, "\" Description=\"display message\" "
	     "Consequence=\"\" Purpose=\"\"/>\n");

    add_to_workplan (id);

    unsigned rep = make_string_rep (message);
    make_action_property (
	"message text", id, rep, "Machining_nc_function"
	);
}
    

void STEPNCWrite::GoHome (const char * label)
{
    if (!file) return;

    // this is a type of workingstep, so it ends the previous one
    end_workingstep();

    unsigned tp = next_id();
    fprintf (file, "\n<!-- RETURN HOME -->\n");

    fprintf (file, "<Machining_rapid_movement id=\"id%u\" Name=\"", tp);
    print_string (label? label: "");
    fprintf (file, "\" Description=\"return home\" "
	     "Consequence=\"\" Purpose=\"\"/>\n");

    // Associate the toolpath with the current operation using a
    // sequence relationship object
    //
    add_to_workplan(tp);

    loc.last_point_id = 0;
    axis.last_point_id = 0;
}


void STEPNCWrite::Stop (const char * label)
{
    if (!file) return;

    end_workingstep();

    unsigned id = next_id();
    fprintf (file, "\n<!-- PROGRAM STOP -->\n");

    fprintf (file, "<Machining_nc_function id=\"id%u\" Name=\"", id);
    print_string (label? label: "");
    fprintf (file, "\" Description=\"program stop\" "
	     "Consequence=\"\" Purpose=\"\"/>\n");

    add_to_workplan (id);
}


void STEPNCWrite::Connector()
{
    if (!file) return;

    // Close out any toolpath
    end_move();

    // Make a new workingstep if needed
    if (!current_op) make_workingstep("");

    unsigned tp = next_id();
    fprintf (file, "\n<!-- CONNECT DIRECT -->\n");

    fprintf (file, "<Machining_toolpath id=\"id%u\" "
	     "Name=\"\" Description=\"connect direct\" "
	     "Consequence=\"\" Purpose=\"\"/>\n", 
	     tp
	);

    // Some common properties for the toolpaths
    //
    if (!shared[id_trajectory_connect]) {
	shared[id_trajectory_connect] = make_string_rep ("connect");
    }
    make_action_property (
	"trajectory type", tp, shared[id_trajectory_connect], 
	"Machining_toolpath"
	);
    

    if (!shared[id_priority]) {
	shared[id_priority] = make_string_rep ("required");
    }
    make_action_property (
	"priority", tp, shared[id_priority], "Machining_toolpath"
	);

    
    // Associate the toolpath with the current operation using a
    // sequence relationship object
    //
    add_to_workingstep(tp);

    loc.last_point_id = 0;
    axis.last_point_id = 0;
}


