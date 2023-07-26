ifndef NAVISERVER
    NAVISERVER  = /usr/local/ns
endif

#
# Module name
#
MOD      =  libtbert.so

#
# Objects to build.
#
MODOBJS     = library.o

include  $(NAVISERVER)/include/Makefile.module