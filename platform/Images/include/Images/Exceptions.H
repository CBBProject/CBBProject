#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <exception>
#include <typeinfo>

#include <Images/Defs.H>

namespace Images {

    typedef enum { UNEXPECTED = 128, IO_EXCPT,
                   BAD_FMT, NO_SUFFIX, NON_MATCH_FMT, BAD_HDR, BAD_DATA, BAD_DIM, UNKN_DIM, BAD_SIZE_SPEC, UNKN_PIX, UNKN_PIX_TYPE,
                   UNKN_FILE_FMT, UNKN_FILE_SUFFIX, UNKN_NAMED_FILE_FMT, NON_MATCH_NAMED_FILE_FMT, NO_FILE_FMT,
                   BAD_PLGIN_LIST, BAD_PLGIN_FILE, BAD_PLGIN, ALREADY_KN_TAG,
                   NO_IMG_ARG, DIFF_IMG } ExceptionCode;

    class Exception: public std::exception {
    public:

        Exception(const std::string& w): whatstring(std::string("Images::Exception: ")+w) { }

        virtual ~Exception() throw() { }

        virtual const char*   what () const throw() { return whatstring.c_str(); }
        virtual ExceptionCode code()  const throw() = 0;

    private:

        std::string          whatstring;
    };
    
    struct UnexpectedException: public Exception {

        UnexpectedException(const std::string& func,const std::string& file,const unsigned line): Exception(message(func,file,line)) { }

        ExceptionCode code() const throw() { return UNEXPECTED; }

    private:

        static std::string message(const std::string& func,const std::string& file,const unsigned line) {
            std::ostringstream ost;
            ost << "Unexpected error in " << func << " at " << file << ':' << line << '.';
            return ost.str();
        }
    };

    struct IOException: public Exception {

        IOException(const std::string& str): Exception(str) { }

        template <typename CharT,typename Traits>
        IOException(std::basic_ios<CharT,Traits>& ios,const std::string& str): Exception(str) { ios.setstate(std::ios::failbit); }

        ExceptionCode code() const throw() { return IO_EXCPT; }
    };

    struct BadFormat: public IOException {

        BadFormat(const std::string& fmtname):                  IOException(message(fmtname))    { }
        BadFormat(std::istream& is,const std::string& fmtname): IOException(is,message(fmtname)) { }

        ExceptionCode code() const throw() { return BAD_FMT; }

    private:

        static std::string message(const std::string& fmtname) { return std::string("Unable to read the input as a "+fmtname+" image file."); }
    };

    struct NoSuffix: public IOException {
        NoSuffix(const std::string& name): IOException(std::string("No identifiable suffix in name ")+name) { }

        ExceptionCode code() const throw() { return NO_SUFFIX; }
    };

    struct NonMatchingFormat: public IOException {

        NonMatchingFormat(const std::string& fmtname):                  IOException(message(fmtname))    { }
        NonMatchingFormat(std::ostream& os,const std::string& fmtname): IOException(os,message(fmtname)) { }

        ExceptionCode code() const throw() { return NON_MATCH_FMT; }

    private:

        static std::string message(const std::string& fmtname) { return std::string("Unable to save the image in the ")+fmtname+" format."; }
    };

    struct BadHeader: public IOException {

        BadHeader(const std::string& fmtname=""):                  IOException(message(fmtname))    { }
        BadHeader(std::istream& is,const std::string& fmtname=""): IOException(is,message(fmtname)) { }

        ExceptionCode code() const throw() { return BAD_HDR; }

    private:

        static std::string message(const std::string& fmtname) { return std::string("Bad ")+fmtname+" file header."; }
    };

    struct BadData: public IOException {

        BadData(const std::string& fmtname):                  IOException(message(fmtname))    { }
        BadData(std::istream& is,const std::string& fmtname): IOException(is,message(fmtname)) { }

        ExceptionCode code() const throw() { return BAD_DATA; }

    private:

        static std::string message(const std::string& fmtname) { return std::string("Bad ")+fmtname+" file data."; }
    };

    struct BadDimension: public IOException {

        BadDimension(const Dimension dim): IOException(message(dim)) { }

        template <typename CharT,typename Traits>
        BadDimension(std::basic_ios<CharT,Traits>& ios,const Dimension dim): IOException(ios,message(dim)) { }

        ExceptionCode code() const throw() { return BAD_DIM; }

    private:

        static std::string message(const Dimension dim) {
            std::ostringstream ost;
            ost << "Incoming image is not of dimension " << dim << '.';
            return ost.str();
        }
    };

