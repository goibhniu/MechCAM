/* $RCSfile: partno.cxx,v $
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

void STEPNCWrite::PartNo(
    const char *wp_name	
    )
{
    // Write a workpiece and associate it with the machining project.
    // This only creates a workpiece the first time it is called and
    // ignores subsequent calls.
    //
    if (!file || shared[id_workpiece_pd]) return;

    /* we are going to write a bunch of objects that may be referenced
     * later, so keep track of the ids we give them
     */
    shared[id_workpiece_product] = next_id();
    shared[id_workpiece_pdf] 	= next_id();
    shared[id_workpiece_pd]  	= next_id();
    shared[id_workpiece_pds]  	= next_id();

    fprintf (file, "<Product id=\"id%u\" Id=\"", 
	     shared[id_workpiece_product]
	);
    print_string (wp_name? wp_name: "unnamed workpiece");
    fprintf (file, "\" Name=\"AP238 CC1\" Frame_of_reference=\"id%u\"/>\n",
	     shared[id_context_product]
	);

    fprintf (file, "<Product_definition_formation id=\"id%u\" "
	     "Id=\"1.0\" Description=\"workpiece\"  Of_product=\"id%u\"/>\n", 
	     shared[id_workpiece_pdf], shared[id_workpiece_product]
	);

    fprintf (file, "<Product_definition id=\"id%u\" "
	     "Id=\"workpiece\" Description=\"\" "
	     "Formation=\"id%u\" Frame_of_reference=\"id%u\"/>\n",
	     shared[id_workpiece_pd], shared[id_workpiece_pdf], 
	     shared[id_context_pd]
	);

    fprintf (file, "<Product_definition_shape id=\"id%u\" "
	     "Name=\"\" Description=\"\">\n",
	     shared[id_workpiece_pds]
	);
    print_select_ref (
	"Definition", "Product_definition", shared[id_workpiece_pd]
	);
    fprintf (file, "</Product_definition_shape>\n");


    fprintf (file, "<Machining_project_workpiece_relationship id=\"id%u\" "
	     "Id=\"\" Name=\"workpiece\" Description=\"\" "
	     "Relating_product_definition=\"id%u\" "
	     "Related_product_definition=\"id%u\"/>\n",
	     next_id(), shared[id_project_pd], shared[id_workpiece_pd]
	);
}
