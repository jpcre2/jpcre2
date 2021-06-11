/**@file test_pr_31.cpp
 *  Test cases for [PR 31 on Github](https://github.com/jpcre2/jpcre2/pull/31)
 * @include test_pr_31.cpp
 * @author [Md Jahidul Hamid](https://github.com/neurobin)
 * @author [Moritz Bunkus](https://github.com/mbunkus)
 * @author [Reuben Thomas](https://github.com/rrthomas)
 * */

// This is a modified version of test_issue_29.cpp that requires
// JPCRE2_UNSET_CAPTURES_NULL to be set.

// Since JPCRE2_UNSET_CAPTURES_NULL requires C++17, stub out the test if
// we don't have C++17.
#ifdef JPCRE2_USE_MINIMUM_CXX_17

#define JPCRE2_UNSET_CAPTURES_NULL

#include <cassert>
#include <iostream>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;

bool string_present(jp::NumSub vec, size_t pos)
{
    try {
#ifdef JPCRE2_UNSET_CAPTURES_NULL
        return vec.at(pos).has_value();
#else
        assert(vec.at(pos) != "dummy");
#endif
    } catch(std::out_of_range const & e){
        return false;
    }
    return true;
}

void test_match(jp::Regex &re, std::string const &subject, size_t captures_expected) {
    jp::RegexMatch rm;
    jp::VecNum matches;
    bool out_of_range_occurred = false;
    size_t num = rm
        .setRegexObject(&re)
        .setNumberedSubstringVector(&matches)
        .setSubject(subject)
        .match();
    assert(num > 0);
    assert(string_present(matches[0], 1)); // idx 1 should always be present
    // idx 2 & 3 should be present if matched
    assert(string_present(matches[0], 2) == !!(captures_expected & 0x2));
    assert(string_present(matches[0], 3) == !!(captures_expected & 0x4));
    out_of_range_occurred = !string_present(matches[0], 4); // idx 4 should never be present

    if(!out_of_range_occurred){
        std::cout<<"E: std::out_of_range exception was not thrown. vecnum has more capture groups than expected.\n";
        exit(1);
    }
}
#endif

int main(){
#ifdef JPCRE2_USE_MINIMUM_CXX_17
    // check with a regex pattern with 3 capture groups
    jp::Regex re("^(\\d+)([abc])?([XYZ])?$");

    test_match(re, "42bZ", 0x7); // All 3 captures.
    test_match(re, "23Z", 0x5); // Captures 1 & 3.
    test_match(re, "54b", 0x3); // Captures 1 & 2.
    test_match(re, "13", 0x1); // Just capture 1.
#endif
    return 0;
}
