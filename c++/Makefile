# MegaShow / Agenda

# define
BIN_DIR = ./bin
BUILD_DIR = ./build
INC_DIR = ./include
SRC_DIR = ./src
CC = g++
ARGS = -std=c++11 -g -Wall -I$(INC_DIR)

# make main
$(BIN_DIR)/Agenda: $(BUILD_DIR)/Agenda.o $(BUILD_DIR)/User.o $(BUILD_DIR)/Date.o \
		$(BUILD_DIR)/Meeting.o $(BUILD_DIR)/Storage.o \
		$(BUILD_DIR)/AgendaService.o $(BUILD_DIR)/AgendaUI.o \
		$(BUILD_DIR)/AgendaLinuxPro.o
	$(CC) $^ $(ARGS) -o $@

# make main.o
$(BUILD_DIR)/Agenda.o: $(SRC_DIR)/Agenda.cpp $(INC_DIR)/*.hpp
	$(CC) $< $(ARGS) -c -o $@

# make object
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp $(INC_DIR)/%.hpp
	$(CC) $< $(ARGS) -c -o $@

# make run
run: $(BIN_DIR)/Agenda
	$<

# make clean
clean:
	rm -f $(BIN_DIR)/*
	rm -f $(BUILD_DIR)/*

# make zip
zip: clean
	zip -r ../backup.zip ../Agenda/