    struct BadSizeSpec: public IOException {

        BadSizeSpec(const Dimension dim): IOException(message(dim)) { }

        template <typename CharT,typename Traits>
        BadSizeSpec(std::basic_ios<CharT,Traits>& ios,const Dimension dim): IOException(ios,message(dim)) { }

        ExceptionCode code() const throw() { return BAD_SIZE_SPEC; }

    private:

        static std::string message(const Dimension dim) {
            std::ostringstream ost;
            ost << "Size specification is not of dimension " << dim << '.';
            return ost.str();
        }
    };

    struct UnknownDimension: public Exception {

        UnknownDimension(const Dimension dim): Exception(message(dim)) { }

        ExceptionCode code() const throw() { return UNKN_DIM; }

    private:

        static std::string message(const Dimension dim) {
            std::ostringstream ost;
            ost << "Image is of unknown dimension " << dim << '.';
            return ost.str();
        }
    };

    struct BadPixelType: public IOException {

        BadPixelType(const std::type_info& image_type,const std::type_info& header_type): IOException(message(image_type,header_type)) { }

        template <typename CharT,typename Traits>
        BadPixelType(std::basic_ios<CharT,Traits>& ios,const std::type_info& image_type,const std::type_info& header_type): IOException(ios,message(image_type,header_type)) { }

        ExceptionCode code() const throw() { return UNKN_PIX; }

    private:

        static std::string message(const std::type_info& image_type,const std::type_info& header_type) {
            return std::string("Incoming image pixel type (") + header_type.name() + ") is not that of the incoming image (" + image_type.name() + ").";
        }
    };

    struct UnknownPixelType: public Exception {

        UnknownPixelType(const std::type_info& type): Exception(message(type)) { }
        UnknownPixelType(const std::string& type,const char* identity): Exception(message(type,identity)) { }

        ExceptionCode code() const throw() { return UNKN_PIX_TYPE; }

    private:

        static std::string message(const std::type_info& type) {
            return std::string("Unknown pixel type (") + type.name() + ").";
        }
        static std::string message(const std::string& type,const char* identity) {
            return std::string("Unknown pixel type (") + type + ") for format "+identity+".";
        }
    };

    struct AlreadyKnownTag: public Exception {

        AlreadyKnownTag(const std::string& tag,const std::string& format): Exception(message(tag,format)) { }

        ExceptionCode code() const throw() { return ALREADY_KN_TAG; }

    private:

        static std::string message(const std::string& tag,const std::string& format) {
            return std::string("Format "+format+": tag "+tag+" already in use.");
        }
    };

    struct UnknownFileFormat: public IOException {

        UnknownFileFormat(const std::string& fmt): IOException(std::string("Unknown ")+fmt+" format.") { }

        template <typename CharT,typename Traits>
        UnknownFileFormat(std::basic_ios<CharT,Traits>& ios): IOException(ios,std::string("Unknown file format.")) { }

        ExceptionCode code() const throw() { return UNKN_FILE_FMT; }
    };

    struct UnknownFileSuffix: public IOException {
        UnknownFileSuffix(const std::string& fmt): IOException(std::string("Unknown ")+fmt+" suffix.") { }
        ExceptionCode code() const throw() { return UNKN_FILE_SUFFIX; }
    };

    struct UnknownNamedFileFormat: public Exception {
        UnknownNamedFileFormat(const std::string& name): Exception(std::string("Unknown format for file "+name+".")) { }
        ExceptionCode code() const throw() { return UNKN_NAMED_FILE_FMT; }
    };

    struct NonMatchingNamedFileFormat: public Exception {
        NonMatchingNamedFileFormat(const std::string& name,const std::string& fmt): Exception(std::string("Cannot save the image "+name+" as a "+fmt+" file.")) { }

        ExceptionCode code() const throw() { return NON_MATCH_NAMED_FILE_FMT; }
    };

    struct NoMatchingFileFormat: public IOException {
        NoMatchingFileFormat(std::ostream& os): IOException(os,std::string("No IO is able to write this type of image.")) { }

        ExceptionCode code() const throw() { return NO_FILE_FMT; }
    };

    //  Revisit (TODO).

    struct NoImageArgument: public Exception {
        NoImageArgument(): Exception("No image argument.") { }

        ExceptionCode code() const throw() { return NO_IMG_ARG; }
    };

    struct DifferentImages: public Exception {
        DifferentImages(): Exception("Different image types.") { }

        ExceptionCode code() const throw() { return DIFF_IMG; }
    };
}
