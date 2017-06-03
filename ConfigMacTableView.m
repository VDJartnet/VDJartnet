//
//  ConfigMacTableView.m
//  VDJartnet
//
//  Created by Jonathan Tanner on 02/06/2017.
//  Copyright © 2017 Jonathan Tanner. All rights reserved.
//

#import "ConfigMacTableView.h"

@implementation ConfigTableView

- (NSDragOperation)draggingSourceOperationMaskForLocal:(BOOL)localDestination {
    return NSDragOperationEvery;
}

@end
