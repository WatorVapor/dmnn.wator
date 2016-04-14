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

static float const fConstCoulombDiff = 0.0001;


/**
 * Constructor
 **/
EinsteinLayer::EinsteinLayer()
{
}

/**
 * Connect a Layer to Net.
 * @param [in] layer
 **/
V1CortexLayer& EinsteinLayer::operator << (V1CortexLayer &layer)
{
    this->top_.push_back(&layer);
    layer.bottom(this);
    INFO_VAR(top_.size());
    return layer;
}


/**
 * update
 * @return None.
 **/
void EinsteinLayer::updateW(void){
}


/**
 * update
 * @return None.
 **/
void EinsteinLayer::update(void)
{
    INFO_VAR("finnish EinsteinLayer::update");
}


/**
 * round
 * @return None.
 **/
void EinsteinLayer::round(void)
{
    this->forward();
    this->update();
    INFO_VAR("finnish EinsteinLayer::round");
    LayerBase::round();
    this->dump();
}


/*
 mat
 x1,x2,x3,x4,x5,x6,x7,x8,x9,x10,x11,x12
 y1,y2,y3,y4,y5,y6,y7,y8,y9,y10,y11,y12
 z1,z2,z3,z4,z5,z6,z7,z8,z9,z10,z11,z12
 u1,u2,u3,u4,u5,u6,u7,u8,u9,u10,u11,u12
 v1,v2,v3,v4,v5,v6,v7,v8,v9,v10,v11,v12
 w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12
 r1,r2,r3,r4,r5,r6,r7,r8,r9,r10,r11,r12
 s1,s2,s3,s4,s5,s6,s7,s8,s9,s10,s11,s12
 t1,t2,t3,t4,t5,t6,t7,t8,t9,t10,t11,t12
 o1,o2,o3,o4,o5,o6,o7,o8,o9,o10,o11,o12
 p1,p2,p3,p4,p5,p6,p7,p8,p9,p10,p11,p12
 q1,q2,q3,q4,q5,q6,q7,q8,q9,q10,q11,q12

 
 ------------------
 image blob
 x1,x2,x3,y1,y2,y3,z1,z2,z3,
 x4,x5,x6,y4,y5,y6,z4,z5,z6,
 x7,x8,x9,y7,y8,y9,z7,z8,z9,
 x10,x11,x12,y10,y11,y12,z10,z11,z12,
 u1,u2,u3,v1,v2,v3,w1,w2,w3,
 u4,u5,u6,v4,v5,v6,w4,w5,w6,
 u7,u8,u9,v7,v8,v9,w7,w8,w9,
 u10,u11,u12,v10,v11,v12,w10,w11,w12,
 r1,r2,r3,s1,s2,s3,t1,t2,t3,
 r4,r5,r6,s4,s5,s6,t4,t5,t6,
 r10,r11,r12,s10,s11,s12,t10,t11,t12,
 
 o1,o2,o3,p1,p2,p3,q1,q2,q3
 o4,o5,o6,p4,p5,p6,q4,q5,q6
 o7,o8,o9,p7,p8,p9,q7,q8,q9
 o10,o11,o12,p10,p11,p12,q10,q11,q12

 ------------------
 coulomb blob
 xyz123,xyz456,xyz789,xyz101112,
 uvw123,uvw456,uvw789,uvw101112,
 rst123,rst456,rst789,rst101112,
 opq123,opq456,opq789,opq101112
 */


/**
 * cal 4 vecotor.
 * @return None.
 **/
void EinsteinLayer::cal4Vec(uint8_t *start,uint8_t &maxDiff,uint8_t &avg)
{
    uint8_t diff1 = std::abs(start[0] - start[1]);
    uint8_t diff2 = std::abs(start[0] - start[2]);
    uint8_t diff3 = std::abs(start[0] - start[3]);
    uint8_t diff4 = std::abs(start[1] - start[2]);
    uint8_t diff5 = std::abs(start[1] - start[3]);
    uint8_t diff6 = std::abs(start[2] - start[3]);
    
    maxDiff = diff1;
    if(diff2 > maxDiff) {
        maxDiff = diff2;
    }
    if(diff3 > maxDiff) {
        maxDiff = diff3;
    }
    if(diff4 > maxDiff) {
        maxDiff = diff4;
    }
    if(diff5 > maxDiff) {
        maxDiff = diff5;
    }
    if(diff6 > maxDiff) {
        maxDiff = diff6;
    }
    
    avg = (start[0] + start[1] + start[2] + start[3])/4;
}


/**
 * forward
 * @return None.
 **/
void EinsteinLayer::forward(void)
{
    blobsRaw2X2_.clear();
    blobsRaw4X4_.clear();
    blobs_.clear();
    for(auto btm:bottom_){
        // 2x2
        LayerInput *input = dynamic_cast<LayerInput*>(btm);
        auto inBlob = input->getBlob(this);
        INFO_VAR(inBlob->w_);
        INFO_VAR(inBlob->h_);
        INFO_VAR(inBlob->ch_);
        this->wGrid2x2_ = inBlob->w_/this->w_;
        this->hGrid2x2_ = inBlob->h_/this->h_;
        INFO_VAR(this->wGrid2x2_);
        INFO_VAR(this->hGrid2x2_);
        auto raw = shared_ptr<Blob<uint8_t>>(new Blob<uint8_t>(this->wGrid2x2_,this->hGrid2x2_,inBlob->ch_));
        auto rawNext = shared_ptr<Blob<uint8_t>>(new Blob<uint8_t>(this->wGrid2x2_,this->hGrid2x2_,inBlob->ch_));
        for(int i = 0 ;i < raw->size_ ;i++) {
            uint8_t maxDiff = 0;
            uint8_t avg = 0;
            int index = i*this->w_*this->h_;
            cal4Vec(&(inBlob->data_[index]),maxDiff,avg);
            raw->data_[i] = maxDiff;
            rawNext->data_[i] = maxDiff;
        }
        blobsRaw2X2_.push_back(raw);
        // 4x4
        for(int channel = 0 ; channel < inBlob->ch_;channel++){
            for(int y = 0;y < this->hGrid2x2_;y++){
                for(int x = 0;x < this->wGrid2x2_;x++){
                    
                }
            }
        }
    }
    INFO_VAR(blobs_.size());
    INFO_VAR("finnish CoulombLayer::forward");
}


/**
 * dump to png
 * @return None.
 **/
void EinsteinLayer::dump(void){
    INFO_VAR(blobs_.size());
    for (auto blob:blobs_) {
        blob->dump(typeid(this).name());
    }
}

/**
 * get ptr
 * @return None.
 **/
shared_ptr<Blob<bool>> EinsteinLayer::getBlob(const LayerBase* who)
{
    int i = 0;
    TRACE_VAR(top_.size());
    for(auto top:top_)
    {
        if(who == top)
        {
            TRACE_VAR(blobs_.size());
            if(blobs_.size() > i)
            {
                return blobs_[i];
            }
            else
            {
                FATAL_VAR("fatal errro");
            }
        }
        i++;
    }
    return nullptr;
}
