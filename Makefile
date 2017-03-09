PROJECT=ngx

BINDIR=./bin
SRCDIR=./src
OBJDIR=./obj
INCDIR=./include
TSTDIR=./test

CXX=afl-gcc
CXXFLAGS=-g3 -m32 -std=gnu99 -I$(INCDIR) -pedantic
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

test: $(BINDIR)/ngxData $(BINDIR)/ngxls $(BINDIR)/ngxTree
	$(BINDIR)/ngxData test.ngx
	$(BINDIR)/ngxTree test.ngx
	$(BINDIR)/ngxls test.ngx

$(BINDIR) $(OBJDIR):
	mkdir -v $@

$(BINDIR)/ngxData: $(OBJECTS) $(TSTDIR)/ngxData.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BINDIR)/ngxls: $(OBJECTS) $(TSTDIR)/ngxls.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BINDIR)/ngxTree: $(OBJECTS) $(TSTDIR)/ngxTree.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BINDIR)/$(PROJECT): $(OBJECTS) $(PROJECT)/entry.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CXX) -c $(CXXFLAGS) -o $@ $<

