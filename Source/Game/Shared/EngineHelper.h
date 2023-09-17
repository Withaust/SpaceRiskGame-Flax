#pragma once

#include <Engine/Scripting/Script.h>
#define LZ4_STATIC_LINKING_ONLY
#define LZ4_HC_STATIC_LINKING_ONLY
#include <Game/Thirdparty/lz4/lz4.h>
#include <Game/Thirdparty/lz4/lz4hc.h>

API_CLASS() class GAME_API EngineHelper : public ScriptingObject
{
    DECLARE_SCRIPTING_TYPE(EngineHelper);

public:

    template <class T>
    static void FindActors(Actor* target, Array<T*>& result)
    {
        for (const auto& child : target->GetChildren<T>())
        {
            result.Add(child);
        }

        for (int32 i = 0; i < target->Children.Count(); i++)
        {
            FindActors(target->Children[i], result);
        }
    }

    template <class T>
    static void FindScripts(Actor* target, Array<T*>& result)
    {
        for (const auto& child : target->GetScripts<T>())
        {
            result.Add(child);
        }

        for (int32 i = 0; i < target->Children.Count(); i++)
        {
            FindScripts(target->Children[i], result);
        }
    }

private:

    static Array<char> _compressBuffer;

public:

    static bool Compress(void* target, int length)
    {
        return Compress(target, length, _compressBuffer);
    }

    static bool Compress(void* target, int length, Array<char>& compressed)
    {
        int bound = LZ4_compressBound(length);

        _compressBuffer.Resize(bound);

        const int compressed_data_size = LZ4_compress_HC((const char*)(target), &_compressBuffer[0], length, bound, LZ4HC_CLEVEL_MAX);

        if (compressed_data_size <= 0)
        {
            return false;
        }

        _compressBuffer.Resize(compressed_data_size);
        compressed = _compressBuffer;
        return true;
    }

    static bool Decompress(const Array<char>& data, int srcSize)
    {
        return Decompress(data, srcSize, _compressBuffer);
    }

    static bool Decompress(const Array<char>& data, int srcSize, Array<char>& decompressed)
    {
        _compressBuffer.Resize(srcSize);

        const int decompressed_size = LZ4_decompress_safe(&data[0], &_compressBuffer[0], data.Count(), srcSize);

        if (decompressed_size < 0 || decompressed_size != srcSize)
        {
            return false;
        }

        decompressed = _compressBuffer;
        return true;
    }

    static const Array<char>& GetCompressBuffer()
    {
        return _compressBuffer;
    }
};
