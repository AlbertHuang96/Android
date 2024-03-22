#ifndef SHADER_H
#define SHADER_H

#include <glm.hpp>
#include <string>

#include <UtilGL.h>
#include <UtilLog.h>
#include <shader.h>


Shader::Shader(const char* vsFilePath, const char* fsFilePath)
{
    char* codeVS = UtilGL::openTextFile(vsFilePath);
    char* codeFS = UtilGL::openTextFile(fsFilePath);
    ID = UtilGL::CreateProgram(codeVS, codeFS);
}



/*bool Shader::loadAsset(const char* filePath, std::vector<char>& shaderCode)
{
    AAssetManager* mgr = AAssetManager_fromJava(env, assetManager);
    // assets are not thread safe
    std::lock_guard<std::mutex> guard(mLock);
    AAsset* asset = AAssetManager_open ( mgr, filePath, AASSET_MODE_STREAMING );

    if ( !asset )
    {
        LOGCATE ( "File::LoadContent - Can't open file %s.", filePath );
        return false;
    }

    auto const size = static_cast<size_t> ( AAsset_getLength ( asset ) );

    if ( !size )
    {
        LOGCATE ( "File::LoadContent - File %s is empty!", filePath );
        AAsset_close ( asset );
        shaderCode.clear ();
        return true;
    }

    shaderCode.resize ( size );
    auto const readBytes = static_cast<size_t> ( AAsset_read ( asset, shaderCode.data (), size ) );
    AAsset_close ( asset );

    if ( size == readBytes )
    {
        return true;
    }

    LOGCATE ( "File::LoadContent - Can't load whole file content %s.", filePath );
    return false;
}*/
#endif