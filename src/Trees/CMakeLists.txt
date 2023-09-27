
# Source files
set(sources	

	"forest.hpp"
	"forest.cpp"
	
	"tree.hpp"
    "tree.cpp"
 
    "Rule.hpp"
    "Rule.cpp"
    
    "Expression.hpp"
    "Expression.cpp"
    
    "Turtle.hpp"
    "Turtle.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
