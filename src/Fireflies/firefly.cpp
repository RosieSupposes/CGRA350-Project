#include "firefly.hpp"

//Placeholder
#include "cgra/cgra_wavefront.hpp"
#include <glm/gtc/matrix_transform.hpp>

using namespace glm;
using namespace cgra;

firefly::firefly(glm::vec3 p, float b, glm::vec3 sp){
    static cgra::gl_mesh sharedMesh = gen_sphere();
		//cgra::load_wavefront_data(CGRA_SRCDIR + std::string("/res//assets//placeholderSphere.obj")).build();
	m_mesh = sharedMesh;
    m_color = glm::vec3(0.8,0.7,0.2);
    

	pos = p;
	brightness = b;
	search_precision = sp;
	//m_texture;
}

void firefly::draw(const mat4 &view, const mat4 &proj, material &material) {
	
	mat4 model = translate(mat4(1), pos);
	model = scale(model, vec3(0.3));
	material.load(model, view, proj);
	glUniform1f(glGetUniformLocation(material.m_shader, "uBrightness"), brightness);
	m_mesh.draw();
}

void firefly::setPos(glm::vec3 p){
	this->pos = p;
}


gl_mesh firefly::gen_sphere()
{
	int azimuth_divisions = 20;
	int elevation_divisions = 20;
	mesh_builder mb;
	
	vec2 placeholder = vec2(0,0);//Replace later
	
	std::vector<vec4> points;
	float elevation_step = radians(180.0f)/elevation_divisions;
	for(int i = 1; i < elevation_divisions; i++){
		vec4 point = vec4(sin(i*elevation_step),0,cos(i*elevation_step), 1);
		points.push_back(point);
	}
	std::vector<mat4> rotations;
	float azimuth_step = radians(360.0f)/azimuth_divisions;
	for(int j = 0; j < azimuth_divisions; j++){
		rotations.push_back(rotate(mat4{1.0}, j*azimuth_step, vec3(0,0,1)));
	}
	//count = 0;
	float xCount = 0;
	for(mat4 rot : rotations){
		float x_u = xCount/azimuth_divisions;
		float yCount = 1;
		for(vec4 point : points){
			float y_v = yCount/elevation_divisions;
			vec4 rP = point*rot;
			vec3 pt = vec3(rP.x, rP.y, rP.z);
			vec3 norm = vec3(pt.x, pt.y, pt.z);
			mb.push_vertex(mesh_vertex{pt, pt, vec2(x_u, y_v)});
			yCount += 1;
			// ImGui::Text("Vec3 %d, x=%.3f, y=%.3f, z=%.3f|", count, point.x, point.y, point.z); count++;
		}
		xCount += 1;
	}
	mb.push_vertex(mesh_vertex{vec3(0,0,1), vec3(0,0,1), vec2(0.5,0)});
	mb.push_vertex(mesh_vertex{vec3(0,0,-1), vec3(0,0,-1), vec2(0.5,1)});
	// ImGui::Text("Vec3 %d, x=%.3f, y=%.3f, z=%.3f|", count, 0.0, 0.0, 1.0); count++;
	// ImGui::Text("Vec3 %d, x=%.3f, y=%.3f, z=%.3f|", count, 0.0, 0.0, -1.0); count++;
	int elevation_offset = elevation_divisions - 1;
	for(int i = 0; i < azimuth_divisions-1; i++){
		for(int j = 1; j < elevation_divisions-1; j++){
			int a = i*elevation_offset + j - 1;
			int b = a + 1;
			int c = a + elevation_offset;
			int d = c + 1;
			mb.push_index(a);
			mb.push_index(b);
			mb.push_index(c);
			mb.push_index(c);
			mb.push_index(b);
			mb.push_index(d);
			// ImGui::Text("i=%d,j=%d", i, j);
			// ImGui::Text("Index a is %d", a);
			// ImGui::Text("Index b is %d", b);
			// ImGui::Text("Index c is %d", c);
			// ImGui::Text("Index d is %d", d);
		}
	}
	//final side wrap around
	for(int j = 1; j < elevation_divisions-1; j++){
		int a = (azimuth_divisions-1)*elevation_offset + j - 1;
		int b = a + 1;
		int c = j - 1;
		int d = c + 1;
		mb.push_index(a);
		mb.push_index(b);
		mb.push_index(c);
		mb.push_index(c);
		mb.push_index(b);
		mb.push_index(d);
		// ImGui::Text("i=%d,j=%d", i, j);
		// ImGui::Text("Index a is %d", a);
		// ImGui::Text("Index b is %d", b);
		// ImGui::Text("Index c is %d", c);
		// ImGui::Text("Index d is %d", d);
	}
	//join ends
	//end 1
	for(int i = 0; i < azimuth_divisions-1; i++){
		int a = i*elevation_offset;
		int b = azimuth_divisions*(elevation_divisions-1);
		int c = a + elevation_offset;
		mb.push_index(a);
		mb.push_index(b);
		mb.push_index(c);
	}
	mb.push_index((azimuth_divisions-1)*elevation_offset);
	mb.push_index(azimuth_divisions*(elevation_divisions-1));
	mb.push_index(0);
	//end 2
	//a=i*elevation_offset-1
	//a=(azimuth_divisions-1)*(elevation_divisions-1)-1
	//c = a + elevation_offset;
	//c = a + (elevation_divisions-1);
	//c = (azimuth_divisions-1)*(elevation_divisions-1)-1 + 1*(elevation_divisions-1)
	//c = azimuth_divisions*(elevation_divisions-1)-1
	for(int i = 1; i < azimuth_divisions; i++){
		int a = i*elevation_offset-1;
		int b = azimuth_divisions*(elevation_divisions-1)+1;
		int c = a + elevation_offset;
		mb.push_index(a);
		mb.push_index(b);
		mb.push_index(c);
	}
	mb.push_index(azimuth_divisions*(elevation_divisions-1)-1);
	mb.push_index(azimuth_divisions*(elevation_divisions-1)+1);
	mb.push_index(elevation_offset-1);
	return mb.build();
}