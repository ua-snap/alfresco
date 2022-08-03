# Using historical fire data

Historical fire polygon data can be downloaded from the [Maps/Imagery/Geospatial page of the AICC website](https://fire.ak.blm.gov/predsvcs/maps.php) as a file geodatabase (see "AlaskaFireHistory_Polygons_1940_*"). Polygons from each year of the file geodatabase can be extracted, rasterized, and used as historical fire inputs for ALFRESCO.

The following Bash command will created GeoTIFFs from the AICC fire history polygons geodatabase for each year from 1950 to 2021, for example:

```
for i in {1950..2021}; do
  gdal_rasterize AlaskaFireHistory_Polygons.gdb \
  AlaskaFireHistory_${i}.tif \
  -a_srs EPSG:3338 -burn 255 -ts 5528 2223 \
  -te -1725223.205807 321412.933 3802776.794 2544412.932644 \
  -ot Byte -sql "SELECT * FROM AK_fire_location_polygons WHERE FIREYEAR = '${i}'"; \
done
```

To input these GeoTIFFs into ALFRESCO, make sure the historical transition type and year are set properly in the JSON configuration, and the historical fire GeoTIFF base path is set. For example:

```
{
  ...
  "Fire": {
    "Types": ["SPATIAL", "HISTORICAL", "SPATIAL"],
    "TypeTransitionYears": [1000, 1950, 2022],
    "Historical": ["", "/path/to/AlaskaFireHistory.tif", ""],
    ...
  }
  ...
}
```