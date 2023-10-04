
# Source files
set(sources	

	"water_sim.hpp"
	"water_sim.cpp"
	"water_particle.hpp"
	"water_particle.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
