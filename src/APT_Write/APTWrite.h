








#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>









class APTWrite {
protected:
    FILE * file;

    // ------------------------------
    // Part 21 id numbers for things that have already been written.
    // We keep track of these so that we don't clutter the file with
    // multiple copies of things.  A value of zero means that we have
    // not written it yet.  (Zero is not a valid Part 21 id)
    //
    // We just keep an array, indexed by an enum.  Easier to keep
    // properly initialized if people want to track additional ids.
    //


    // ------------------------------
    // PREFERRED UNITS
    // A little local enum to keep track of the units that we should
    // use for different quantities.  The file can specify arbitrary
    // units, but normally you use a group of related ones.
    //
    enum unit_system_type {
        inch_deg_ipm_rpm,
        mm_deg_mmpm_rpm
    };

    unit_system_type unit_system;

    // ------------------------------
    // FEEDS AND SPEEDS
    // A little local struct to keep track of machining technology
    // constructs for feeds and speeds so we can avoid duplication
    // when we switch back and forth between a couple of feeds.
    //
    struct techdef {
        techdef * next_tech;
        unsigned tech_id;
        double feed;
        double speed;
    };
    double current_speed;
    double current_feed;
    techdef * current_tech;
    techdef * all_techs;

    int is_rapid;

    // ------------------------------
    // COOLANT STATE
    // A little local struct to keep track of machine functions
    // constructs for coolant so we can avoid duplication when we
    // switch coolant on and off.  Machine functions has other
    // parameters that we do not use at the moment.
    //
    struct mfuncdef {
        mfuncdef * next_mfunc;
        unsigned mfunc_id;
        int is_coolant;
        int is_mist;
    };
    int current_coolant;
    int current_mist;
    mfuncdef * current_mfunc;
    mfuncdef * all_mfuncs;

    unsigned ws_count;
    unsigned tp_count;

    unsigned current_op;
    unsigned current_tp;
    unsigned current_tpidx;


    // ------------------------------
    // CUTTING TOOL
    // In the file, a tool points to all of the operations that use
    // it, so we must save all the cutter definitions until the end,
    // when we know all of the workingsteps.  Keep little local struct
    // with the parameters and a list of using workingsteps.
    //
    struct tooldef {
        tooldef * next_tool;

        double diameter;
        double corner_radius;
        double corner_radius_horizontal;
        double corner_radius_vertical;
        double tip_outer_angle;
        double taper_angle;
        double height;

        unsigned tool_num;
    };
    tooldef * current_tool;
    tooldef * all_tools;


    // ------------------------------
    // LOCATION CURVE STATE
    // Track the points that make up a polyline and the curves that
    // make up segments so that we can write the basis curves, tool
    // axis curves and surface normal curves.
    //
    struct curvedef {
        double 		last_point[3];
        unsigned 	last_point_id;
    };

    curvedef loc;
    curvedef axis;

public:
    // ------------------------------
    // MAIN PUBLIC INTERFACE
    //
    // Create an instance of the STEPNC writer, then open a file with
    // the Open command.  This will return NULL if the file could not
    // be opened for some reason. Issue a sequence of commands and
    // then Close() the file.  Reset() will also close an open file.
    //
    APTWrite();
    ~APTWrite();

    FILE * Open(
        const char * filename,
        const char * project_name = 0,
        const char * system = 0
        );

    void Close();
    void Reset();

    // ------------------------------
    // PART AND UNIT INFORMATION
    //
    void PartNo (const char * name);

    void Inches() 	{ unit_system = inch_deg_ipm_rpm; }
    void Millimeters() 	{ unit_system = mm_deg_mmpm_rpm; }

    // ------------------------------
    // SPEEDS, FEEDS AND COOLANT
    //
    // Sets the spindle, feedrate, and coolant state.

    void SpindleSpeed (double speed);

    void Rapid();
    void Feedrate (double speed);

    void CoolantOn();
    void CoolantOff();
    void CoolantMist();

    // ------------------------------
    // TOOL DEFINITIONS
    //
    // Defines a cutter, but does not assign it a tool number.
    void Cutter(
        double diameter,
        double corner_radius,
        double corner_radius_horizontal,
        double corner_radius_vertical,
        double tip_outer_angle,
        double taper_angle,
        double height
        );

