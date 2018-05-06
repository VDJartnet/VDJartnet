//
//  ConfigMacViewController.mm
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

#import "ConfigMacViewController.h"

@implementation ConfigViewController {
    CVDJartnet* vdjArtnet;
    
    NSScrollView* scrollView;
    ConfigIpAddress* ipAddress;
    ConfigIpPort* ipPort;
    NSTextField* ipLabel;
}

- (id)initWithVDJartnet:(CVDJartnet*)vdjArtnetTMP {
    if ( self = [super initWithNibName:nil bundle:nil] ) {
        vdjArtnet = vdjArtnetTMP;
        return self;
    } else {
        return nil;
    }
}

- (void)loadView {
    self.view = [[NSView alloc] init];

    _tableView = [[ConfigTableView alloc] initWithVDJartnet:vdjArtnet];

    scrollView = [[NSScrollView alloc] initWithFrame:NSZeroRect];
    [scrollView setDocumentView:_tableView];
    [scrollView setHasVerticalScroller:YES];
    [[self view] addSubview:scrollView];
    
    ipAddress = [[ConfigIpAddress alloc] initWithFrame:NSZeroRect VDJartnet:vdjArtnet];
    [[self view] addSubview:ipAddress];
    
    ipPort = [[ConfigIpPort alloc] initWithFrame:NSZeroRect VDJartnet:vdjArtnet];
    [[self view] addSubview:ipPort];

    ipLabel = [[NSTextField alloc] initWithFrame:NSZeroRect];
    [ipLabel setStringValue: NSLocalizedString(@"IP address:", @"")];
    [ipLabel setEditable:NO];
    [ipLabel setSelectable:NO];
    [ipLabel setBordered:NO];
    [ipLabel setBackgroundColor:[NSColor clearColor]];
    [[self view] addSubview:ipLabel];
    
    [scrollView setTranslatesAutoresizingMaskIntoConstraints:false];
    [ipAddress setTranslatesAutoresizingMaskIntoConstraints:false];
    [ipLabel setTranslatesAutoresizingMaskIntoConstraints:false];
    [ipPort setTranslatesAutoresizingMaskIntoConstraints:false];

    [[[scrollView bottomAnchor] constraintEqualToAnchor:[[self view] bottomAnchor]] setActive:true];
    [[[scrollView topAnchor] constraintEqualToAnchor:[ipAddress bottomAnchor] constant: 5] setActive:true];
    [[[scrollView leftAnchor] constraintEqualToAnchor:[[self view] leftAnchor]] setActive:true];
    [[[scrollView rightAnchor] constraintEqualToAnchor:[[self view] rightAnchor]] setActive:true];

    [[[ipAddress topAnchor] constraintEqualToAnchor:[[self view] topAnchor] constant:30] setActive:true];
    [[[ipAddress leftAnchor] constraintEqualToAnchor:[ipLabel rightAnchor]] setActive:true];
    [[[ipAddress rightAnchor] constraintEqualToAnchor:[ipPort leftAnchor]] setActive:true];
    
    [[[ipLabel centerYAnchor] constraintEqualToAnchor:[ipAddress centerYAnchor]] setActive:true];
    [[[ipLabel leftAnchor] constraintEqualToAnchor:[[self view] leftAnchor]] setActive:true];

    [[[ipPort centerYAnchor] constraintEqualToAnchor:[ipAddress centerYAnchor]] setActive:true];
    [[[ipPort rightAnchor] constraintEqualToAnchor:[[self view] rightAnchor]] setActive:true];
    [[[ipPort widthAnchor] constraintEqualToConstant:50] setActive:true];
}

- (void)viewDidLayout {
    [super viewDidLayout];
}

@end
