linkTarget = RayTracerApplication

LIBS = -lSDL2

CFLAGS = -std=c++17

# def used objs
objects = main.o \
	Application.o \
	$(patsubst %.cpp,%.0,$(wildcard ./RayTracerApplication/*.cpp))

# def rebuildables
rebuildables = $(objects) $(linkTarget)

# build
$(linkTarget) : $(objects)
	g++ -g -o $(linkTarget) $(objects) $(LIBS) $(CFLAGS)

# create .o files
%.o: %.cpp
	g++ -o $@ -c $< $(CFLAGS)

.PHONEY:
clean:
	rm $(rebuildables)