#ifndef PARTICLE_H
#define PARTICLE_H

struct Vec3f {
	float x, y, z;

	Vec3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
};

struct Color {
	float r, g, b;

	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) {}
};

struct Particle {
	Vec3f position; // the x, y and z position in world coordinates
	Vec3f velocity; // the x, y, and z velocities in world coordinates
	Color color; // the red, blue, green values

	Particle(Vec3f& pos, Vec3f& vel) 
		: position(pos), velocity(vel), color(1.0f,1.0f,1.0f) {}

	Particle(Vec3f& pos, Vec3f& vel, Color& col)
		: position(pos), velocity(vel), color(col) {} 
};

#endif
	
