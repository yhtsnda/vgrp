CC = g++
CCMACFLAGS = -bind_at_load `pkg-config --cflags opencv` -g
LIBS = `pkg-config --libs opencv`
SRC = Camera.cpp groundtruth.cpp Detector.cpp FeatureFinders.cpp FeatureTrackers.cpp

SUFFIX = $(firstword $(suffix $(SRC)))
OBJ = $(SRC:$(SUFFIX)=.o)

all:
	$(CC) $(CCMACFLAGS) $(SRC) -O3 -o groundtruth $(LIBS)
  
clean:
	rm -f *.o core groundtruth
