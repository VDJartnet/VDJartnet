//
//  NSRange.mm
//  VDJartnet
//
//  Created by Jonathan Tanner on 13/05/2018.
//  Copyright © 2018 Jonathan Tanner. All rights reserved.
//

#import "NSRange.h"

NSRange NSMakeRange(CSRange csRange) {
    return NSMakeRange(csRange.pos, csRange.count);
}
