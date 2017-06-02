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
}

- (id) initWithVDJartnet:(CVDJartnet*)vdjArtnet {
    if ( self = [super init] ) {
        _window = [[NSWindow alloc] initWithContentRect:NSMakeRect(200, 200, 200, 200) styleMask:(NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable | NSWindowStyleMaskResizable | NSWindowStyleMaskTitled | NSWindowStyleMaskFullSizeContentView) backing:NSBackingStoreRetained defer:false screen:nullptr];
//        windowController = [[NSWindowController alloc] initWithWindow:_window];
//        [windowController showWindow:self];

        viewController = [[ConfigViewController alloc] initWithNibName:nil bundle:nil];
        viewController.vdjArtnet = vdjArtnet;

        [_window setReleasedWhenClosed: NO];
//        [_window setDelegate:self];
        [_window setContentView:[viewController view]];
//        [_window setContentView:[[NSView alloc] init]];
//        [_window setContentViewController:viewController];
//        [_window setTitle: @"VDJartnetConfig"];
//        [_window center];
        [_window makeKeyAndOrderFront:self];

        return self;
    } else {
        return nil;
    }
}

- (void) dealloc {
//    [_window close];
}
@end
