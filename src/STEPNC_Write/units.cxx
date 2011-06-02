/* $RCSfile: units.cxx,v $
 * $Revision: 1.5 $ $Date: 2006/08/21 14:15:51 $
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
// Construct the various common SI and English units.
//
// These functions only create one instance of a given unit in a
// design.  The first time the unit is created, we save a shortcut in
// the nametable of the design.  This lets us quickly find the unit
// again if we need it, which keeps us from creating duplicate unit
// definitions.
//
unsigned STEPNCWrite::make_length_unit()
{
    switch (unit_system) {
    case inch_deg_ipm_rpm:  	return make_inch_unit();
    case mm_deg_mmpm_rpm:   	return make_mm_unit();
    default:			return make_mm_unit();
    }
}

unsigned STEPNCWrite::make_mm_unit()
{
    if (!file) return 0;
    if (shared[id_unit_mm]) return shared[id_unit_mm];

    unsigned id = next_id();
    shared[id_unit_mm] = id;

    fprintf (file, "\n<!-- SHARED MILLIMETER UNIT -->\n");
    fprintf (file, "<exp:complexEntity id=\"id%u\" "
	     "entities=\"length_unit si_unit\">\n",
	     id
	);
    fprintf (file, "  <Named_unit/>\n");
    fprintf (file, "  <Length_unit/>\n");
    fprintf (file, "  <Si_unit Prefix=\"milli\" Name=\"metre\"/>\n");
    fprintf (file, "</exp:complexEntity>\n");
    return id;
}


unsigned STEPNCWrite::make_inch_unit()
{
    if (!file) return 0;
    if (shared[id_unit_inch]) return shared[id_unit_inch];
    
    unsigned mm = make_mm_unit();
    unsigned id = next_id();
    shared[id_unit_inch] = id;

    fprintf (file, "\n<!-- SHARED INCH UNIT -->\n");
    fprintf (file, "<exp:complexEntity id=\"id%u\" "
	     "entities=\"conversion_based_unit length_unit\">\n",
	     id
	);
    fprintf (file, "  <Named_unit Dimensions=\"id%u\"/>\n", id+2);
    fprintf (file, "  <Length_unit/>\n");
    fprintf (file, "  <Conversion_based_unit Name=\"inch\" "
	     "Conversion_factor=\"id%u\"/>\n",
	     id+1
	);
    fprintf (file, "</exp:complexEntity>\n");


    fprintf (file, "<Length_measure_with_unit id=\"id%u\">\n", next_id());
    fprintf (file, "     <Value_component>\n");
    fprintf (file, "\t<Length_measure-wrapper>25.4</Length_measure-wrapper>\n");
    fprintf (file, "     </Value_component>\n");
    fprintf (file, "     <Unit_component>\n");
    print_ref ("exp:complexEntity", mm);
    fprintf (file, "     </Unit_component>\n");
    fprintf (file, "</Length_measure_with_unit>\n");

    fprintf (file, "<Dimensional_exponents id=\"id%u\" "
	     "Length_exponent=\"1\" Mass_exponent=\"0\" "
	     "Time_exponent=\"0\" Electric_current_exponent=\"0\" "
	     "Thermodynamic_temperature_exponent=\"0\" "
	     "Amount_of_substance_exponent=\"0\" "
	     "Luminous_intensity_exponent=\"0\"/>\n",
	     next_id()
	);

    return id;
}


unsigned STEPNCWrite::make_angle_unit()
{
    return make_degree_unit();
}

unsigned STEPNCWrite::make_radian_unit()
{
    if (!file) return 0;
    if (shared[id_unit_radian]) return shared[id_unit_radian];
    
    unsigned id = next_id();
    shared[id_unit_radian] = id;

    fprintf (file, "\n<!-- SHARED RADIAN UNIT -->\n");
    fprintf (file, "<exp:complexEntity id=\"id%u\" "
	     "entities=\"plane_angle_unit si_unit\">\n",
	     id
	);
    fprintf (file, "  <Named_unit/>\n");
    fprintf (file, "  <Plane_angle_unit/>\n");
    fprintf (file, "  <Si_unit Name=\"radian\"/>\n");
    fprintf (file, "</exp:complexEntity>\n");

    return id;
}



unsigned STEPNCWrite::make_degree_unit()
{
    if (!file) return 0;
    if (shared[id_unit_degree]) return shared[id_unit_degree];
    
    unsigned rad = make_radian_unit();
    unsigned id = next_id();
    shared[id_unit_degree] = id;

    fprintf (file, "\n<!-- SHARED DEGREE UNIT -->\n");
    fprintf (file, "<exp:complexEntity id=\"id%u\" "
	     "entities=\"conversion_based_unit plane_angle_unit\">\n",
	     id
	);
    fprintf (file, "  <Named_unit Dimensions=\"id%u\"/>\n", id+2);
    fprintf (file, "  <Plane_angle_unit/>\n");
    fprintf (file, "  <Conversion_based_unit Name=\"degree\" "
	     "Conversion_factor=\"id%u\"/>\n",
	     id+1
	);
    fprintf (file, "</exp:complexEntity>\n");


    fprintf (file, "<Plane_angle_measure_with_unit id=\"id%u\">\n", next_id());
    fprintf (file, "     <Value_component>\n");
    fprintf (file, "\t<Plane_angle_measure-wrapper>0.01745329252"
	     "</Plane_angle_measure-wrapper>\n");
    fprintf (file, "     </Value_component>\n");
    fprintf (file, "     <Unit_component>\n");
    print_ref ("exp:complexEntity", rad);
    fprintf (file, "     </Unit_component>\n");
    fprintf (file, "</Plane_angle_measure_with_unit>\n");


    fprintf (file, "<Dimensional_exponents id=\"id%u\" "
	     "Length_exponent=\"0\" Mass_exponent=\"0\" "
	     "Time_exponent=\"0\" Electric_current_exponent=\"0\" "
	     "Thermodynamic_temperature_exponent=\"0\" "
	     "Amount_of_substance_exponent=\"0\" "
	     "Luminous_intensity_exponent=\"0\"/>\n",
	     next_id()
	);

    return id;
}



unsigned STEPNCWrite::make_steradian_unit()
{
    if (!file) return 0;
    if (shared[id_unit_steradian]) return shared[id_unit_steradian];
    
    unsigned id = next_id();
    shared[id_unit_steradian] = id;

    fprintf (file, "\n<!-- SHARED STERADIAN UNIT -->\n");
    fprintf (file, "<exp:complexEntity id=\"id%u\" "
	     "entities=\"solid_angle_unit si_unit\">\n",
	     id
	);
    fprintf (file, "  <Named_unit/>\n");
    fprintf (file, "  <Solid_angle_unit/>\n");
    fprintf (file, "  <Si_unit Name=\"steradian\"/>\n");
    fprintf (file, "</exp:complexEntity>\n");

    return id;
}


unsigned STEPNCWrite::make_second_unit() 
{
    if (!file) return 0;
    if (shared[id_unit_second]) return shared[id_unit_second];
    
    unsigned id = next_id();
    shared[id_unit_second] = id;

    fprintf (file, "\n<!-- SHARED SECOND UNIT -->\n");
    fprintf (file, "<exp:complexEntity id=\"id%u\" "
	     "entities=\"time_unit si_unit\">\n",
	     id
	);
    fprintf (file, "  <Named_unit/>\n");
    fprintf (file, "  <Time_unit/>\n");
    fprintf (file, "  <Si_unit Name=\"second\"/>\n");
    fprintf (file, "</exp:complexEntity>\n");

    return id;
}

unsigned STEPNCWrite::make_minute_unit()
{
    if (!file) return 0;
    if (shared[id_unit_minute]) return shared[id_unit_minute];
    
    unsigned sec = make_second_unit();
    unsigned id = next_id();
    shared[id_unit_minute] = id;

    fprintf (file, "\n<!-- SHARED MINUTE UNIT -->\n");
    fprintf (file, "<exp:complexEntity id=\"id%u\" "
	     "entities=\"conversion_based_unit time_unit\">\n",
	     id
	);
    fprintf (file, "  <Named_unit Dimensions=\"id%u\"/>\n", id+2);
    fprintf (file, "  <Time_unit/>\n");
    fprintf (file, "  <Conversion_based_unit Name=\"minute\" "
	     "Conversion_factor=\"id%u\"/>\n",
	     id+1
	);
    fprintf (file, "</exp:complexEntity>\n");


    fprintf (file, "<Time_measure_with_unit id=\"id%u\">\n", next_id());
    fprintf (file, "     <Value_component>\n");
    fprintf (file, "\t<Time_measure-wrapper>60</Time_measure-wrapper>\n");
    fprintf (file, "     </Value_component>\n");
    fprintf (file, "     <Unit_component>\n");
    print_ref ("exp:complexEntity", sec);
    fprintf (file, "     </Unit_component>\n");
    fprintf (file, "</Time_measure_with_unit>\n");

    fprintf (file, "<Dimensional_exponents id=\"id%u\" "
	     "Length_exponent=\"0\" Mass_exponent=\"0\" "
	     "Time_exponent=\"1\" Electric_current_exponent=\"0\" "
	     "Thermodynamic_temperature_exponent=\"0\" "
	     "Amount_of_substance_exponent=\"0\" "
	     "Luminous_intensity_exponent=\"0\"/>\n",
	     next_id()
	);

    return id;
}


unsigned STEPNCWrite::make_feed_unit()
{
    switch (unit_system) {
    case inch_deg_ipm_rpm:  	return make_ipm_unit();
    case mm_deg_mmpm_rpm:   	return make_mmpm_unit();
    default:			return make_mmpm_unit();
    }
}

unsigned STEPNCWrite::make_mmpm_unit()
{
    if (!file) return 0;
    if (shared[id_unit_mmpm]) return shared[id_unit_mmpm];
    
    unsigned mm = make_mm_unit();
    unsigned min = make_minute_unit();
    unsigned id = next_id();
    shared[id_unit_mmpm] = id;

    fprintf (file, "\n<!-- SHARED MM/MIN UNIT -->\n");
    fprintf (file, "<Derived_unit id=\"id%u\" Elements=\"id%u id%u\"/>\n", 
	     id, id+2, id+3
	);

    fprintf (file, "<Name_attribute id=\"id%u\" "
	     "Attribute_value=\"millimetre/minute\">\n", 
	     next_id()
	);
    print_select_ref ("Named_item", "Derived_unit", id);
    fprintf (file, "</Name_attribute>\n");


    fprintf (file, "<Derived_unit_element id=\"id%u\" "
	     "Unit=\"id%u\" Exponent=\"1\"/>\n", 
	     next_id(), mm
	);

    fprintf (file, "<Derived_unit_element id=\"id%u\" "
	     "Unit=\"id%u\" Exponent=\"-1\"/>\n", 
	     next_id(), min
	);

    return id;
}



unsigned STEPNCWrite::make_ipm_unit()
{
    if (!file) return 0;
    if (shared[id_unit_ipm]) return shared[id_unit_ipm];
    
    unsigned inch = make_inch_unit();
    unsigned min = make_minute_unit();
    unsigned id = next_id();
    shared[id_unit_ipm] = id;

    fprintf (file, "\n<!-- SHARED IN/MIN UNIT -->\n");
    fprintf (file, "<Derived_unit id=\"id%u\" Elements=\"id%u id%u\"/>\n", 
	     id, id+2, id+3
	);

    fprintf (file, "<Name_attribute id=\"id%u\" "
	     "Attribute_value=\"inch/minute\">\n", 
	     next_id()
	);
    print_select_ref ("Named_item", "Derived_unit", id);
    fprintf (file, "</Name_attribute>\n");


    fprintf (file, "<Derived_unit_element id=\"id%u\" "
	     "Unit=\"id%u\" Exponent=\"1\"/>\n", 
	     next_id(), inch
	);

    fprintf (file, "<Derived_unit_element id=\"id%u\" "
	     "Unit=\"id%u\" Exponent=\"-1\"/>\n", 
	     next_id(), min
	);

    return id;
}



unsigned STEPNCWrite::make_spindle_unit()
{
    return make_rpm_unit();
}

unsigned STEPNCWrite::make_rps_unit()
{
    if (!file) return 0;
    if (shared[id_unit_rps]) return shared[id_unit_rps];
    unsigned sec = make_second_unit();
    unsigned id = next_id();
    shared[id_unit_rps] = id;

    fprintf (file, "\n<!-- SHARED REV/SEC UNIT -->\n");
    fprintf (file, "<Derived_unit id=\"id%u\" Elements=\"id%u\"/>\n", 
	     id, id+2
	);

    fprintf (file, "<Name_attribute id=\"id%u\" "
	     "Attribute_value=\"revolution/second\">\n", 
	     next_id()
	);
    print_select_ref ("Named_item", "Derived_unit", id);
    fprintf (file, "</Name_attribute>\n");

    fprintf (file, "<Derived_unit_element id=\"id%u\" "
	     "Unit=\"id%u\" Exponent=\"-1\"/>\n", 
	     next_id(), sec
	);

    return id;
}


unsigned STEPNCWrite::make_rpm_unit()
{
    if (!file) return 0;
    if (shared[id_unit_rpm]) return shared[id_unit_rpm];
    
    unsigned min = make_minute_unit();
    unsigned id = next_id();
    shared[id_unit_rpm] = id;

    fprintf (file, "\n<!-- SHARED REV/MIN UNIT -->\n");
    fprintf (file, "<Derived_unit id=\"id%u\" Elements=\"id%u\"/>\n", 
	     id, id+2
	);

    fprintf (file, "<Name_attribute id=\"id%u\" "
	     "Attribute_value=\"revolution/minute\">\n", 
	     next_id()
	);
    print_select_ref ("Named_item", "Derived_unit", id);
    fprintf (file, "</Name_attribute>\n");

    fprintf (file, "<Derived_unit_element id=\"id%u\" "
	     "Unit=\"id%u\" Exponent=\"-1\"/>\n", 
	     next_id(), min
	);

    return id;
}


