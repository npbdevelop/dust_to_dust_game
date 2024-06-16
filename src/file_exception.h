#ifndef FILE_EXCEPTION_H
#define FILE_EXCEPTION_H

#include <string>
#include <string_view>
#include <format>

/*
Exception class for file handling. Capable of returning specific exceptions for: failure to open 
file, failure to parse line
*/
class FileException
{
private:
    std::string mPath;
    std::string mError;
    std::string mType;
    std::string mText;
    int mLineNum;

public:
    FileException(const std::string_view path, std::string_view type, std::string_view text = "NONE", int lineNum = -1)
        : mPath { path }, mType { type }, mText { text }, mLineNum { lineNum }
    {
        if (type == "fopen")
        {
            mError = std::format("Failed to open \'{}\'", mPath);
        }
        else if (type == "parse")
        {
            mError = std::format("Failed to parse line {} from \'{}\'\nReceived input: \'{}\'", mLineNum, mPath, mText); 
        }
        else if (type == "load")
        {
            mError = std::format("Failed to load asset at \'{}\'", mPath);
        }
        else
        {
            mError = std::format("File \'{}\' returned an uknown file exception", mPath);
        }
    }

    const std::string& fileError() const { return mError; }
};

#endif