#include "Turtle.hpp"

Circle::Circle(vec3 center, float radius, vec3 rotation){
    this->center = center;
    this->radius = radius;
    this->rotation = rotation;
}

Circle::Circle(glm::vec3 center, float radius, int resolution, glm::mat4 rotation){
    this->center = center;
    this->radius = radius;
    this->rotation = vec3(0, 0, 0);
    this->matrixRotation = rotation;
    for (int i = 0; i < resolution*4; i++) {//this resolution is multiplied by 4 to ensure the resolution is divisible by 4
        addPoint((i * 2 * pi<float>())/(resolution *4.0f));
    }
}

Vertex Circle::getPoint(float angle){
    vec3 point = vec3(radius * sin(angle), 0, radius * cos(angle));
    mat4 rotationMatrix = glm::rotate(mat4(1), rotation.x, vec3(1, 0, 0))
        * glm::rotate(mat4(1), rotation.y, vec3(0, 1, 0))
        * glm::rotate(mat4(1), rotation.z, vec3(0, 0, 1));
    mat4 translateToPosition = glm::translate(mat4(1), center);
    Vertex vertex;
    vertex.pos = translateToPosition * rotationMatrix * this->matrixRotation * vec4(point, 1);
    return vertex;
}

void Circle::addPoint(float angle){
    points.push_back(getPoint(angle));
}

std::vector<Vertex> Circle::getPoints(){
    std::vector<Vertex> readOnly;
    for (int i = 0; i < (int)points.size(); i++) {
        readOnly.push_back(points.at(i));
    }
    return readOnly;
}


void Turtle::loadRules(vector<string> rules){
    for (int i = 0; i < (int)rules.size(); i++) {
        string input;
        string output;
        bool foundColon = false;
        for (char c : rules.at(i)) {
            if (foundColon) {
                output.push_back(c);
            }
            else {
                if (c == ':') {
                    foundColon = true;
                }
                else {
                    input.push_back(c);
                }
            }
        }
        if (input != "") {
            this->ruleList.push_back(Rule(input, output));
        }
    }
}

Expression Turtle::parseExpression(std::string token) {
    for (int i = 0; i < (int)token.size(); i++) {
        if (token.at(i) == '(') {
            int d = 1;
            int j = i;
            for (i++; d > 0; i++) {
                if (token.at(i) == '(') {
                    d++;
                }
                if (token.at(i) == ')') {
                    d--;
                }
            }
            i--;
            if (i == 1) {
                return Expression(); //empty brackets: ()
            }
            if (i != (int)token.size() - 1) {
                //s.substr(pos, length) NOT s.substr(pos1, pos2)
                //if length > s.size() - pos, length is shrunk to fit string
                return Expression(parseExpression(token.substr(j + 1, i - 1 - j)), token.at(i + 1), parseExpression(token.substr(i + 2, token.size())));
            }
            else {
                if (j != 0) {
                    return  Expression(parseExpression(token.substr(0, j - 1)), token.at(j - 1),parseExpression(token.substr(j + 1, token.size() - 2 - j)));
                }
                return parseExpression(token.substr(1, token.size() - 2));
            }
        }
    }
    for (int i = 0; i < (int)token.size(); i++) {
        if (token.at(i) == '+') {
            return Expression(parseExpression(token.substr(0, i)), '+', parseExpression(token.substr(i + 1, token.size())));
        }
        if (token.at(i) == '-') {
            return Expression(parseExpression(token.substr(0, i)), '-', parseExpression(token.substr(i + 1, token.size())));
        }
    }
    for (int i = 0; i < (int)token.size(); i++) {
        if (token.at(i) == '*') {
            return Expression(parseExpression(token.substr(0, i)),'*', parseExpression(token.substr(i + 1, token.size())));
        }
        if (token.at(i) == '/') {
            return Expression(parseExpression(token.substr(0, i)), '/', parseExpression(token.substr(i + 1, token.size())));
        }
        if (token.at(i) == '>') {
            return Expression(parseExpression(token.substr(0, i)), '>', parseExpression(token.substr(i + 1, token.size())));
        }
    }
    if (token.at(0) == 'd') {
        return Expression(this->distance);
    }
    if (token.at(0) == 'a') {
        return Expression(this->angle);
    }
    if (token.at(0) == 'l') {
        return Expression(this->level);
    }
    if (token.at(0) == 'r') {
        return Expression(this->radius);
    }
    return Expression(std::stof(token, 0));//it must be a float now if correct
}

