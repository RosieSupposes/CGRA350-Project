#include "firefly_cluster.hpp"

#include <iostream>
#include <random>
#include "cgra/cgra_gui.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace std;

firefly_cluster::firefly_cluster(int count) {
	min_bounds = vec3(-10, 5, -20);
	max_bounds = vec3(10, 10, 20);
	reload(count);

	//gen = mt19937(rd()); // seed the generator
	float range = 2.5f;
	random_small_vector = uniform_real_distribution<>(-range, range); // define the range
}

void firefly_cluster::reload(int count) {
	reset_flies(count);
}

void firefly_cluster::reset_flies(int fireflyCount) {
	fireflies.clear();
	for (int f = 0; f < fireflyCount; f++) {
		float range = 38;
		//vec3 randomPosition = vec3(
		//	-(range / 2) + range * ((float)std::rand()) / RAND_MAX,//range between -10 to 10
		//	10 + 10 * ((float)std::rand()) / RAND_MAX, //range between 10 to 20
		//	-(range / 2) + range * ((float)std::rand()) / RAND_MAX //range between -10 to 10
		//);
		vec3 randomPosition = vec3(
			get_random_num(min_bounds.x, max_bounds.x),
			get_random_num(min_bounds.y, max_bounds.y),
			get_random_num(min_bounds.z, max_bounds.z)
		);


		int brightness_step = rand() % (max_brightness_step + 1);
		vec3 search_precison((rand() % 2) / 100.0f, (rand() % 2) / 100.0f, (rand() % 2) / 100.0f);

		firefly fly = firefly(randomPosition, brightness_step, search_precison);
		fly.brightness = get_brightness(brightness_step);
		fireflies.push_back(fly);
	}
}

float firefly_cluster::get_brightness(int s) {
	int m = max_brightness_step;
	int p = 9 * max_brightness_step / 10;
	if (s < p) {
		float b = 0.053; //slider
		float c = -10;
		float a = 1.0f / (pow(Math_E, (p * b) + c));
		return a * pow(Math_E, ((b * s) + c));
	}
	else {
		//if 150 < s < 180
		float f = -0.15; //slider
		float g = -10 - (f * m);
		float d = 1.0f / (pow(Math_E, (f * p + g)));
		return d * pow(Math_E, (f * s + g));
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
	//vec3 v(0);
	//vec3 brightest_pos;
	//float brightest_value = -1;
	//for (firefly* o : f.neighbours) {
	//	float other_brightness = o->brightness;
	//	//cout << other_brightness << endl;
	//	if (other_brightness > f.brightness && other_brightness > brightest_value) {
	//		//vec3 m(attraction(o, f) * (o.pos - f.pos) + alpha * o.search_precision);
	//		//cout << move.x << "," << move.y << "," << move.z << endl;
	//		brightest_pos = o->pos;
	//		brightest_value = other_brightness;
	//		if (brightest_value >= 0.95f) {
	//			break;
	//		}
	//		//cout << "bright set" << endl;
	//	}
	//}

	//if (brightest_value > -1) {
	//	vec3 m((brightest_pos - f.pos) * 0.5f);
	//	v = m;
	//	//cout << "no bright" << endl;
	//}
	//else {
	//	float x = random_small_vector(gen);
	//	float y = random_small_vector(gen);
	//	float z = random_small_vector(gen);
	//	v = vec3(x, y, z);
	//	//v = vec3(max);
	//}

	vec3 v(0.0f);
	for (firefly* j : f.neighbours) {
		//if ((*j).brightness > f.brightness) {
		vec3 m(attraction(f, (*j)) * ((*j).pos - f.pos) + alpha * (*j).search_precision);
		//cout << move.x << "," << move.y << "," << move.z << endl;
		v = v + m;
		//}
	}
	//cout << v.x << "," << v.y << "," << v.z << endl;

	return 0.5f * v;
}

vec3 firefly_cluster::away_from_each_other(firefly f) {
	float d = 4.0f;
	vec3 v(0);
	for (firefly* o : f.neighbours) {
		float space = distance((*o).pos, f.pos);
		if (space <= 0.7) {
			return -v;
		}
		else if (space <= d) {
			v = v - ((*o).pos - f.pos);
		}
	}
	return v;
}

vec3 firefly_cluster::within_bounds(firefly f) {
	vec3 v(0);
	float d = 0.5f;
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
	return lower + (float(rand()) / float(RAND_MAX)) * (upper - lower);
}

void firefly_cluster::simulate() {
	for (firefly& f : fireflies) {
		get_closest_fireflies(f);
		//cout << f.neighbours.size() << endl;
	}

	//cout << fireflies[0].pos.x << "," << fireflies[0].pos.y << "," << fireflies[0].pos.z << endl;
	//update position
	//

	for (firefly& f : fireflies) {
		//cout << f.brightness << endl;

		vec3 v1 = towards_brightest(f);
		//v1 = vec3(0);

		vec3 v2 = away_from_each_other(f);
		vec3 v3 = within_bounds(f);
		f.velocity = f.velocity + (2.0f * v1) + (1.0f * v2) + (4.0f * v3);
		limit_velocity(f);
		f.pos = f.pos + f.velocity;
	}

	//update brightness
	for (firefly& i : fireflies) {
		if (i.brightness_step >= max_brightness_step) {
			i.brightness_step = 0;
		}
		else {
			i.brightness_step += 1;
		}
		//cout << i.brightness << ":" << i.brightness_step << endl;

		for (firefly* j : i.neighbours) {
			if ((*j).brightness_step >= max_brightness_step) {
				i.brightness_step += 1;
			}
		}
		i.brightness = get_brightness(i.brightness_step);
	}
}

void firefly_cluster::get_closest_fireflies(firefly& f) {
	f.neighbours.clear();
	for (firefly& o : fireflies) {
		if (&f != &o && distance(o.pos, f.pos) <= neighbourhood_size) {
			f.neighbours.push_back(&o);
			//cout << "neighbour " << endl;
		}
	}
}

void firefly_cluster::draw(const mat4& view, const mat4& proj, material& material) {
	//cout << fireflies[0].pos.x << "," << fireflies[0].pos.y << "," << fireflies[0].pos.z << endl;
	for (firefly ff : fireflies) {
		ff.draw(view, proj, material);
	}
}

void firefly_cluster::renderGUI(int height, int pos) {
	ImGui::SetNextWindowPos(ImVec2(5, pos), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(300, height), ImGuiSetCond_Once);
	ImGui::Begin("Fireflies", 0);

	ImGui::Text("Fireflies");
	if (ImGui::InputInt("Fireflies", &fireflyCount)) {
		this->reload(fireflyCount);
	}

	// finish creating window
	ImGui::End();
}
