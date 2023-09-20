
# Source files
set(sources	
	
	"firefly.hpp"
	"firefly.cpp"
	
	"firefly_cluster.hpp"
	"firefly_cluster.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
