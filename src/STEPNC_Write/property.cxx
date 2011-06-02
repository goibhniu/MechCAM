/* $RCSfile: property.cxx,v $
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

//------------------------------------------------------------
// Construct various representation contexts
//  
// The value of STEP properties are given by representation objects.
// These have an associated context to give any additional information
// needed to interpret them.  When a representation contains geometry,
// the context specifies the units that apply.  A simple placeholder
// context is used for things, like measure values and descriptive
// strings that are already fully specified.
//
unsigned STEPNCWrite::make_simple_context()
{
    if (!file) return 0;
    if (shared[id_context_simple]) return shared[id_context_simple];

    unsigned id = next_id();
    shared[id_context_simple] = id;

    fprintf (file, "<Representation_context id=\"id%u\" "
	     "Context_identifier=\"\" "
	     "Context_type=\"units not necessary\"/>\n",
	     id
	);

    return id;
}

unsigned STEPNCWrite::make_geometry_context()
{
    if (!file) return 0;

    if (unit_system == inch_deg_ipm_rpm) {
	if (shared[id_context_english]) return shared[id_context_english];
	
	unsigned inch = make_inch_unit();
	unsigned deg  = make_degree_unit();
	unsigned sr   = make_steradian_unit();

	unsigned id = next_id();
	shared[id_context_english] = id;

	fprintf (file, "<exp:complexEntity id=\"id%u\" entities=\""
		 "geometric_representation_context "
		 "global_unit_assigned_context\">\n",
		 id
	    );
	fprintf (file, "  <Representation_context "
		 "Context_identifier=\"INCH DEGREE STERADIAN\" "
		 "Context_type=\"\"/>\n"
	    );

	fprintf (file, "  <Geometric_representation_context "
		 "Coordinate_space_dimension=\"3\"/>\n"
	    );

	fprintf (file, "  <Global_unit_assigned_context>\n");
	fprintf (file, "     <Units>\n");
	print_ref ("exp:complexEntity", inch);
	print_ref ("exp:complexEntity", deg);
	print_ref ("exp:complexEntity", sr);
	fprintf (file, "     </Units>\n");
	fprintf (file, "  </Global_unit_assigned_context>\n");
	fprintf (file, "</exp:complexEntity>\n");

	return id;
    }
    else {
	if (shared[id_context_metric]) return shared[id_context_metric];
	
	unsigned mm  = make_mm_unit();
	unsigned deg = make_degree_unit();
	unsigned sr  = make_steradian_unit();

	unsigned id = next_id();
	shared[id_context_metric] = id;

	fprintf (file, "<exp:complexEntity id=\"id%u\" entities=\""
		 "geometric_representation_context "
		 "global_unit_assigned_context\">\n",
		 id
	    );
	fprintf (file, "  <Representation_context "
		 "Context_identifier=\"MILLIMETRE DEGREE STERADIAN\" "
		 "Context_type=\"\"/>\n"
	    );

	fprintf (file, "  <Geometric_representation_context "
		 "Coordinate_space_dimension=\"3\"/>\n"
	    );

	fprintf (file, "  <Global_unit_assigned_context>\n");
	fprintf (file, "     <Units>\n");
	print_ref ("exp:complexEntity", mm);
	print_ref ("exp:complexEntity", deg);
	print_ref ("exp:complexEntity", sr);
	fprintf (file, "     </Units>\n");
	fprintf (file, "  </Global_unit_assigned_context>\n");
	fprintf (file, "</exp:complexEntity>\n");

	return id;
    }
}



//------------------------------------------------------------
// Properties and descriptive strings
//  
unsigned STEPNCWrite::make_action_property (
    const char * name, 
    unsigned am,
    unsigned rep,
    const char * object_type  // for the XML typing
    )
{
    if (!file) return 0;

    unsigned ap = next_id();
    fprintf (file, "<Action_property id=\"id%u\" Name=\"", ap);
    print_string (name? name: "");
    fprintf (file, "\" Description=\"milling\">\n");
    print_select_ref ("Definition", object_type, am);
    fprintf (file, "</Action_property>\n");

    fprintf (file, "<Action_property_representation id=\"id%u\" "
	     "Name=\"\" Description=\"milling\" "
	     "Property=\"id%u\" Representation=\"id%u\"/>\n",
	     next_id(), ap, rep
	);

    return ap;
}


unsigned STEPNCWrite::make_string_rep (
    const char * name
    )
{
    if (!file) return 0;
    unsigned repitem = next_id();

    // these are always shared, so just give the rep item and
    // representation names of 'constant' to emphasise the point
    //
    fprintf (file, "<Descriptive_representation_item id=\"id%u\" "
	     "Name=\"constant\" Description=\"", 
	     repitem
	);
    print_string (name);
    fprintf (file, "\"/>\n");


    unsigned ctx = make_simple_context();
    unsigned rep = next_id();

    fprintf (file, "<Representation id=\"id%u\" "
	     "Name=\"constant\" Items=\"id%u\" "
	     "Context_of_items=\"id%u\"/>\n",
	     rep, repitem, ctx
	);

    return rep;
}
