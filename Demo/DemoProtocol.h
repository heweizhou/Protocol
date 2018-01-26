//
//  DeviceRegiterProtocol.hpp
//  Parser
//
//  Created by 周贺伟 on 2018/1/18.
//

#ifndef DeviceRegiterProtocol_hpp
#define DeviceRegiterProtocol_hpp

#include <stdio.h>

#include <string>
#include <string.h>
#include "protocol.h"


typedef struct _tag_protcol_demo_1 : public IProtcolBase{
    std::string     service_name;
    int32           device_type;
    std::string     reverse_data;
    
    std::string pack(){
        return CStaticProtocolBase::pack<std::string, int32, std::string>(service_name, device_type, reverse_data);
    }
    
    bool unPack(std::string& data){
        return CStaticProtocolBase::unPack<std::string, int32, std::string>(data, service_name, device_type, reverse_data);
    }
    
}protcol_demo_1;


class protcol_demo_2{
public:
    std::string pack(){
        return proto_packer.pack(m_service_name, m_device_type, m_reverse_data);
    }
    
    bool unPack(std::string& data){
        return proto_packer.unPack(data, m_service_name, m_device_type, m_reverse_data);
    }

private:
    std::string     m_service_name;
    int32           m_device_type;
    std::string     m_reverse_data;
    
    CProtocolBase<std::string, int32, std::string>    proto_packer;
}protcol_demo_2;



#endif /* DeviceRegiterProtocol_hpp */
