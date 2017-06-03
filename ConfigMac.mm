//
//  ConfigMac.mm
//  VDJartnet
//
//  Created by Jonathan Tanner on 17/05/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

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
        _window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0, 0, 600, 600) styleMask:(NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView) backing:NSBackingStoreRetained defer:false screen:nullptr];
        _presetWindow = [[NSWindow alloc] initWithContentRect:NSMakeRect(600, 0, 200, 600) styleMask:(NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView) backing:NSBackingStoreRetained defer:false screen:nullptr];
//        windowController = [[NSWindowController alloc] initWithWindow:_window];
//        [windowController showWindow:self];

        viewController = [[ConfigViewController alloc] initWithNibName:nil bundle:nil];
        viewController.vdjArtnet = vdjArtnet;

        presetTableView = [[ConfigTableView alloc] initWithFrame:[_presetWindow frame]];
        [presetTableView addTableColumn:[[NSTableColumn alloc] initWithIdentifier:@"Names"]];
        [[presetTableView tableColumns][0] setTitle:@"Name"];
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
        [_presetWindow setTitle: @"presets"];
        [_presetWindow makeKeyAndOrderFront:self];
        
        [_window setReleasedWhenClosed: NO];
        [_window setContentView:[viewController view]];
        [_window setDelegate:viewController];
        [_window setTitle: @"VDJartnetConfig"];
        [_window makeKeyAndOrderFront:self];

        NSMenu* editMenu = [[NSMenu alloc] initWithTitle:@"Edit"];

        NSMenuItem* undoItem = [[NSMenuItem alloc] initWithTitle:@"Undo" action:@selector(undo) keyEquivalent:@"z"];
        [undoItem setTarget:[_window undoManager]];
        [undoItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        [editMenu addItem:undoItem];

        NSMenuItem* redoItem = [[NSMenuItem alloc] initWithTitle:@"Redo" action:@selector(redo) keyEquivalent:@"z"];
        [redoItem setTarget:[_window undoManager]];
        [redoItem setKeyEquivalentModifierMask:(NSEventModifierFlagCommand | NSEventModifierFlagShift)];
        [editMenu addItem:redoItem];

        NSMenuItem* copyItem = [[NSMenuItem alloc] initWithTitle:@"Copy" action:@selector(copy) keyEquivalent:@"c"];
        [copyItem setTarget:self];
        [copyItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        [editMenu addItem:copyItem];

        NSMenuItem* pasteItem = [[NSMenuItem alloc] initWithTitle:@"Paste" action:@selector(paste) keyEquivalent:@"v"];
        [pasteItem setTarget:self];
        [pasteItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand];
        [editMenu addItem:pasteItem];

        NSMenuItem* deleteItem = [[NSMenuItem alloc] initWithTitle:@"Delete" action:@selector(deleteRow) keyEquivalent:[NSString stringWithFormat:@"%c", NSBackspaceCharacter]];
        [deleteItem setTarget:self];
        [deleteItem setKeyEquivalentModifierMask:0];
        [editMenu addItem:deleteItem];

        editMenuItem = [[[NSApplication sharedApplication] mainMenu] addItemWithTitle:@"Edit" action:nil keyEquivalent:@""];
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

- (void) copy {
    [[NSPasteboard generalPasteboard] declareTypes:[NSArray<NSString*> arrayWithObject:NSStringPboardType] owner:self];
    [[NSPasteboard generalPasteboard] setString:[viewController tableView:[viewController tableView] objectValueForTableColumn:[[viewController tableView] tableColumns][1] row:[[viewController tableView] selectedRow]] forType:NSStringPboardType];

}

- (void) paste {
    [viewController tableView:[viewController tableView] setObjectValue:[[NSPasteboard generalPasteboard] stringForType:NSStringPboardType] forTableColumn:[[viewController tableView] tableColumns][1] row:[[viewController tableView] selectedRow]];
}

- (void) deleteRow {
    [viewController tableView:[viewController tableView] setObjectValue:@"" forTableColumn:[[viewController tableView] tableColumns][1] row:[[viewController tableView] selectedRow]];
}

@end
