PROJECT=ngx

BINDIR=./bin
SRCDIR=./src
OBJDIR=./obj
INCDIR=./include

CXX=gcc
CXXFLAGS=-g3 -std=gnu99 -I$(INCDIR)
CXXLIBS=-lGL -lX11

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(patsubst %.c,%.o,$(SOURCES)))

.PHONY: clean run

all: $(BINDIR) $(OBJDIR) $(BINDIR)/$(PROJECT)
	@echo "Done!"

run: $(BINDIR) $(OBJDIR) $(BINDIR)/$(PROJECT)
	$(BINDIR)/$(PROJECT)

clean:
	rm -rvf $(BINDIR)/
	rm -rvf $(OBJDIR)/

$(BINDIR) $(OBJDIR):
	mkdir -v $@

$(BINDIR)/$(PROJECT): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CXX) -c $(CXXFLAGS) -o $@ $<

