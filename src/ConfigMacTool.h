//
//  ConfigMacTool.h
//  VDJartnet
//
//  Copyright © 2017-18 Jonathan Tanner. All rights reserved.
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
//
//Additional permission under GNU GPL version 3 section 7
//
//If you modify this Program, or any covered work, by linking or
//combining it with VirtualDJ, the licensors of this Program grant you
//additional permission to convey the resulting work.

#ifndef ConfigMacTool_h
#define ConfigMacTool_h

#include <stdio.h>

#include <CoreFoundation/CoreFoundation.h>
#include <AppKit/AppKit.h>

#ifdef __cplusplus
#include "Config.hpp"
#endif

#include "ConfigMacWindow.h"
#include "ConfigMacPresetWindow.h"

#include <functional>

/** A config tool to help the user write a correctly formatted config file */
@interface ConfigMacTool : NSObject <NSWindowDelegate>

- (id)initWithConfig:(Config*)config Destructor:(std::function<void()>)destructor; /**< Initialise a config tool with the given instance of the plugin */
- (void)dealloc; /**< Destruct the config tool */

@end

#endif /* ConfigMacTool_h */
