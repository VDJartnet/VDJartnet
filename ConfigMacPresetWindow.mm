//
//  ConfigMacPresetWindow.mm
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

#import "ConfigMacPresetWindow.h"

@implementation ConfigPresetWindow {
    CVDJartnet* vdjArtnet;
    
    NSTableView* presetTableView;
    NSScrollView* presetScrollView;
    ConfigPresetDataSource* presetDataSource;
}

- (id) initWithVDJartnet:(CVDJartnet*)vdjArtnetTMP {
    if ( self = [super initWithContentRect:NSMakeRect(600, 0, 200, 600) styleMask:(NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView) backing:NSBackingStoreBuffered defer:false screen:nullptr] ) {
        
        vdjArtnet = vdjArtnetTMP;
        
        presetTableView = [[NSTableView alloc] initWithFrame:[self frame]];
        [presetTableView addTableColumn:[[NSTableColumn alloc] initWithIdentifier:@"Names"]];
        [[presetTableView tableColumns][0] setTitle:NSLocalizedString(@"Name", @"Name of preset")];
        [[presetTableView tableColumns][0] setEditable:NO];
        [presetTableView setHeaderView:nil];
        
        presetDataSource = [[ConfigPresetDataSource alloc] initWithVDJartnet:vdjArtnet];
        [presetTableView setDataSource:presetDataSource];
        
        [presetTableView setDraggingSourceOperationMask:NSDragOperationCopy forLocal:YES];
        [presetTableView setDraggingSourceOperationMask:NSDragOperationCopy forLocal:NO];
        
        presetScrollView = [[NSScrollView alloc] initWithFrame:[self frame]];
        [presetScrollView setDocumentView:presetTableView];
        [presetScrollView setHasVerticalScroller:YES];
        
        [self setReleasedWhenClosed: NO];
        [self setContentView:presetScrollView];
        [self setTitle: NSLocalizedString(@"Presets", @"Presets window title")];
        [[self standardWindowButton:NSWindowCloseButton] setHidden:YES];
        [[self standardWindowButton:NSWindowMiniaturizeButton] setHidden:YES];
        [[self standardWindowButton:NSWindowZoomButton] setHidden:YES];
        [self orderFront:self];
        
        return self;
    } else {
        return nil;
    }
}

@end
