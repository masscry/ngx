PROJECT=ngx

BINDIR=./bin
SRCDIR=./src
OBJDIR=./obj
INCDIR=./include
TSTDIR=./test

CNFLIBDIR=./config

CNFLIB=$(CNFLIBDIR)/lib/libngxcnf.a

NGXLIBS=$(CNFLIB)
NGXINCL=$(CNFLIBDIR)/include

CXX=gcc
CXXFLAGS=-g3 -std=gnu99 -I$(INCDIR) -I$(NGXINCL) -pedantic -Werror=implicit-function-declaration -Wno-overlength-strings
CXXLIBS=-lGL -lGLU -lX11 -lm

export

SOURCES=$(wildcard $(SRCDIR)/*.c)
OBJECTS=$(patsubst $(SRCDIR)/%,$(OBJDIR)/%,$(patsubst %.c,%.o,$(SOURCES))) $(NGXLIBS)

.PHONY: clean run test conflib

all: $(BINDIR) $(OBJDIR) $(BINDIR)/$(PROJECT) $(BINDIR)/ngxfl $(BINDIR)/ngxData $(BINDIR)/ngxls $(BINDIR)/ngxTree $(BINDIR)/ngxar conflib
	@echo "Done!"

run: $(BINDIR) $(OBJDIR) $(BINDIR)/$(PROJECT)
	$(BINDIR)/$(PROJECT)

clean:
	rm -rvf $(BINDIR)/
	rm -rvf $(OBJDIR)/
	make -C $(CNFLIBDIR) clean

test: $(BINDIR)/ngxData $(BINDIR)/ngxls $(BINDIR)/ngxTree $(BINDIR)/ngxar
	rm -fv test.ngx
	rm -fv src.ngx
	$(BINDIR)/ngxData test.ngx
	$(BINDIR)/ngxTree test.ngx
	$(BINDIR)/ngxls test.ngx
	$(BINDIR)/ngxar src.ngx src/*.c include/*.h test/*.c
	make -C $(CNFLIBDIR) test

conflib: $(CNFLIB)

$(BINDIR) $(OBJDIR):
	mkdir -v $@

$(BINDIR)/ngxData: $(OBJECTS) $(TSTDIR)/ngxData.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BINDIR)/ngxls: $(OBJECTS) $(TSTDIR)/ngxls.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BINDIR)/ngxar: $(OBJECTS) $(TSTDIR)/ngxar.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BINDIR)/ngxTree: $(OBJECTS) $(TSTDIR)/ngxTree.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BINDIR)/$(PROJECT): $(OBJECTS) $(PROJECT)/entry.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(BINDIR)/ngxfl: $(OBJECTS) flx/junks.c flx/font.c flx/cell.c flx/flx.c
	$(CXX) $(CXXFLAGS) -o $@ $^ $(CXXLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CXX) -c $(CXXFLAGS) -o $@ $<

$(CNFLIB):
	$(MAKE) -C $(CNFLIBDIR)
