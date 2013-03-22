

#include <errno.h>
#include "sigar.h"
#include "sigar_fileinfo.h"
#include "sigar_log.h"
#include "sigar_format.h"
#include "sigar_ptql.h"
#include "./node_sigar_utils.cc"

#include "v8u.hpp"

class NodeSigar : public node::ObjectWrap {
public:
	NodeSigar(sigar_t* ptr);
	~NodeSigar();
	V8_SCTOR();

	static V8_SCB(Init);
	static V8_SCB(SigarVersionGet);
	static V8_SCB(SigarPasswordGet);
	static V8_SCB(SigarFormatSize);

	static V8_SCB(SigarTestNetAdress);

	static V8_SCB(GetMem);
	static V8_SCB(GetSwap);
	static V8_SCB(SigarUptimeGet);
	static V8_SCB(SigarLoadavgGet);
	static V8_SCB(SigarResourceLimitGet);
	static V8_SCB(SigarWhoListGet);
	static V8_SCB(SigarSysInfoGet);
	static V8_SCB(SigarFqdnGet);
	static V8_SCB(SigarCpuGet);
	static V8_SCB(SigarCpuListGet);
	static V8_SCB(SigarCpuInfoListGet);
	static V8_SCB(SigarProcListGet);
	static V8_SCB(SigarProcStatGet);
	static V8_SCB(SigarProcMemGet);
	static V8_SCB(SigarProcCredGet);
	static V8_SCB(SigarProcTimeGet);
	static V8_SCB(SigarProcCpuGet);
	static V8_SCB(SigarProcStateGet);
	static V8_SCB(SigarProcArgsGet);
	static V8_SCB(SigarProcEnvGet);
	static V8_SCB(SigarProcFdGet);
	static V8_SCB(SigarProcExeGet);
	static V8_SCB(SigarProcModulesGet);
	static V8_SCB(SigarProcPortGet);
	static V8_SCB(SigarThreadCpuGet);
	static V8_SCB(SigarFileSystemListGet);
	static V8_SCB(SigarFileSystemUsageGet);
	static V8_SCB(SigarFileSystemPingGet);
	static V8_SCB(SigarNetInfoGet);
	static V8_SCB(SigarNetRouteListGet);
	static V8_SCB(SigarNetInterfaceListGet);
	static V8_SCB(SigarNetInterfaceConfigGet);
	static V8_SCB(SigarNetInterfaceConfigPrimaryGet);
	static V8_SCB(SigarNetInterfaceStatGet);
	static V8_SCB(SigarNetConnectionListGet);
	static V8_SCB(SigarNetListenAddressGet);
	static V8_SCB(SigarNetStatGet);
	static V8_SCB(SigarNetStatPortGet);
	static V8_SCB(SigarTcpGet);
	static V8_SCB(SigarNfsClientV2Get);
	static V8_SCB(SigarNfsServerV2Get);
	static V8_SCB(SigarArpListGet);
	static V8_SCB(SigarRpcPing);

	NODE_STYPE(NodeSigar);
private:
	sigar_t *sigar;
};

