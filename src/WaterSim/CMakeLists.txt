
# Source files
set(sources	

	"water_sim.hpp"
	"water_sim.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
