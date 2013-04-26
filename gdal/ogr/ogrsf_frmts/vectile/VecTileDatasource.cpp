/******************************************************************************
 * $Id$
 *
 * Project:  
 * Purpose:  Implements VecTiles OGR driver.
 * Author:   Michal Migurski, mike@teczno.com
 *           Ragi Yaser Burhum, ragi@burhum.com
 *
 ******************************************************************************
 * Copyright (c) 2013, Michal Migurski
 * Copyright (c) 2013, Ragi Yaser Burhum
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ****************************************************************************/
#include <cpl_http.h>
#include "ogr_vectile.h"
/*
#include "ogr_geojson.h"
#include "ogrgeojsonutils.h"
#include "ogrgeojsonreader.h"
#include <jsonc/json.h> // JSON-C
*/
#include <cstddef>
#include <cstdlib>
using namespace std;

/************************************************************************/
/*                           VecTileDataSource()                        */
/************************************************************************/

VecTileDataSource::VecTileDataSource()
    : _pszName(NULL), _pszGeoData(NULL),
        _papoLayers(NULL), _nLayers(0), _fpOut(NULL),
        _flTransGeom( VecTileDataSource::eGeometryPreserve ),
        _flTransAttrs( VecTileDataSource::eAtributesPreserve ),
        _bFpOutputIsSeekable( FALSE ),
        _nBBOXInsertLocation(0)
{
    // I've got constructed. Lunch time!
}

/************************************************************************/
/*                           ~VecTileDataSource()                       */
/************************************************************************/

VecTileDataSource::~VecTileDataSource()
{
    if( NULL != _fpOut )
    {
        VSIFCloseL( _fpOut );
        _fpOut = NULL;
    }
}

/************************************************************************/
/*                           Open()                                     */
/************************************************************************/

int VecTileDataSource::Open( const char* pszName, int pUpdate )
{
    CPLAssert( NULL != pszName );

/* -------------------------------------------------------------------- */
/*      Release resources allocated during previous request.            */
/* -------------------------------------------------------------------- */
    if( NULL != _papoLayers )
    {
        CPLAssert( _nLayers > 0 );
    }
    
    CPLHTTPResult * psResult = CPLHTTPFetch( "http://tile.openstreetmap.us/vectiles-highroad/0/0/0.mvt", NULL );
    
    if (!psResult ) {
        // fail
    }
    
    printf( "%d", psResult->nStatus );
    
    return 0;
}

/************************************************************************/
/*                           GetName()                                  */
/************************************************************************/

const char* VecTileDataSource::GetName()
{
    return _pszName ? _pszName : "";
}

/************************************************************************/
/*                           GetLayerCount()                            */
/************************************************************************/

int VecTileDataSource::GetLayerCount()
{
    return _nLayers;
}

/************************************************************************/
/*                           GetLayer()                                 */
/************************************************************************/

OGRLayer* VecTileDataSource::GetLayer( int nLayer )
{
    if( 0 <= nLayer || nLayer < _nLayers )
    {
        return _papoLayers[nLayer];
    }

    return NULL;
}

/************************************************************************/
/*                           TestCapability()                           */
/************************************************************************/

int VecTileDataSource::TestCapability( const char* pszCap )
{
    return FALSE;
}

/************************************************************************/
/*                           SetGeometryTranslation()                   */
/************************************************************************/

void
VecTileDataSource::SetGeometryTranslation( GeometryTranslation type )
{
    _flTransGeom = type;
}

/************************************************************************/
/*                           SetAttributesTranslation()                 */
/************************************************************************/

void VecTileDataSource::SetAttributesTranslation( AttributesTranslation type )
{
    _flTransAttrs = type;
}

/************************************************************************/
/*                           LoadLayer()                                */
/************************************************************************/

VecTileLayer* VecTileDataSource::LoadLayer()
{
    return NULL;
}
