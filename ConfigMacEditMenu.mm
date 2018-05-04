//
//  ConfigMacEditMenu.mm
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

#import "ConfigMacEditMenu.h"

@implementation ConfigEditMenu {
    ConfigViewController* viewController;
}

- (id)initWithUndoManager:(NSUndoManager*)undoManager viewController:(ConfigViewController*)viewControllerTMP {
    if ( self = [super initWithTitle:NSLocalizedString(@"Edit", @"Edit menu title")] ) {
        viewController = viewControllerTMP;
    
        NSMenuItem* undoItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Undo", @"Undo menu item") action:@selector(undo) keyEquivalent:@"z"];
        [undoItem setTarget:undoManager];
        [undoItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        [self addItem:undoItem];
        
        NSMenuItem* redoItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Redo", @"Redo menu item") action:@selector(redo) keyEquivalent:@"z"];
        [redoItem setTarget:undoManager];
        [redoItem setKeyEquivalentModifierMask:(NSEventModifierFlagCommand | NSEventModifierFlagShift)];
        [self addItem:redoItem];
        
        NSMenuItem* copyItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Copy", @"Copy menu item") action:@selector(copyRow) keyEquivalent:@"c"];
        [copyItem setTarget:self];
        [copyItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        [self addItem:copyItem];
        
        NSMenuItem* pasteItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Paste", @"Paste menu item") action:@selector(pasteRow) keyEquivalent:@"v"];
        [pasteItem setTarget:self];
        [pasteItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        [self addItem:pasteItem];
        
        NSMenuItem* deleteItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Delete", @"Delete menu item") action:@selector(deleteRow) keyEquivalent:[NSString stringWithFormat:@"%c", NSBackspaceCharacter]];
        [deleteItem setTarget:self];
        [deleteItem setKeyEquivalentModifierMask:0];
        [self addItem:deleteItem];

        return self;
    } else {
        return nil;
    }
}

- (void) copyRow {
    [[NSPasteboard generalPasteboard] declareTypes:[NSArray<NSString*> arrayWithObject:NSStringPboardType] owner:self];
    [[NSPasteboard generalPasteboard] setString:[[viewController tableView] tableView:[viewController tableView] objectValueForTableColumn:[[viewController tableView] tableColumns][1] row:(NSInteger)[[viewController tableView] selectedRow]] forType:NSStringPboardType];
    
}

- (void) pasteRow {
    [[viewController tableView] tableView:[viewController tableView] setObjectValue:[[NSPasteboard generalPasteboard] stringForType:NSStringPboardType] forTableColumn:[[viewController tableView] tableColumns][1] row:(NSInteger)[[viewController tableView] selectedRow]];
}

- (void) deleteRow {
    [[viewController tableView] tableView:[viewController tableView] setObjectValue:@"" forTableColumn:[[viewController tableView] tableColumns][1] row:(NSInteger)[[viewController tableView] selectedRow]];
}

@end
