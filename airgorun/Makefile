.POSIX:
.PHONY: all clean
.SUFFIXES: .cxx .o .xcf .png .svg .rc .res .exx

-include windurrs.mk
SDL2_PKG = sdl2 SDL2_image SDL2_ttf

LIBS = $$(pkg-config --libs $(SDL2_PKG))
MY_CXXFLAGS = -Wall -std=c++0x $$(pkg-config --cflags $(SDL2_PKG))
EXEEXT = .exe
OBJ = \
	src/game_mode.o \
	src/main.o \
	src/menu_mode.o \
	src/mode.o \
	src/sprite.o

RASTERS = \
	images/favicon.png \
	\
	images/menu/menu_back.png \
	images/menu/choose_bar.png
	

MY_GIMP = $${GIMP-gimp}

all: testsdl$(EXEEXT) $(RASTERS)
testsdl$(EXEEXT): $(OBJ) $(RES) windurrs.mk
	./ecommand.sh $(CXX) $(MY_CXXFLAGS) $(CXXFLAGS) -o '$(@)' $(OBJ) $(RES) $(LIBS)

windurrs.mk: Makefile
	echo > windurrs.mk
	n="$$(which ls)"; test "$${n##*ls}" = ".exe" && echo 'RES = main.res' >> windurrs.mk
main.res: main.rc images/favicon.ico

.cxx.o:
	./ecommand.sh $(CXX) -c $(MY_CXXFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o '$(@)' '$(<)'
.cxx.exx:
	./ecommand.sh $(CXX) -E $(MY_CXXFLAGS) $(CXXFLAGS) $(CPPFLAGS) -o '$(@)' '$(<)'
.xcf.png:
	if [[ '$(<)' == *.animation.xcf ]]; then \
		$(MY_GIMP) -i -b "(let* ((output-meta-file (open-output-file \"$(@).sprite\")) (image (car (gimp-file-load RUN-NONINTERACTIVE \"$(<)\" \"$(<)\"))) (layers (reverse (vector->list (cadr (gimp-image-get-layers image))))) (x 0) (y 0) (w (car (gimp-drawable-width (car layers)))) (h (car (gimp-drawable-height (car layers))))) (write (list (length layers) w h) output-meta-file) (close-output-port output-meta-file) (while (> (length layers) 0) (if (> (+ x w) 8192) (list (set! x 0) (set! y (+ y h)))) (gimp-layer-resize (car layers) w h 0 0) (gimp-layer-translate (car layers) x y) (set! x (+ x w)) (set! layers (cdr layers))) (gimp-image-resize-to-layers image) (file-png-save-defaults RUN-NONINTERACTIVE image (car (gimp-image-merge-visible-layers image CLIP-TO-IMAGE)) \"$(@)\" \"$(@)\"))" -b '(gimp-quit FALSE)'; \
	else \
		$(MY_GIMP) -i -b '(let* ((image (car (gimp-file-load RUN-NONINTERACTIVE "$(<)" "$(<)")))) (file-png-save-defaults RUN-NONINTERACTIVE image (car (gimp-image-merge-visible-layers image CLIP-TO-IMAGE)) "$(@)" "$(@)"))' -b '(gimp-quit FALSE)'; \
	fi
.svg.png:
	inkscape -e '$(@)' -d 120 '$(<)'
.rc.res:
	windres '$(<)' -O coff -o '$(@)'

$(OBJ) $(RASTERS): Makefile

clean:
	rm -f $(OBJ) $(RASTERS) $(RES) testsdl$(EXEEXT)

