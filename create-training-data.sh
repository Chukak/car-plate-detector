#!/bin/bash

CURRENT_DIR=$(pwd)
MODEL_FILENAME=rus-car-plate-number

mkdir -p /usr/share/fonts/car-plate-detector-fonts/
cp ./data/tesseract/fonts/RoadNumbers.otf /usr/share/fonts/car-plate-detector-fonts/
fc-cache -f -v
fc-list | grep "Road"

# Create a training data
cd data/tesseract/generated/
text2image --text=../text/rus.RoadNumbers.exp0.gt.txt --outputbase=rus.RoadNumbers.exp0 --font='RoadNumbers' --fonts_dir=/usr/share/fonts/car-plate-detector-fonts/
text2image --text=../text/rus.RoadNumbers.exp1.gt.txt --outputbase=rus.RoadNumbers.exp1 --font='RoadNumbers' --fonts_dir=/usr/share/fonts/car-plate-detector-fonts/
tesseract rus.RoadNumbers.exp0.tif rus.RoadNumbers.exp0 --psm 10 lstm.train
tesseract rus.RoadNumbers.exp1.tif rus.RoadNumbers.exp1 --psm 10 lstm.train
cd -

# Use tesstraint to training
cd ../ 
git clone https://github.com/tesseract-ocr/tesstrain.git || true
chmod o+w -R tesstrain
cd tesstrain
rm -rf data/$MODEL_FILENAME-ground-truth
mkdir -p data/$MODEL_FILENAME-ground-truth
cp -r $CURRENT_DIR/data/tesseract/generated/* ./data/$MODEL_FILENAME-ground-truth/
cp -r $CURRENT_DIR/data/tesseract/text/* ./data/$MODEL_FILENAME-ground-truth/

# Trainig
# see https://github.com/tesseract-ocr/tesstrain/issues/269
# run it without START_MODEL!
# /usr/share/tesseract-ocr/4.00/tessdata - for tesseract 4.00!
make training MODEL_NAME=$MODEL_FILENAME PSM=10 TESSDATA=/usr/share/tesseract-ocr/4.00/tessdata MAX_ITERATIONS=10000 #START_MODEL=rus
make clean-output MODEL_NAME=$MODEL_FILENAME # START_MODEL=rus 
cp data/$MODEL_FILENAME.traineddata $CURRENT_DIR/data/tesseract/models/
