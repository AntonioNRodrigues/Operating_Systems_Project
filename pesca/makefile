#
# Makefile - makefile for SO1516 project, group #33
# Pedro Cândido n.º 15674
# António Rodrigues n.º 40853
#
GG=33
VV=02
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
__obj := $(src:.c=.o)
_obj  := $(patsubst src/%,obj/%,$(__obj))  # project object files
obj  := $(subst obj/logplayer.o,,$(_obj))  # project object files

# compiler and linker flags
LDFLAGS = -lm -lpthread -lrt
CFLAGS = -iquote $(incdir) 

all: $(bindir)/logplayer $(bindir)/pesca

# target to build project executable
$(bindir)/logplayer: $(objdir)/logplayer.o 
	mkdir -p $(bindir); $(CC) -o $@ $^ $(LDFLAGS) 

# target to build project executable
$(bindir)/pesca: $(obj)
	mkdir -p $(bindir); $(CC) -o $@ $^ $(LDFLAGS) 

# target to buid object files
$(objdir)/%.o: $(srcdir)/%.c
	mkdir -p $(objdir); $(CC) $(CFLAGS) -o $@ -c $<

# include dependency files in makefile
-include $(dep)

# target to build dependency files (using C preprocessor)
%.d: %.c
	@$(CPP) $(CFLAGS) $< -MM -MT $(objdir)/$(@F:.d=.o) >$@

# target to produce a zip file with the project source files
archive:
	zip -r pesca$(GG)$(VV)$(AAAAMMDD).zip src/*.c include makefile

# remove object files and executable
.PHONY: clean
clean:
	rm -f $(obj) $(bindir)/pesca $(bindir)/logplayer

# remove dependency files
.PHONY: cleandep
cleandep:
	rm -f $(dep)
