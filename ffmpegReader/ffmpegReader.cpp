/*
 OFX ffmpegReader plugin.
 Reads a video input file using the libav library.
 
 Copyright (C) 2013 INRIA
 Author Alexandre Gauthier-Foichat alexandre.gauthier-foichat@inria.fr
 
 Redistribution and use in source and binary forms, with or without modification,
 are permitted provided that the following conditions are met:
 
 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 Redistributions in binary form must reproduce the above copyright notice, this
 list of conditions and the following disclaimer in the documentation and/or
 other materials provided with the distribution.
 
 Neither the name of the {organization} nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 INRIA
 Domaine de Voluceau
 Rocquencourt - B.P. 105
 78153 Le Chesnay Cedex - France
 
 
 The skeleton for this source file is from:
 OFX Basic Example plugin, a plugin that illustrates the use of the OFX Support library.
 
 Copyright (C) 2004-2005 The Open Effects Association Ltd
 Author Bruno Nicoletti bruno@thefoundry.co.uk
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice,
 this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 * Neither the name The Open Effects Association Ltd, nor the names of its
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 
 The Open Effects Association Ltd
 1 Wardour St
 London W1D 6PA
 England
 
 */


#include "GenericReader.h"


class FfmpegReaderPlugin : public GenericReaderPlugin {
    
public:
    
    FfmpegReaderPlugin(OfxImageEffectHandle handle)
    : GenericReaderPlugin(handle)
    {
        
    }
    
    virtual void changedParam(const OFX::InstanceChangedArgs &args, const std::string &paramName);
    
private:
    
    virtual void decode(const std::string& filename,OfxTime time,OFX::Image* dstImg);
    
    virtual void initializeLut();
    
    virtual bool getTimeDomainForVideoStream(const std::string& filename,OfxRangeD &range);
    
    virtual bool areHeaderAndDataTied(const std::string& filename,OfxTime time) const;
    
    virtual void getFrameRegionOfDefinition(const std::string& /*filename*/,OfxTime time,OfxRectD& rod);
};


void FfmpegReaderPlugin::changedParam(const OFX::InstanceChangedArgs &args, const std::string &paramName) {
    
}

void FfmpegReaderPlugin::decode(const std::string& filename,OfxTime time,OFX::Image* dstImg){
    
}

void FfmpegReaderPlugin::initializeLut() {
    
}

bool FfmpegReaderPlugin::getTimeDomainForVideoStream(const std::string& filename,OfxRangeD &range) {
    return false;
}

bool FfmpegReaderPlugin::areHeaderAndDataTied(const std::string& filename,OfxTime time) const {
    return false;
}

void FfmpegReaderPlugin::getFrameRegionOfDefinition(const std::string& /*filename*/,OfxTime time,OfxRectD& rod) {

}

using namespace OFX;
mDeclarePluginFactory(FfmpegReaderPluginFactory, {}, {});

namespace OFX
{
    namespace Plugin
    {
        void getPluginIDs(OFX::PluginFactoryArray &ids)
        {
            static FfmpegReaderPluginFactory p("net.sf.openfx:ffmpegReader", 1, 0);
            ids.push_back(&p);
        }
    };
};

/** @brief The basic describe function, passed a plugin descriptor */
void FfmpegReaderPluginFactory::describe(OFX::ImageEffectDescriptor &desc)
{
    // basic labels
    desc.setLabels("FfmpegReaderOFX", "FfmpegReaderOFX", "FfmpegReaderOFX");
    desc.setPluginDescription("Reads image or video file using the libav");
    
    OFX::Plugin::describeGenericReader(desc);
    
}

/** @brief The describe in context function, passed a plugin descriptor and a context */
void FfmpegReaderPluginFactory::describeInContext(OFX::ImageEffectDescriptor &desc, ContextEnum context)
{
    
    
}

/** @brief The create instance function, the plugin must return an object derived from the \ref OFX::ImageEffect class */
ImageEffect* FfmpegReaderPluginFactory::createInstance(OfxImageEffectHandle handle, ContextEnum context)
{
    return new FfmpegReaderPlugin(handle);
}
