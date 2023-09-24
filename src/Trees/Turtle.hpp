#include "Rule.hpp"
#include "Expression.hpp"

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

// project
#include "opengl.hpp"

using namespace std;
using namespace glm;

class Turtle {
    vector<Rule> ruleList;
    
public:
    void loadRules(vector<string> rules); //populates ruleList
};
