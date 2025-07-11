#include "ray-tracer.h"
#include "color.h"

using namespace std;

const Color Color::RED = Color(255, 0, 0);
const Color Color::GREEN = Color(0, 255, 0);
const Color Color::BLUE = Color(0, 0, 255);

int main()
{
	cout << Color::RED.to_string();
	return 0;
}