string Turtle::getCommand(string axiom, int depth){
    string command = axiom;
    for(int i = 0; i < depth; i++){
        for (Rule rule : ruleList) {
            command = rule.applyRule(command);
        }
    }
    return command; //currently only applies one rule
}

vector<gl_mesh> Turtle::generateMesh(string command, int resolution, mt19937 randomNumberGenerator){
    vector<gl_mesh> cylinders;

    for (int i = 0; i < (int)command.size(); i++) {
        string token;
        token.push_back(command.at(i));
        if (token.at(0) == 'F') {
            vec3 startPosition = position;
            mat4 rotation = glm::rotate(mat4(1), this->angleH, vec3(0, 1, 0))
                * glm::rotate(mat4(1), this->angleL, vec3(0, 0, 1))
                * glm::rotate(mat4(1), this->angleU, vec3(1, 0, 0));
            this->position += vec3(this->rotation * rotation * vec4(0, 1, 0, 1) * distance);
            
            //vec3(0,1,0) is the initial normal of the circle
            Circle end(position, radius, resolution, this->rotation * rotation);
            this->rotation = this->rotation * rotation ;
            shared_ptr<Circle> start = make_shared<Circle>(Circle());
            if (cylinders.size() == 0) {
                start = make_shared<Circle>(Circle(startPosition, radius, resolution, mat4(1)));
            }
            else if (moved) {
                start = circle;
            }
            else {
                start = lastCircle;
            }
            gl_mesh cylinder = this->cylinder(*start, end);
            cylinders.push_back(cylinder);
            lastCircle = make_shared<Circle>(end);
            circle = lastCircle;
            moved = false;
        }
        else if (token.at(0) == 'f') {
            mat4 rotation = rotate(mat4(1), this->angleH, vec3(0, 1, 0))
                *rotate(mat4(1), this->angleL, vec3(0, 0, 1))
                *rotate(mat4(1), this->angleU, vec3(1, 0, 0));
            this->position += vec3(rotation * vec4(0, 1, 0, 1) * distance);
            circle = make_shared<Circle>(Circle(position, radius, resolution, mat4(1)));
            moved = true;
        }
        else if (token.at(0) == '[') {
            Point point;
            point.position = position;
            point.angleH = this->angleH;
            point.angleL = this->angleL;
            point.angleU = this->angleU;
            point.rotation = this->rotation;
            point.distance = this->distance;
            point.angle = this->angle;
            point.radius = this->radius;
            this->level++;
            point.circle = this->circle;
            stack.push_back(point);
        }
        else if (token.at(0) == ']') {
            Point point = stack.at(stack.size() - 1);
            stack.pop_back();
            this->position = point.position;
            this->angleH = point.angleH;
            this->angleL = point.angleL;
            this->angleU = point.angleU;
            this->rotation = point.rotation;
            this->distance = point.distance;
            this->angle = point.angle;
            this->radius = point.radius;
            this->level--;
            this->circle = point.circle;
            moved = true;
        }
        else if (token.at(0) == '+') {
            this->angleL = angle;
        }
        else if (token.at(0) == '-') {
            this->angleL = -angle;
        }
        else if (token.at(0) == '&') {//pitch down
            this->angleU = -angle;
        }
        else if (token.at(0) == '^') {//pitch up
            this->angleU = angle;
        }
        else if (token.at(0) == '\\') {
            this->angleH = angle;
        }
        else if (token.at(0) == '/') {
            this->angleH = -angle;
        }
        else if (token.at(0) == '|') {
            angleL = pi<float>();
        }
        else if (token.at(0) == 'D') {
            token = "";
            i += 2;
            int j = 0;
            while (true) {
                if (command.at(i) == ')') {
                    if (j == 0) {
                        break;
                    }
                    token += command.at(i);
                    i++;
                    j--;
                }
                else {
                    if (command.at(i) == '(') {
                        j++;
                    }
                    token += command.at(i);
                    i++;
                }
            }
            this->distance = parseExpression(token).evaluate(randomNumberGenerator);
        }
        else if (token.at(0) == 'A') {
            token = "";
            i += 2;
            int j = 0;
            while (true) {
                if (command.at(i) == ')') {
                    if (j == 0) {
                        break;
                    }
                    token += command.at(i);
                    i++;
                    j--;
                }
                else {
                    if (command.at(i) == '(') {
                        j++;
                    }
                    token += command.at(i);
                    i++;
                }
            }
            this->angle = parseExpression(token).evaluate(randomNumberGenerator);
        }
        else if (token.at(0) == 'R') {
        token = "";
        i += 2;
        int j = 0;
        while (true) {
            if (command.at(i) == ')') {
                if (j == 0) {
                    break;
                }
                token += command.at(i);
                i++;
                j--;
            }
            else {
                if (command.at(i) == '(') {
                    j++;
                }
                token += command.at(i);
                i++;
            }
        }
        this->radius = parseExpression(token).evaluate(randomNumberGenerator);
        }
    }
    return cylinders;
}

