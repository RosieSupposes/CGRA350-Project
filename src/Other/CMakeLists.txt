
# Source files
set(sources	

	"basic_model.hpp"
	"basic_model.cpp"
	
	"terrain.hpp"
	"terrain.cpp"
	
	"collider.hpp"
	"collider.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
