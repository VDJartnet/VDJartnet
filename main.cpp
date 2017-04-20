//
//  main.cpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 09/02/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#include "VDJartnet.hpp"

// This is the standard DLL loader for COM object.

HRESULT VDJ_API DllGetClassObject(const GUID &rclsid,const GUID &riid,void** ppObject)
{
    if (memcmp(&rclsid,&CLSID_VdjPlugin8,sizeof(GUID))==0 && memcmp(&riid,&IID_IVdjPluginBasic8,sizeof(GUID))==0)
        {
        *ppObject=new CVDJartnet();
        }
    else
        {
        return CLASS_E_CLASSNOTAVAILABLE;
        }
    
    return NO_ERROR;
}
