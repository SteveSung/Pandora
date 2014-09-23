OBJS=conv2d.o readjpeg.o

%.o: %.cpp readjpeg.h
	g++ -O2 -c -fopenmp $<

all: $(OBJS)
	g++ -fopenmp $(OBJS) -o conv2d -ljpeg
