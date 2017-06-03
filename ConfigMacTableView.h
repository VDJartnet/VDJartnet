//
//  ConfigMacTableView.h
//  VDJartnet
//
//  Created by Jonathan Tanner on 02/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#ifndef ConfigMacTableView_h
#define ConfigMacTableView_h

#include <AppKit/AppKit.h>

@interface ConfigTableView : NSTableView

- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)localDestination;

@end

#endif /* ConfigMacTableView_h */
