//
//  NSAttributedString.mm
//  CppStep
//
//  Copyright � 2018 Jonathan Tanner. All rights reserved.
//
//This file is part of CppStep.
//
//CppStep is free software: you can redistribute it and/or modify
//it under the terms of the GNU General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.
//
//CppStep is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with CppStep.  If not, see <http://www.gnu.org/licenses/>.

#import "NSAttributedString.h"

@implementation NSAttributedString (CSAttributedString)

- (id)initWithCSAttributedString:(CSAttributedString*)csAttributedString {
    if ( NSMutableAttributedString* mutableString = [[NSMutableAttributedString alloc] initWithString: @(csAttributedString->getPlainString().c_str())] ) {
        for (CSAttributedString::RangedAttribute attribute : csAttributedString->getRangedAttributes()) {
            [mutableString addAttributes:[NSDictionary attributesWithCSStringAttribute:attribute.second] range:NSMakeRange(attribute.first)];
        }
        
        self = mutableString;
        return self;
    } else {
        return nil;
    }
}

@end
