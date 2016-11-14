# Configuration area
CXX = g++
LDFLAGS =  -lopencv_legacy -lopencv_highgui -lopencv_core -lopencv_ml -lopencv_video
#LDFLAGS = -I/usr/local/Cellar/opencv/2.4.13_3/include/opencv -I/usr/local/Cellar/opencv/2.4.13_3/include -lm -lopencv_legacy -lopencv_highgui -lopencv_core -lopencv_ml -lopencv_video -lopencv_imgproc
#LDFLAGS = -I/usr/local/include `pkg-config --cflags opencv` `pkg-config --libs opencv`
#LDFLAGS = -L/usr/local/Cellar/opencv/2.4.13_3/lib  -lopencv_calib3d -lopencv_contrib -lopencv_core -lopencv_features2d -lopencv_flann -lopencv_gpu -lopencv_highgui -lopencv_imgproc -lopencv_legacy -lopencv_ml -lopencv_nonfree -lopencv_objdetect -lopencv_ocl -lopencv_photo -lopencv_stitching -lopencv_superres -lopencv_ts -lopencv_video -lopencv_videostab -I/usr/local/Cellar/opencv/2.4.13_3/include/opencv -I/usr/local/Cellar/opencv/2.4.13_3/include
CXXFLAGS = -O0 $(LDFLAGS)
PROJ_DIR := opencv_test
OUTPUT := test.out


objects := $(patsubst %.cpp,%.o,$(wildcard $(PROJ_DIR)/*.cpp))

.PHONY: all clean

all: $(OUTPUT)

$(OUTPUT): $(objects)
	$(CXX) $(CXXFLAGS) $(objects) -o $@

clean:
	rm $(PROJ_DIR)/*.o

