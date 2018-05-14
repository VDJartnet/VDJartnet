//
//  CSAttributedString.hpp
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

#ifndef CSAttributedString_hpp
#define CSAttributedString_hpp

#include "CSRange.hpp"

#include <string>
#include <vector>
#include <list>
#include <algorithm>
#include <iterator>

#import <Foundation/Foundation.h>

template<typename T>
void operator += (std::list<T>& lhs, std::list<T>&& rhs) {
    lhs.splice(lhs.end(), rhs);
}

template<typename T>
std::list<T> operator + (std::list<T>&& lhs, std::list<T>&& rhs) {
    lhs += rhs;
    return lhs;
}

/** An attribute */
struct CSStringAttribute {
    typedef std::pair<NSString*, id> NSAttribute;
    virtual std::list<NSAttribute> getNSAttributes() const = 0;
};

struct CSNSStringAttribute : public CSStringAttribute {
public:
    virtual std::list<NSAttribute> getNSAttributes() const {
        std::list<NSAttribute> nsAttribute;
        nsAttribute.push_back(attribute);
        return nsAttribute;
    }
    CSNSStringAttribute(NSString* key, id value) {
        attribute = std::make_pair(key, value);
    }
private:
    NSAttribute attribute;
};

struct CSStringAttributes : public CSStringAttribute {
public:
    virtual std::list<NSAttribute> getNSAttributes() const;
    
    friend CSStringAttributes operator | (const CSStringAttribute& lhs, const CSStringAttribute& rhs);
private:
    std::pair<CSStringAttribute const*, CSStringAttribute const*> attributes;
    CSStringAttributes(CSStringAttribute const* first, CSStringAttribute const* second) {
        attributes = std::make_pair(first, second);
    }
};

CSStringAttributes operator | (const CSStringAttribute& lhs, const CSStringAttribute& rhs);

/** An attributed string */
class CSAttributedString {
public:
    typedef std::pair<CSRange, CSStringAttribute*> RangedAttribute;
    typedef std::vector<RangedAttribute> RangedAttributes;
    
    CSAttributedString(std::string stringTMP, CSStringAttribute* attribute); /**< Construct an attributed string */

    std::string getPlainString() const; /**< Get a std::string version of the string */
    RangedAttributes getRangedAttributes() const; /**< Get a copy of the attributes and the ranges on which they act */
    
    friend CSAttributedString operator + (const CSAttributedString& lhs, const CSAttributedString& rhs); /**< Concatenate two attributed strings */

private:
    std::string string; /**< The plain string */
    RangedAttributes rangedAttributes; /**< The attributes and the ranges on which they act */

    CSAttributedString(std::string stringTMP, RangedAttributes attributesTMP); /**< Construct an attributed string */
};

CSAttributedString operator + (const CSAttributedString& lhs, const CSAttributedString& rhs);

#endif /* CSAttributedString_hpp */
