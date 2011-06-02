/* $RCSfile: STEPNCWrite.cxx,v $
 * $Revision: 1.10 $ $Date: 2006/08/31 19:45:37 $
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

#include <sys/types.h>		/* used by the iso timestamp */
#include <time.h>		/* used by the iso timestamp */

STEPNCWrite::STEPNCWrite() 
{
    file = 0;
    all_techs = 0;
    all_mfuncs = 0;
    all_tools = 0;

    Reset();  	/* clear all of the shared ids too */
    Inches(); 	/* Set default units */
}

STEPNCWrite::~STEPNCWrite() 
{
    Reset();  // close file and delete any state info
}



void STEPNCWrite::Reset()
{
    int i;

    // make sure file is closed if we had an open one 
    if (file) fclose (file);
    file = 0;

    current_id = 9;

    /* clear all shared ids */
    for (i=0; i<ID_MAXIMUM_SHARED; i++) 
	shared[i]=0;

    /* clear the curve states */
    for (i=0; i<3; i++) loc.last_point[i]=0;
    for (i=0; i<3; i++) axis.last_point[i]=0;

    loc.last_point_id = 0;
    axis.last_point_id = 0;

    /* delete all techs */
    techdef * t = all_techs;
    while (t) {
	techdef * dt = t;
	t = t-> next_tech;
	delete dt;
    }
    current_speed = 0;
    current_feed = 0;
    current_tech = 0;
    all_techs = 0;
    is_rapid = 0;

    /* delete all mfuncs */
    mfuncdef * mf = all_mfuncs;
    while (mf) {
	mfuncdef * dt = mf;
	mf = mf-> next_mfunc;
	delete dt;
    }
    current_coolant = 0;
    current_mist = 0;
    current_mfunc = 0;
    all_mfuncs = 0;

    /* delete all tool defs */
    tooldef * tl = all_tools;
    while (tl) {
	tooldef * dt = tl;
	tl = tl-> next_tool;
	delete dt;
    }
    current_tool = 0;
    all_tools = 0;

    ws_count = 0;
    tp_count = 0;
    current_op = 0;
    current_tp = 0;
    current_tpidx = 0;

    plan_ids.empty();
    plan_idxs.empty();
}


// Create a new file and populate it with a machining project,
// stub workpiece, and machining workplan.
//
FILE * STEPNCWrite::Open (
    const char * filename,
    const char * project_name,
    const char * system
    )
{
    if (file) Close();
    file = fopen (filename, "w");
    if (!file) {
	fprintf (stderr, "Can't open output file %s\n", filename);
    }

    /* ------------------------------
     * PART 28 XML FILE HEADER -- This gives some basic information
     * about the file.  The FILE_SCHEMA and implementation_level
     * should not be changed, but you can provide your own values for
     * the other fields if you wish.
     */
    fputs ("<?xml version=\"1.0\" ?>\n", file);
    fputs ("<iso_10303_28_terse\n", file);
    fputs ("  xmlns=\"urn:oid:1.0.10303.238.1.0.1\"\n", file);
    fputs ("  xmlns:exp=\"urn:oid:1.0.10303.28.2.1.1\"\n", file);
    fputs ("  xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n", file);
    fputs ("  schema=\"integrated_cnc_schema\">\n\n", file);


    fputs ("<exp:header>\n", file);
    fputs ("  <exp:name>", file);
    print_string (project_name? project_name: filename);
    fputs ("</exp:name>\n", file);

    fputs ("  <exp:time_stamp>", file);
    print_timestamp();
    fputs ("</exp:time_stamp>\n", file);

    fputs ("  <exp:author></exp:author>\n", file);
    fputs ("  <exp:organization></exp:organization>\n", file);

    fputs ("  <exp:preprocessor_version>", file);
    fputs ("STEPNCWrite Library " STEPNC_WRITE_VERSION, file);
    fputs ("</exp:preprocessor_version>\n", file);

    fputs ("  <exp:originating_system>", file);
    print_string (system? system: "");
    fputs ("</exp:originating_system>\n", file);

    fputs ("</exp:header>\n\n", file);



    /* ------------------------------
     * MACHINING PROJECT -- Create the machining project.  A STEP-NC
     * file only has one and it takes you to the main workplan. 
     */

    /* we are going to write a bunch of objects that may be referenced
     * later, so keep track of the ids we give them
     */
    shared[id_project_product] 	= next_id();
    shared[id_project_pdf] 	= next_id();
    shared[id_project_pd]  	= next_id();
    shared[id_context_product] 	= next_id();
    shared[id_context_pd] 	= next_id();
    shared[id_context_ap] 	= next_id();

    fprintf (file, "<Machining_project id=\"id%u\" Id=\"", 
	     shared[id_project_product]
	);
    print_string (project_name? project_name: "unnamed project");
    fprintf (file, "\" Name=\"\" Frame_of_reference=\"id%u\"/>\n", 
	     shared[id_context_product]
	);

    fprintf (file, "<Product_definition_formation id=\"id%u\" "
	     "Id=\"1.0\" Description=\"\"  Of_product=\"id%u\"/>\n", 
	     shared[id_project_pdf], shared[id_project_product]
	);

    fprintf (file, "<Product_definition id=\"id%u\" "
	     "Id=\"\" Description=\"\" "
	     "Formation=\"id%u\" Frame_of_reference=\"id%u\"/>\n",
	     shared[id_project_pd], shared[id_project_pdf], 
	     shared[id_context_pd]
	);

    fprintf (file, "<Product_context id=\"id%u\" "
	     "Name=\"CNC Machining\"  Frame_of_reference=\"id%u\" "
	     "Discipline_type=\"manufacturing\"/>\n",
	     shared[id_context_product], shared[id_context_ap]
	);

    fprintf (file, "<Product_definition_context id=\"id%u\" "
	     "Name=\"CNC Machining\"  Frame_of_reference=\"id%u\" "
	     "Life_cycle_stage=\"manufacturing\"/>\n",
	     shared[id_context_pd], shared[id_context_ap]
	);

    fprintf (file, "<Application_context id=\"id%u\" "
	     "Application=\"Application protocol for the exchange "
	     "of CNC data\"/>\n",
	     shared[id_context_ap]
	);

    fprintf (file, "<Application_protocol_definition id=\"id%u\" "
	     "Status=\"international standard\" "
	     "Application_interpreted_model_schema_name=\"integrated_cnc_schema\" "
	     "Application_protocol_year=\"2006\" Application=\"id%u\"/>\n",
	     next_id(), shared[id_context_ap]
	);


    /* ------------------------------
     * MAIN WORKPLAN -- Create the main workplan.  This is associated
     * with the machining project so that we know where to start.
     */
    make_workplan ("main workplan");

    return file;
}
	

