# Makefile for woodstock
# $Id: $

CC = gcc
LIBS  = -ldl
CFLAGS = -O2 -W -Wall -g

all: woodstock.so 

woodstock.so:  woodstock.c woodstock.h
			$(CC) $(CFLAGS) -shared -fPIC -fomit-frame-pointer woodstock.c -owoodstock.so $(LIBS)
install: all 
			install -d -m 755 $(DESTDIR)/lib
			install -m 755 woodstock.so $(DESTDIR)/lib
clean:
			rm -f woodstock.so
