//
//  ConfigMacTableView.h
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

#ifndef ConfigMacTableView_h
#define ConfigMacTableView_h

#include <AppKit/AppKit.h>
#ifdef __cplusplus
#include "VDJartnet.hpp"
#endif

/** A list of commands */
@interface ConfigMacTableView : NSTableView <NSTableViewDataSource, NSTableViewDelegate>

- (id)initWithVDJartnet:(CVDJartnet*)vdjArtnetTMP; /**< Initialise a list of commands with the given instance of the plugin */

- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)localDestination; /**< Get which dragging operations are supported */

- (NSInteger)numberOfRowsInTableView:(NSTableView*)tableView; /**< The number of commands in the list */
- (id)tableView:(NSTableView*)tableView objectValueForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row; /**< Get the value of the command in the given row */
- (void)tableView:(NSTableView*)tableView setObjectValue:(id)object forTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row; /**< Set the value of the command in the given row */

- (BOOL)tableView:(NSTableView*)tableView writeRowsWithIndexes:(NSIndexSet*)rowIndexes toPasteboard:(NSPasteboard*)pboard; /**< Write the contents of the given rows to the clipboard */
- (NSDragOperation)tableView:(NSTableView*)tableView validateDrop:(id<NSDraggingInfo>)info proposedRow:(NSInteger)row proposedDropOperation:(NSTableViewDropOperation)dropOperation; /**< Check if the given row is ready to recieve the given drop */
- (BOOL)tableView:(NSTableView*)tableView acceptDrop:(id<NSDraggingInfo>)info row:(NSInteger)row dropOperation:(NSTableViewDropOperation)dropOperation; /**< Insert the contents of the drop into the given row */

@end

#endif /* ConfigMacTableView_h */
