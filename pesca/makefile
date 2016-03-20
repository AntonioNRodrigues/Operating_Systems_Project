#
# Makefile - makefile for SO1516 project, group #33
# Pedro Cândido n.º 15674
# António Rodrigues n.º 40853
#
GG=33
VV=01
AAAAMMDD=`date +"%Y%m%d"`

CC=gcc
CPP=cpp

# directories for source, header, object and binary files
srcdir = src
incdir = include
objdir = obj
bindir = bin
 
src := $(wildcard src/*.c)  # project .c source files
dep := $(src:.c=.d)  # one .d dependency file per .c source
_obj := $(src:.c=.o)
obj  := $(patsubst src/%,obj/%,$(_obj))  # project object files

# compiler and linker flags
LDFLAGS = -lm -lpthread -lrt
CFLAGS = -iquote $(incdir) 

# target to build project executable
$(bindir)/pesca: $(obj)
	$(CC) -o $@ $^ $(LDFLAGS) 

# target to buid object files
$(objdir)/%.o: $(srcdir)/%.c
	$(CC) $(CFLAGS) -o $@ -c $<

# include dependency files in makefile
-include $(dep)

# target to build dependency files (using C preprocessor)
%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(objdir)/$(@F:.d=.o) >$@

# target to produce a zip file with the project source files
archive:
	zip -r pesca$(GG)$(VV)$(AAAAMMDD).zip src/*.c include 

# remove object files and executable
.PHONY: clean
clean:
	rm -f $(obj) $(bindir)/pesca

# remove dependency files
.PHONY: cleandep
cleandep:
	rm -f $(dep)
