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

#ifndef _OGR_VecTile_H_INCLUDED
#define _OGR_VecTile_H_INCLUDED

#include "ogrsf_frmts.h"
#include "cpl_http.h"

#include <vector>

/************************************************************************/
/*                             VecTileLayer                          */
/************************************************************************/
class VecTileDataSource;

class VecTileLayer : public OGRLayer
{
protected:
    VecTileDataSource* poDS;

    OGRFeatureDefn*    poFeatureDefn;
    OGRSpatialReference *poSRS;

    int                nNextInSeq;

    int                iGeometryField;
    int                iLatitudeField;
    int                iLongitudeField;
    int                bHiddenGeometryField;

    OGRFeature *       GetNextRawFeature();

    int                nOffset;
    int                bEOF;
    virtual int                FetchNextRows() = 0;

    std::vector<CPLString> aosRows;

    int                 bFirstTokenIsFID;
    OGRFeature*         BuildFeatureFromSQL(const char* pszLine);

    static CPLString    LaunderColName(const char* pszColName);

  public:
                         VecTileLayer(VecTileDataSource* poDS);
                        ~VecTileLayer();

    virtual void                ResetReading();
    virtual OGRFeature *        GetNextFeature();

    virtual OGRFeatureDefn *    GetLayerDefn();

    virtual int                 TestCapability( const char * );

    virtual OGRSpatialReference*GetSpatialRef();

    virtual const char *        GetGeometryColumn();

    virtual OGRErr              SetNextByIndex( long nIndex );

    const char *        GetDefaultGeometryColumnName() { return "geometry"; }

    static int                  ParseCSVResponse(char* pszLine,
                                                 std::vector<CPLString>& aosRes);
    static CPLString            PatchSQL(const char* pszSQL);

    int                         GetGeometryFieldIndex() { return iGeometryField; }
    int                         GetLatitudeFieldIndex() { return iLatitudeField; }
    int                         GetLongitudeFieldIndex() { return iLongitudeField; }

    int                         GetFeaturesToFetch() { return atoi(CPLGetConfigOption("VecTile_PAGE_SIZE", "500")); }
};

/************************************************************************/
/*                         VecTileTableLayer                         */
/************************************************************************/

class VecTileTableLayer : public VecTileLayer
{
    CPLString         osTableName;
    CPLString         osTableId;
    CPLString         osGeomColumnName;

    int                bHasTriedCreateTable;
    void               CreateTableIfNecessary();

    CPLString           osWHERE;
    CPLString           osQuery;

    void                BuildWhere(void);

    CPLString          osTransaction;
    int                bInTransaction;
    int                nFeaturesInTransaction;

    int                FetchDescribe();
    virtual int                FetchNextRows();

    OGRwkbGeometryType eGTypeForCreation;

    std::vector<CPLString>  aosColumnInternalName;

    public:
            VecTileTableLayer(VecTileDataSource* poDS,
                                 const char* pszTableName,
                                 const char* pszTableId = "",
                                 const char* pszGeomColumnName = "");
            ~VecTileTableLayer();

    virtual void                ResetReading();

    virtual OGRFeatureDefn *    GetLayerDefn();

    virtual const char *        GetName() { return osTableName.c_str(); }
    virtual int         GetFeatureCount( int bForce = TRUE );

    virtual OGRFeature *        GetFeature( long nFID );

    virtual void        SetSpatialFilter( OGRGeometry * );
    virtual OGRErr      SetAttributeFilter( const char * );

    virtual OGRErr      CreateField( OGRFieldDefn *poField,
                                     int bApproxOK = TRUE );
    virtual OGRErr      CreateFeature( OGRFeature *poFeature );
    virtual OGRErr      SetFeature( OGRFeature *poFeature );
    virtual OGRErr      DeleteFeature( long nFID );

    virtual OGRErr      StartTransaction();
    virtual OGRErr      CommitTransaction();
    virtual OGRErr      RollbackTransaction();

    const CPLString&            GetTableId() const { return osTableId; }

    virtual int                 TestCapability( const char * );

    void                SetGeometryType(OGRwkbGeometryType eGType);
};

/************************************************************************/
/*                        VecTileResultLayer                         */
/************************************************************************/

class VecTileResultLayer : public VecTileLayer
{
    CPLString   osSQL;
    int         bGotAllRows;

    virtual int                FetchNextRows();

    public:
            VecTileResultLayer(VecTileDataSource* poDS,
                                  const char* pszSQL);
            ~VecTileResultLayer();

    virtual void                ResetReading();

    int     RunSQL();
};

/************************************************************************/
/*                           VecTileDataSource                       */
/************************************************************************/

class VecTileDataSource : public OGRDataSource
{
    char*               pszName;

    OGRLayer**          papoLayers;
    int                 nLayers;

    int                 bReadWrite;

    int                 bUseHTTPS;

    CPLString           osAuth;
    CPLString           osAccessToken;
    CPLString           osRefreshToken;
    CPLString           osAPIKey;

    void                DeleteLayer( const char *pszLayerName );

    int                 bMustCleanPersistant;

    static CPLStringList ParseSimpleJson(const char *pszJSon);

  public:
        VecTileDataSource();
        ~VecTileDataSource();

    int                 Open( const char * pszFilename,
                              int bUpdate );

    virtual const char* GetName() { return pszName; }

    virtual int         GetLayerCount() { return nLayers; }
    virtual OGRLayer*   GetLayer( int );
    virtual OGRLayer    *GetLayerByName(const char *);

    virtual int         TestCapability( const char * );

    virtual OGRLayer   *CreateLayer( const char *pszName,
                                     OGRSpatialReference *poSpatialRef = NULL,
                                     OGRwkbGeometryType eGType = wkbUnknown,
                                     char ** papszOptions = NULL );
    virtual OGRErr      DeleteLayer(int);

    virtual OGRLayer*  ExecuteSQL( const char *pszSQLCommand,
                                   OGRGeometry *poSpatialFilter,
                                   const char *pszDialect );
    virtual void       ReleaseResultSet( OGRLayer * poLayer );

    const CPLString&            GetAccessToken() const { return osAccessToken;}
    const char*                 GetAPIURL() const;
    int                         IsReadWrite() const { return bReadWrite; }
    char**                      AddHTTPOptions(char** papszOptions = NULL);
    CPLHTTPResult*              RunSQL(const char* pszUnescapedSQL);
};

/************************************************************************/
/*                             VecTileDriver                         */
/************************************************************************/

class VecTileDriver : public OGRSFDriver
{
  public:

    VecTileDriver();
    ~VecTileDriver();

    virtual const char*         GetName();
    virtual OGRDataSource*      Open( const char *, int );
    virtual OGRDataSource*      CreateDataSource( const char * pszName,
                                                  char **papszOptions );
    virtual int                 TestCapability( const char * );
};

char **VecTileCSVSplitLine( const char *pszString, char chDelimiter );
char* VecTileGotoNextLine(char* pszData);

#endif /* ndef _OGR_VecTile_H_INCLUDED */
