/*
 * Implementation of the Microsoft Installer (msi.dll)
 *
 * Copyright 2005 Mike McCormack for CodeWeavers
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include <stdarg.h>

#include "windef.h"
#include "winbase.h"
#include "winreg.h"
#include "winnls.h"
#include "shlwapi.h"
#include "wine/debug.h"
#include "msi.h"
#include "msipriv.h"

WINE_DEFAULT_DEBUG_CHANNEL(msi);

static void MSI_ClosePreview( MSIOBJECTHDR *arg )
{
    MSIPREVIEW *preview = (MSIPREVIEW *) arg;

    msiobj_release( &preview->db->hdr );
}

MSIPREVIEW *MSI_EnableUIPreview( MSIDATABASE *db )
{
    MSIPREVIEW *preview;

    preview = alloc_msiobject( MSIHANDLETYPE_PREVIEW, sizeof (MSIPREVIEW),
                               MSI_ClosePreview );
    if( preview )
    {
        preview->db = db;
        msiobj_addref( &db->hdr );
    }
    return preview;
}

UINT WINAPI MsiEnableUIPreview( MSIHANDLE hdb, MSIHANDLE* phPreview )
{
    MSIDATABASE *db;
    MSIPREVIEW *preview;
    UINT r = ERROR_FUNCTION_FAILED;

    TRACE("%ld %p\n", hdb, phPreview);

    db = msihandle2msiinfo( hdb, MSIHANDLETYPE_DATABASE );
    if( !db )
        return ERROR_INVALID_HANDLE;
    preview = MSI_EnableUIPreview( db );
    if( preview )
    {
        *phPreview = alloc_msihandle( &preview->hdr );
        msiobj_release( &preview->hdr );
        r = ERROR_SUCCESS;
    }
    msiobj_release( &db->hdr );

    return r;
}

UINT WINAPI MsiPreviewDialogW( MSIHANDLE hPreview, LPCWSTR szDialogName )
{
    FIXME("%ld %s\n", hPreview, debugstr_w(szDialogName));
    return ERROR_CALL_NOT_IMPLEMENTED;
}

UINT WINAPI MsiPreviewDialogA( MSIHANDLE hPreview, LPCSTR szDialogName )
{
    UINT r, len;
    LPWSTR strW = NULL;

    TRACE("%ld %s\n", hPreview, debugstr_a(szDialogName));

    if( szDialogName )
    {
        len = MultiByteToWideChar( CP_ACP, 0, szDialogName, -1, NULL, 0 );
        strW = HeapAlloc( GetProcessHeap(), 0, len*sizeof(WCHAR) );
        MultiByteToWideChar( CP_ACP, 0, szDialogName, -1, strW, len );
    }
    r = MsiPreviewDialogW( hPreview, strW );
    HeapFree( GetProcessHeap(), 0, strW );
    return r;
}

UINT WINAPI MsiPreviewBillboardW( MSIHANDLE hPreview, LPCWSTR szControlName,
                                  LPCWSTR szBillboard)
{
    FIXME("%ld %s %s\n", hPreview, debugstr_w(szControlName),
          debugstr_w(szBillboard));
    return ERROR_CALL_NOT_IMPLEMENTED;
}

UINT WINAPI MsiPreviewBillboardA( MSIHANDLE hPreview, LPCSTR szControlName,
                                  LPCSTR szBillboard)
{
    FIXME("%ld %s %s\n", hPreview, debugstr_a(szControlName),
          debugstr_a(szBillboard));
    return ERROR_CALL_NOT_IMPLEMENTED;
}
