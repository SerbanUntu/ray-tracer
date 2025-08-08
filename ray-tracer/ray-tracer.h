#pragma once

#include <iostream>
#include "util/vec3.h"

enum ViewType {
	ORTHOGRAPHIC,
	PERSPECTIVE
};

struct Material {
	Vec3 diffuse;
	Vec3 specular;
	int phong_exp;
};

struct Ray {
	Vec3 origin;
	Vec3 direction;
};

struct Pixel {
	int x;
	int y;
};
