
# Source files
set(sources	

	"forest.hpp"
	"forest.cpp"
	
	"tree.hpp"
	"tree.cpp"

	"CMakeLists.txt"
)

# Add these sources to the project target
target_relative_sources(${CGRA_PROJECT} ${sources})
