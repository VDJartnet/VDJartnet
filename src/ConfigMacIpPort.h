//
//  ConfigMacIpPort.h
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

#include <AppKit/AppKit.h>
#ifdef __cplusplus
#include "Config.hpp"
#endif

/** A port field */
@interface ConfigMacIpPort : NSTextField <NSTextFieldDelegate>

- (id) initWithFrame:(CGRect)frame Config:(Config*)configTMP; /**< Initialise an IP address field with the given frame and instance of the plugin */
- (BOOL)control:(NSControl*)control textShouldEndEditing:(NSText*)fieldEditor; /**< The text has been edited */

@end
