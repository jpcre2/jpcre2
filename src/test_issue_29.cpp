/**@file test_issue_29.cpp
 *  Test cases for [issue 29 on Github](https://github.com/jpcre2/jpcre2/issues/29)
 * @include test_issue_29.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * @author [Moritz Bunkus](https://github.com/mbunkus)
 * */

#include <cassert>
#include <iostream>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;

void test_match(jp::Regex &re, std::string const &subject) {
    jp::RegexMatch rm;
    jp::VecNum matches;
    size_t num = rm
        .setRegexObject(&re)
        .setNumberedSubstringVector(&matches)
        .setSubject(subject)
        .match();
    assert(num > 0);
    assert(matches[0][1] != "dummy"); // idx 1 should be accessible
    assert(matches[0][2] != "dummy"); // idx 2 should be accessible
    assert(matches[0][3] != "dummy"); // idx 3 should be accessible
}

int main(){
    // check with a regex pattern with 3 capture groups
    jp::Regex re("^(\\d+)([abc])?([XYZ])?$");

    test_match(re, "42bZ");
    test_match(re, "23Z");
    test_match(re, "54b");
    test_match(re, "13");

    return 0;
}
