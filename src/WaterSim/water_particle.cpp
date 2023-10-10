#include "water_particle.hpp"

#include "cgra/cgra_geometry.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;
using namespace cgra;

void Particle::simulate(float dt){
    
}

void Particle::draw(const mat4 &view, const mat4 &proj, material &material){
    mat4 modelview = view;
    modelview = translate(modelview, position);
    material.load(modelview, proj);
	drawSphere();
}

void drawSphere() {
    const float vert[] = {
        0, 0, 1, 0, 0, 1, 0, 0,
        0, 0, 1, 0, 0, 1, 0.05, 0,
        0, 0, 1, 0, 0, 1, 0.1, 0,
        0, 0, 1, 0, 0, 1, 0.15, 0,
        0, 0, 1, 0, 0, 1, 0.2, 0,
        -0, 0, 1, -0, 0, 1, 0.25, 0,
        -0, 0, 1, -0, 0, 1, 0.3, 0,
        -0, 0, 1, -0, 0, 1, 0.35, 0,
        -0, 0, 1, -0, 0, 1, 0.4, 0,
        -0, 0, 1, -0, 0, 1, 0.45, 0,
        -0, -0, 1, -0, -0, 1, 0.5, 0,
        -0, -0, 1, -0, -0, 1, 0.55, 0,
        -0, -0, 1, -0, -0, 1, 0.6, 0,
        -0, -0, 1, -0, -0, 1, 0.65, 0,
        -0, -0, 1, -0, -0, 1, 0.7, 0,
        0, -0, 1, 0, -0, 1, 0.75, 0,
        0, -0, 1, 0, -0, 1, 0.8, 0,
        0, -0, 1, 0, -0, 1, 0.85, 0,
        0, -0, 1, 0, -0, 1, 0.9, 0,
        0, -0, 1, 0, -0, 1, 0.95, 0,
        0, 0, 1, 0, 0, 1, 1, 0,
        0.309017, 0, 0.951057, 0.309017, 0, 0.951057, 0, 0.1,
        0.293893, 0.0954915, 0.951057, 0.293893, 0.0954915, 0.951057, 0.05, 0.1,
        0.25, 0.181636, 0.951057, 0.25, 0.181636, 0.951057, 0.1, 0.1,
        0.181636, 0.25, 0.951057, 0.181636, 0.25, 0.951057, 0.15, 0.1,
        0.0954915, 0.293893, 0.951057, 0.0954915, 0.293893, 0.951057, 0.2, 0.1,
        0, 0.309017, 0.951057, 0, 0.309017, 0.951057, 0.25, 0.1,
        -0.0954915, 0.293893, 0.951057, -0.0954915, 0.293893, 0.951057, 0.3, 0.1,
        -0.181636, 0.25, 0.951057, -0.181636, 0.25, 0.951057, 0.35, 0.1,
        -0.25, 0.181636, 0.951057, -0.25, 0.181636, 0.951057, 0.4, 0.1,
        -0.293893, 0.0954915, 0.951057, -0.293893, 0.0954915, 0.951057, 0.45, 0.1,
        -0.309017, 0, 0.951057, -0.309017, 0, 0.951057, 0.5, 0.1,
        -0.293893, -0.0954916, 0.951057, -0.293893, -0.0954916, 0.951057, 0.55, 0.1,
        -0.25, -0.181636, 0.951057, -0.25, -0.181636, 0.951057, 0.6, 0.1,
        -0.181636, -0.25, 0.951057, -0.181636, -0.25, 0.951057, 0.65, 0.1,
        -0.0954914, -0.293893, 0.951057, -0.0954914, -0.293893, 0.951057, 0.7, 0.1,
        0, -0.309017, 0.951057, 0, -0.309017, 0.951057, 0.75, 0.1,
        0.0954915, -0.293893, 0.951057, 0.0954915, -0.293893, 0.951057, 0.8, 0.1,
        0.181636, -0.25, 0.951057, 0.181636, -0.25, 0.951057, 0.85, 0.1,
        0.25, -0.181636, 0.951057, 0.25, -0.181636, 0.951057, 0.9, 0.1,
        0.293893, -0.0954915, 0.951057, 0.293893, -0.0954915, 0.951057, 0.95, 0.1,
        0.309017, 0, 0.951057, 0.309017, 0, 0.951057, 1, 0.1,
        0.587785, 0, 0.809017, 0.587785, 0, 0.809017, 0, 0.2,
        0.559017, 0.181636, 0.809017, 0.559017, 0.181636, 0.809017, 0.05, 0.2,
        0.475528, 0.345491, 0.809017, 0.475528, 0.345491, 0.809017, 0.1, 0.2,
        0.345491, 0.475528, 0.809017, 0.345491, 0.475528, 0.809017, 0.15, 0.2,
        0.181636, 0.559017, 0.809017, 0.181636, 0.559017, 0.809017, 0.2, 0.2,
        0, 0.587785, 0.809017, 0, 0.587785, 0.809017, 0.25, 0.2,
        -0.181636, 0.559017, 0.809017, -0.181636, 0.559017, 0.809017, 0.3, 0.2,
        -0.345492, 0.475528, 0.809017, -0.345492, 0.475528, 0.809017, 0.35, 0.2,
        -0.475528, 0.345491, 0.809017, -0.475528, 0.345491, 0.809017, 0.4, 0.2,
        -0.559017, 0.181636, 0.809017, -0.559017, 0.181636, 0.809017, 0.45, 0.2,
        -0.587785, 0, 0.809017, -0.587785, 0, 0.809017, 0.5, 0.2,
        -0.559017, -0.181636, 0.809017, -0.559017, -0.181636, 0.809017, 0.55, 0.2,
        -0.475528, -0.345492, 0.809017, -0.475528, -0.345492, 0.809017, 0.6, 0.2,
        -0.345491, -0.475528, 0.809017, -0.345491, -0.475528, 0.809017, 0.65, 0.2,
        -0.181635, -0.559017, 0.809017, -0.181635, -0.559017, 0.809017, 0.7, 0.2,
        0, -0.587785, 0.809017, 0, -0.587785, 0.809017, 0.75, 0.2,
        0.181636, -0.559017, 0.809017, 0.181636, -0.559017, 0.809017, 0.8, 0.2,
        0.345492, -0.475528, 0.809017, 0.345492, -0.475528, 0.809017, 0.85, 0.2,
        0.475528, -0.345492, 0.809017, 0.475528, -0.345492, 0.809017, 0.9, 0.2,
        0.559017, -0.181636, 0.809017, 0.559017, -0.181636, 0.809017, 0.95, 0.2,
        0.587785, 0, 0.809017, 0.587785, 0, 0.809017, 1, 0.2,
        0.809017, 0, 0.587785, 0.809017, 0, 0.587785, 0, 0.3,
        0.769421, 0.25, 0.587785, 0.769421, 0.25, 0.587785, 0.05, 0.3,
        0.654509, 0.475528, 0.587785, 0.654509, 0.475528, 0.587785, 0.1, 0.3,
        0.475528, 0.654509, 0.587785, 0.475528, 0.654509, 0.587785, 0.15, 0.3,
        0.25, 0.769421, 0.587785, 0.25, 0.769421, 0.587785, 0.2, 0.3,
        0, 0.809017, 0.587785, 0, 0.809017, 0.587785, 0.25, 0.3,
        -0.25, 0.769421, 0.587785, -0.25, 0.769421, 0.587785, 0.3, 0.3,
        -0.475528, 0.654508, 0.587785, -0.475528, 0.654508, 0.587785, 0.35, 0.3,
        -0.654509, 0.475528, 0.587785, -0.654509, 0.475528, 0.587785, 0.4, 0.3,
        -0.769421, 0.25, 0.587785, -0.769421, 0.25, 0.587785, 0.45, 0.3,
        -0.809017, 0, 0.587785, -0.809017, 0, 0.587785, 0.5, 0.3,
        -0.769421, -0.25, 0.587785, -0.769421, -0.25, 0.587785, 0.55, 0.3,
        -0.654508, -0.475528, 0.587785, -0.654508, -0.475528, 0.587785, 0.6, 0.3,
        -0.475528, -0.654509, 0.587785, -0.475528, -0.654509, 0.587785, 0.65, 0.3,
        -0.25, -0.769421, 0.587785, -0.25, -0.769421, 0.587785, 0.7, 0.3,
        0, -0.809017, 0.587785, 0, -0.809017, 0.587785, 0.75, 0.3,
        0.25, -0.769421, 0.587785, 0.25, -0.769421, 0.587785, 0.8, 0.3,
        0.475528, -0.654508, 0.587785, 0.475528, -0.654508, 0.587785, 0.85, 0.3,
        0.654508, -0.475528, 0.587785, 0.654508, -0.475528, 0.587785, 0.9, 0.3,
        0.769421, -0.25, 0.587785, 0.769421, -0.25, 0.587785, 0.95, 0.3,
        0.809017, 0, 0.587785, 0.809017, 0, 0.587785, 1, 0.3,
        0.951057, 0, 0.309017, 0.951057, 0, 0.309017, 0, 0.4,
        0.904509, 0.293893, 0.309017, 0.904509, 0.293893, 0.309017, 0.05, 0.4,
        0.769421, 0.559017, 0.309017, 0.769421, 0.559017, 0.309017, 0.1, 0.4,
        0.559017, 0.769421, 0.309017, 0.559017, 0.769421, 0.309017, 0.15, 0.4,
        0.293893, 0.904509, 0.309017, 0.293893, 0.904509, 0.309017, 0.2, 0.4,
        0, 0.951057, 0.309017, 0, 0.951057, 0.309017, 0.25, 0.4,
        -0.293893, 0.904508, 0.309017, -0.293893, 0.904508, 0.309017, 0.3, 0.4,
        -0.559017, 0.769421, 0.309017, -0.559017, 0.769421, 0.309017, 0.35, 0.4,
        -0.769421, 0.559017, 0.309017, -0.769421, 0.559017, 0.309017, 0.4, 0.4,
        -0.904508, 0.293893, 0.309017, -0.904508, 0.293893, 0.309017, 0.45, 0.4,
        -0.951057, 0, 0.309017, -0.951057, 0, 0.309017, 0.5, 0.4,
        -0.904508, -0.293893, 0.309017, -0.904508, -0.293893, 0.309017, 0.55, 0.4,
        -0.769421, -0.559017, 0.309017, -0.769421, -0.559017, 0.309017, 0.6, 0.4,
        -0.559017, -0.769421, 0.309017, -0.559017, -0.769421, 0.309017, 0.65, 0.4,
        -0.293892, -0.904509, 0.309017, -0.293892, -0.904509, 0.309017, 0.7, 0.4,
        0, -0.951057, 0.309017, 0, -0.951057, 0.309017, 0.75, 0.4,
        0.293893, -0.904508, 0.309017, 0.293893, -0.904508, 0.309017, 0.8, 0.4,
        0.559017, -0.769421, 0.309017, 0.559017, -0.769421, 0.309017, 0.85, 0.4,
        0.769421, -0.559017, 0.309017, 0.769421, -0.559017, 0.309017, 0.9, 0.4,
        0.904509, -0.293893, 0.309017, 0.904509, -0.293893, 0.309017, 0.95, 0.4,
        0.951057, 0, 0.309017, 0.951057, 0, 0.309017, 1, 0.4,
        1, 0, 0, 1, 0, 0, 0, 0.5,
        0.951057, 0.309017, 0, 0.951057, 0.309017, 0, 0.05, 0.5,
        0.809017, 0.587785, 0, 0.809017, 0.587785, 0, 0.1, 0.5,
        0.587785, 0.809017, 0, 0.587785, 0.809017, 0, 0.15, 0.5,
        0.309017, 0.951057, 0, 0.309017, 0.951057, 0, 0.2, 0.5,
        0, 1, 0, 0, 1, 0, 0.25, 0.5,
        -0.309017, 0.951056, 0, -0.309017, 0.951056, 0, 0.3, 0.5,
        -0.587785, 0.809017, 0, -0.587785, 0.809017, 0, 0.35, 0.5,
        -0.809017, 0.587785, 0, -0.809017, 0.587785, 0, 0.4, 0.5,
        -0.951056, 0.309017, 0, -0.951056, 0.309017, 0, 0.45, 0.5,
        -1, 0, 0, -1, 0, 0, 0.5, 0.5,
        -0.951056, -0.309017, 0, -0.951056, -0.309017, 0, 0.55, 0.5,
        -0.809017, -0.587785, 0, -0.809017, -0.587785, 0, 0.6, 0.5,
        -0.587785, -0.809017, 0, -0.587785, -0.809017, 0, 0.65, 0.5,
        -0.309017, -0.951057, 0, -0.309017, -0.951057, 0, 0.7, 0.5,
        0, -1, 0, 0, -1, 0, 0.75, 0.5,
        0.309017, -0.951056, 0, 0.309017, -0.951056, 0, 0.8, 0.5,
        0.587785, -0.809017, 0, 0.587785, -0.809017, 0, 0.85, 0.5,
        0.809017, -0.587785, 0, 0.809017, -0.587785, 0, 0.9, 0.5,
        0.951057, -0.309017, 0, 0.951057, -0.309017, 0, 0.95, 0.5,
        1, 0, 0, 1, 0, 0, 1, 0.5,
        0.951056, 0, -0.309017, 0.951056, 0, -0.309017, 0, 0.6,
        0.904508, 0.293893, -0.309017, 0.904508, 0.293893, -0.309017, 0.05, 0.6,
        0.769421, 0.559017, -0.309017, 0.769421, 0.559017, -0.309017, 0.1, 0.6,
        0.559017, 0.769421, -0.309017, 0.559017, 0.769421, -0.309017, 0.15, 0.6,
        0.293893, 0.904508, -0.309017, 0.293893, 0.904508, -0.309017, 0.2, 0.6,
        0, 0.951056, -0.309017, 0, 0.951056, -0.309017, 0.25, 0.6,
        -0.293893, 0.904508, -0.309017, -0.293893, 0.904508, -0.309017, 0.3, 0.6,
        -0.559017, 0.769421, -0.309017, -0.559017, 0.769421, -0.309017, 0.35, 0.6,
        -0.769421, 0.559017, -0.309017, -0.769421, 0.559017, -0.309017, 0.4, 0.6,
        -0.904508, 0.293893, -0.309017, -0.904508, 0.293893, -0.309017, 0.45, 0.6,
        -0.951056, 0, -0.309017, -0.951056, 0, -0.309017, 0.5, 0.6,
        -0.904508, -0.293893, -0.309017, -0.904508, -0.293893, -0.309017, 0.55, 0.6,
        -0.769421, -0.559017, -0.309017, -0.769421, -0.559017, -0.309017, 0.6, 0.6,
        -0.559017, -0.769421, -0.309017, -0.559017, -0.769421, -0.309017, 0.65, 0.6,
        -0.293892, -0.904509, -0.309017, -0.293892, -0.904509, -0.309017, 0.7, 0.6,
        0, -0.951056, -0.309017, 0, -0.951056, -0.309017, 0.75, 0.6,
        0.293893, -0.904508, -0.309017, 0.293893, -0.904508, -0.309017, 0.8, 0.6,
        0.559017, -0.769421, -0.309017, 0.559017, -0.769421, -0.309017, 0.85, 0.6,
        0.769421, -0.559017, -0.309017, 0.769421, -0.559017, -0.309017, 0.9, 0.6,
        0.904508, -0.293893, -0.309017, 0.904508, -0.293893, -0.309017, 0.95, 0.6,
        0.951056, 0, -0.309017, 0.951056, 0, -0.309017, 1, 0.6,
        0.809017, 0, -0.587785, 0.809017, 0, -0.587785, 0, 0.7,
        0.769421, 0.25, -0.587785, 0.769421, 0.25, -0.587785, 0.05, 0.7,
        0.654508, 0.475528, -0.587785, 0.654508, 0.475528, -0.587785, 0.1, 0.7,
        0.475528, 0.654508, -0.587785, 0.475528, 0.654508, -0.587785, 0.15, 0.7,
        0.25, 0.769421, -0.587785, 0.25, 0.769421, -0.587785, 0.2, 0.7,
        0, 0.809017, -0.587785, 0, 0.809017, -0.587785, 0.25, 0.7,
        -0.25, 0.769421, -0.587785, -0.25, 0.769421, -0.587785, 0.3, 0.7,
        -0.475528, 0.654508, -0.587785, -0.475528, 0.654508, -0.587785, 0.35, 0.7,
        -0.654508, 0.475528, -0.587785, -0.654508, 0.475528, -0.587785, 0.4, 0.7,
        -0.769421, 0.25, -0.587785, -0.769421, 0.25, -0.587785, 0.45, 0.7,
        -0.809017, 0, -0.587785, -0.809017, 0, -0.587785, 0.5, 0.7,
        -0.769421, -0.25, -0.587785, -0.769421, -0.25, -0.587785, 0.55, 0.7,
        -0.654508, -0.475528, -0.587785, -0.654508, -0.475528, -0.587785, 0.6, 0.7,
        -0.475528, -0.654509, -0.587785, -0.475528, -0.654509, -0.587785, 0.65, 0.7,
        -0.25, -0.769421, -0.587785, -0.25, -0.769421, -0.587785, 0.7, 0.7,
        0, -0.809017, -0.587785, 0, -0.809017, -0.587785, 0.75, 0.7,
        0.25, -0.769421, -0.587785, 0.25, -0.769421, -0.587785, 0.8, 0.7,
        0.475528, -0.654508, -0.587785, 0.475528, -0.654508, -0.587785, 0.85, 0.7,
        0.654508, -0.475528, -0.587785, 0.654508, -0.475528, -0.587785, 0.9, 0.7,
        0.769421, -0.25, -0.587785, 0.769421, -0.25, -0.587785, 0.95, 0.7,
        0.809017, 0, -0.587785, 0.809017, 0, -0.587785, 1, 0.7,
        0.587785, 0, -0.809017, 0.587785, 0, -0.809017, 0, 0.8,
        0.559017, 0.181636, -0.809017, 0.559017, 0.181636, -0.809017, 0.05, 0.8,
        0.475528, 0.345491, -0.809017, 0.475528, 0.345491, -0.809017, 0.1, 0.8,
        0.345491, 0.475528, -0.809017, 0.345491, 0.475528, -0.809017, 0.15, 0.8,
        0.181636, 0.559017, -0.809017, 0.181636, 0.559017, -0.809017, 0.2, 0.8,
        0, 0.587785, -0.809017, 0, 0.587785, -0.809017, 0.25, 0.8,
        -0.181636, 0.559017, -0.809017, -0.181636, 0.559017, -0.809017, 0.3, 0.8,
        -0.345492, 0.475528, -0.809017, -0.345492, 0.475528, -0.809017, 0.35, 0.8,
        -0.475528, 0.345491, -0.809017, -0.475528, 0.345491, -0.809017, 0.4, 0.8,
        -0.559017, 0.181636, -0.809017, -0.559017, 0.181636, -0.809017, 0.45, 0.8,
        -0.587785, 0, -0.809017, -0.587785, 0, -0.809017, 0.5, 0.8,
        -0.559017, -0.181636, -0.809017, -0.559017, -0.181636, -0.809017, 0.55, 0.8,
        -0.475528, -0.345492, -0.809017, -0.475528, -0.345492, -0.809017, 0.6, 0.8,
        -0.345491, -0.475528, -0.809017, -0.345491, -0.475528, -0.809017, 0.65, 0.8,
        -0.181635, -0.559017, -0.809017, -0.181635, -0.559017, -0.809017, 0.7, 0.8,
        0, -0.587785, -0.809017, 0, -0.587785, -0.809017, 0.75, 0.8,
        0.181636, -0.559017, -0.809017, 0.181636, -0.559017, -0.809017, 0.8, 0.8,
        0.345492, -0.475528, -0.809017, 0.345492, -0.475528, -0.809017, 0.85, 0.8,
        0.475528, -0.345491, -0.809017, 0.475528, -0.345491, -0.809017, 0.9, 0.8,
        0.559017, -0.181636, -0.809017, 0.559017, -0.181636, -0.809017, 0.95, 0.8,
        0.587785, 0, -0.809017, 0.587785, 0, -0.809017, 1, 0.8,
        0.309017, 0, -0.951056, 0.309017, 0, -0.951056, 0, 0.9,
        0.293893, 0.0954915, -0.951056, 0.293893, 0.0954915, -0.951056, 0.05, 0.9,
        0.25, 0.181636, -0.951056, 0.25, 0.181636, -0.951056, 0.1, 0.9,
        0.181636, 0.25, -0.951056, 0.181636, 0.25, -0.951056, 0.15, 0.9,
        0.0954915, 0.293893, -0.951056, 0.0954915, 0.293893, -0.951056, 0.2, 0.9,
        0, 0.309017, -0.951056, 0, 0.309017, -0.951056, 0.25, 0.9,
        -0.0954915, 0.293893, -0.951056, -0.0954915, 0.293893, -0.951056, 0.3, 0.9,
        -0.181636, 0.25, -0.951056, -0.181636, 0.25, -0.951056, 0.35, 0.9,
        -0.25, 0.181636, -0.951056, -0.25, 0.181636, -0.951056, 0.4, 0.9,
        -0.293893, 0.0954915, -0.951056, -0.293893, 0.0954915, -0.951056, 0.45, 0.9,
        -0.309017, 0, -0.951056, -0.309017, 0, -0.951056, 0.5, 0.9,
        -0.293893, -0.0954916, -0.951056, -0.293893, -0.0954916, -0.951056, 0.55, 0.9,
        -0.25, -0.181636, -0.951056, -0.25, -0.181636, -0.951056, 0.6, 0.9,
        -0.181636, -0.25, -0.951056, -0.181636, -0.25, -0.951056, 0.65, 0.9,
        -0.0954914, -0.293893, -0.951056, -0.0954914, -0.293893, -0.951056, 0.7, 0.9,
        0, -0.309017, -0.951056, 0, -0.309017, -0.951056, 0.75, 0.9,
        0.0954916, -0.293893, -0.951056, 0.0954916, -0.293893, -0.951056, 0.8, 0.9,
        0.181636, -0.25, -0.951056, 0.181636, -0.25, -0.951056, 0.85, 0.9,
        0.25, -0.181636, -0.951056, 0.25, -0.181636, -0.951056, 0.9, 0.9,
        0.293893, -0.0954915, -0.951056, 0.293893, -0.0954915, -0.951056, 0.95, 0.9,
        0.309017, 0, -0.951056, 0.309017, 0, -0.951056, 1, 0.9,
        0, -0, -1, 0, -0, -1, 0, 1,
        0, 0, -1, 0, 0, -1, 0.05, 1,
        0, 0, -1, 0, 0, -1, 0.1, 1,
        0, 0, -1, 0, 0, -1, 0.15, 1,
        0, 0, -1, 0, 0, -1, 0.2, 1,
        0, 0, -1, 0, 0, -1, 0.25, 1,
        0, 0, -1, 0, 0, -1, 0.3, 1,
        0, 0, -1, 0, 0, -1, 0.35, 1,
        0, 0, -1, 0, 0, -1, 0.4, 1,
        0, 0, -1, 0, 0, -1, 0.45, 1,
        0, 0, -1, 0, 0, -1, 0.5, 1,
        0, 0, -1, 0, 0, -1, 0.55, 1,
        0, 0, -1, 0, 0, -1, 0.6, 1,
        0, 0, -1, 0, 0, -1, 0.65, 1,
        0, 0, -1, 0, 0, -1, 0.7, 1,
        0, 0, -1, 0, 0, -1, 0.75, 1,
        0, 0, -1, 0, 0, -1, 0.8, 1,
        0, 0, -1, 0, 0, -1, 0.85, 1,
        0, 0, -1, 0, 0, -1, 0.9, 1,
        0, 0, -1, 0, 0, -1, 0.95, 1,
        0, 0, -1, 0, 0, -1, 1, 1
    };

    const unsigned int idx[] = {
        0, 21, 22, 0, 22, 1, 1, 22, 23, 1,
        23, 2, 2, 23, 24, 2, 24, 3, 3, 24,
        25, 3, 25, 4, 4, 25, 26, 4, 26, 5,
        5, 26, 27, 5, 27, 6, 6, 27, 28, 6,
        28, 7, 7, 28, 29, 7, 29, 8, 8, 29,
        30, 8, 30, 9, 9, 30, 31, 9, 31, 10,
        10, 31, 32, 10, 32, 11, 11, 32, 33, 11,
        33, 12, 12, 33, 34, 12, 34, 13, 13, 34,
        35, 13, 35, 14, 14, 35, 36, 14, 36, 15,
        15, 36, 37, 15, 37, 16, 16, 37, 38, 16,
        38, 17, 17, 38, 39, 17, 39, 18, 18, 39,
        40, 18, 40, 19, 19, 40, 41, 19, 41, 20,
        21, 42, 43, 21, 43, 22, 22, 43, 44, 22,
        44, 23, 23, 44, 45, 23, 45, 24, 24, 45,
        46, 24, 46, 25, 25, 46, 47, 25, 47, 26,
        26, 47, 48, 26, 48, 27, 27, 48, 49, 27,
        49, 28, 28, 49, 50, 28, 50, 29, 29, 50,
        51, 29, 51, 30, 30, 51, 52, 30, 52, 31,
        31, 52, 53, 31, 53, 32, 32, 53, 54, 32,
        54, 33, 33, 54, 55, 33, 55, 34, 34, 55,
        56, 34, 56, 35, 35, 56, 57, 35, 57, 36,
        36, 57, 58, 36, 58, 37, 37, 58, 59, 37,
        59, 38, 38, 59, 60, 38, 60, 39, 39, 60,
        61, 39, 61, 40, 40, 61, 62, 40, 62, 41,
        42, 63, 64, 42, 64, 43, 43, 64, 65, 43,
        65, 44, 44, 65, 66, 44, 66, 45, 45, 66,
        67, 45, 67, 46, 46, 67, 68, 46, 68, 47,
        47, 68, 69, 47, 69, 48, 48, 69, 70, 48,
        70, 49, 49, 70, 71, 49, 71, 50, 50, 71,
        72, 50, 72, 51, 51, 72, 73, 51, 73, 52,
        52, 73, 74, 52, 74, 53, 53, 74, 75, 53,
        75, 54, 54, 75, 76, 54, 76, 55, 55, 76,
        77, 55, 77, 56, 56, 77, 78, 56, 78, 57,
        57, 78, 79, 57, 79, 58, 58, 79, 80, 58,
        80, 59, 59, 80, 81, 59, 81, 60, 60, 81,
        82, 60, 82, 61, 61, 82, 83, 61, 83, 62,
        63, 84, 85, 63, 85, 64, 64, 85, 86, 64,
        86, 65, 65, 86, 87, 65, 87, 66, 66, 87,
        88, 66, 88, 67, 67, 88, 89, 67, 89, 68,
        68, 89, 90, 68, 90, 69, 69, 90, 91, 69,
        91, 70, 70, 91, 92, 70, 92, 71, 71, 92,
        93, 71, 93, 72, 72, 93, 94, 72, 94, 73,
        73, 94, 95, 73, 95, 74, 74, 95, 96, 74,
        96, 75, 75, 96, 97, 75, 97, 76, 76, 97,
        98, 76, 98, 77, 77, 98, 99, 77, 99, 78,
        78, 99, 100, 78, 100, 79, 79, 100, 101, 79,
        101, 80, 80, 101, 102, 80, 102, 81, 81, 102,
        103, 81, 103, 82, 82, 103, 104, 82, 104, 83,
        84, 105, 106, 84, 106, 85, 85, 106, 107, 85,
        107, 86, 86, 107, 108, 86, 108, 87, 87, 108,
        109, 87, 109, 88, 88, 109, 110, 88, 110, 89,
        89, 110, 111, 89, 111, 90, 90, 111, 112, 90,
        112, 91, 91, 112, 113, 91, 113, 92, 92, 113,
        114, 92, 114, 93, 93, 114, 115, 93, 115, 94,
        94, 115, 116, 94, 116, 95, 95, 116, 117, 95,
        117, 96, 96, 117, 118, 96, 118, 97, 97, 118,
        119, 97, 119, 98, 98, 119, 120, 98, 120, 99,
        99, 120, 121, 99, 121, 100, 100, 121, 122, 100,
        122, 101, 101, 122, 123, 101, 123, 102, 102, 123,
        124, 102, 124, 103, 103, 124, 125, 103, 125, 104,
        105, 126, 127, 105, 127, 106, 106, 127, 128, 106,
        128, 107, 107, 128, 129, 107, 129, 108, 108, 129,
        130, 108, 130, 109, 109, 130, 131, 109, 131, 110,
        110, 131, 132, 110, 132, 111, 111, 132, 133, 111,
        133, 112, 112, 133, 134, 112, 134, 113, 113, 134,
        135, 113, 135, 114, 114, 135, 136, 114, 136, 115,
        115, 136, 137, 115, 137, 116, 116, 137, 138, 116,
        138, 117, 117, 138, 139, 117, 139, 118, 118, 139,
        140, 118, 140, 119, 119, 140, 141, 119, 141, 120,
        120, 141, 142, 120, 142, 121, 121, 142, 143, 121,
        143, 122, 122, 143, 144, 122, 144, 123, 123, 144,
        145, 123, 145, 124, 124, 145, 146, 124, 146, 125,
        126, 147, 148, 126, 148, 127, 127, 148, 149, 127,
        149, 128, 128, 149, 150, 128, 150, 129, 129, 150,
        151, 129, 151, 130, 130, 151, 152, 130, 152, 131,
        131, 152, 153, 131, 153, 132, 132, 153, 154, 132,
        154, 133, 133, 154, 155, 133, 155, 134, 134, 155,
        156, 134, 156, 135, 135, 156, 157, 135, 157, 136,
        136, 157, 158, 136, 158, 137, 137, 158, 159, 137,
        159, 138, 138, 159, 160, 138, 160, 139, 139, 160,
        161, 139, 161, 140, 140, 161, 162, 140, 162, 141,
        141, 162, 163, 141, 163, 142, 142, 163, 164, 142,
        164, 143, 143, 164, 165, 143, 165, 144, 144, 165,
        166, 144, 166, 145, 145, 166, 167, 145, 167, 146,
        147, 168, 169, 147, 169, 148, 148, 169, 170, 148,
        170, 149, 149, 170, 171, 149, 171, 150, 150, 171,
        172, 150, 172, 151, 151, 172, 173, 151, 173, 152,
        152, 173, 174, 152, 174, 153, 153, 174, 175, 153,
        175, 154, 154, 175, 176, 154, 176, 155, 155, 176,
        177, 155, 177, 156, 156, 177, 178, 156, 178, 157,
        157, 178, 179, 157, 179, 158, 158, 179, 180, 158,
        180, 159, 159, 180, 181, 159, 181, 160, 160, 181,
        182, 160, 182, 161, 161, 182, 183, 161, 183, 162,
        162, 183, 184, 162, 184, 163, 163, 184, 185, 163,
        185, 164, 164, 185, 186, 164, 186, 165, 165, 186,
        187, 165, 187, 166, 166, 187, 188, 166, 188, 167,
        168, 189, 190, 168, 190, 169, 169, 190, 191, 169,
        191, 170, 170, 191, 192, 170, 192, 171, 171, 192,
        193, 171, 193, 172, 172, 193, 194, 172, 194, 173,
        173, 194, 195, 173, 195, 174, 174, 195, 196, 174,
        196, 175, 175, 196, 197, 175, 197, 176, 176, 197,
        198, 176, 198, 177, 177, 198, 199, 177, 199, 178,
        178, 199, 200, 178, 200, 179, 179, 200, 201, 179,
        201, 180, 180, 201, 202, 180, 202, 181, 181, 202,
        203, 181, 203, 182, 182, 203, 204, 182, 204, 183,
        183, 204, 205, 183, 205, 184, 184, 205, 206, 184,
        206, 185, 185, 206, 207, 185, 207, 186, 186, 207,
        208, 186, 208, 187, 187, 208, 209, 187, 209, 188,
        189, 210, 211, 189, 211, 190, 190, 211, 212, 190,
        212, 191, 191, 212, 213, 191, 213, 192, 192, 213,
        214, 192, 214, 193, 193, 214, 215, 193, 215, 194,
        194, 215, 216, 194, 216, 195, 195, 216, 217, 195,
        217, 196, 196, 217, 218, 196, 218, 197, 197, 218,
        219, 197, 219, 198, 198, 219, 220, 198, 220, 199,
        199, 220, 221, 199, 221, 200, 200, 221, 222, 200,
        222, 201, 201, 222, 223, 201, 223, 202, 202, 223,
        224, 202, 224, 203, 203, 224, 225, 203, 225, 204,
        204, 225, 226, 204, 226, 205, 205, 226, 227, 205,
        227, 206, 206, 227, 228, 206, 228, 207, 207, 228,
        229, 207, 229, 208, 208, 229, 230, 208, 230, 209
    };

    static GLuint m = 0;
    static int c = 0;

    if (m == 0) {
        int v = sizeof(vert) / sizeof(vert[0]);
        c = sizeof(idx) / sizeof(idx[0]);
        m = compileDrawVAO(vert, v, idx, c);
    }
    glBindVertexArray(m);
    glDrawElements(GL_TRIANGLES, c, GL_UNSIGNED_INT, 0);
}