void STEPNCWrite::Close() 
{
    if (!file) return;

    // close out any toolpath curves and outstanding tools;
    end_move();

    // force a workpiece if we have not written one yet;
    if (!shared[id_workpiece_pd]) PartNo("unknown workpiece");

    print_all_tools();

    fputs ("</iso_10303_28_terse>\n", file);

    fclose (file);

    file = 0;
    Reset();  // clear all of the shared ids too
}



void STEPNCWrite::print_string (const char * str)
{
    if (!file) return;
    if (!str)  return;

    // watch for out of band chars that need to be escaped
    while (*str) {
	char c = *str;
	switch (c) {

	case '"':  fputs("&quot;", file); break;
	case '&':  fputs("&amp;", file);  break;
	case '<':  fputs("&lt;", file);  break;
	case '>':  fputs("&gt;", file); break;
	case '\\': fputs("\\\\", file); break;

	    // to avoid occasional email problems.  For full
	    // explanation, see comments below in print_real()
	    //
	case '.':  fputs("&#46;", file); break;

	case 0x08: fputs ("&#xf0000;", file); break;
	case 0x0b: fputs ("&#xf0001;", file); break;
	case 0x0c: fputs ("&#xf0002;", file); break;
	    
	default:
	    if (c > 0 && c < 0x20) 
		fprintf (file, "&#xf01%02x;", c);
	    else if (c & 0x80) 
		fprintf (file, "&#x%02x;", c & 0xff);
	    else 
		putc (c, file);
	}
	str++;
    }
}


