CFLAGS = -O2 -std=c++1z -MMD -ggdb -w -lpthread
FILES = src/main.cc src/fileEdit.cc src/fileEdit.h src/dealer.cc src/dealer.h src/httpServer.h src/httpServer.cc src/account.cc src/account.h
AIM = server

.PHONY: build run clean

build: $(FILES)
	g++ $(FILES) -o $(AIM) $(CFLAGS)

run: build
	./$(AIM)

clean: 
	rm -f $(AIM)
