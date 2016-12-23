// use in string_view. 
// Example:
//    std::string_view s(SV_("hello,world"));
// Compare to std::string_view s("hello,world"), there is no need to call strlen in runtime, instead compute "s" len in compile time
// inspired by ngx_string
#pragma once

#ifndef SV_
#define SV_(a) a,sizeof(a)-1
#endif
