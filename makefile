LIBS = -Wall
CC   = g++

my_ray_tracer: main.o math.o image.o scene.o object.o perlin.o raytracer.o stats.o
	$(CC) $(LIBS) -o my_ray_tracer main.o math.o image.o scene.o object.o perlin.o raytracer.o stats.o

main.o: main.cpp
	$(CC) -c main.cpp

math.o: math.cpp
	$(CC) -c math.cpp

image.o: image.cpp
	$(CC) -c image.cpp

scene.o: scene.cpp
	$(CC) -c scene.cpp

object.o: object.cpp
	$(CC) -c object.cpp

perlin.o: perlin.cpp
	$(CC) -c perlin.cpp

raytracer.o: raytracer.cpp
	$(CC) -c raytracer.cpp

stats.o: stats.cpp
	$(CC) -c stats.cpp

all: my_ray_tracer clean

clean:
	rm *.o

