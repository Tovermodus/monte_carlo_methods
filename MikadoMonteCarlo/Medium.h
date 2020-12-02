//
// Created by tovermodus on 12/2/20.
//

#ifndef _MEDIUM_H_
#define _MEDIUM_H_

#include <vector>
#include "Rod.h"
#include "MediumParameters.h"
class Medium {
private:
	std::vector<Rod> rods;
	MediumParameters parameters;
public:
	//rod_density stands for rods per square meter, all units in meters
	Medium(MediumParameters params);
};

#endif //_MEDIUM_H_
