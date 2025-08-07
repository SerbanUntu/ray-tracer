#pragma once

#include <iostream>
#include "util/vec3.h"

enum ViewType {
	ORTOGRAPHIC,
	PERSPECTIVE
};

struct Ray {
	Vec3 origin;
	Vec3 direction;
};

