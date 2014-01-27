## Process this file with automake to produce Makefile.in

DOXYGEN= @DOXYGEN@

%.doc-install: %
	$(mkinstalldirs) $(DESTDIR)$(refdir)/$<
	$(install_sh) $</*  $(DESTDIR)$(refdir)/$<

SUBDIRS = tutorial

refdir="$(prefix)/share/doc/Odyssee++/Image++/reference"

all: html/index.html

html/index.html: Doxyfile
	$(DOXYGEN) $<

#install-data-local: html/index.html latex/refman.pdf html.doc-install man/man3.doc-install rtf.doc-install
install-data-local: html/index.html latex/refman.pdf html.doc-install man/man3.doc-install

latex/refman.pdf:
	cd latex; make

DISTCLEANFILES= Doxyfile
CLEANFILES= html/* man/man3/* rtf/* latex/*
