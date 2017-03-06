#include <iostream>
#include "jpcre2.hpp"

typedef jpcre2::select<char> jp;

jp::String callback1(const jp::NumSub& m, void*, void*){
    return "("+m[0]+")";
}

jp::String callback2(void*, const jp::MapNas& m2, void*){
    return "("+m2.at("total")+")";
}

int main(){
	jp::Regex re("(?<total>\\w+)", "n");
	jp::RegexReplace rr(&re);

	jp::String s3 = "I am a string 879879 fdsjkll ১ ২ ৩ ৪ অ আ ক খ গ ঘ ";

	rr.setSubject(&s3) //just s3 would do too, but passing with pointer will be faster.
	  .setPcre2Option(PCRE2_SUBSTITUTE_GLOBAL);
	  
	std::cout<<"\n\n### 1\n"<<
			rr.nreplace(jp::MatchEvaluator(callback1));
	
	#if __cplusplus >= 201103L
	//example with lambda
    std::cout<<"\n\n### Lambda\n"<<
    		rr.nreplace(
                jp::MatchEvaluator(
                    [](const jp::NumSub& m1, const jp::MapNas& m2, void*){
                        return "("+m1[0]+"/"+m2.at("total")+")";
                    }
                ));
	#endif
	
	
    //MatchEvaluator itself has an nreplace() function:
    //Actually the jp::RegexReplace::nreplace(MatchEvaluator me) is just a wrapper of jp::MatchEvaluator::nreplace().
    std::cout<<"\n\n### 1 Calling directly MatchEvaluator::nreplace()\n"
             <<jp::MatchEvaluator(callback1)
                                 .setSubject(&s3)
                                 .setRegexObject(&re) //without this, there would be an assertion failure.
                                 .setFindAll()
                                 .nreplace();
    //note the setFindAll() in above, without it, only single replacement would occur because there would be only one match.
    
    
    /* *****************************************************************
     * Re-using same MatchEvaluator for different replace operation
     * by using existing match data with different callback function:
     * ****************************************************************/
    
    jp::MatchEvaluator cme(jp::callback::POPULATE_FCN);
    //perform a match to populate all those vectos with match data.
    cme.setSubject(&s3).setRegexObject(&re).setFindAll().match();
    
    //this performs the match again (redundant)
    std::cout<<"\n\n### callback1: \n"<<cme.setMatchEvaluatorCallback(callback1).nreplace();

    //this one uses the match data from previous match (note the 'false' in nreplace)
    std::cout<<"\n\n### callback2: \n"<<cme.setMatchEvaluatorCallback(callback2).nreplace(false);
    std::cout<<"\n\n### callback1: \n"<<cme.setMatchEvaluatorCallback(callback1).nreplace(false);
    
	
	return 0;
}
