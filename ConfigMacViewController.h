//
//  ConfigMacViewController.h
//  VDJartnet
//
//  Created by Jonathan Tanner on 02/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigMacViewController_h
#define ConfigMacViewController_h

#include <AppKit/AppKit.h>
#ifdef __cplusplus
#include "VDJartnet.hpp"
#endif

@interface ConfigViewController : NSViewController <NSTableViewDataSource>

@property CVDJartnet* vdjArtnet;
@property (strong) NSTableView* tableView;

- (void)loadView;
- (void)viewDidLayout;

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView;
- (id)tableView:(NSTableView *)tableView objectValueForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row;
- (void)tableView:(NSTableView *)tableView setObjectValue:(id)object forTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row;
@end

#endif /* ConfigMacViewController_h */
