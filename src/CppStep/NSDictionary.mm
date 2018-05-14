//
//  NSDictionary.mm
//  VDJartnet
//
//  Created by Jonathan Tanner on 13/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#import "NSDictionary.h"

@implementation NSDictionary (CSStringAttributes)

+ (NSDictionary*)attributesWithCSStringAttribute:(CSStringAttribute*)csStringAttribute {
    if ( NSMutableDictionary* mutableAttributes = [[NSMutableDictionary alloc] init] ) {
        for (CSStringAttribute::NSAttribute attribute : csStringAttribute->getNSAttributes()) {
            mutableAttributes[attribute.first] = attribute.second;
        }
        return mutableAttributes;
    } else {
        return nil;
    }
}

@end
