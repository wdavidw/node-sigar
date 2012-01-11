
#include <v8.h>
#include <node.h>

#include "./node_sigar_format.cc"

using namespace node;
using namespace v8;

Handle<String> node_sigar_net_address_to_string(sigar_net_address_t *address)
{
	char addr_str[SIGAR_INET6_ADDRSTRLEN];
	sigar_net_address_to_string(NULL, address, addr_str);
    return String::New(addr_str);
}

// TODO: This is completly wrong
int node_sigar_str2net_address(Handle<String> bytes, sigar_net_address_t *address)
{
    //    long len = bytes->Length();
    //    long len = bytes->Utf8Length();
    String::Utf8Value name(bytes->ToString());
    long len = sizeof(name);
    switch (len) {
      case 4:
        address->family = sigar_net_address_t::SIGAR_AF_INET;
        break;
      case 4*4:
        address->family = sigar_net_address_t::SIGAR_AF_INET6;
        break;
      default:
        return EINVAL;
    }
    memcpy(*name, &address->addr.in6, len);
    return SIGAR_OK;
}