    // Loads a particular tool number.  Assigns the tool number to the
    // last defined cutter if the tool number does not already exist.
    //
    void LoadTool (unsigned num);

    // Assigns tool number to last defined cutter
    void SELCTLTool (unsigned num);


    // ------------------------------
    // MOTION COMMANDS
    //
    // Linear move to a given location.   Tool axis is (0,0,1)
    void GoToXYZ(
        const char * label,
        double x,
        double y,
        double z
        );

    // Linear move to a given location and tool axis.
    void GoToXYZ_IJK(
        const char * label,
        double x,
        double y,
        double z,
        double i,
        double j,
        double k
        );

    // Arc from the current location to a new location, given a
    // center, radius and clockwise/ccw direction.
    //
    void Arc(
        const char * label,
        double new_x,
        double new_y,
        double new_z,
        double cx,
        double cy,
        double cz,
        double radius,
        int ccw
        );

    // Set direction components used to establish the plane that Arc
    // commands use.
    void SetDirection(
        double zi, double zj, double zk, 	// z axis
        double xa, double xb, double xc		// x axis
        );


    // ------------------------------
    // FINE TUNING OF PROGRAM STRUCTURE
    //
    // By default, a STEP-NC file only has a single workplan, and
    // workingsteps are created as needed to support the requested
    // motions and speeds.  The following functions force additional
    // workingsteps or nested workplans.
    //
    // Ending the top level workplan will close the file.
    //
    void NestWorkplan (const char * name);
    void EndWorkplan();

    void Workingstep (const char * name);


    // Various NC functions
    void Connector();
    void GoHome (const char * label);
    void Stop (const char * label);

    void PPrint(
        const char * label,
        const char * message
        );


    // Unused, from earlier interfaces
    void MultaxOn()	{ /* not needed, automatically handled */ }
    void MultaxOff()	{ /* not needed, automatically handled */ }

protected:
    // ------------------------------
    // Below this line are utility functions used to write specific
    // STEP-NC data entities.  The functions use the make_ prefix and
    // may either write new data objects to the file or return the id
    // of a shared, previously-written object.
    //
    //unsigned next_id() 		{ return ++current_id; }

    void print_string (const char * str);
    void print_real (double val);
    void print_timestamp();
    void print_all_tools();
    void print_select_ref (
        const char * select_name,
        const char * object_type,
        unsigned id
        );

    void print_ref (
        const char * object_type,
        unsigned id
        );

    unsigned make_workplan (const char * label);
    void end_workplan();
    void add_to_workplan (unsigned id);

    unsigned make_workingstep (const char * label);
    void end_workingstep();
    void add_to_workingstep (unsigned id);

    unsigned make_toolpath (const char * label);
    unsigned make_toolpath_feature();

    void start_move(const char * label);
    void end_move();

    unsigned make_technology();
    unsigned make_mfunc();

    unsigned make_point (
        const char * label,
        double x,
        double y,
        double z
        );

    unsigned make_curve (
        const char * label,
        curvedef &def
        );

    unsigned make_polyline (
        const char * label

        );

    unsigned make_composite_curve (
        const char * label
        );

    unsigned make_string_rep (const char * name);
    unsigned make_action_property (
        const char * name,
        unsigned am,
        unsigned rep,
        const char * object_type
        );

    unsigned make_simple_context();
    unsigned make_geometry_context();

    // --------------------
    // Units and measures
    unsigned make_length_measure(
        const char * label,
        double val
        );
    unsigned make_length_unit();
    unsigned make_inch_unit();
    unsigned make_mm_unit();

    unsigned make_angle_measure(
        const char * label,
        double val
        );
    unsigned make_angle_unit();
    unsigned make_radian_unit();
    unsigned make_degree_unit();

    unsigned make_steradian_unit();

    unsigned make_second_unit();
    unsigned make_minute_unit();

    unsigned make_feed_measure(
        const char * label,
        double val
        );
    unsigned make_feed_unit();
    unsigned make_mmpm_unit();
    unsigned make_ipm_unit();

    unsigned make_spindle_measure(
        const char * label,
        double val
        );
    unsigned make_spindle_unit();
    unsigned make_rps_unit();
    unsigned make_rpm_unit();
};
