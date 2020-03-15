#include <typeinfo>
#include "Network.hpp"
#include "utils/utils_internal_interface.hpp"
#include "linalg.hpp"

using namespace std;

namespace cytnx_extension{
    using namespace cytnx;
    void Network_base::Fromfile(const std::string &fname){
        cytnx_error_msg(true,"[ERROR][Network][Fromfile] call from uninitialize network.%s","\n");
    }

    void Network_base::PutCyTensor(const std::string &name, const CyTensor &utensor, const bool &is_clone){
        cytnx_error_msg(true,"[ERROR][Network][PutCyTensor] call from uninitialize network.%s","\n");
    }
    void Network_base::PutCyTensor(const cytnx_uint64 &idx, const CyTensor &utensor, const bool &is_clone){
        cytnx_error_msg(true,"[ERROR][Network][PutCyTensor] call from uninitialize network.%s","\n");
    }
    void Network_base::Clear(){
        cytnx_error_msg(true,"[ERROR][Network][Clear] call from uninitialize network.%s","\n");
    }
    CyTensor Network_base::Launch(){
        cytnx_error_msg(true,"[ERROR][Network][Launch] call from uninitialize network.%s","\n");
        return CyTensor();
    }
    boost::intrusive_ptr<Network_base> Network_base::clone(){
        cytnx_error_msg(true,"[ERROR][Network][clone] call from uninitialize network. %s","\n");
        return nullptr;
    }

}//namespace cytnx