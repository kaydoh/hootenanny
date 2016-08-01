#!/bin/bash
set -e

# Test GGDMv30 Translation

inputDir=test-files/GGDMv30
outputDir=test-output/ggdm30_translation

mkdir -p $outputDir
rm -f $outputDir/*

#  jam all of the shapefiles into one OSM file
hoot --ogr2osm $HOOT_HOME/translations/GGDMv30.js $outputDir/ggdm30.osm $inputDir/*.shp

# compareFiles. Test taken from MGCP test
hoot --is-match $outputDir/ggdm30.osm $inputDir/GGDMv30.osm || diff $outputDir/ggdm3.osm $inputDir/GGDMv30.osm

# Make shapefiles
hoot --osm2ogr $HOOT_HOME/translations/GGDMv30.js $outputDir/ggdm30.osm $outputDir".shp"

#
# This is commented out until Jenkins has python-gdal support
#
# Now look at the individual shapefiles
#for x in $inputDir/*.shp; do
#    echo $(basename $x) "  Forward"
#    $HOOT_HOME/scripts/CompareShapefiles.py  $x $outputDir/$(basename $x)
#    echo $(basename $x) "  Backward"
#    $HOOT_HOME/scripts/CompareShapefiles.py  $outputDir/$(basename $x) $x
#    echo 
#done
