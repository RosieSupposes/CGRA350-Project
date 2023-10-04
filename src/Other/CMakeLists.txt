
# Source files
set(sources	

	"basic_model.hpp"
	"basic_model.cpp"

	"camera.hpp"
	"camera.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
