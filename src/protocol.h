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
    
//    std::string pack(IProtcolBase base){
//        return base.pack();
//    }
    
    std::string pack(IProtcolBase& base){
        return base.pack();
    }
    
    std::string pack(std::shared_ptr<IProtcolBase> base){
        return base->pack();
    }
    
    std::string pack(std::string arg){
        long len = arg.length();
        return std::string((char*)&len, sizeof(len)) + arg;
    }
    
    std::string pack(float arg){
        return std::string((char*)&arg, sizeof(arg));
    }
    
    std::string pack(double arg){
        return std::string((char*)&arg, sizeof(arg));
    }
    
    std::string pack(long arg){
        return std::string((char*)&arg, sizeof(arg));
    }
    
    std::string pack(int arg){
        return std::string((char*)&arg, sizeof(arg));
    }
public:
    
    
    bool unPack(std::string& data, IProtcolBase* base){
        return base->unPack(data);
    }
    
//    bool unPack(std::string& data, IProtcolBase base){
//        return base.pack();
//    }
    
    bool unPack(std::string& data, IProtcolBase& base){
        return base.unPack(data);
    }
    
    bool unPack(std::string& data, std::shared_ptr<IProtcolBase> base){
        return base->unPack(data);
    }
    
    bool unPack(std::string& data, std::string& arg){
        
        if (data.length() < sizeof(long)) {
            return false;
        }
        
        const char* pBuffer = data.c_str();
        long len = *(long*)pBuffer;
        
        if (data.length() < len + sizeof(long)) {
            return false;
        }
        
        arg = data.substr(sizeof(long), len);
        data = data.substr(sizeof(long) + len);
        
        return true;
    }
    
    bool unPack(std::string& data, float& arg){
        
        if (data.length() < sizeof(float)) {
            return false;
        }
        
        const char* pBuffer = data.c_str();
        arg = *(float*)pBuffer;
        data = data.substr(sizeof(float));
        
        return true;
    }
    
    bool unPack(std::string& data, double& arg){
        
        if (data.length() < sizeof(double)) {
            return false;
        }
        
        const char* pBuffer = data.c_str();
        arg = *(double*)pBuffer;
        data = data.substr(sizeof(double));
        
        return true;
    }
    
    bool unPack(std::string& data, long& arg){
        
        if (data.length() < sizeof(long)) {
            return false;
        }
        
        const char* pBuffer = data.c_str();
        arg = *(long*)pBuffer;
        data = data.substr(sizeof(long));
        
        return true;
    }
    
    bool unPack(std::string& data, int& arg){
        
        if (data.length() < sizeof(int)) {
            return false;
        }
        
        const char* pBuffer = data.c_str();
        arg = *(int*)pBuffer;
        data = data.substr(sizeof(int));
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
    
//    template <class _F>
//    static std::string pack( typename  std::enable_if<std::is_base_of<IProtcolBase, _F>::value && !std::is_reference<_F>::value, _F>::type __f){
//        return __f.pack();
//    }
    
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
        long len = __f.length();
        return std::string((char*)&len, sizeof(len)) + __f;
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
        
        if (data.length() < sizeof(long)) {
            return false;
        }
        
        const char* pBuffer = data.c_str();
        long len = *(long*)pBuffer;
        
        if (data.length() < len + sizeof(long)) {
            return false;
        }
        
        __f = data.substr(sizeof(long), len);
        data = data.substr(sizeof(long) + len);
        
        return true;
    }
    
    template <class _F>
    static bool unPack(std::string& data, typename std::enable_if<std::is_arithmetic<_F>::value, _F>::type& __f){
        if (data.length() < sizeof(_F)) {
            return false;
        }
        
        const char* pBuffer = data.c_str();
        __f = *(_F*)pBuffer;
        data = data.substr(sizeof(_F));
        
        return true;
    }
    
    template <class _F, class... _R>
    static bool unPack(std::string& data, _F& __f, _R&... __r){
        return CStaticProtocolBase::unPack<_F>(data, __f) & CStaticProtocolBase::unPack<_R...>(data, __r...);
    }
    
};



#endif /* protocol_h */
