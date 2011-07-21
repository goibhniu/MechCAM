/*
 * sketch.h
 *
 *  Created on: Jul 19, 2011
 *      Author: lhousner
 */

#ifndef SKETCH_H_
#define SKETCH_H_

#include "CADObject.h"

class Sketch: public CADObject {
public:
	Sketch();
	virtual ~Sketch();
	std::list<CADObject*> objects;
	std::list<CADObject*>::iterator LoopIt;
};

#endif /* SKETCH_H_ */
