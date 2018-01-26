//
//  protocol.h
//  test
//
//  Created by 周贺伟 on 2018/1/21.
//  Copyright © 2018年 周贺伟. All rights reserved.
//

#ifndef protocol_h
#define protocol_h

#include <string>
#include <memory>
#include <type_traits>
#include <inttypes.h>

#if __SIZEOF_LONG__ == 4
typedef char        char8;
typedef short       short16;
typedef float       float32;
typedef long long   int64;
typedef int         int32;
typedef double      double64;
#elif __SIZEOF_LONG__ == 8
typedef char        char8;
typedef short       short16;
typedef float       float32;
typedef long        int64;
typedef int         int32;
typedef double      double64;
#endif

class IProtcolBase{
public:
    virtual std::string pack() = 0;
    virtual bool unPack(std::string& data) = 0;
};

template <typename... Args>
class CProtocolBase;

template <typename T>
class CProtocolBase<T>{
public:
    
    std::string pack(IProtcolBase* base){
        return base->pack();
    }
    
    std::string pack(IProtcolBase& base){
        return base.pack();
    }
    
    std::string pack(std::shared_ptr<IProtcolBase> base){
        return base->pack();
    }
    
    std::string pack(std::string arg){
        int32 len = arg.length();
        return std::string((char8*)&len, sizeof(len)) + arg;
    }
    
    std::string pack(float32 arg){
        return std::string((char8*)&arg, sizeof(arg));
    }
    
    std::string pack(double64 arg){
        return std::string((char8*)&arg, sizeof(arg));
    }
    
    std::string pack(int64 arg){
        return std::string((char8*)&arg, sizeof(arg));
    }
    
    std::string pack(int32 arg){
        return std::string((char8*)&arg, sizeof(arg));
    }
    
    std::string pack(char8 arg){
        return std::string((char8*)&arg, sizeof(arg));
    }
    
    std::string pack(short16 arg){
        return std::string((char8*)&arg, sizeof(arg));
    }
    
public:
    
    
    bool unPack(std::string& data, IProtcolBase* base){
        return base->unPack(data);
    }
    
    bool unPack(std::string& data, IProtcolBase& base){
        return base.unPack(data);
    }
    
    bool unPack(std::string& data, std::shared_ptr<IProtcolBase> base){
        return base->unPack(data);
    }
    
    bool unPack(std::string& data, std::string& arg){
        
        if (data.length() < sizeof(int32)) {
            return false;
        }
        
        const char8* pBuffer = data.c_str();
        int32 len = *(int32*)pBuffer;
        
        if (data.length() < len + sizeof(int32)) {
            return false;
        }
        
        arg = data.substr(sizeof(int32), len);
        data = data.substr(sizeof(int32) + len);
        
        return true;
    }
    
    bool unPack(std::string& data, float32& arg){
        
        if (data.length() < sizeof(float32)) {
            return false;
        }
        
        const char8* pBuffer = data.c_str();
        arg = *(float32*)pBuffer;
        data = data.substr(sizeof(float32));
        
        return true;
    }
    
    bool unPack(std::string& data, double64& arg){
        
        if (data.length() < sizeof(double64)) {
            return false;
        }
        
        const char8* pBuffer = data.c_str();
        arg = *(double64*)pBuffer;
        data = data.substr(sizeof(double64));
        
        return true;
    }
    
    bool unPack(std::string& data, int64& arg){
        
        if (data.length() < sizeof(int64)) {
            return false;
        }
        
        const char8* pBuffer = data.c_str();
        arg = *(int64*)pBuffer;
        data = data.substr(sizeof(int64));
        
        return true;
    }
    
    bool unPack(std::string& data, int32& arg){
        
        if (data.length() < sizeof(int32)) {
            return false;
        }
        
        const char8* pBuffer = data.c_str();
        arg = *(int32*)pBuffer;
        data = data.substr(sizeof(char8));
        return true;
    }
    
    bool unPack(std::string& data, char8& arg){
        
        if (data.length() < sizeof(char8)) {
            return false;
        }
        
        const char8* pBuffer = data.c_str();
        arg = *(char8*)pBuffer;
        data = data.substr(sizeof(char8));
        return true;
    }
    
