/******************************************************************************
 * $Id$
 *
 * Project:  OpenGIS Simple Features Reference Implementation
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

#include "ogr_vectile.h"
#include "cpl_conv.h"

CPL_CVSID("$Id$");

extern "C" void RegisterOGRVecTile();

/************************************************************************/
/*                            VecTileDriver()                           */
/************************************************************************/
VecTileDriver::VecTileDriver():
OGRSFDriver()
{
}

/************************************************************************/
/*                            ~VecTileDriver()                          */
/************************************************************************/
VecTileDriver::~VecTileDriver()

{
}


/************************************************************************/
/*                              GetName()                               */
/************************************************************************/

const char *VecTileDriver::GetName()

{
    return "VecTile";
}

/************************************************************************/
/*                                Open()                                */
/************************************************************************/

OGRDataSource *VecTileDriver::Open( const char* pszURL, int bUpdate )

{
    
    VecTileDataSource* pDS = new VecTileDataSource();

    if(!pDS->Open( pszURL, bUpdate ) )
    {
        delete pDS;
        return NULL;
    }
    
    printf( "VecTile says hello\n" );

    return pDS;
}

/***********************************************************************/
/*                     CreateDataSource()                              */
/***********************************************************************/

OGRDataSource* VecTileDriver::CreateDataSource( const char * conn,
                                                char **papszOptions)
{
    
    CPLError( CE_Failure, CPLE_AppDefined,
                  "Creation not supported\n" );
    return NULL;

}


/***********************************************************************/
/*                         TestCapability()                            */
/***********************************************************************/

int VecTileDriver::TestCapability( const char * pszCap )
{
    // very uncapable server

    return FALSE;
}

/***********************************************************************/
/*                       RegisterOGRVecTile()                          */
/***********************************************************************/

void RegisterOGRVecTile()

{
    if (! GDAL_CHECK_VERSION("OGR VECTILE"))
        return;
    OGRSFDriverRegistrar::GetRegistrar()->RegisterDriver( new VecTileDriver );
}
