/* $RCSfile: cutter.cxx,v $
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

/* Machining tools are a step action_resource, which means they
 * contain a list of things that use them.  Because of this, we can
 * not write the tool data until we are done with the workplan.
 */

void STEPNCWrite::Cutter(
    double diameter, 
    double corner_radius, 
    double corner_radius_horizontal, 
    double corner_radius_vertical, 
    double tip_outer_angle, 
    double taper_angle, 
    double height
    )
{
    // Create a new cutter record for these parameters.
    tooldef * t = new tooldef;
    t-> diameter = diameter;
    t-> corner_radius = corner_radius;
    t-> corner_radius_horizontal = corner_radius_horizontal;
    t-> corner_radius_vertical = corner_radius_vertical;
    t-> tip_outer_angle = tip_outer_angle;
    t-> taper_angle = taper_angle;
    t-> height = height;

    t-> tool_num = 0;
    t-> next_tool = all_tools;
    all_tools = t;

    // do not change current_tool, only load tool does that.
}


void STEPNCWrite::LoadTool (unsigned num)
{
    // Tool change starts a new workingstep
    end_workingstep();
    
    // Loads a particular tool number.  Assigns the number to the last
    // defined cutter if the tool number does not already exist.
    //
    tooldef * t = all_tools;
    while (t) {
	if (t-> tool_num == num) {
	    current_tool = t;
	    return;
	}
	t = t-> next_tool;
    }

    // No such tool, assign one possibly creating it, afterwards the
    // latest tool will be in all_tools
    //
    SELCTLTool (num);
    current_tool = all_tools;
}


/* assigns tool number to last defined cutter */
void STEPNCWrite::SELCTLTool(unsigned num)
{
    // Error!  Zero is not a valid tool number
    if (!num) return;

    // Use the previous definition for existing tool number
    tooldef * t = all_tools;
    while (t) {
	if (t-> tool_num == num) return;
	t = t-> next_tool;
    }

    // If no new tool has been defined, just copy the last one
    t = all_tools;

    if (!all_tools) {
	// no tool defined, just make one 
	Cutter (0,0,0,0,0,0,0);
    } 
    else if (all_tools-> tool_num != 0) {
	// last cutter already has a number, make a copy 
	Cutter (all_tools-> diameter,
		all_tools-> corner_radius,
		all_tools-> corner_radius_horizontal,
		all_tools-> corner_radius_vertical,
		all_tools-> tip_outer_angle,
		all_tools-> taper_angle,
		all_tools-> height
	    );
    }

    // Give the last tool the number
    all_tools-> tool_num = num;
}


void STEPNCWrite::print_all_tools()
{
    // Go through all of the tool definitions and write them to the
    // file.
    //
    unsigned rt = next_id();
    fprintf (file, "\n<!-- CUTTING TOOLS -->\n");
    fprintf (file, "<Action_resource_type id=\"id%u\" "
	     "Name=\"milling cutting tool\"/>\n", 
	     rt
	);

    tooldef * t = all_tools;
    while (t) {
	unsigned i,sz;
	unsigned repitem[7];
	unsigned toolid = next_id();

	fprintf (file, "\n<!-- CUTTING TOOL %u -->\n", t-> tool_num);
	fprintf (file, "<Machining_tool id=\"id%u\" Name=\"", toolid);
	if (t-> tool_num)
	    fprintf (file, "%u", t-> tool_num);
	else
	    fprintf (file, "unused tool");

	fprintf (file, "\" Description=\"user defined milling tool\" "
		 "Kind=\"id%u\">\n", 
		 rt
	    );

	fprintf (file, "  <Usage>\n");
	for (i=0,sz=t->users.size(); i<sz; i++) 
	{
	    // right now we only create freeform ops, so the users
	    // structure does not keep track of the type of operation
	    // we are referring to.  If start making other types of op
	    // this will need to be extended because, unlike P21, XML
	    // needs to know the type of object on both sides of the
	    // reference for selects.
	    print_ref ("Freeform_milling_operation", t->users[i]);
	}
	fprintf (file, "  </Usage>\n");
	fprintf (file, "</Machining_tool>\n");



	// print the dimensional parameters 
	sz = 0;
	repitem[sz++] = make_length_measure(
	    "effective cutting diameter", 
	    t-> diameter
	    );

	// Always write the diameter and height, but omit these if
	// they are zero
	if (STEPNC_FLOAT_NOT_ZERO(t-> corner_radius))
	    repitem[sz++] = make_length_measure(
		"corner radius", 
		t-> corner_radius
		);

	if (STEPNC_FLOAT_NOT_ZERO(t-> corner_radius_horizontal))
	    repitem[sz++] = make_length_measure(
		"corner radius center horizontal", 
		t-> corner_radius_horizontal
		);

	if (STEPNC_FLOAT_NOT_ZERO(t-> corner_radius_vertical))
	    repitem[sz++] = make_length_measure(
		"corner radius center vertical", 
		t-> corner_radius_vertical
		);

	if (STEPNC_FLOAT_NOT_ZERO(t-> taper_angle))
	    repitem[sz++] = make_angle_measure(
		"taper angle", 
		t-> taper_angle
		);

	if (STEPNC_FLOAT_NOT_ZERO(t-> tip_outer_angle))
	    repitem[sz++] = make_angle_measure(
		"tip outer angle", 
		t-> tip_outer_angle
		);

	// The APT height parameter specifies the cutting portion of the
	// tool.  The milling tool base type allows specification of both
	// maximum_depth_of_cut and overall_assembly_length.
	//
	repitem[sz++] = make_length_measure(
	    "maximum depth of cut", 
	    t-> height
	    );


	// Tie all of the parameters together into a tool body
	unsigned ctx = make_simple_context();
	unsigned rep = next_id();
	fprintf (file, "<Machining_tool_body_representation id=\"id%u\" "
		 "Name=\"\" Items=\"",
		 rep
	    );
	for (i=0; i<sz; i++) 
	{
	    if (i) fputc (' ', file);
	    fprintf (file, "id%u", repitem[i]);
	}
	fprintf (file, "\" Context_of_items=\"id%u\"/>\n", ctx);


	// Link the rep back to the machining tool
	unsigned prop = next_id();
	fprintf (file, "<Resource_property id=\"id%u\" "
		 "Name=\"tool body\" Description=\"\">\n",
		 prop
	    );
	print_select_ref ("Resource", "Machining_tool", toolid);
	fprintf (file, "</Resource_property>\n");


	fprintf (file, "<Resource_property_representation id=\"id%u\" "
		 "Name=\"\" Description=\"\" "
		 "Property=\"id%u\" Representation=\"id%u\"/>\n",
		 next_id(), prop, rep
	    );

	t = t-> next_tool;
    }
}
