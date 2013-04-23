include ../../../GDALmake.opt

OBJ	=	VecTileDatasource.o VecTileDriver.o VecTileLayer.o

CPPFLAGS	:=	$(GDAL_INCLUDE) $(VECTILE_INC) $(CPPFLAGS)

default:	$(O_OBJ:.o=.$(OBJ_EXT))

clean:
	rm -f *.o $(O_OBJ)

$(O_OBJ):	ogr_vectile.h
