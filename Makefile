COMPILER = g++-14
CXXFLAGS = -std=c++20 -fmodules-ts -Wall -g
TARGET = watan

SRCS = main.cc \
       resources.cc resources-impl.cc \
       dice.cc dice-impl.cc \
       geese.cc geese-impl.cc \
       tile.cc tile-impl.cc \
       student.cc student-impl.cc \
       criterion.cc criterion-impl.cc \
       goal.cc goal-impl.cc \
       board.cc board-impl.cc \
       game.cc game-impl.cc

OBJS = $(SRCS:.cc=.o)

GCM_DIR = gcm.cache
GCM = $(GCM_DIR)/iostream.gcm $(GCM_DIR)/sstream.gcm $(GCM_DIR)/fstream.gcm \
      $(GCM_DIR)/vector.gcm $(GCM_DIR)/algorithm.gcm $(GCM_DIR)/random.gcm \
      $(GCM_DIR)/map.gcm $(GCM_DIR)/memory.gcm $(GCM_DIR)/string.gcm \
      $(GCM_DIR)/stdexcept.gcm

MODULE_OBJS = \
    resources.o resources-impl.o \
    dice.o dice-impl.o \
    geese.o geese-impl.o \
    tile.o tile-impl.o \
    student.o student-impl.o \
    criterion.o criterion-impl.o \
    goal.o goal-impl.o \
    board.o board-impl.o \
    game.o game-impl.o

all: prebuilt $(TARGET)

prebuilt: $(GCM)

$(GCM_DIR):
	mkdir -p $(GCM_DIR)

$(GCM_DIR)/iostream.gcm: $(GCM_DIR)
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header iostream -o $@

$(GCM_DIR)/sstream.gcm: $(GCM_DIR)/iostream.gcm
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header sstream -o $@

$(GCM_DIR)/fstream.gcm: $(GCM_DIR)/sstream.gcm
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header fstream -o $@

$(GCM_DIR)/vector.gcm: $(GCM_DIR)/fstream.gcm
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header vector -o $@

$(GCM_DIR)/algorithm.gcm: $(GCM_DIR)/vector.gcm
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header algorithm -o $@

$(GCM_DIR)/random.gcm: $(GCM_DIR)/algorithm.gcm
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header random -o $@

$(GCM_DIR)/map.gcm: $(GCM_DIR)/random.gcm
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header map -o $@

$(GCM_DIR)/memory.gcm: $(GCM_DIR)/map.gcm
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header memory -o $@

$(GCM_DIR)/string.gcm: $(GCM_DIR)/memory.gcm
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header string -o $@

$(GCM_DIR)/stdexcept.gcm: $(GCM_DIR)/string.gcm
	$(COMPILER) $(CXXFLAGS) -c -x c++-system-header stdexcept -o $@

%.o: %.cc
	$(COMPILER) $(CXXFLAGS) -c $< -o $@


resources.o: $(GCM)
dice.o: $(GCM)
geese.o: $(GCM)
tile.o: $(GCM)
student.o: resources.o dice.o $(GCM)
criterion.o: student.o $(GCM)
goal.o: student.o $(GCM)
board.o: tile.o criterion.o goal.o student.o $(GCM)
game.o: board.o student.o geese.o $(GCM)
resources-impl.o: resources.o $(GCM)
dice-impl.o: dice.o $(GCM)
geese-impl.o: geese.o $(GCM)
tile-impl.o: tile.o $(GCM)
student-impl.o: student.o resources.o dice.o $(GCM)
criterion-impl.o: criterion.o student.o $(GCM)
goal-impl.o: goal.o student.o $(GCM)
board-impl.o: board.o student.o $(GCM)
game-impl.o: game.o board.o student.o geese.o $(GCM)
main.o: $(MODULE_OBJS)

$(TARGET): $(MODULE_OBJS) main.o
	$(COMPILER) $(CXXFLAGS) $(MODULE_OBJS) main.o -o $(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
	rm -rf $(GCM_DIR)

.PHONY: all prebuilt clean
