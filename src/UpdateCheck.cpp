#include "UpdateCheck.hpp"

UpdateCheck::Result UpdateCheck::isLatest(){
    struct addrinfo *ai;

    int result = getaddrinfo(hostname, "80", NULL, &ai);

    if(result != 0){
        return Result::error; //some error has occured during the lookup
    }

    char buf[INET6_ADDRSTRLEN];
    int entryfound=0;

    //loop through linked list structure of address info
    while(ai != NULL){
        if(ai->ai_family == AF_INET6){
            //v6 data
            struct sockaddr_in6 *addr = (struct sockaddr_in6*)ai->ai_addr;


            inet_ntop(AF_INET6, &(addr->sin6_addr), buf, INET6_ADDRSTRLEN);
            entryfound=1;
        }
        
        ai = ai->ai_next;
    }

    //no v6 address found
    if(entryfound!=1){
        return Result::error;
    }

    std::string *vstring = new std::string(buf);
    std::string vstring_lower;
    vstring_lower.resize(vstring->length());

    std::transform(vstring->begin(), vstring->end(), vstring_lower.begin(), ::tolower);
    if(vstring_lower == currentVersion){
        return Result::latest;
    } else {
        return Result::notLatest;
    }
}