void STEPNCWrite::print_real (double val)
{
    // Originally, we just printed the number with %.15G, but when a
    // file was emailed, occasionally the decimal points of one or two
    // numbers (out of thousands) would be corrupted by the email
    // client. They would come out either doubled (odd, but fixable)
    // or missing (undetectable -- the number just becomes BIG, which
    // could have serious consequences in a toolpath!).
    //
    // The fix is to write the decimal point as &#46; which is the XML
    // full stop entity.  XML parsers will automatically replace it
    // with a dot when reading, so it is transparent to applications.
    //
    // Why would this happen?  When you email an XML file, the client
    // first MIME encodes it, usually as quoted-printable.  Long lines
    // are broken, usually at column 75.  If that line break occurs
    // right before a dot (like the decimal point in a number), then
    // the dot will be the first character of the next line.
    //
    // Unfortunately, when the SMTP protocol (used to transfer all
    // email) was designed, a single dot as the first character of the
    // line was used to indicate the end of message.  Depending on the
    // email client, the dot may be MIME escaped correctly, doubled,
    // or just stripped.  On the other side, the receiving email
    // client reassembles all of the broken lines, and suddenly there
    // may be a number with two dots where previously there was one!
    //
    // Further confusing the issue, some email clients may undouble
    // initial dots, so the problem only appears if the sender and
    // recipient use different email clients.
    //
    // Zipping the file before mailing would also prevent the problem,
    // because it forces the mail to be MIME encoded as binary rather
    // than quoted-printable, but it is impractical to expect people
    // to always do that.  By writing the decimal point as &#46; we
    // avoid the whole problem because there are no longer literal
    // dots (.) in the file.
    // 

    char work[64];
    char final[64];

    register char *src=work;
    register char *dst=final;

    sprintf (work, "%.15G", val);

    /* Replace dot (.) with equivalent XML entity (&#46;) */
    while (*src) {
	if (*src == '.') {
	    src++;
	    *dst++ = '&';
	    *dst++ = '#';
	    *dst++ = '4';
	    *dst++ = '6';
	    *dst++ = ';';
	}
	else *dst++ = *src++;
    }
    *dst = 0;

    fputs (final, file);
}


void STEPNCWrite::print_timestamp() 
{
    time_t nowtime;

    if (!file) return;
    if ( (nowtime = time(0)) == -1) return;

    struct tm * time_struct;
    struct tm local_tm;
    struct tm gm_tm;

    int utc_ofs, utc_days;

    time_struct = localtime (&nowtime);
    local_tm.tm_sec = 	time_struct->tm_sec;
    local_tm.tm_min = 	time_struct->tm_min;
    local_tm.tm_hour = 	time_struct->tm_hour;
    local_tm.tm_mday = 	time_struct->tm_mday;
    local_tm.tm_mon = 	time_struct->tm_mon;
    local_tm.tm_year = 	time_struct->tm_year;
    local_tm.tm_wday = 	time_struct->tm_wday;
    local_tm.tm_yday = 	time_struct->tm_yday;


    time_struct = gmtime (&nowtime);
    gm_tm.tm_sec =	time_struct->tm_sec;
    gm_tm.tm_min =	time_struct->tm_min;
    gm_tm.tm_hour = 	time_struct->tm_hour;
    gm_tm.tm_mday = 	time_struct->tm_mday;
    gm_tm.tm_mon = 	time_struct->tm_mon;
    gm_tm.tm_year = 	time_struct->tm_year;
    gm_tm.tm_wday = 	time_struct->tm_wday;
    gm_tm.tm_yday = 	time_struct->tm_yday;

    /* ----------------------------------------
     * compute UTC offset ==> local - gm 
     */
    /* check hours first */
    utc_ofs = local_tm.tm_hour - gm_tm.tm_hour;

    /* check for wraparound */
    utc_days = local_tm.tm_yday - gm_tm.tm_yday;

    /* local is a day ahead, day/year wrap */
    if ((utc_days == 1) || (utc_days < -1))
	utc_ofs += 24;

    /* local is a day behind, day/year wrap */
    else if ((utc_days == -1) || (utc_days > 1))
	utc_ofs -= 24;

    /* Format the result */
    fprintf (file, "%04d-%02d-%02dT%02d:%02d:%02d%0+3d:00",
	     1900 + local_tm.tm_year,
	     local_tm.tm_mon+1,	/* month is 0-11 */
	     local_tm.tm_mday,
	     local_tm.tm_hour,
	     local_tm.tm_min,
	     local_tm.tm_sec,
	     utc_ofs);
}



void STEPNCWrite::print_select_ref (
    const char * select_name,
    const char * object_type,
    unsigned id
    )
{
    // Unlike Part 21 files, selects with entity refs are not
    // transparent in the file.  XML Part 28 call out selects as a
    // separate element and requires us to declare the type of the
    // thing being referred to.

    fprintf (file, "  <%s>\n", select_name);
    fprintf (file, "\t<%s ref=\"id%u\" xsi:nil=\"true\"/>\n",
	     object_type, id
	);
    fprintf (file, "  </%s>\n", select_name);
}


void STEPNCWrite::print_ref (
    const char * object_type,
    unsigned id
    )
{
    // For use in cases where we have a list of selects and need to
    // write a bunch of these.
    
    fprintf (file, "\t<%s ref=\"id%u\" xsi:nil=\"true\"/>\n",
	     object_type, id
	);
}
