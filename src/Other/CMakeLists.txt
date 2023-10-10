
# Source files
set(sources	

	"basic_model.hpp"
	"basic_model.cpp"

	"camera.hpp"
	"camera.cpp"

	"collider.hpp"
	"collider.cpp"

	"keyboard_controller.hpp"
	"keyboard_controller.cpp"

	"material.hpp"
	"material.cpp"

	"RNG.hpp"
	"RNG.cpp"

	"terrain.hpp"
	"terrain.cpp"
	
	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
