//
//  main.cpp
//  VDJartnet
//
//  Created by Jonathan Tanner on 09/02/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//
//This file is part of VDJartnet.
//
//VDJartnet is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//VDJartnet is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with VDJartnet.  If not, see <http://www.gnu.org/licenses/>.

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
