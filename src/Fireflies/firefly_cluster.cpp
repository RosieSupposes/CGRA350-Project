#include "firefly_cluster.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

firefly_cluster::firefly_cluster(int count) {
	sharedMesh = cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//placeholderSphere.obj")).build();
	reload(count);
	min_bounds = vec3(-20, 0, -20);
	max_bounds = vec3(20, 5, 20);
}

void firefly_cluster::reload(int count) {
	reset_flies(count);
}

void firefly_cluster::reset_flies(int fireflyCount) {
	fireflies.clear();
	for (int f = 0; f < fireflyCount; f++) {
		float range = 38;
		vec3 randomPosition = vec3(
			-(range / 2) + range * ((float)std::rand()) / RAND_MAX,//range between -10 to 10
			10 + 10 * ((float)std::rand()) / RAND_MAX, //range between 10 to 20
			-(range / 2) + range * ((float)std::rand()) / RAND_MAX //range between -10 to 10
		);

		float brightness = (rand() % 11) / 10.0f;
		vec3 search_precison((rand() % 2) / 100.0f, (rand() % 2) / 100.0f, (rand() % 2) / 100.0f);
		fireflies.push_back(firefly(randomPosition, brightness, search_precison));
	}
}

float firefly_cluster::relative_brightness(firefly i, firefly j) {
	float dist = distance(i.pos, j.pos);
	return i.brightness * pow(Math_E, -absorption * pow(dist, 2));
}

float firefly_cluster::attraction(firefly i, firefly j) {
	float dist = distance(j.pos, i.pos);
	return max_attraction * pow(Math_E, -absorption * pow(dist, 2));
}

vec3 firefly_cluster::towards_brightest(firefly f) {
	vec3 v(0);
	vec3 brightest_pos;
	float brightest_value = -1;
	for (firefly o : fireflies) {
		if (o.brightness > brightest_value && o.brightness > f.brightness) {
			//vec3 m(attraction(o, f) * (o.pos - f.pos) + alpha * o.search_precision);
			//cout << move.x << "," << move.y << "," << move.z << endl;
			brightest_pos = o.pos;
			brightest_value = o.brightness;

		}
	}
	if (brightest_value > -1) {
		vec3 m((brightest_pos - f.pos) * 0.02f);
		v = m;
	}
	else {
		float x = get_random_num(-0.1f, 0.1f);
		float y = get_random_num(-0.1f, 0.1f);
		float z = get_random_num(-0.1f, 0.1f);
		v = vec3(x, y, z);
	}
	return v;
}

vec3 firefly_cluster::away_from_each_other(firefly f) {
	float distance = 1.5f;
	vec3 v(0);
	for (firefly o : fireflies) {
		if (o.pos != f.pos) {
			if (length(o.pos - f.pos) < distance) {
				v = v - (o.pos - f.pos);
			}
		}
	}
	return v;
}

vec3 firefly_cluster::within_bounds(firefly f) {
	vec3 v(0);
	float d = 0.05f;
	if (f.pos.x < min_bounds.x) {
		v.x = d;
	}
	else if (f.pos.x > max_bounds.x) {
		v.x = -d;
	}

	if (f.pos.y < min_bounds.y) {
		v.y = d;
	}
	else if (f.pos.y > max_bounds.y) {
		v.y = -d;
	}

	if (f.pos.z < min_bounds.z) {
		v.z = d;
	}
	else if (f.pos.z > max_bounds.z) {
		v.z = -d;
	}

	return v;
}

vec3 firefly_cluster::fly_to_centre(firefly f) {
	//TODO
	return glm::vec3();
}

vec3 firefly_cluster::match_velocity(firefly f) {
	//TODO
	return glm::vec3();
}

void firefly_cluster::limit_velocity(firefly& f) {
	if (length(f.velocity) > speed_limit) {
		f.velocity = normalize(f.velocity) * speed_limit;
	}
}

float firefly_cluster::get_random_num(float lower, float upper) {
	return lower + (rand() / RAND_MAX) * (upper - lower);
}

void firefly_cluster::simulate() {

	//cout << fireflies[0].pos.x << "," << fireflies[0].pos.y << "," << fireflies[0].pos.z << endl;
	//update position
	//for (firefly& i : fireflies) {
	//	vec3 move(0, 0, 0);
	//	for (firefly j : fireflies) {
	//		if (j.brightness > i.brightness) {
	//			vec3 m(attraction(i, j) * (j.pos - i.pos) + alpha * j.search_precision);
	//			//cout << move.x << "," << move.y << "," << move.z << endl;
	//			move = move + m;
	//		}
	//	}
	//	//cout << move.x << "," << move.y << "," << move.z << endl;
	//	i.pos = i.pos + 0.1f * move;
	//}

	for (firefly& f : fireflies) {
		vec3 v1 = towards_brightest(f);
		//v1 = vec3(0);

		vec3 v2 = away_from_each_other(f);
		vec3 v3 = within_bounds(f);
		f.velocity = f.velocity + v1 + v2 + v3;
		limit_velocity(f);
		f.pos = f.pos + f.velocity;
	}

	//update brightness
	for (firefly& i : fireflies) {
		if (i.brightness >= max_brightness) {
			i.brightness = 0;
		}
		else {
			i.brightness += brightness_step;
		}

		for (firefly& j : fireflies) {
			if (j.brightness >= max_brightness) {
				i.brightness += brightness_step;
			}
		}
	}
}

void firefly_cluster::draw(const mat4& view, const mat4& proj, GLuint shader) {
	//cout << fireflies[0].pos.x << "," << fireflies[0].pos.y << "," << fireflies[0].pos.z << endl;
	for (firefly ff : fireflies) {
		ff.draw(view, proj, shader);
	}
}
