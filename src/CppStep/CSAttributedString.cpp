//
//  CSAttributedString.cpp
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

#include "CSAttributedString.hpp"

std::list<CSStringAttribute::NSAttribute> CSStringAttributes::getNSAttributes() const {
    std::list<CSStringAttribute::NSAttribute> nsAttributes;
    if (attributes.first != nullptr) {
        nsAttributes += attributes.first->getNSAttributes();
    }
    if (attributes.second != nullptr) {
        nsAttributes += attributes.second->getNSAttributes();
    }
    return nsAttributes;
}

CSStringAttributes operator | (const CSStringAttribute& lhs, const CSStringAttribute& rhs) {
    return CSStringAttributes(&lhs, &rhs);
}

CSAttributedString::CSAttributedString(std::string stringTMP, CSStringAttribute* attribute) {
    string = stringTMP;
    rangedAttributes.push_back(std::make_pair(CSRange(0, string.size()), attribute));
}

std::string CSAttributedString::getPlainString() const {
    return string;
}

CSAttributedString::RangedAttributes CSAttributedString::getRangedAttributes() const {
    return rangedAttributes;
}

CSAttributedString::CSAttributedString(std::string stringTMP, RangedAttributes rangedAttributesTMP) {
    string = stringTMP;
    rangedAttributes = rangedAttributesTMP;
}

CSAttributedString operator + (const CSAttributedString& lhs, const CSAttributedString& rhs) {
    CSAttributedString::RangedAttributes rangedAttributes;
    rangedAttributes.reserve(lhs.rangedAttributes.size() + rhs.rangedAttributes.size());
    for (CSAttributedString::RangedAttribute rangedAttribute : lhs.rangedAttributes) {
        rangedAttributes.push_back(rangedAttribute);
    }
    for (CSAttributedString::RangedAttribute rangedAttribute : rhs.rangedAttributes) {
        rangedAttribute.first.pos += lhs.string.size();
        rangedAttributes.push_back(rangedAttribute);
    }
    return CSAttributedString(lhs.string + rhs.string, rangedAttributes);
}
