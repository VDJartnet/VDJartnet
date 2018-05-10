//
//  ConfigMacWindow.mm
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

#import "ConfigMacWindow.h"

@implementation ConfigMacWindow {
    Config* config; /**< A pointer to the config */
    std::function<void()> destructor; /**< A function to be called to close the window */

    ConfigMacViewController* viewController; /**< The view controller */
    NSMenuItem* editMenuItem; /**< The "Edit" menu */
}

- (id) initWithConfig:(Config*)configTMP Destructor:(std::function<void()>)destructorTMP {
    if ( self = [super initWithContentRect:NSMakeRect(0, 0, 600, 600) styleMask:(NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView) backing:NSBackingStoreBuffered defer:false screen:nullptr] ) {
        
        config = configTMP;
        destructor = destructorTMP;
        
        viewController = [[ConfigMacViewController alloc] initWithConfig:config];
                
        [self setReleasedWhenClosed: NO];
        [self setContentView:[viewController view]];
        [self setDelegate:self];
        [self setTitle: NSLocalizedString(@"VDJartnetConfig", @"Config window title")];
        [self makeKeyAndOrderFront:self];
        
        ConfigMacEditMenu* editMenu = [[ConfigMacEditMenu alloc] initWithUndoManager:[self undoManager] viewController:viewController];
        editMenuItem = [[[NSApplication sharedApplication] mainMenu] addItemWithTitle:NSLocalizedString(@"Edit", @"Edit menu title") action:nil keyEquivalent:@""];
        [editMenuItem setSubmenu:editMenu];
        
        return self;
    } else {
        return nil;
    }
}

- (void)windowWillClose:(NSNotification*)notification {
    for (NSWindow* childWindow in [self childWindows]) {
        [childWindow performClose:self];
    }
    destructor();
}

- (void)dealloc {
    [[[NSApplication sharedApplication] mainMenu] removeItem:editMenuItem];
}

@end
