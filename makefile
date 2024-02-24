# Define the link target.
linkTarget = RTApp

# Define the libraries that we need.
LIBS = -lSDL2 -lSDL2_image

# Define any flags.
CFLAGS = -std=c++17 -Ofast -pthread

# Define the object files that we need to use.
objects =	main.o \
			Application.o \
			$(patsubst %.cpp,%.o,$(wildcard ./dependencies/*.cpp)) \
			$(patsubst %.cpp,%.o,$(wildcard ./dependencies/lights/*.cpp)) \
			$(patsubst %.cpp,%.o,$(wildcard ./dependencies/materials/*.cpp)) \
			$(patsubst %.cpp,%.o,$(wildcard ./dependencies/objects/*.cpp)) \
			$(patsubst %.cpp,%.o,$(wildcard ./dependencies/scenes/*.cpp)) \
			$(patsubst %.cpp,%.o,$(wildcard ./dependencies/textures/*.cpp))
					
# Define the rebuildables.
rebuildables = $(objects) $(linkTarget)

# Rule to actually perform the build.
$(linkTarget): $(objects)
	g++ -g -o $(linkTarget) $(objects) $(LIBS) $(CFLAGS)
	
# Rule to create the .o (object) files.
%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS)
	
.PHONEY:
clean:
	rm $(rebuildables)