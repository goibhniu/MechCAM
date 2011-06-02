/* $RCSfile: measures.cxx,v $
 * $Revision: 1.3 $ $Date: 2006/08/31 19:45:37 $
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

unsigned STEPNCWrite::make_length_measure (
    const char * label,
    double val
    )
{	
    if (!file) return 0;
    unsigned u = make_length_unit();
    unsigned id = next_id();

    fprintf (file, "<exp:complexEntity id=\"id%u\" entities=\""
	     "length_measure_with_unit "
	     "measure_representation_item\">\n",
	     id
	);

    fprintf (file, "  <Measure_with_unit>\n");
    fprintf (file, "     <Value_component>\n");
    fprintf (file, "\t<Length_measure-wrapper>");
    print_real(val);
    fprintf (file, "</Length_measure-wrapper>\n");

    fprintf (file, "     </Value_component>\n");
    fprintf (file, "     <Unit_component>\n");
    print_ref ("exp:complexEntity", u);
    fprintf (file, "     </Unit_component>\n");
    fprintf (file, "  </Measure_with_unit>\n");

    fprintf (file, "  <Length_measure_with_unit/>\n");

    fprintf (file, "  <Representation_item Name=\"");
    print_string (label? label: "");
    fprintf (file, "\"/>\n");

    fprintf (file, "  <Measure_representation_item/>\n");
    fprintf (file, "</exp:complexEntity>\n");

    return id;
}


unsigned STEPNCWrite::make_angle_measure (
    const char * label,
    double val
    )
{	
    if (!file) return 0;
    unsigned u = make_angle_unit();
    unsigned id = next_id();

    fprintf (file, "<exp:complexEntity id=\"id%u\" entities=\""
	     "plane_angle_measure_with_unit "
	     "measure_representation_item\">\n",
	     id
	);

    fprintf (file, "  <Measure_with_unit>\n");
    fprintf (file, "     <Value_component>\n");
    fprintf (file, "\t<Plane_angle_measure-wrapper>");
    print_real(val);
    fprintf (file, "</Plane_angle_measure-wrapper>\n");

    fprintf (file, "     </Value_component>\n");
    fprintf (file, "     <Unit_component>\n");
    print_ref ("exp:complexEntity", u);
    fprintf (file, "     </Unit_component>\n");
    fprintf (file, "  </Measure_with_unit>\n");

    fprintf (file, "  <Plane_angle_measure_with_unit/>\n");

    fprintf (file, "  <Representation_item Name=\"");
    print_string (label? label: "");
    fprintf (file, "\"/>\n");

    fprintf (file, "  <Measure_representation_item/>\n");
    fprintf (file, "</exp:complexEntity>\n");

    return id;
}

unsigned STEPNCWrite::make_feed_measure (
    const char * label,
    double val
    )
{	
    if (!file) return 0;
    unsigned u = make_feed_unit();
    unsigned id = next_id();

    fprintf (file, "<Measure_representation_item id=\"id%u\" Name=\"", id);
    print_string (label? label: "");
    fprintf (file, "\">\n");

    fprintf (file, "     <Value_component>\n");
    fprintf (file, "\t<Numeric_measure-wrapper>");
    print_real(val);
    fprintf (file, "</Numeric_measure-wrapper>\n");

    fprintf (file, "     </Value_component>\n");
    fprintf (file, "     <Unit_component>\n");
    print_ref ("Derived_unit", u);
    fprintf (file, "     </Unit_component>\n");

    fprintf (file, "</Measure_representation_item>\n");

    return id;
}



unsigned STEPNCWrite::make_spindle_measure (
    const char * label,
    double val
    )
{	
    if (!file) return 0;
    unsigned u = make_spindle_unit();
    unsigned id = next_id();

    fprintf (file, "<Measure_representation_item id=\"id%u\" Name=\"", id);
    print_string (label? label: "");
    fprintf (file, "\">\n");

    fprintf (file, "     <Value_component>\n");
    fprintf (file, "\t<Numeric_measure-wrapper>");
    print_real(val);
    fprintf (file, "</Numeric_measure-wrapper>\n");

    fprintf (file, "     </Value_component>\n");
    fprintf (file, "     <Unit_component>\n");
    print_ref ("Derived_unit", u);
    fprintf (file, "     </Unit_component>\n");

    fprintf (file, "</Measure_representation_item>\n");

    return id;
}

