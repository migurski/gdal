

include ../../../GDALmake.opt

OBJ	=	ogrgrassdriver.o ogrgrassdatasource.o ogrgrasslayer.o

CPPFLAGS	:=	-DUSE_CPL -DGRASS_GISBASE=\"$(GRASS_GISBASE)\" -I.. -I../.. $(GRASS_INCLUDE) $(GDAL_INCLUDE) $(CPPFLAGS)

default:	$(O_OBJ:.o=.$(OBJ_EXT))

clean:
	rm -f *.o $(O_OBJ)
