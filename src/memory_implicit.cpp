/*
Copyright (c) 2015, Wator Vapor
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

* Redistributions of source code must retain the above copyright notice, this
  list of conditions and the following disclaimer.

* Redistributions in binary form must reproduce the above copyright notice,
  this list of conditions and the following disclaimer in the documentation
  and/or other materials provided with the distribution.

* Neither the name of wator nor the names of its
  contributors may be used to endorse or promote products derived from
  this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include "wator.hpp"
using namespace Wator;

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
using namespace boost::property_tree;


const uint64_t iuConstMaxNum25 =0b1111100000000000000000000;
map<uint64_t,bool> ImplicitMemory::shotThough_;

/**
 * Constructor
 **/
ImplicitMemory::ImplicitMemory()
{
    for(uint64_t i = 0;i < iuConstMaxNum25 ;i++)
    {
        shotThough_[i] = true;
    }
}
/**
 * Constructor
 * @param [in] info layer parameter.
 **/
ImplicitMemory::ImplicitMemory(const pt::ptree& info)
:serial_(info)
{
}



/**
 * update
 **/
void ImplicitMemory::update(int pinch,uint64_t index,int sparse)
{
    std::bitset<64> memBit(index);
    if (memBit.count() < sparse) {
        return;
    }
    
    // add hold space
    for(int i = memory_.size();i < pinch + 1 ;i++) {
        memory_.push_back({});
    }
    INFO_VAR(memory_.size());
    auto &memory = memory_.at(pinch);
    auto it = memory.find(index);
    if (it != memory.end()) {
        it->second++;
   } else {
        memory.insert({index,1});
    }
}

/**
 * sort
 **/
void ImplicitMemory::sort()
{
    memRanking_.clear();
    for(int index = 0; index < memory_.size();index++) {
        memRanking_.push_back({});
    }
    for(int index = 0; index < memory_.size();index++) {
        //        map<uint64_t,vector<uint64_t>> sortByCount;
        auto &memory = memory_[index];
        for (auto it:memory) {
            auto memCount = it.second;
            std::bitset<25> memBit(it.first);
            TRACE_VAR(memBit);
            TRACE_VAR(it.second);
            auto itSort = memRanking_[index].find(memCount);
            if (memRanking_[index].end() != itSort) {
                itSort->second.push_back(it.first);
            } else {
                memRanking_[index][memCount] = {it.first};
            }
        }
        for (auto it:memRanking_[index]) {
            auto memCount = it.first;
            INFO_VAR(it.second.size());
            INFO_VAR(memCount);
            for (auto mem:it.second)
            {
                std::bitset<25> memBit(mem);
                INFO_VAR(memBit);
            }
        }
        TRACE_VAR(memory.size());
        TRACE_VAR(memRanking_[index].size());
    }
    INFO_VAR(memory_.size());
    for(const auto &memory :memory_){
        INFO_VAR(memory.size());
#if 0
        for(const auto &pair:memory){
            std::bitset<25> memBit(pair.first);
            INFO_VAR(memBit);
            INFO_VAR(pair.second);
        }
#endif
    }
}

/*
 1000010000100000000000000
 1000010000100000000000000
 */


/*
 
 cant not list all.
 
 #define __S(a,b,c,d,e) {0b##a##b##c##d##e,#a#b#c#d#e}
 const map<uint64_t,string> ImplicitMemory::shotThough_ = {
 __S(11111,
 00000,
 00000,
 00000,
 00000),
 
 __S(00000,
 11111,
 00000,
 00000,
 00000),
 
 __S(00000,
 00000,
 11111,
 00000,
 00000),
 
 __S(00000,
 00000,
 00000,
 11111,
 00000),
 
 __S(00000,
 00000,
 00000,
 00000,
 11111),
 
 
 __S(10000,
 10000,
 10000,
 10000,
 10000),
 
 __S(01000,
 01000,
 01000,
 01000,
 01000),
 
 __S(00100,
 00100,
 00100,
 00100,
 00100),
 
 __S(00010,
 00010,
 00010,
 00010,
 00010),
 
 __S(00001,
 00001,
 00001,
 00001,
 00001),
 
 
 __S(10000,
 01000,
 00100,
 00010,
 00001),
 
 __S(00001,
 00010,
 00100,
 01000,
 10000),
 
 
 __S(10000,
 01000,
 01000,
 01000,
 01000),
 
 
 __S(10000,
 01000,
 10000,
 10000,
 10000),
 
 
 
 __S(00001,
 00010,
 00100,
 01000,
 10000),
 };


 
 00000
 00000
 01100
 01100
 00000
 
 
 01100
 01100
 00000
 00000
 00000

 
 00100
 00100
 00100
 00100
 00000
 
 00000
 01111
 10000
 00000
 00000
 
 
 00010,
 10000,
 00010,
 10000

 
 10000,
 00111,
 00010,
 10000

 
 ),

 */


