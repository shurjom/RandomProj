#!/bin/sh
INPUT=${1}
OUTPUT=${2}
echo "in=${INPUT} out=${OUTPUT}"
if expr "${INPUT}" : '.*\.animation.xcf' >/dev/null; then
	${GIMP-gimp} -i -b "(let* ((output-meta-file (open-output-file \"${OUTPUT}.sprite\")) (image (car (gimp-file-load RUN-NONINTERACTIVE \"${INPUT}\" \"${INPUT}\"))) (layers (reverse (vector->list (cadr (gimp-image-get-layers image))))) (x 0) (y 0) (w (car (gimp-drawable-width (car layers)))) (h (car (gimp-drawable-height (car layers))))) (write (list (length layers) w h) output-meta-file) (close-output-port output-meta-file) (while (> (length layers) 0) (if (> (+ x w) 8192) (list (set! x 0) (set! y (+ y h)))) (gimp-layer-resize (car layers) w h 0 0) (gimp-layer-translate (car layers) x y) (set! x (+ x w)) (set! layers (cdr layers))) (gimp-image-resize-to-layers image) (file-png-save-defaults RUN-NONINTERACTIVE image (car (gimp-image-merge-visible-layers image CLIP-TO-IMAGE)) \"${OUTPUT}\" \"${OUTPUT}\"))" -b '(gimp-quit FALSE)'
else
	${GIMP-gimp} -i -b "(let* ((image (car (gimp-file-load RUN-NONINTERACTIVE \"${INPUT}\" \"${INPUT}\")))) (file-png-save-defaults RUN-NONINTERACTIVE image (car (gimp-image-merge-visible-layers image CLIP-TO-IMAGE)) \"${OUTPUT}\" \"${OUTPUT}\"))'" -b '(gimp-quit FALSE)'
fi
exit ${?}
