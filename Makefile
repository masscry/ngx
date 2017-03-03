PROJECT=ngx

BINDIR=./bin
SRCDIR=./src
OBJDIR=./obj
INCDIR=./include
TSTDIR=./test

CXX=gcc
CXXFLAGS=-g3 -std=gnu99 -I$(INCDIR) -pedantic
CXXLIBS=-lGL -lGLU -lX11

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(patsubst %.c,%.o,$(SOURCES)))

.PHONY: clean run test

all: $(BINDIR) $(OBJDIR) $(BINDIR)/$(PROJECT)
	@echo "Done!"

run: $(BINDIR) $(OBJDIR) $(BINDIR)/$(PROJECT)
	$(BINDIR)/$(PROJECT)

clean:
	rm -rvf $(BINDIR)/
	rm -rvf $(OBJDIR)/

test: $(BINDIR)/ngxData
	$(BINDIR)/ngxData

$(BINDIR) $(OBJDIR):
	mkdir -v $@

$(BINDIR)/ngxData: $(OBJECTS) $(TSTDIR)/ngxData.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BINDIR)/$(PROJECT): $(OBJECTS) $(PROJECT)/entry.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CXX) -c $(CXXFLAGS) -o $@ $<

