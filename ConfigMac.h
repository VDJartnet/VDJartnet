//
//  ConfigMac.h
//  VDJartnet
//
//  Created by Jonathan Tanner on 17/05/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigMac_h
#define ConfigMac_h

#include <stdio.h>

#include <AppKit/AppKit.h>

#ifdef __cplusplus
#include "VDJartnet.hpp"
#endif

#include "ConfigMacViewController.h"
#include "ConfigMacPresetDataSource.h"

@interface ConfigWindow : NSObject

@property (strong) NSWindow* window;
@property (strong) NSWindow* presetWindow;

- (id) initWithVDJartnet:(CVDJartnet*)vdjArtnet;
- (void) dealloc;

- (void) copy;
- (void) paste;
- (void) deleteRow;
@end

#endif /* ConfigMac_h */