    bool unPack(std::string& data, short16& arg){
        
        if (data.length() < sizeof(short16)) {
            return false;
        }
        
        const char8* pBuffer = data.c_str();
        arg = *(short16*)pBuffer;
        data = data.substr(sizeof(short16));
        return true;
    }
};

template <class _F, class... _R>
class CProtocolBase<_F, _R...> {
    
public:
    
    std::string pack(_F __f, _R... __r){
        return CProtocolBase<_F>().pack(__f) + CProtocolBase<_R...>().pack(__r...);
    }
    
    bool unPack(std::string data, _F& __f, _R&... __r){
        return CProtocolBase<_F>().unPack(data, __f) & CProtocolBase<_R...>().unPack(data, __r...);
    }
};


class CStaticProtocolBase
{
public: //pack functions
    
    template <class _F>
    static std::string pack( typename  std::enable_if<std::is_base_of<IProtcolBase, _F>::value, _F>::type& __f){
        return __f.pack();
    }
    
    template <class _F>
    static std::string pack( typename  std::enable_if<std::is_base_of<IProtcolBase, _F>::value, _F>::type* __f){
        return __f->pack();
    }
    
    template <class _F>
    static std::string pack( std::shared_ptr<typename  std::enable_if<std::is_base_of<IProtcolBase, _F>::value, _F>::type> __f){
        return __f->pack();
    }
    
    
    template <class _F>
    static std::string pack( typename std::enable_if<std::is_arithmetic<_F>::value, _F>::type __f){
        return std::string((char*)&__f, sizeof(__f));
    }
    
    template <class _F>
    static std::string pack(std::string __f){
        int32 len = __f.length();
        return std::string((char8*)&len, sizeof(len)) + __f;
    }
    
    template <class _F, class... _R>
    static std::string pack(_F __f, _R... __r){
        return CStaticProtocolBase::template pack<_F>(__f) + CStaticProtocolBase:: template pack<_R...>(__r...);
    }
    
    
public://unpack functions
    template <class _F>
    static bool unPack(std::string& data, typename  std::enable_if<std::is_base_of<IProtcolBase, _F>::value, _F>::type& __f){
        return __f.unPack(data);
    }
    
    template <class _F>
    static bool unPack(std::string& data, typename  std::enable_if<std::is_base_of<IProtcolBase, _F>::value, _F>::type* __f){
        return __f->unPack(data);
    }
    
    template <class _F>
    static bool unPack(std::string& data, std::shared_ptr<typename  std::enable_if<std::is_base_of<IProtcolBase, _F>::value, _F>::type> __f){
        return __f.unPack(data);
    }
    
   
    template <class _F>
    static bool unPack(std::string& data, std::string& __f){
        
        if (data.length() < sizeof(int32)) {
            return false;
        }
        
        const char8* pBuffer = data.c_str();
        int32 len = *(int32*)pBuffer;
        
        if (data.length() < len + sizeof(int32)) {
            return false;
        }
        
        __f = data.substr(sizeof(int32), len);
        data = data.substr(sizeof(int32) + len);
        
        return true;
    }
    
    template <class _F>
    static bool unPack(std::string& data, typename std::enable_if<std::is_arithmetic<_F>::value, _F>::type& __f){
        if (data.length() < sizeof(_F)) {
            return false;
        }
        
        const char8* pBuffer = data.c_str();
        __f = *(_F*)pBuffer;
        data = data.substr(sizeof(_F));
        
        return true;
    }
    
    template <class _F, class... _R>
    static bool unPack(std::string& data, _F& __f, _R&... __r){
        return CStaticProtocolBase::unPack<_F>(data, __f) & CStaticProtocolBase::unPack<_R...>(data, __r...);
    }
    
public: //PeekMessage
    
    template <class... _R>
    static bool peek(std::string data, _R&... __r){
        return CStaticProtocolBase::unPack<_R...>(data, __r...);
    }
    
};



#endif /* protocol_h */
