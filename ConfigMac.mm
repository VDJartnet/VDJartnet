//
//  ConfigMac.mm
//  VDJartnet
//
//  Created by Jonathan Tanner on 17/05/2017.
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
//
//Additional permission under GNU GPL version 3 section 7
//
//If you modify this Program, or any covered work, by linking or
//combining it with VirtualDJ, the licensors of this Program grant you
//additional permission to convey the resulting work.

#include "ConfigMac.h"

@implementation ConfigWindow {
//    NSWindowController* windowController;
    ConfigViewController* viewController;

    ConfigTableView* presetTableView;
    NSScrollView* presetScrollView;
    ConfigPresetDataSource* presetDataSource;

    NSMenuItem* editMenuItem;
}

- (id) initWithVDJartnet:(CVDJartnet*)vdjArtnet {
    if ( self = [super init] ) {
        _window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 600, 600) styleMask:(NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView) backing:NSBackingStoreBuffered defer:false screen:nullptr];
        _presetWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(600, 0, 200, 600) styleMask:(NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView) backing:NSBackingStoreBuffered defer:false screen:nullptr];
//        windowController = [[NSWindowController alloc] initWithWindow:_window];
//        [windowController showWindow:self];

        viewController = [[ConfigViewController alloc] initWithNibName:nil bundle:nil];
        viewController.vdjArtnet = vdjArtnet;

        presetTableView = [[ConfigTableView alloc] initWithFrame:[_presetWindow frame]];
        [presetTableView addTableColumn:[[NSTableColumn alloc] initWithIdentifier:@"Names"]];
        [[presetTableView tableColumns][0] setTitle:NSLocalizedString(@"Name", @"Name of preset")];
        [[presetTableView tableColumns][0] setEditable:NO];
        [presetTableView setHeaderView:nil];

        presetDataSource = [[ConfigPresetDataSource alloc] initWithVDJartnet:vdjArtnet];
        [presetTableView setDataSource:presetDataSource];

        [presetTableView setDraggingSourceOperationMask:NSDragOperationCopy forLocal:YES];
        [presetTableView setDraggingSourceOperationMask:NSDragOperationCopy forLocal:NO];

        presetScrollView = [[NSScrollView alloc] initWithFrame:[_presetWindow frame]];
        [presetScrollView setDocumentView:presetTableView];
        [presetScrollView setHasVerticalScroller:YES];

        [NSWindow setAllowsAutomaticWindowTabbing: NO];

        [_presetWindow setReleasedWhenClosed: NO];
        [_presetWindow setContentView:presetScrollView];
        [_presetWindow setTitle: NSLocalizedString(@"Presets", @"Presets window title")];
        [_presetWindow makeKeyAndOrderFront:self];
        
        [_window setReleasedWhenClosed: NO];
        [_window setContentView:[viewController view]];
        [_window setDelegate:viewController];
        [_window setTitle: NSLocalizedString(@"VDJartnetConfig", @"Config window title")];
        [_window makeKeyAndOrderFront:self];

        NSMenu* editMenu = [[NSMenu alloc] initWithTitle:NSLocalizedString(@"Edit", @"Edit menu title")];

        NSMenuItem* undoItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Undo", @"Undo menu item") action:@selector(undo) keyEquivalent:@"z"];
        [undoItem setTarget:[_window undoManager]];
        [undoItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        [editMenu addItem:undoItem];

        NSMenuItem* redoItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Redo", @"Redo menu item") action:@selector(redo) keyEquivalent:@"z"];
        [redoItem setTarget:[_window undoManager]];
        [redoItem setKeyEquivalentModifierMask:(NSEventModifierFlagCommand | NSEventModifierFlagShift)];
        [editMenu addItem:redoItem];

        NSMenuItem* copyItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Copy", @"Copy menu item") action:@selector(copyRow) keyEquivalent:@"c"];
        [copyItem setTarget:self];
        [copyItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        [editMenu addItem:copyItem];

        NSMenuItem* pasteItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Paste", @"Paste menu item") action:@selector(pasteRow) keyEquivalent:@"v"];
        [pasteItem setTarget:self];
        [pasteItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        [editMenu addItem:pasteItem];

        NSMenuItem* deleteItem = [[NSMenuItem alloc] initWithTitle:NSLocalizedString(@"Delete", @"Delete menu item") action:@selector(deleteRow) keyEquivalent:[NSString stringWithFormat:@"%c", NSBackspaceCharacter]];
        [deleteItem setTarget:self];
        [deleteItem setKeyEquivalentModifierMask:0];
        [editMenu addItem:deleteItem];

        editMenuItem = [[[NSApplication sharedApplication] mainMenu] addItemWithTitle:NSLocalizedString(@"Edit", @"Edit menu title") action:nil keyEquivalent:@""];
        [editMenuItem setSubmenu:editMenu];

        return self;
    } else {
        return nil;
    }
}

- (void) dealloc {
    [_presetWindow close];
    [_window close];
    [[[NSApplication sharedApplication] mainMenu] removeItem:editMenuItem];
}

- (void) copyRow {
    [[NSPasteboard generalPasteboard] declareTypes:[NSArray<NSString*> arrayWithObject:NSStringPboardType] owner:self];
    [[NSPasteboard generalPasteboard] setString:[viewController tableView:[viewController tableView] objectValueForTableColumn:[[viewController tableView] tableColumns][1] row:(NSInteger)[[viewController tableView] selectedRow]] forType:NSStringPboardType];

}

- (void) pasteRow {
    [viewController tableView:[viewController tableView] setObjectValue:[[NSPasteboard generalPasteboard] stringForType:NSStringPboardType] forTableColumn:[[viewController tableView] tableColumns][1] row:(NSInteger)[[viewController tableView] selectedRow]];
}

- (void) deleteRow {
    [viewController tableView:[viewController tableView] setObjectValue:@"" forTableColumn:[[viewController tableView] tableColumns][1] row:(NSInteger)[[viewController tableView] selectedRow]];
}

@end