gl_mesh Turtle::cylinder(Circle base, Circle branch){
    vector<mesh_vertex> vertices;
    vector<unsigned int> indices;
    vec3 center = (branch.center + base.center) / 2.0f;

    // base center vertex
    mesh_vertex vertex;
    vertex.pos = base.center;
    vertex.norm = base.center - center;
    vertex.uv = vec2(0.5f, 0.0f);
    vertices.push_back(vertex);
    
    //base circle vertices
    for (int i = 0; i < (int)base.points.size(); i++) {
        Vertex point = base.getPoints().at(i);
        point.id = id++;
        mesh_vertex vertex;
        vertex.pos = point.pos;
        vertex.norm = point.pos - center;
        
        float u = static_cast<float>(i) / static_cast<float>(base.points.size());
        float v = 0.0f;
        vertex.uv = vec2(u, v);
        
        vertices.push_back(vertex);
    }

    //branch center vertex
    vertex.pos = branch.center;
    vertex.norm = branch.center - center;
    vertex.uv = vec2(0.5f, 1.0f);
    vertices.push_back(vertex);
    
    //branch circle vertices
    for (int i = 0; i < (int)branch.points.size(); i++) {
        Vertex point = branch.getPoints().at(i);
        point.id = id++;
        mesh_vertex vertex;
        vertex.pos = point.pos;
        vertex.norm = point.pos - center;
        
        float u = static_cast<float>(i) / static_cast<float>(base.points.size());
        float v = 1.0f;
        vertex.uv = vec2(u, v);
        
        vertices.push_back(vertex);
    }

    for (int i = 1; i <= (int)base.points.size(); i++) {
        //link base
        indices.push_back(0);
        indices.push_back(i);
        if (i == (int)base.points.size()) {
            indices.push_back(1);
        }
        else {
            indices.push_back(i + 1);
        }
        //link sides
        int j = i + (int)base.points.size() + 1;
        int k = i + (int)base.points.size() + 2;
        if (k == (int)vertices.size()) {
            k = (int)base.points.size() + 2;
        }
        indices.push_back(i);
        indices.push_back(j);
        indices.push_back(k);
        j = i + 1;
        if (i == (int)base.points.size()) {
            j = 1;
        }
        indices.push_back(i);
        indices.push_back(j);
        indices.push_back(k);
    }
    for (int i = (int)base.points.size() + 2; i < (int)vertices.size(); i++) {
        //link branch
        indices.push_back((int)base.points.size() + 1);
        indices.push_back(i);
        if (i == (int)vertices.size() - 1) {
            indices.push_back((int)base.points.size() + 2);
        }
        else {
            indices.push_back(i + 1);
        }
    }

    mesh_builder builder;
    builder.indices = indices;
    builder.vertices = vertices;
    builder.mode = GL_TRIANGLES;
    return builder.build();
}
