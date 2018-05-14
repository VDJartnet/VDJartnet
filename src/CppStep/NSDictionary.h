//
//  NSDictionary.h
//  VDJartnet
//
//  Created by Jonathan Tanner on 13/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#include "CSAttributedString.hpp"
#import <Foundation/Foundation.h>

@interface NSDictionary (CSStringAttributes)

+ (NSDictionary*)attributesWithCSStringAttribute:(CSStringAttribute*)csStringAttribute; /**< Initialise an NSDictionary with the given CSStringAttribute */

@end
