AM_CPPFLAGS = -DPKGDATADIR=\"$(pkgdatadir)\" -Isrc
SUFFIXES = .o .rc
ACLOCAL_AMFLAGS = -Im4

bin_PROGRAMS = spacenomad

spacenomad_CXXFLAGS = $(SDL2_CFLAGS)
spacenomad_LDADD = $(SDL2_LIBS)
spacenomad_SOURCES = \
	$(RES) \
	src/block.cxx src/block.hxx \
	src/camera.cxx src/camera.hxx \
	src/field.cxx src/field.hxx \
	src/game_mode.cxx src/game_mode.hxx \
	src/main.cxx src/main.hxx \
	src/menu_mode.cxx src/menu_mode.hxx \
	src/mode.cxx src/mode.hxx \
	src/object.cxx src/object.hxx \
	src/object_block.cxx src/object_block.hxx \
	src/object_field.cxx src/object_field.hxx \
	src/planet.cxx src/planet.hxx \
	src/planet_generation/brush.cxx src/planet_generation/brush.hxx \
	src/planet_generation/planet_generation_mode.cxx src/planet_generation/planet_generation_mode.hxx \
	src/planet_generation/planet_generator.cxx src/planet_generation/planet_generator.hxx \
	src/planet_generation/planet_type.cxx src/planet_generation/planet_type.hxx \
	src/player.cxx src/player.hxx \
	src/sprite.cxx src/sprite.hxx \
	src/star_block.cxx src/star_block.hxx \
	src/star_field.cxx src/star_field.hxx \
	src/universe.cxx src/universe.hxx
if WINDRES
spacenomad_SOURCES += mainres.rc
endif

nobase_nodist_pkgdata_DATA = \
	images/favicon.png \
	images/cursor.png \
	\
	images/astronaut/space_suit.png \
	images/planet/plain.png \
	\
	images/menu/choose_bar.png \
	images/menu/main.png \
	\
	images/star_field/star_0.png \
	images/star_field/star_1.png \
	images/star_field/star_2.png 

mainres.o: mainres.rc images/favicon.ico app.manifest

.xcf.png:
	$(srcdir)/p-xcf2png.sh '$(<)' '$(@)'

if USE_INKSCAPE
.svg.png:
	$(INKSCAPE) -e '$(@)' -d $(RASTER_DPI) '$(<)'
endif
if USE_RSVG
.svg.png:
	$(RSVG) -d $(RASTER_DPI) '$(@)' 
endif

.rc.o:
	windres '$(<)' -O coff -o '$(@)'

CLEANFILES = \
	$(nobase_nodist_pkgdata_DATA) \
	$(RES)

EXTRA_DIST = \
	images/favicon.xcf \
	images/cursor.xcf \
	\
	images/astronaut/space_suit.svg \
	images/planet/plain.xcf \
	\
	images/menu/choose_bar.xcf \
	images/menu/main.xcf \
	\
	images/star_field/star_0.xcf \
	images/star_field/star_1.xcf \
	images/star_field/star_2.xcf
