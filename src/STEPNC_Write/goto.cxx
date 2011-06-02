/* $RCSfile: goto.cxx,v $
 * $Revision: 1.11 $ $Date: 2006/08/31 19:45:37 $
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

void STEPNCWrite::GoToXYZ(
    const char * label, 
    double x, 
    double y, 
    double z
    )
{
    if (!file) return;


    // if we were in 5-axis mode and the tool axis not is at 0,0,1,
    // then generate a 5-axis move to the new location and move the
    // tool axis to 0,0,1, otherwise start a 3-axis toolpath.
    //
    if (axis.last_point_id &&
	! (STEPNC_FLOAT_EQUAL (axis.last_point[0], 0) &&
	   STEPNC_FLOAT_EQUAL (axis.last_point[1], 0) &&
	   STEPNC_FLOAT_EQUAL (axis.last_point[2], 1)) )
    {
	GoToXYZ_IJK(label, x,y,z, 0,0,1);
	end_move(); // next move is just a 3-axis move
	axis.last_point_id = 0;
	return;
    }

    if (axis.last_point_id) {
	end_move(); // go from 5-axis to 3-axis toolpaths
	axis.last_point_id = 0;
    }
    
    start_move (label);

    // if this is the first point in a polyline and we are at a
    // particular location from a previous toolpath, reuse the last
    // point from the previous move as our starting point.
    //
    if (loc.last_point_id && !loc.points.size())
    {
	loc.points.append (loc.last_point_id);
    }

    loc.last_point_id = make_point (label, x, y, z);
    loc.last_point[0] = x;
    loc.last_point[1] = y;
    loc.last_point[2] = z;
    
    loc.points.append (loc.last_point_id);
}


void STEPNCWrite::GoToXYZ_IJK(
    const char * label, 
    double x, 
    double y, 
    double z,
    double i,
    double j, 
    double k
    )
{
    if (!file) return;

    start_move (label);

    // If our last move was in 3axis mode, close any existing toolpath
    // and start a new 5-axis one from the last point.
    //
    if (loc.last_point_id && !axis.last_point_id)
    {
	// If we have more than a single point, we must start a new
	// toolpath.  Add the last location point as our initial point
	// with a 0,0,1 tool axis vector.
	if (loc.curves.size() || loc.points.size() > 1) {
	    end_move();
	    start_move (label);
	}

	// A single initial 3-axis move is ok, othewise the toolpath
	// will be separated and we have to re-add the last point
	if (!loc.points.size()) 
	    loc.points.append (loc.last_point_id);

	// the default tool axis
	axis.points.append (make_point ("", 0,0,1));
    }

    // if this is the first point in a polyline and we are at a
    // particular location from a previous toolpath, reuse the last
    // point from the previous move as our starting point.
    //
    if (loc.last_point_id && !loc.points.size())
    {
	loc.points.append (loc.last_point_id);

	// if we were not in 5-axis mode, assume the tool axis is 0,0,1.
	//
	if (!axis.last_point_id) {
	    axis.last_point_id = make_point (label, 0,0,1);
	}
	axis.points.append (axis.last_point_id);
    }

    loc.last_point_id = make_point (label, x, y, z);
    loc.last_point[0] = x;
    loc.last_point[1] = y;
    loc.last_point[2] = z;
    loc.points.append (loc.last_point_id);


    // If the tool axis has not changed, we can reuse the old one,
    // otherwise we have to create a new one as below.
    //
    if (!axis.last_point_id ||
	! (STEPNC_FLOAT_EQUAL (axis.last_point[0], i) &&
	   STEPNC_FLOAT_EQUAL (axis.last_point[1], j) &&
	   STEPNC_FLOAT_EQUAL (axis.last_point[2], k)) )
    {
	axis.last_point_id = make_point (label, i, j, k);
	axis.last_point[0] = i;
	axis.last_point[1] = j;
	axis.last_point[2] = k;
    }
    axis.points.append (axis.last_point_id);
}


void STEPNCWrite::Arc(
    const char * label, 
    double new_x, 
    double new_y, 
    double new_z, 
    double cx, 
    double cy, 
    double cz, 
    double radius, 
    int ccw
    )
{
    if (!file) return;

    unsigned arc_trim;
    unsigned arc_circle;
    unsigned arc_ap3d;
    unsigned arc_center;
    unsigned arc_start;
    unsigned arc_end;

    start_move (label);

    // No rapids in arcs because the machines do not do that.
    if (is_rapid) {
	fprintf (stderr, "%s : RAPID arc not allowed\n", 
		 label? label: "arc");
	return;
    }

    // Must have done at least one goto to give the start point
    if (!loc.last_point_id) {
	fprintf (stderr, "%s : Missing goto for start point of arc", 
		 label? label: "arc");
	return;
    }

    arc_start = loc.last_point_id;

    // arcs can only be described as part of a composite curve, so we
    // must start a new composite curve segment.  Close out a polyline
    // that happens to be open.  If the list only has one point then
    // we do not need to make a polyline.
    //
    if (loc.points.size() == 1) {
	loc.points.empty();
    }
    if (loc.points.size() > 1) {
	loc.curves.append (make_polyline ("", loc.points));
	loc.points.empty();
    }

    // Make a trimmed curve based upon a circle to represent the arc.
    // Add it to the composite curve segments so that we can write
    // everything out when we end the toolpath.
    char * nm = new char [strlen(label? label: "") + 100];
    sprintf (nm,"Arc end for %s in WS %u TP %u", 
	     (label? label: "-"), ws_count, tp_count);

    arc_end = make_point (nm, new_x,new_y,new_z);

    sprintf (nm,"Arc center for %s in WS %u TP %u", 
	     (label? label: "-"), ws_count, tp_count);

    arc_center = make_point (nm, cx,cy,cz);


    // Write an axis placement with the canonical Z and X directions
    if (!shared[id_direction_z] ||
	!shared[id_direction_x]) 
    {
	SetDirection (0,0,1, 1,0,0);
    }

    sprintf (nm,"Arc placement for %s in WS %u TP %u", 
	     (label? label: "-"), ws_count, tp_count);

    arc_ap3d = next_id();

    fprintf (file, "<Axis2_placement_3d id=\"id%u\" Name=\"", arc_ap3d);
    print_string (nm);
    fprintf (file, "\" Location=\"id%u\" "
	     "Axis=\"id%u\" "
	     "Ref_direction=\"id%u\"/>\n", 
	     arc_center, 
	     shared[id_direction_z], 
	     shared[id_direction_x]
	);

    // Write a circle at that location with the provided radius
    sprintf (nm,"Circle for %s in WS %u TP %u", 
	     (label? label: "-"), ws_count, tp_count);

    arc_circle = next_id();
    fprintf (file, "<Circle id=\"id%u\" Name=\"", arc_circle);
    print_string (nm);
    fprintf (file, "\" Radius=\"");
    print_real (radius);
    fprintf (file, "\">\n");

    print_select_ref ("Position", "Axis2_placement_3d", arc_ap3d);
    fprintf (file, "</Circle>\n");


    // Trim the circle at the start and end points
    sprintf (nm,"Arc for %s in WS %u TP %u", 
	     (label? label: "-"), ws_count, tp_count);

    arc_trim = next_id();
    fprintf (file, "<Trimmed_curve id=\"id%u\" Name=\"", arc_trim);
    print_string (nm);
    fprintf (file, "\" Basis_curve=\"id%u\" "
	     "Sense_agreement=\"%s\" "
	     "Master_representation=\"cartesian\">\n",
	     arc_circle, (ccw? "true": "false")
	);

    print_select_ref ("Trim_1", "Cartesian_point", arc_start);
    print_select_ref ("Trim_2", "Cartesian_point", arc_end);
    fprintf (file, "</Trimmed_curve>\n");


    // push the trimmed curve onto the composite curves segments list
    // so that we tie it together with the othe curves at the end of
    // the toolpath.
    loc.curves.append (arc_trim);
    loc.last_point_id = arc_end;
    loc.last_point[0] = new_x;
    loc.last_point[1] = new_y;
    loc.last_point[2] = new_z;

    
    // TOOL AXIS - if the arc is being machined in 5-axis mode, then
    // we need a two point poly line describing the tool axis at the
    // start and end.  These two points will be the same because cncs
    // can only do an arc in a plane.  It is not clear whether the
    // tool axis and the normal for the circle must be the same, but
    // we just use the incoming tool axis.
    //
    // As with the location curve, check to see if we have an open
    // polyline with a single point.  If we do, add one more point
    // otherwise we make a new polyline.
    //

    if (axis.last_point_id)
    {
	unsigned arc_axis = axis.last_point_id;

	// if we have a non-trivial open polyline, close it and start
	// another one
	//
	if (axis.points.size() > 1) {
	    axis.curves.append (make_polyline ("", axis.points));
	    axis.points.empty();
	}

	// create a two point polyline and append it.
	if (axis.points.size() == 1) {
	    axis.points.append (arc_axis);
	}
	else {
	    axis.points.append (arc_axis);
	    axis.points.append (arc_axis);
	}

	axis.curves.append (make_polyline ("", axis.points));
	axis.points.empty();
    }

    delete nm;
}





void  STEPNCWrite::SetDirection(
    double zi, double zj, double zk, 	// z axis
    double xa, double xb, double xc		// x axis
    )
{
    if (!file) return;

    // We could check to make sure that Z dot X is not zero


    // Create directions with given Z and X directions.  Will be used
    // in the placement for the circle in ARCs.  Replace any previous
    // directions.
    //

    // Share the Z direction vector for 001
    if (STEPNC_FLOAT_EQUAL (zi, 0) && 
	STEPNC_FLOAT_EQUAL (zj, 0) &&
	STEPNC_FLOAT_EQUAL (zk, 1)) 
    {
	if (!shared[id_direction_z_const]) {
	    shared[id_direction_z_const] = next_id();
	    fprintf (file, "<Direction id=\"id%u\" "
		     "Name=\"Z direction\" Direction_ratios=\"0 0 1\"/>\n",
		     shared[id_direction_z_const]
		);
	}
	shared[id_direction_z] = shared[id_direction_z_const];
    }
    else {
	shared[id_direction_z] = next_id();
	fprintf (file, "<Direction id=\"id%u\" "
		 "Name=\"Z direction\" Direction_ratios=\"",
		 shared[id_direction_z]
	    );
	print_real(zi);  fputc (' ', file);
	print_real(zj);  fputc (' ', file);
	print_real(zk);

	fprintf (file, "\"/>\n");
    }


    // Share the X direction vector for 100
    if (STEPNC_FLOAT_EQUAL (xa, 1) && 
	STEPNC_FLOAT_EQUAL (xb, 0) &&
	STEPNC_FLOAT_EQUAL (xc, 0)) 
    {
	// Write an axis placement with the canonical Z and X directions
	if (!shared[id_direction_x_const]) {
	    shared[id_direction_x_const] = next_id();
	    fprintf (file, "<Direction id=\"id%u\" "
		     "Name=\"X direction\" Direction_ratios=\"1 0 0\"/>\n",
		     shared[id_direction_x_const]
		);
	}
	shared[id_direction_x] = shared[id_direction_x_const];
    }
    else {
	shared[id_direction_x] = next_id();
	fprintf (file, "<Direction id=\"id%u\" "
		 "Name=\"X direction\" Direction_ratios=\"",
		 shared[id_direction_x]
	    );

	print_real(xa);  fputc (' ', file);
	print_real(xb);  fputc (' ', file);
	print_real(xc);

	fprintf (file, "\"/>\n");
    }
}



void STEPNCWrite::NestWorkplan (const char * name)
{
    make_workplan (name);
}

void STEPNCWrite::EndWorkplan()
{
    end_workplan();
}



void STEPNCWrite::Workingstep (const char * name)
{
    end_workingstep();
    make_workingstep (name);
}






// ------------------------------------------------------------
// TOOLPATH RELATED FUNCTIONS
// ------------------------------------------------------------

void STEPNCWrite::start_move(const char * label)
{
    if (!file) return;

    // Create a workingstep, freeform operation, feed/speed technology
    // and machine functions (coolant etc.) if needed
    //
    if (!current_op) make_workingstep(label);
    if (!current_tp) make_toolpath(label);
   
}

void STEPNCWrite::end_move()
{
    if (!file || !current_tp) return;

    // close out the toolpaths and write the curves, associate the
    // basis curve with the toolpath. 

    char nm[100];
    sprintf (nm, "basic curve for WS %u TP %u", ws_count, tp_count);

    // LOCATION CURVE -- There should always be a location curve.
    unsigned repitem = make_curve (nm, loc);
    if (repitem) {
	unsigned ctx = make_geometry_context();
	unsigned rep = next_id();
	fprintf (file, "<Representation id=\"id%u\" "
		 "Name=\"\" Items=\"id%u\" "
		 "Context_of_items=\"id%u\"/>\n",
		 rep, repitem, ctx
	    );

	make_action_property (
	    "basic curve", current_tp, rep, "Machining_toolpath"
	    );
    }

    // AXIS CURVE -- Only for 5-axis work
    sprintf (nm, "axis curve for WS %u TP %u", ws_count, tp_count);
    repitem = make_curve (nm, axis);
    if (repitem) {
	unsigned ctx = make_geometry_context();
	unsigned rep = next_id();
	fprintf (file, "<Representation id=\"id%u\" "
		 "Name=\"\" Items=\"id%u\" "
		 "Context_of_items=\"id%u\"/>\n",
		 rep, repitem, ctx
	    );

	make_action_property (
	    "tool axis", current_tp, rep, "Machining_toolpath"
	    );
    }

    current_tp = 0;
}

unsigned STEPNCWrite::make_toolpath (const char * label)
{
    if (!file) return 0;

    // Finish up any previous toolpath
    end_move();


    char * nm = new char [strlen(label? label: "") + 30];
    if (!label || !*label)
	sprintf (nm, "WS %u TP %u", ws_count, ++tp_count);
    else 
	sprintf (nm, "%s WS %u TP %u", label, ws_count, ++tp_count);


    // For now, just write a cutter location toolpath.  A cutter
    // contact toolpath can be added later as a refinement.
    //
    unsigned tp = next_id();

    fprintf (file, "\n<!-- TOOLPATH %s -->\n", nm);
    fprintf (file, "<Machining_toolpath id=\"id%u\" Name=\"", tp);
    print_string (nm);
    fprintf (file, "\" Description=\"cutter location trajectory\" "
	     "Consequence=\"\" Purpose=\"\"/>\n");


   // Associate the toolpath with the current operation using a
    // sequence relationship object
    //
    add_to_workingstep(tp);


    // Always give the toolpath a technology
    unsigned mt = make_technology();
    fprintf (file, "<Machining_technology_relationship id=\"id%u\" "
	     "Name=\"\" Description=\"\" "
	     "Relating_method=\"id%u\" "
	     "Related_method=\"id%u\"/>\n",
	     next_id(), tp, mt
	);

    // Only give the toolpath a machine functions if the coolant state
    // has changed  DO LATER


    // Rapid is handled by an overriding speed profile parameter on
    // the toolpath rather than by anything in the technology
    //
    if (is_rapid) {
	if (!shared[id_rapid]) {
	    unsigned repitem = next_id();

	    fprintf (file, "<Descriptive_representation_item id=\"id%u\" "
		     "Name=\"\" Description=\"rapid\"/>\n",
		     repitem
		);

	    unsigned ctx = make_simple_context();
	    unsigned rep = next_id();

	    fprintf (file, "<Machining_toolpath_speed_profile_representation "
		     "id=\"id%u\" Name=\"\" Items=\"id%u\" "
		     "Context_of_items=\"id%u\"/>\n",
		     rep, repitem, ctx
		);

	    shared[id_rapid] = rep;
	}

	unsigned ap = next_id();
	
	fprintf (file, "<Action_property id=\"id%u\" "
		 "Name=\"speed profile\" Description=\"rapid\">\n",
		 ap
		);
	print_select_ref ("Definition", "Machining_toolpath", tp);
	fprintf (file, "</Action_property>\n");


	fprintf (file, "<action_property_representation id=\"id%u\" "
		 "Name=\"\" Description=\"rapid\" "
		 "Property=\"id%u\" Representation=\"id%u\"/>\n",
		 next_id(), ap, shared[id_rapid]
	    );
    }

    // Some common properties for the toolpaths
    //
    if (!shared[id_trajectory_path]) {
	shared[id_trajectory_path] = make_string_rep ("trajectory path");
    }
    make_action_property (
	"trajectory type", tp, shared[id_trajectory_path], "Machining_toolpath"
	);
    
    if (!shared[id_priority]) {
	shared[id_priority] = make_string_rep ("required");
    }
    make_action_property (
	"priority", tp, shared[id_priority], "Machining_toolpath"
	);


    // The basis, tool axis, and surface normal curves can not be
    // associated with the toolpath until they are finished writing.

    current_tp = tp;
    return tp;
}




// ------------------------------------------------------------
// WORKPLAN RELATED FUNCTIONS
// ------------------------------------------------------------

unsigned STEPNCWrite::make_workplan (const char * label)
{
    if (!file) return 0;

    unsigned id = next_id();
    fprintf (file, "\n<!-- WORKPLAN %s  -->\n", label);

    fprintf (file, "<Machining_workplan id=\"id%u\" Name=\"", id);
    print_string (label? label: "");
    fprintf (file, "\" Description=\"\" Consequence=\"\" Purpose=\"\"/>\n");


    // Associate the new workplan with the current workplan if it
    // exists, or as the main workplan of the project otherwise.
    //
    if (!plan_ids.size()) {
	// first one, add to the project
	unsigned ppa = next_id();
	unsigned pdp = next_id();

	fprintf (file, "<Process_product_association id=\"id%u\" "
		 "Name=\"\" Description=\"\" Process=\"id%u\">\n",
		 ppa, pdp
	    );
	print_select_ref (
	    "Defined_product", "Product_definition", shared[id_project_pd]
	    );
	fprintf (file, "</Process_product_association>\n");


	fprintf (file, "<Product_definition_process id=\"id%u\" "
		 "Name=\"machining\" Description=\"\" "
		 "Chosen_method=\"id%u\" Identification=\"\"/>\n",
		 pdp, id
	    );
    }
    else {
	// add to existing workplan
	add_to_workplan (id);
    }

    // This is now the active workplan
    plan_ids.push(id);
    plan_idxs.push(1);  // start numbering at zero

    current_op = 0;
    current_tp = 0;

    return id;
}

void STEPNCWrite::end_workplan()
{
    if (!file) return;
    if (!plan_ids.size()) return;

    end_workingstep();

    // Pop back to the last workplan
    plan_ids.pop();
    plan_idxs.pop(); 

    // if we end the last workplan, close the file
    if (!plan_ids.size()) Close();
}

void STEPNCWrite::add_to_workplan(
    unsigned id 
    )
{
    // Associate the id with the current workplan using a sequence
    // relationship object
    //
    unsigned seq = plan_idxs.top();
    plan_idxs.pop();
    plan_idxs.push(seq+1);

    fprintf (file, "<Machining_process_sequence_relationship id=\"id%u\" "
	     "Name=\"\" Description=\"\" "
	     "Relating_method=\"id%u\" "
	     "Related_method=\"id%u\" "
	     "Sequence_position=\"%u\"/>\n",
	     next_id(), plan_ids.top(), id, seq
	);
}




// ------------------------------------------------------------
// WORKINGSTEP RELATED FUNCTIONS
// ------------------------------------------------------------

// Make a workingstep and associated operation
//
unsigned STEPNCWrite::make_workingstep (const char * label)
{
    if (!file) return 0;

    char * nm = new char [strlen(label? label: "") + 30];
    if (!label || !*label)
	sprintf (nm, "WS %u", ++ws_count);
    else 
	sprintf (nm, "%s WS %u", label, ++ws_count);


    unsigned ws = next_id();
    fprintf (file, "\n<!-- WORKINGSTEP %s -->\n", nm);
    fprintf (file, "<Machining_workingstep id=\"id%u\" Name=\"", ws);
    print_string (nm);
    fprintf (file, "\" Description=\"machining\" Consequence=\"\" Purpose=\"\"/>\n");


    // Associate the workingstep with the current workplan using a
    // sequence relationship object
    //
    add_to_workplan (ws);

    // Associate the workingstep with our single toolpath feature
    //
    unsigned feat = make_toolpath_feature();
    fprintf (file, "<Machining_feature_relationship id=\"id%u\" "
	     "Name=\"\" Description=\"\" "
	     "Relating_method=\"id%u\" "
	     "Related_method=\"id%u\"/>\n",
	     next_id(), ws, feat
	);

    // Create the operation.  This is what the toolpaths, technology,
    // and machine functions (coolant and such) are associated with so
    // that is what we return.
    //
    current_op = next_id();
    current_tp = 0;
    current_tpidx = 0;

    fprintf (file, "<Freeform_milling_operation id=\"id%u\" Name=\"",
	     current_op
	);
    print_string (nm);
    fprintf (file, "\" Description=\"\" Consequence=\"\" Purpose=\"\"/>\n");


    // Link the operation to the workingstep
    fprintf (file, "<Machining_operation_relationship id=\"id%u\" "
	     "Name=\"\" Description=\"\" "
	     "Relating_method=\"id%u\" "
	     "Related_method=\"id%u\"/>\n",
	     next_id(), ws, current_op
	);

    unsigned mt = make_technology();
    fprintf (file, "<Machining_technology_relationship id=\"id%u\" "
	     "Name=\"\" Description=\"\" "
	     "Relating_method=\"id%u\" "
	     "Related_method=\"id%u\"/>\n",
	     next_id(), current_op, mt
	);

    unsigned mf = make_mfunc();
    fprintf (file, "<Machining_functions_relationship id=\"id%u\" "
	     "Name=\"\" Description=\"\" "
	     "Relating_method=\"id%u\" "
	     "Related_method=\"id%u\"/>\n",
	     next_id(), current_op, mf
	);

    // The workingstep operation needs a tool, use the current one or
    // create a default one if needed.
    //
    if (!current_tool) 
	LoadTool(1);

    current_tool-> users.append (current_op);

    delete nm;
    return current_op;
}

void STEPNCWrite::end_workingstep()
{
    end_move();
    current_op = 0;
    current_tp = 0;
}


void STEPNCWrite::add_to_workingstep(
    unsigned tp
    )
{
    // Associate the toolpath with the current operation using a
    // sequence relationship object
    //
    fprintf (file, "<Machining_toolpath_sequence_relationship id=\"id%u\" "
	     "Name=\"\" Description=\"\" "
	     "Relating_method=\"id%u\" "
	     "Related_method=\"id%u\" "
	     "Sequence_position=\"%u\"/>\n",
	     next_id(), current_op, tp, ++current_tpidx
	);
}




unsigned STEPNCWrite::make_toolpath_feature()
{
    if (!file) return 0;

    if (shared[id_feature_mfp]) return shared[id_feature_mfp];


    // Only one toolpath feature is created and it is associated with
    // all of the workingsteps.  It is a step shape aspect and refers
    // to the workpiece product definition shape.
    //
    if (!shared[id_workpiece_pds]) {
	PartNo ("unspecified part");
    }

    shared[id_feature] = next_id();
    shared[id_feature_mfp] = next_id();
    unsigned pp = next_id();

    fprintf (file, "\n<!-- SHARED TOOLPATH FEATURE -->\n");

    fprintf (file, "<Instanced_feature id=\"id%u\" "
	     "Characterized_object.Name=\"\" "
	     "Characterized_object.Description=\"toolpath\" "
	     "Shape_aspect.Name=\"\" "
	     "Shape_aspect.Description=\"toolpath\" "
	     "Of_shape=\"id%u\" "
	     "Product_definitional=\"true\"/>\n",
	     shared[id_feature], shared[id_workpiece_pds]
	);


    // The machining feature process is what all of the workingsteps
    // refer to when they attach to a feature.
    //
    fprintf (file, "<Machining_feature_process id=\"id%u\" "
	     "Name=\"\" Description=\"machining\" "
	     "Consequence=\"\" Purpose=\"\"/>\n",
	     shared[id_feature_mfp]
	);

    fprintf (file, "<Property_process id=\"id%u\" "
	     "Name=\"machining\" Description=\"machining\" "
	     "Chosen_method=\"id%u\" Identification=\"\"/>\n",
	     pp, shared[id_feature_mfp]
	);

    fprintf (file, "<Process_property_association id=\"id%u\" "
	     "Name=\"\" Description=\"machining\" Process=\"id%u\">\n",
	     next_id(), pp
	);
    print_select_ref (
	    "Property_or_shape", "Instanced_feature", shared[id_feature]
	    );
    fprintf (file, "</Process_property_association>\n");


    // Give the feature a simple representation with a placement
    // at (0,0,0)
    //
    unsigned context = make_geometry_context();
    unsigned pds = next_id();
    unsigned sdr = next_id();
    unsigned rep = next_id();
    unsigned ap3d = next_id();
    unsigned origin = next_id();

    fprintf (file, "<Product_definition_shape id=\"id%u\" "
	     "Name=\"orientation\" Description=\"toolpath\">\n",
	     pds
	);
    print_select_ref (
	"Definition", "Instanced_feature", shared[id_feature]
	);
    fprintf (file, "</Product_definition_shape>\n");


    fprintf (file, "<Shape_definition_representation id=\"id%u\" "
	     "Used_representation=\"id%u\">\n",
	     sdr, rep
	);
    print_select_ref (
	"Definition", "Product_definition_shape", pds
	);
    fprintf (file, "</Shape_definition_representation>\n");



    fprintf (file, "<Shape_representation_with_parameters id=\"id%u\" "
	     "Name=\"\" Items=\"id%u\" Context_of_items=\"id%u\"/>\n",
	     rep, ap3d, context
	);

    fprintf (file, "<Axis2_placement_3d id=\"id%u\" "
	     "Name=\"orientation\" Location=\"id%u\"/>\n", 
	     ap3d, origin
	);

    fprintf (file, "<Cartesian_point id=\"id%u\" "
	     "Name=\"origin\" Coordinates=\"0 0 0\"/>\n",
	     origin
	);

    return shared[id_feature_mfp];
}



// ------------------------------------------------------------
// GEOMETRY RELATED FUNCTIONS
// ------------------------------------------------------------


unsigned STEPNCWrite::make_point (
    const char * label, 
    double x, 
    double y, 
    double z)
{
    if (!file) return 0;

    unsigned id = next_id();

    fprintf (file, "<Cartesian_point id=\"id%u\" Name=\"", id);
    print_string (label? label: "");

    fprintf (file, "\" Coordinates=\"");
    print_real(x);  fputc (' ', file);
    print_real(y);  fputc (' ', file);
    print_real(z);

    fprintf (file, "\"/>\n");

    return id;
}



unsigned STEPNCWrite::make_curve (
    const char * label, 
    curvedef &def
    )
{
    // close out an open composite curve.
    //
    unsigned id = 0;
    if (def.curves.size()) {

	// close out any last polyline segment
	if (def.points.size()) {
	    def.curves.append (make_polyline ("", def.points));
	    def.points.empty();
	}

	id = make_composite_curve (label, def.curves);
	def.curves.empty();
    }

    // entire basis curve is only a polyline
    else if (def.points.size()) {
	id = make_polyline (label, def.points);
	def.points.empty();
    }
    return id;
}



unsigned STEPNCWrite::make_polyline (
    const char * label, 
    STEPNCIDList &ids
    )
{
    if (!file) return 0;
    if (!ids.size()) return 0;

    unsigned i;
    unsigned pl = next_id();

    fprintf (file, "<Polyline id=\"id%u\" Name=\"", pl);
    print_string (label? label: "");
    fprintf (file, "\" Points=\"");
    for (i=0; i< ids.size(); i++) {
	if (i) fputc (' ', file);
	fprintf (file, "id%u", ids[i]);
    }
    fputs ("\"/>\n", file);
    return pl;
}


unsigned STEPNCWrite::make_composite_curve (
    const char * label, 
    STEPNCIDList &ids
    )
{
    if (!file) return 0;
    if (!ids.size()) return 0;

    // These are all going to be sequential so just keep track of the
    // base id.  If we had objects in between, we would need to keep a
    // separate list.
    //
    unsigned base_id;
    unsigned i;

    for (i=0; i< ids.size(); i++) {
	unsigned id = next_id();
	if (!i) base_id = id; // save the first

	fprintf (file, "<Composite_curve_segment id=\"id%u\" "
		 "Transition=\"%s\" Same_sense=\"true\" "
		 "Parent_curve=\"id%u\"/>\n",
		 id, (i+1 == ids.size()? "discontinuous": "continuous"),
		 ids[i]
	    );
    }

    unsigned cc = next_id();

    fprintf (file, "<Composite_curve id=\"id%u\" Name=\"", cc);
    print_string (label? label: "");
    fprintf (file, "\" Segments=\"");
    for (i=0; i< ids.size(); i++) {
	if (i) fputc (' ', file);
	fprintf (file, "id%u", base_id+i);
    }
    fputs ("\" Self_intersect=\"unknown\"/>\n", file);
    return cc;
}

