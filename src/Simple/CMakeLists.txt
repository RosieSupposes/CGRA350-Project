
# Source files
set(sources	

	"simple_water.hpp"
	"simple_water.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
