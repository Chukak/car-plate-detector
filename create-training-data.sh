#!/bin/bash

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
mkdir -p data/rus-car-plate-number-ground-truth
cp -r ../car-plate-detector/data/tesseract/generated/* ./data/rus-car-plate-number-ground-truth/
cp -r ../car-plate-detector/data/tesseract/text/* ./data/rus-car-plate-number-ground-truth/

# Trainig
make training MODEL_NAME=rus-car-plate-number START_MODEL=rus PSM=10 TESSDATA=/usr/share/tesseract-ocr/4.00/tessdata MAX_ITERATIONS=1000
make clean-output MODEL_NAME=rus-car-plate-number START_MODEL=rus 
