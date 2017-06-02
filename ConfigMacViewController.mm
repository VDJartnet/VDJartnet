//
//  ConfigMacViewController.m
//  VDJartnet
//
//  Created by Jonathan Tanner on 02/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#import "ConfigMacViewController.h"

@implementation ConfigViewController {
    NSScrollView* scrollView;
}

- (void)loadView {
    self.view = [[NSView alloc] init];

    _tableView = [[NSTableView alloc] init];
    [_tableView addTableColumn:[[NSTableColumn alloc] initWithIdentifier:@"Channel"]];
    [_tableView addTableColumn:[[NSTableColumn alloc] initWithIdentifier:@"VDJscript"]];
    [[_tableView tableColumns][0] setTitle:@"Channel"];
    [[_tableView tableColumns][0] setEditable:NO];
    [[_tableView tableColumns][1] setTitle:@"VDJscript"];
    [_tableView setDataSource:self];
    [_tableView reloadData];

    scrollView = [[NSScrollView alloc] initWithFrame:CGRectMake(0,0,100,100)];
    [scrollView setDocumentView:_tableView];
    [scrollView setHasVerticalScroller:YES];
    [[self view] addSubview:scrollView];
}

- (void)viewDidLayout {
    [super viewDidLayout];

    CGPoint origin = [[self view] frame].origin;
    CGSize size = [[[self view] window] frame].size;
    [scrollView setFrame:CGRectMake(origin.x, origin.y, size.width, size.height)];
    [[_tableView tableColumns][0] setWidth:50];
    [[_tableView tableColumns][1] setWidth:[[[self view] window] frame].size.width - [[_tableView tableColumns][0] width] - 10];
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
    return 512;
}

- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    if ([[tableColumn identifier]  isEqual: @"Channel"]) {
        return [[NSString alloc] initWithFormat:@"%ld", row + 1 ];
    } else if ([[tableColumn identifier]  isEqual: @"VDJscript"]) {
        return [NSString stringWithCString:_vdjArtnet->channelCommands[row].c_str() encoding:[NSString defaultCStringEncoding]];
    } else {
        return @"";
    }
}

- (void)tableView:(NSTableView *)tableView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {
    _vdjArtnet->channelCommands[row] = [object cStringUsingEncoding:[NSString defaultCStringEncoding]];
    _vdjArtnet->OnParameter(CVDJartnet::ID_SAVE);
}

@end
