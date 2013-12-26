/*
 OFX GenericReader plugin.
 A base class for all OpenFX-based decoders.
 
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

 */

#ifndef Io_GenericReader_h
#define Io_GenericReader_h

#include <ofxsImageEffect.h>

namespace OFX {
    namespace Color {
        class Lut;
    }
}

/**
 * @brief A generic reader plugin, derive this to create a new reader for a specific file format.
 * This class propose to handle the common stuff among readers: 
 * - common params
 * - a tiny cache to speed-up the successive getRegionOfDefinition() calls
 * - a way to inform the host about the colour-space of the data.
 **/
class GenericReaderPlugin : public OFX::ImageEffect {
    
public:
    
    GenericReaderPlugin(OfxImageEffectHandle handle);
    
    virtual ~GenericReaderPlugin();
    
    /**
     * @brief Don't override this function, the GenericReaderPlugin class already does the rendering. The "decoding" of the frame
     * must be done by the pure virtual function decode(...) instead.
     **/
    void render(const OFX::RenderArguments &args);
    
    /**
     * @brief Don't override this. Basically this function will call getTimeDomainForVideoStream(...),
     * which your reader should implement to read from a video-stream the time range. 
     * If the file is not a video stream, the function getTimeDomainForVideoStream() should return false, indicating that
     * we're reading a sequence of images and that the host should get the time domain for us.
     **/
    bool getTimeDomain(OfxRangeD &range);
    
    /**
     * @brief Don't override this. If the pure virtual function areHeaderAndDataTied() returns true, this
     * function will call decode() to read the region of definition of the image and cache away the decoded image
     * into the _dstImg member.
     * If areHeaderAndDataTied() returns false instead, this function will call the virtual function
     * getFrameRegionOfDefinition() which should read the header of the image to only extract the region of
     * definition of the image.
     **/
    bool getRegionOfDefinition(const OFX::RegionOfDefinitionArguments &args, OfxRectD &rod);
    
    
    /**
     * @brief You can override this to take actions in response to a param change. 
     * Make sure you call the base-class version of this function at the end: i.e:
     * 
     * void MyReader::changedParam(const OFX::InstanceChangedArgs &args, const std::string &paramName) {
     *      if (.....) {
     *      
     *      } else if(.....) {
     *
     *      } else {
     *          GenericReaderPlugin::changedParam(args,paramName);
     *      }
     * }
     **/
    virtual void changedParam(const OFX::InstanceChangedArgs &args, const std::string &paramName);
    
    
protected:
    
    /**
     * @brief Override this function to actually decode the image contained in the file pointed to by filename.
     * If the file is a video-stream then you should decode the frame at the time given in parameters.
     * You must write the decoded image into dstImg. This function should convert the read pixels into the
     * bitdepth of the dstImg. You can inform the host of the bitdepth you support in the describe() function.
     * Note that many hosts work with linear colors and we intend that this function transfer from the
     * image file's color-space to linear. To help you do this you can use the color-space conversion
     * suite written for this purpose.
     * You can always skip the color-space conversion, but for all linear hosts it would produce either
     * false colors or sub-par performances in the case the end-user has to append a color-space conversion
     * effect her/himself.
     **/
    virtual void decode(const std::string& filename,OfxTime time,OFX::Image* dstImg) = 0;
    
    /**
     * @brief This function must initialize the _lut member. This lut can be used to do all
     * conversions from the image's file format's color-space to linear.
     **/
    virtual void initializeLut()  = 0;
    
    /**
     * @brief Override to indicate the time domain of a video stream. Return false if you know that the
     * file isn't a video-stream, true when you can find-out the frame range.
     **/
    virtual bool getTimeDomainForVideoStream(const std::string& /*filename*/,OfxRangeD &/*range*/){ return false; }
    
    /**
     * @brief Override to indicate whether a frame needs to be decoded entirely to extract only its
     * meta-data (i.e: bitdepth & image bounds)
     **/
    virtual bool areHeaderAndDataTied(const std::string& filename,OfxTime time) const = 0;
    
    /**
     * @brief Overload this function to exctract the region of definition out of the header
     * of the image targeted by the filename. 
     **/
    virtual void getFrameRegionOfDefinition(const std::string& /*filename*/,OfxTime time,OfxRectD& rod){}
    
    
    OFX::Clip *_outputClip; //< Mandated output clip
    OFX::StringParam  *_fileParam; //< The input file
    
    OFX::Color::Lut* _lut;//< the lut used to convert from the image's file format's color-space to linear.
private:
    
    OFX::Image* _dstImg; //< ptr to the output img, when this ptr is not NULL it means the image
                         //has already been decoded

};


namespace OFX
{
    namespace Plugin
    {
        /**
         * @brief Call this in the describeInContext(...) function of the overloaded reader's factory to
         * add the common params to all readers.
         **/
        void defineGenericReaderParamsInContext(OFX::ImageEffectDescriptor& desc,OFX::ContextEnum context);
        
        /**
         * @brief Call this in the describe(...) function of the overloaded reader's factory to
         * add the common properties to all readers. See definition for more explanation.
         **/
        void describeGenericReader(OFX::ImageEffectDescriptor& desc);
    };
};

#endif