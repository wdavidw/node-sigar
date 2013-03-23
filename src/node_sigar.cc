
#include "node_sigar.h"
#include "v8u.hpp"

using namespace node;
using namespace v8;
NodeSigar::NodeSigar(sigar_t* ptr): sigar(ptr) {}
NodeSigar::~NodeSigar() {
	sigar_close(sigar);
}

V8_ESCTOR(NodeSigar) { V8_CTOR_NO_JS }

NODE_ETYPE(NodeSigar, "NodeSigar") {
	//Local<Function> func = templ->GetFunction();
	target->Set(v8u::Symbol("init"), v8u::Func(Init)->GetFunction());
	target->Set(v8u::Symbol("version"), v8u::Func(SigarVersionGet)->GetFunction());
	target->Set(v8u::Symbol("password"), v8u::Func(SigarPasswordGet)->GetFunction());
	target->Set(v8u::Symbol("formatSize"), v8u::Func(SigarFormatSize)->GetFunction());

	// Testing, see './test/TodoTest.coffee'
	target->Set(v8u::Symbol("testNetAdress"), v8u::Func(SigarTestNetAdress)->GetFunction());

	// System
	NODE_SET_PROTOTYPE_METHOD(templ, "mem", GetMem);
	NODE_SET_PROTOTYPE_METHOD(templ, "swap", GetSwap);
	NODE_SET_PROTOTYPE_METHOD(templ, "uptime", SigarUptimeGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "loadavg", SigarLoadavgGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "resourceLimit", SigarResourceLimitGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "whoList", SigarWhoListGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "sysInfo", SigarSysInfoGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "fqdn", SigarFqdnGet);
	// CPU
	NODE_SET_PROTOTYPE_METHOD(templ, "cpu", SigarCpuGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "cpuList", SigarCpuListGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "cpuInfoList", SigarCpuInfoListGet);
	// Process
	NODE_SET_PROTOTYPE_METHOD(templ, "procList", SigarProcListGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procStat", SigarProcStatGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procMem", SigarProcMemGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procCred", SigarProcCredGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procTime", SigarProcTimeGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procCpu", SigarProcCpuGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procState", SigarProcStateGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procArgs", SigarProcArgsGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procEnv", SigarProcEnvGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procFd", SigarProcFdGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procExe", SigarProcExeGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procModules", SigarProcModulesGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "procPort", SigarProcPortGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "threadCpu", SigarThreadCpuGet);
	// Disk
	NODE_SET_PROTOTYPE_METHOD(templ, "fileSystemList", SigarFileSystemListGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "fileSystemUsage", SigarFileSystemUsageGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "fileSystemPing", SigarFileSystemPingGet);
	// Network
	NODE_SET_PROTOTYPE_METHOD(templ, "netInfo", SigarNetInfoGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "netRouteList", SigarNetRouteListGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "netInterfaceList", SigarNetInterfaceListGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "netInterfaceConfig", SigarNetInterfaceConfigGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "netInterfaceConfigPrimary", SigarNetInterfaceConfigPrimaryGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "netInterfaceStat", SigarNetInterfaceStatGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "netConnectionList", SigarNetConnectionListGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "netListenAddress", SigarNetListenAddressGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "netStat", SigarNetStatGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "netStatPort", SigarNetStatPortGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "tcp", SigarTcpGet);
	NODE_SET_PROTOTYPE_METHOD(templ, "nfsClientV2", SigarNfsClientV2Get);
	NODE_SET_PROTOTYPE_METHOD(templ, "nfsServerV2", SigarNfsServerV2Get);
	NODE_SET_PROTOTYPE_METHOD(templ, "arpList", SigarArpListGet);
	// Misc
	NODE_SET_PROTOTYPE_METHOD(templ, "rpcPing", SigarRpcPing);
	//target->Set(v8u::Symbol("NodeSigar"), s_ct->GetFunction());

} NODE_TYPE_END()
V8_POST_TYPE(NodeSigar);

V8_SCB(NodeSigar::Init) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));

	sigar_t* out;
	int status = sigar_open(&out);

	if(status == SIGAR_OK) return (new NodeSigar(out))->Wrapped();
	V8_STHROW(v8u::Err("unknown error initializing"));
}

V8_SCB(NodeSigar::SigarVersionGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	//V8_M_UNWRAP(NodeSigar, args.This());

	sigar_version_t *version = sigar_version_get();
	Handle<Object> h_version = Object::New();
	h_version->Set(v8u::Symbol("build_date"), String::New(version->build_date));
	h_version->Set(v8u::Symbol("scm_revision"), String::New(version->scm_revision));
	h_version->Set(v8u::Symbol("version"), String::New(version->version));
	h_version->Set(v8u::Symbol("archname"), String::New(version->archname));
	h_version->Set(v8u::Symbol("archlib"), String::New(version->archlib));
	h_version->Set(v8u::Symbol("binname"), String::New(version->binname));
	h_version->Set(v8u::Symbol("description"), String::New(version->description));
	h_version->Set(v8u::Symbol("major"), Number::New(version->major));
	h_version->Set(v8u::Symbol("minor"), Number::New(version->minor));
	h_version->Set(v8u::Symbol("maint"), Number::New(version->maint));
	h_version->Set(v8u::Symbol("build"), Number::New(version->build));
	return h_version;
}

V8_SCB(NodeSigar::SigarTestNetAdress) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	//V8_M_UNWRAP(NodeSigar, args.This());

	Handle<String> h_source = args[0]->ToString();
	sigar_net_address_t address;
	int status = node_sigar_str2net_address(h_source, &address);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "node_sigar_str2net_address error");
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<String> h_result = node_sigar_net_address_to_string(&address);
	return h_result;
}

V8_SCB(NodeSigar::SigarPasswordGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	//V8_M_UNWRAP(NodeSigar, args.This());

	String::Utf8Value prompt(args[0]->ToString());
	char* password = sigar_password_get(*prompt);
	return String::New(password);
}

V8_SCB(NodeSigar::SigarFormatSize) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	//V8_M_UNWRAP(NodeSigar, args.This());

	sigar_uint64_t size = args[0]->IntegerValue();
//		char *buffer;
	char buffer[56];
	char *result = sigar_format_size(size, buffer);
	return String::New(result);
}

V8_CB(NodeSigar::GetMem) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_mem_t mem;
	int status = sigar_mem_get(inst->sigar, &mem);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_mem_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_mem = Object::New();
	h_mem->Set(v8u::Symbol("ram"), Number::New(mem.ram));
	h_mem->Set(v8u::Symbol("total"), Number::New(mem.total));
	h_mem->Set(v8u::Symbol("used"), Number::New(mem.used));
	h_mem->Set(v8u::Symbol("free"), Number::New(mem.free));
	h_mem->Set(v8u::Symbol("actual_used"), Number::New(mem.actual_used));
	h_mem->Set(v8u::Symbol("actual_free"), Number::New(mem.actual_free));
	h_mem->Set(v8u::Symbol("used_percent"), Number::New(mem.used_percent));
	h_mem->Set(v8u::Symbol("free_percent"), Number::New(mem.free_percent));

	return scope.Close(h_mem);
} V8_CB_END();

V8_CB(NodeSigar::GetSwap) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_swap_t swap;
	int status = sigar_swap_get(inst->sigar, &swap);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_swap_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_swap = Object::New();
	h_swap->Set(v8u::Symbol("total"), Number::New(swap.total));
	h_swap->Set(v8u::Symbol("used"), Number::New(swap.used));
	h_swap->Set(v8u::Symbol("free"), Number::New(swap.free));
	h_swap->Set(v8u::Symbol("page_in"), Number::New(swap.page_in));
	h_swap->Set(v8u::Symbol("page_out"), Number::New(swap.page_out));
	return scope.Close(h_swap);
} V8_CB_END();

V8_CB(NodeSigar::SigarUptimeGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_uptime_t uptime;
	int status = sigar_uptime_get(inst->sigar, &uptime);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_uptime_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	return scope.Close(Number::New(uptime.uptime));
} V8_CB_END();

V8_CB(NodeSigar::SigarLoadavgGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_loadavg_t loadavg;
	int status = sigar_loadavg_get(inst->sigar, &loadavg);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_uptime_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_loadavgs = Array::New(3);
	h_loadavgs->Set(0, Number::New(loadavg.loadavg[0]));
	h_loadavgs->Set(1, Number::New(loadavg.loadavg[1]));
	h_loadavgs->Set(2, Number::New(loadavg.loadavg[2]));
	return scope.Close(h_loadavgs);
} V8_CB_END();

V8_CB(NodeSigar::SigarResourceLimitGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_resource_limit_t resourcelimit;
	int status = sigar_resource_limit_get(inst->sigar, &resourcelimit);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_cpu_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_resourcelimit= Object::New();
	h_resourcelimit->Set(v8u::Symbol("cpu_cur"), Number::New(resourcelimit.cpu_cur));
	h_resourcelimit->Set(v8u::Symbol("cpu_max"), Number::New(resourcelimit.cpu_max));
	h_resourcelimit->Set(v8u::Symbol("file_size_cur"), Number::New(resourcelimit.file_size_cur));
	h_resourcelimit->Set(v8u::Symbol("file_size_max"), Number::New(resourcelimit.file_size_max));
	h_resourcelimit->Set(v8u::Symbol("pipe_size_cur"), Number::New(resourcelimit.pipe_size_cur));
	h_resourcelimit->Set(v8u::Symbol("pipe_size_max"), Number::New(resourcelimit.pipe_size_max));
	h_resourcelimit->Set(v8u::Symbol("data_cur"), Number::New(resourcelimit.data_cur));
	h_resourcelimit->Set(v8u::Symbol("data_max"), Number::New(resourcelimit.data_max));
	h_resourcelimit->Set(v8u::Symbol("stack_cur"), Number::New(resourcelimit.stack_cur));
	h_resourcelimit->Set(v8u::Symbol("stack_max"), Number::New(resourcelimit.stack_max));
	h_resourcelimit->Set(v8u::Symbol("core_cur"), Number::New(resourcelimit.core_cur));
	h_resourcelimit->Set(v8u::Symbol("core_max"), Number::New(resourcelimit.core_max));
	h_resourcelimit->Set(v8u::Symbol("memory_cur"), Number::New(resourcelimit.memory_cur));
	h_resourcelimit->Set(v8u::Symbol("memory_max"), Number::New(resourcelimit.memory_max));
	h_resourcelimit->Set(v8u::Symbol("processes_cur"), Number::New(resourcelimit.processes_cur));
	h_resourcelimit->Set(v8u::Symbol("processes_max"), Number::New(resourcelimit.processes_max));
	h_resourcelimit->Set(v8u::Symbol("open_files_cur"), Number::New(resourcelimit.open_files_cur));
	h_resourcelimit->Set(v8u::Symbol("open_files_max"), Number::New(resourcelimit.open_files_max));
	h_resourcelimit->Set(v8u::Symbol("virtual_memory_cur"), Number::New(resourcelimit.virtual_memory_cur));
	h_resourcelimit->Set(v8u::Symbol("virtual_memory_max"), Number::New(resourcelimit.virtual_memory_max));
	return scope.Close(h_resourcelimit);
} V8_CB_END();

V8_CB(NodeSigar::SigarWhoListGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_who_list_t wholist;
	int status = sigar_who_list_get(inst->sigar, &wholist);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_who_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_wholist = Array::New(wholist.number);
	unsigned i;
	for (i=0; i<wholist.number; i++) {
		sigar_who_t who = wholist.data[i];
		Handle<Object> h_who = Object::New();
		h_who->Set(v8u::Symbol("user"), String::New(who.user));
		h_who->Set(v8u::Symbol("device"), String::New(who.device));
		h_who->Set(v8u::Symbol("host"), String::New(who.host));
		h_who->Set(v8u::Symbol("time"), Number::New(who.time));
		h_wholist->Set(i, h_who);
	}
	sigar_who_list_destroy(inst->sigar, &wholist);
	return scope.Close(h_wholist);
} V8_CB_END();

V8_CB(NodeSigar::SigarSysInfoGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_sys_info_t sysinfo;
	int status = sigar_sys_info_get(inst->sigar, &sysinfo);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_who_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_sysinfo = Object::New();
	h_sysinfo->Set(v8u::Symbol("name"), String::New(sysinfo.name));
	h_sysinfo->Set(v8u::Symbol("version"), String::New(sysinfo.version));
	h_sysinfo->Set(v8u::Symbol("arch"), String::New(sysinfo.arch));
	h_sysinfo->Set(v8u::Symbol("machine"), String::New(sysinfo.machine));
	h_sysinfo->Set(v8u::Symbol("description"), String::New(sysinfo.description));
	h_sysinfo->Set(v8u::Symbol("patch_level"), String::New(sysinfo.patch_level));
	h_sysinfo->Set(v8u::Symbol("vendor"), String::New(sysinfo.vendor));
	h_sysinfo->Set(v8u::Symbol("vendor_version"), String::New(sysinfo.vendor_version));
	h_sysinfo->Set(v8u::Symbol("vendor_name"), String::New(sysinfo.vendor_name));
	h_sysinfo->Set(v8u::Symbol("vendor_code_name"), String::New(sysinfo.vendor_code_name));
	return scope.Close(h_sysinfo);
} V8_CB_END();

V8_CB(NodeSigar::SigarFqdnGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	char fqdn[SIGAR_FQDN_LEN];
	int status = sigar_fqdn_get(inst->sigar, fqdn, sizeof(fqdn));
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_fqdn_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	return scope.Close(String::New(fqdn));
} V8_CB_END();

V8_CB(NodeSigar::SigarCpuGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_cpu_t cpu;
	int status = sigar_cpu_get(inst->sigar, &cpu);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_cpu_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_cpu = Object::New();
	h_cpu->Set(v8u::Symbol("user"), Number::New(cpu.user));
	h_cpu->Set(v8u::Symbol("sys"), Number::New(cpu.sys));
	h_cpu->Set(v8u::Symbol("nice"), Number::New(cpu.nice));
	h_cpu->Set(v8u::Symbol("idle"), Number::New(cpu.idle));
	h_cpu->Set(v8u::Symbol("wait"), Number::New(cpu.wait));
	h_cpu->Set(v8u::Symbol("irq"), Number::New(cpu.irq));
	h_cpu->Set(v8u::Symbol("soft_irq"), Number::New(cpu.soft_irq));
	h_cpu->Set(v8u::Symbol("stolen"), Number::New(cpu.stolen));
	h_cpu->Set(v8u::Symbol("total"), Number::New(cpu.total));
	return scope.Close(h_cpu);
} V8_CB_END();

V8_CB(NodeSigar::SigarCpuListGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_cpu_list_t cpulist;
	int status = sigar_cpu_list_get(inst->sigar, &cpulist);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_cpu_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_cpus = Array::New(cpulist.number);
	unsigned i;
	for (i=0; i<cpulist.number; i++) {
		sigar_cpu_t cpu = cpulist.data[i];
		Handle<Object> h_cpu = Object::New();
		h_cpu->Set(v8u::Symbol("user"), Number::New(cpu.user));
		h_cpu->Set(v8u::Symbol("sys"), Number::New(cpu.sys));
		h_cpu->Set(v8u::Symbol("nice"), Number::New(cpu.nice));
		h_cpu->Set(v8u::Symbol("idle"), Number::New(cpu.idle));
		h_cpu->Set(v8u::Symbol("wait"), Number::New(cpu.wait));
		h_cpu->Set(v8u::Symbol("irq"), Number::New(cpu.irq));
		h_cpu->Set(v8u::Symbol("soft_irq"), Number::New(cpu.soft_irq));
		h_cpu->Set(v8u::Symbol("stolen"), Number::New(cpu.stolen));
		h_cpu->Set(v8u::Symbol("total"), Number::New(cpu.total));
		h_cpus->Set(i, h_cpu);
	}
	sigar_cpu_list_destroy(inst->sigar, &cpulist);
	return scope.Close(h_cpus);
} V8_CB_END();

V8_CB(NodeSigar::SigarCpuInfoListGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_cpu_info_list_t cpuinfo;
	int status = sigar_cpu_info_list_get(inst->sigar, &cpuinfo);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_cpu_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_cpus = Array::New(cpuinfo.number);
	unsigned i;
	for (i=0; i<cpuinfo.number; i++) {
		sigar_cpu_info_t cpu = cpuinfo.data[i];
		Handle<Object> h_cpu = Object::New();
		h_cpu->Set(v8u::Symbol("vendor"), String::New(cpu.vendor));
		h_cpu->Set(v8u::Symbol("model"), String::New(cpu.model));
		h_cpu->Set(v8u::Symbol("mhz"), Number::New(cpu.mhz));
		h_cpu->Set(v8u::Symbol("mhz_max"), Number::New(cpu.mhz_max));
		h_cpu->Set(v8u::Symbol("mhz_min"), Number::New(cpu.mhz_min));
		h_cpu->Set(v8u::Symbol("cache_size"), Number::New(cpu.cache_size));
		h_cpu->Set(v8u::Symbol("total_sockets"), Number::New(cpu.total_sockets));
		h_cpu->Set(v8u::Symbol("total_cores"), Number::New(cpu.total_cores));
		h_cpu->Set(v8u::Symbol("cores_per_socket"), Number::New(cpu.cores_per_socket));
		h_cpus->Set(i, h_cpu);
	}
	sigar_cpu_info_list_destroy(inst->sigar, &cpuinfo);
	return scope.Close(h_cpus);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcListGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_list_t proclist;
	int status = sigar_proc_list_get(inst->sigar, &proclist);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_proclist = Array::New(proclist.number);
	unsigned i;
	for (i=0; i<proclist.number; i++) {
		sigar_pid_t pid = proclist.data[i];
		h_proclist->Set(i, Number::New(pid));
	}
	sigar_proc_list_destroy(inst->sigar, &proclist);
	return scope.Close(h_proclist);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcStatGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_stat_t procstat;
	int status = sigar_proc_stat_get(inst->sigar, &procstat);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_procstat = Object::New();
	h_procstat->Set(v8u::Symbol("total"), Number::New(procstat.total));
	h_procstat->Set(v8u::Symbol("sleeping"), Number::New(procstat.sleeping));
	h_procstat->Set(v8u::Symbol("running"), Number::New(procstat.running));
	h_procstat->Set(v8u::Symbol("zombie"), Number::New(procstat.zombie));
	h_procstat->Set(v8u::Symbol("stopped"), Number::New(procstat.stopped));
	h_procstat->Set(v8u::Symbol("idle"), Number::New(procstat.idle));
	h_procstat->Set(v8u::Symbol("threads"), Number::New(procstat.threads));
	return scope.Close(h_procstat);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcMemGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_mem_t procmem;
	int status = sigar_proc_mem_get(inst->sigar, args[0]->IntegerValue(), &procmem);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_procmem = Object::New();
	h_procmem->Set(v8u::Symbol("size"), Number::New(procmem.size));
	h_procmem->Set(v8u::Symbol("resident"), Number::New(procmem.resident));
	h_procmem->Set(v8u::Symbol("share"), Number::New(procmem.share));
	h_procmem->Set(v8u::Symbol("minor_faults"), Number::New(procmem.minor_faults));
	h_procmem->Set(v8u::Symbol("major_faults"), Number::New(procmem.major_faults));
	h_procmem->Set(v8u::Symbol("page_faults"), Number::New(procmem.page_faults));
	return scope.Close(h_procmem);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcCredGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_cred_t proccred;
	int status = sigar_proc_cred_get(inst->sigar, args[0]->IntegerValue(), &proccred);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_proccred = Object::New();
	h_proccred->Set(v8u::Symbol("uid"), Number::New(proccred.uid));
	h_proccred->Set(v8u::Symbol("gid"), Number::New(proccred.gid));
	h_proccred->Set(v8u::Symbol("euid"), Number::New(proccred.euid));
	h_proccred->Set(v8u::Symbol("egid"), Number::New(proccred.egid));
	return scope.Close(h_proccred);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcTimeGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_time_t proctime;
	int status = sigar_proc_time_get(inst->sigar, args[0]->IntegerValue(), &proctime);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_proctime = Object::New();
	h_proctime->Set(v8u::Symbol("start_time"), Number::New(proctime.start_time));
	h_proctime->Set(v8u::Symbol("user"), Number::New(proctime.user));
	h_proctime->Set(v8u::Symbol("sys"), Number::New(proctime.sys));
	h_proctime->Set(v8u::Symbol("total"), Number::New(proctime.total));
	return scope.Close(h_proctime);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcCpuGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_cpu_t proccpu;
	int status = sigar_proc_cpu_get(inst->sigar, args[0]->IntegerValue(), &proccpu);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Object> h_proccpu = Object::New();
	h_proccpu->Set(v8u::Symbol("start_time"), Number::New(proccpu.start_time));
	h_proccpu->Set(v8u::Symbol("user"), Number::New(proccpu.user));
	h_proccpu->Set(v8u::Symbol("sys"), Number::New(proccpu.sys));
	h_proccpu->Set(v8u::Symbol("total"), Number::New(proccpu.total));
	h_proccpu->Set(v8u::Symbol("last_time"), Number::New(proccpu.last_time));
	h_proccpu->Set(v8u::Symbol("percent"), Number::New(proccpu.percent));
	return scope.Close(h_proccpu);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcStateGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_state_t procstate;
	int status = sigar_proc_state_get(inst->sigar, args[0]->IntegerValue(), &procstate);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	// TODO: state is probably not handled correctly,
	// eg, moving the next 2 lines after h_procstate creation will add extra characters
	char state[0];
	state[0] = procstate.state;
	Handle<Object> h_procstate = Object::New();
	h_procstate->Set(v8u::Symbol("name"), String::New(procstate.name));
	h_procstate->Set(v8u::Symbol("state"), String::New(state));
	h_procstate->Set(v8u::Symbol("ppid"), Number::New(procstate.ppid));
	h_procstate->Set(v8u::Symbol("tty"), Number::New(procstate.tty));
	h_procstate->Set(v8u::Symbol("priority"), Number::New(procstate.priority));
	h_procstate->Set(v8u::Symbol("nice"), Number::New(procstate.nice));
	h_procstate->Set(v8u::Symbol("processor"), Number::New(procstate.processor));
	h_procstate->Set(v8u::Symbol("threads"), Number::New(procstate.threads));
	return scope.Close(h_procstate);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcArgsGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_args_t procargs;
	int status = sigar_proc_args_get(inst->sigar, args[0]->IntegerValue(), &procargs);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_args_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_procargs = Array::New(procargs.number);
	unsigned i;
	for (i=0; i<procargs.number; i++) {
		char* arg = procargs.data[i];
		h_procargs->Set(i, String::New(arg));
	}
	sigar_proc_args_destroy(inst->sigar, &procargs);
	return scope.Close(h_procargs);
} V8_CB_END();

static int SigarProcEnvGet_getter(void *data,
							const char *key, int klen,
								 char *val, int vlen)
{
	Handle<Object> d = *((Handle<Object>*)data);
	d->Set(String::New(key, klen), String::New(val, vlen));
	return SIGAR_OK;
}

V8_CB(NodeSigar::SigarProcEnvGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_env_t procenv;
	procenv.type = procenv.SIGAR_PROC_ENV_ALL;
	procenv.env_getter = SigarProcEnvGet_getter;
	Handle<Object> data = Object::New();
	procenv.data = &data;
	int status = sigar_proc_env_get(inst->sigar, args[0]->IntegerValue(), &procenv);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_env_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	return scope.Close(data);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcFdGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_fd_t procfd;
	int status = sigar_proc_fd_get(inst->sigar, args[0]->IntegerValue(), &procfd);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_fd_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	// Note, return an object because only total but a comment
	// suggest they might add other properties
	Handle<Object> h_procfd = Object::New();
	h_procfd->Set(v8u::Symbol("total"), Number::New(procfd.total));
	return scope.Close(h_procfd);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcExeGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_exe_t procexe;
	int status = sigar_proc_exe_get(inst->sigar, args[0]->IntegerValue(), &procexe);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_exe_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
		Handle<Object> h_procexe = Object::New();
		h_procexe->Set(v8u::Symbol("name"), String::New(procexe.name));
		h_procexe->Set(v8u::Symbol("cwd"), String::New(procexe.cwd));
		h_procexe->Set(v8u::Symbol("root"), String::New(procexe.root));
	return scope.Close(h_procexe);
} V8_CB_END();

static int SigarProcModulesGet_getter(void *data,
								 char *key, int klen)
{
	Handle<Array> d = *((Handle<Array>*)data);
	// Not sure if index arg is meant to work
	d->Set(d->Length(), String::New(key, klen));
	return SIGAR_OK;
}

V8_CB(NodeSigar::SigarProcModulesGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_proc_modules_t procmodule;
	procmodule.module_getter = SigarProcModulesGet_getter;
	Handle<Array> data = Array::New();
	procmodule.data = &data;
	int status = sigar_proc_modules_get(inst->sigar, args[0]->IntegerValue(), &procmodule);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_module_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	return scope.Close(data);
} V8_CB_END();

V8_CB(NodeSigar::SigarProcPortGet) {
	if(args.Length() != 2) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	int protocol = args[0]->Int32Value();
	//int protocol = SIGAR_NETCONN_TCP;
	//int protocol = SIGAR_NETCONN_UDP;
	//int protocol = SIGAR_NETCONN_RAW;
	//int protocol = SIGAR_NETCONN_UNIX;
	unsigned long port = args[1]->Int32Value();
	sigar_pid_t pid;
	int status = sigar_proc_port_get(inst->sigar, protocol, port, &pid);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_proc_port_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	return scope.Close(Number::New(pid));
} V8_CB_END();

V8_CB(NodeSigar::SigarThreadCpuGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_thread_cpu_t threadcpu;
	int status = sigar_thread_cpu_get(inst->sigar, args[0]->IntegerValue(), &threadcpu);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_thread_cpu_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_threadcpu = Object::New();
	h_threadcpu->Set(v8u::Symbol("user"), Number::New(threadcpu.user));
	h_threadcpu->Set(v8u::Symbol("sys"), Number::New(threadcpu.sys));
	h_threadcpu->Set(v8u::Symbol("total"), Number::New(threadcpu.total));
	return scope.Close(h_threadcpu);
} V8_CB_END();

V8_CB(NodeSigar::SigarFileSystemListGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_file_system_list_t filesystemlist;
	int status = sigar_file_system_list_get(inst->sigar, &filesystemlist);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_file_system_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_filesystemlist = Array::New(filesystemlist.number);
	unsigned i;
	for (i=0; i<filesystemlist.number; i++) {
		sigar_file_system_t filesystem = filesystemlist.data[i];
		Handle<Object> h_filesystem = Object::New();
		h_filesystem->Set(v8u::Symbol("dir_name"), String::New(filesystem.dir_name));
		h_filesystem->Set(v8u::Symbol("dev_name"), String::New(filesystem.dev_name));
		h_filesystem->Set(v8u::Symbol("type_name"), String::New(filesystem.type_name));
		h_filesystem->Set(v8u::Symbol("sys_type_name"), String::New(filesystem.sys_type_name));
		h_filesystem->Set(v8u::Symbol("options"), String::New(filesystem.options));
		h_filesystem->Set(v8u::Symbol("type"), Number::New(filesystem.type));
		h_filesystem->Set(v8u::Symbol("flags"), Number::New(filesystem.flags));
		h_filesystemlist->Set(i, h_filesystem);
	}
	sigar_file_system_list_destroy(inst->sigar, &filesystemlist);
	return scope.Close(h_filesystemlist);
} V8_CB_END();

V8_CB(NodeSigar::SigarFileSystemUsageGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_file_system_usage_t filesystemusage;
	String::Utf8Value dirname(args[0]->ToString());
	int status = sigar_file_system_usage_get(inst->sigar,  *dirname, &filesystemusage);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_file_system_usage_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_filesystemusage = Object::New();
	Handle<Object> h_diskusage= Object::New();
	sigar_disk_usage_t diskusage = filesystemusage.disk;
	h_diskusage->Set(v8u::Symbol("reads"), Number::New(diskusage.reads));
	h_diskusage->Set(v8u::Symbol("writes"), Number::New(diskusage.writes));
	h_diskusage->Set(v8u::Symbol("write_bytes"), Number::New(diskusage.write_bytes));
	h_diskusage->Set(v8u::Symbol("read_bytes"), Number::New(diskusage.read_bytes));
	h_diskusage->Set(v8u::Symbol("rtime"), Number::New(diskusage.rtime));
	h_diskusage->Set(v8u::Symbol("wtime"), Number::New(diskusage.wtime));
	h_diskusage->Set(v8u::Symbol("qtime"), Number::New(diskusage.qtime));
	h_diskusage->Set(v8u::Symbol("time"), Number::New(diskusage.time));
	h_diskusage->Set(v8u::Symbol("snaptime"), Number::New(diskusage.snaptime));
	h_diskusage->Set(v8u::Symbol("service_time"), Number::New(diskusage.service_time));
	h_diskusage->Set(v8u::Symbol("queue"), Number::New(diskusage.queue));
	h_filesystemusage->Set(v8u::Symbol("disk"), h_diskusage);
	h_filesystemusage->Set(v8u::Symbol("use_percent"), Number::New(filesystemusage.use_percent));
	h_filesystemusage->Set(v8u::Symbol("total"), Number::New(filesystemusage.total));
	h_filesystemusage->Set(v8u::Symbol("free"), Number::New(filesystemusage.free));
	h_filesystemusage->Set(v8u::Symbol("used"), Number::New(filesystemusage.used));
	h_filesystemusage->Set(v8u::Symbol("avail"), Number::New(filesystemusage.avail));
	h_filesystemusage->Set(v8u::Symbol("files"), Number::New(filesystemusage.files));
	h_filesystemusage->Set(v8u::Symbol("free_files"), Number::New(filesystemusage.free_files));
	return scope.Close(h_filesystemusage);
} V8_CB_END();

V8_CB(NodeSigar::SigarFileSystemPingGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_file_system_t filesystem;
	int status = sigar_file_system_ping(inst->sigar, &filesystem);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_file_system_ping error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_filesystem = Object::New();
	//printf("TEST %s | %u \n", filesystem.dir_name, filesystem.dev_name);
	h_filesystem->Set(v8u::Symbol("dir_name"), String::New(filesystem.dir_name));
	h_filesystem->Set(v8u::Symbol("dev_name"), String::New(filesystem.dev_name));
	h_filesystem->Set(v8u::Symbol("type_name"), String::New(filesystem.type_name));
	h_filesystem->Set(v8u::Symbol("sys_type_name"), String::New(filesystem.sys_type_name));
	h_filesystem->Set(v8u::Symbol("options"), String::New(filesystem.options));
	h_filesystem->Set(v8u::Symbol("type"), Number::New(filesystem.type));
	h_filesystem->Set(v8u::Symbol("flags"), Number::New(filesystem.flags));
	return scope.Close(h_filesystem);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetInfoGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_info_t netinfo;
	int status = sigar_net_info_get(inst->sigar, &netinfo);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_file_system_ping error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_netinfo = Object::New();
	h_netinfo->Set(v8u::Symbol("default_gateway"), String::New(netinfo.default_gateway));
	h_netinfo->Set(v8u::Symbol("default_gateway_interface"), String::New(netinfo.default_gateway_interface));
	h_netinfo->Set(v8u::Symbol("host_name"), String::New(netinfo.host_name));
	h_netinfo->Set(v8u::Symbol("domain_name"), String::New(netinfo.domain_name));
	h_netinfo->Set(v8u::Symbol("primary_dns"), String::New(netinfo.primary_dns));
	h_netinfo->Set(v8u::Symbol("secondary_dns"), String::New(netinfo.secondary_dns));
	return scope.Close(h_netinfo);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetRouteListGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_route_list_t netroutelist;
	int status = sigar_net_route_list_get(inst->sigar, &netroutelist);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_net_route_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_netroutelist = Array::New(netroutelist.number);
	unsigned i;
	for (i=0; i<netroutelist.number; i++) {
		sigar_net_route_t netroute = netroutelist.data[i];
		Handle<Object> h_netroute = Object::New();

		h_netroute->Set(v8u::Symbol("destination"), node_sigar_net_address_to_string(&netroute.destination));
		h_netroute->Set(v8u::Symbol("gateway"), node_sigar_net_address_to_string(&netroute.gateway));
		h_netroute->Set(v8u::Symbol("mask"), node_sigar_net_address_to_string(&netroute.mask));

		h_netroute->Set(v8u::Symbol("flags"), Number::New(netroute.flags));
		h_netroute->Set(v8u::Symbol("refcnt"), Number::New(netroute.refcnt));
		h_netroute->Set(v8u::Symbol("use"), Number::New(netroute.use));
		h_netroute->Set(v8u::Symbol("metric"), Number::New(netroute.metric));
		h_netroute->Set(v8u::Symbol("mtu"), Number::New(netroute.mtu));
		h_netroute->Set(v8u::Symbol("window"), Number::New(netroute.window));
		h_netroute->Set(v8u::Symbol("irtt"), Number::New(netroute.irtt));
		h_netroute->Set(v8u::Symbol("ifname"), String::New(netroute.ifname));
		h_netroutelist->Set(i, h_netroute);
	}
	sigar_net_route_list_destroy(inst->sigar, &netroutelist);
	return scope.Close(h_netroutelist);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetInterfaceListGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_interface_list_t netinterfacelist;
	int status = sigar_net_interface_list_get(inst->sigar, &netinterfacelist);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_net_route_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_netinterfacelist = Array::New(netinterfacelist.number);
	unsigned i;
	for (i=0; i<netinterfacelist.number; i++) {
		char *name = netinterfacelist.data[i];
		Handle<String> h_name= String::New(name);
		h_netinterfacelist->Set(i, h_name);
	}
	sigar_net_interface_list_destroy(inst->sigar, &netinterfacelist);
	return scope.Close(h_netinterfacelist);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetInterfaceConfigGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_interface_config_t netinterface;
	String::Utf8Value name(args[0]->ToString());
	int status = sigar_net_interface_config_get(inst->sigar, *name, &netinterface);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_net_interface_config_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_netinterface = Object::New();
	h_netinterface->Set(v8u::Symbol("name"), String::New(netinterface.name));
	h_netinterface->Set(v8u::Symbol("type"), String::New(netinterface.type));
	h_netinterface->Set(v8u::Symbol("description"), String::New(netinterface.description));
	h_netinterface->Set(v8u::Symbol("hwaddr"), node_sigar_net_address_to_string(&netinterface.hwaddr));
	h_netinterface->Set(v8u::Symbol("address"), node_sigar_net_address_to_string(&netinterface.address));
	h_netinterface->Set(v8u::Symbol("destination"), node_sigar_net_address_to_string(&netinterface.destination));
	h_netinterface->Set(v8u::Symbol("broadcast"), node_sigar_net_address_to_string(&netinterface.broadcast));
	h_netinterface->Set(v8u::Symbol("netmask"), node_sigar_net_address_to_string(&netinterface.netmask));
	h_netinterface->Set(v8u::Symbol("address6"), node_sigar_net_address_to_string(&netinterface.address6));
	h_netinterface->Set(v8u::Symbol("prefix6_length"), Number::New(netinterface.prefix6_length));
	h_netinterface->Set(v8u::Symbol("scope6"), Number::New(netinterface.scope6));
	h_netinterface->Set(v8u::Symbol("flags"), Number::New(netinterface.flags));
	h_netinterface->Set(v8u::Symbol("mtu"), Number::New(netinterface.mtu));
	h_netinterface->Set(v8u::Symbol("metric"), Number::New(netinterface.metric));
	h_netinterface->Set(v8u::Symbol("tx_queue_len"), Number::New(netinterface.tx_queue_len));
	return scope.Close(h_netinterface);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetInterfaceConfigPrimaryGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_interface_config_t netinterface;
	int status = sigar_net_interface_config_primary_get(inst->sigar, &netinterface);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_net_interface_config_primary_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_netinterface = Object::New();
	h_netinterface->Set(v8u::Symbol("name"), String::New(netinterface.name));
	h_netinterface->Set(v8u::Symbol("type"), String::New(netinterface.type));
	h_netinterface->Set(v8u::Symbol("description"), String::New(netinterface.description));
	h_netinterface->Set(v8u::Symbol("hwaddr"), node_sigar_net_address_to_string(&netinterface.hwaddr));
	h_netinterface->Set(v8u::Symbol("address"), node_sigar_net_address_to_string(&netinterface.address));
	h_netinterface->Set(v8u::Symbol("destination"), node_sigar_net_address_to_string(&netinterface.destination));
	h_netinterface->Set(v8u::Symbol("broadcast"), node_sigar_net_address_to_string(&netinterface.broadcast));
	h_netinterface->Set(v8u::Symbol("netmask"), node_sigar_net_address_to_string(&netinterface.netmask));
	h_netinterface->Set(v8u::Symbol("address6"), node_sigar_net_address_to_string(&netinterface.address6));
	h_netinterface->Set(v8u::Symbol("prefix6_length"), Number::New(netinterface.prefix6_length));
	h_netinterface->Set(v8u::Symbol("scope6"), Number::New(netinterface.scope6));
	h_netinterface->Set(v8u::Symbol("flags"), Number::New(netinterface.flags));
	h_netinterface->Set(v8u::Symbol("mtu"), Number::New(netinterface.mtu));
	h_netinterface->Set(v8u::Symbol("metric"), Number::New(netinterface.metric));
	h_netinterface->Set(v8u::Symbol("tx_queue_len"), Number::New(netinterface.tx_queue_len));
	return scope.Close(h_netinterface);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetInterfaceStatGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_interface_stat_t netinterface;
	String::Utf8Value name(args[0]->ToString());
	int status = sigar_net_interface_stat_get(inst->sigar, *name, &netinterface);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_net_interface_stat_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_netinterface = Object::New();
	h_netinterface->Set(v8u::Symbol("rx_packets"), Number::New(netinterface.rx_packets));
	h_netinterface->Set(v8u::Symbol("rx_bytes"), Number::New(netinterface.rx_bytes));
	h_netinterface->Set(v8u::Symbol("rx_errors"), Number::New(netinterface.rx_errors));
	h_netinterface->Set(v8u::Symbol("rx_dropped"), Number::New(netinterface.rx_dropped));
	h_netinterface->Set(v8u::Symbol("rx_overruns"), Number::New(netinterface.rx_overruns));
	h_netinterface->Set(v8u::Symbol("rx_frame"), Number::New(netinterface.rx_frame));
	h_netinterface->Set(v8u::Symbol("tx_packets"), Number::New(netinterface.tx_packets));
	h_netinterface->Set(v8u::Symbol("tx_bytes"), Number::New(netinterface.tx_bytes));
	h_netinterface->Set(v8u::Symbol("tx_errors"), Number::New(netinterface.tx_errors));
	h_netinterface->Set(v8u::Symbol("tx_dropped"), Number::New(netinterface.tx_dropped));
	h_netinterface->Set(v8u::Symbol("tx_overruns"), Number::New(netinterface.tx_overruns));
	h_netinterface->Set(v8u::Symbol("tx_collisions"), Number::New(netinterface.tx_collisions));
	h_netinterface->Set(v8u::Symbol("tx_carrier"), Number::New(netinterface.tx_carrier));
	h_netinterface->Set(v8u::Symbol("speed"), Number::New(netinterface.speed));
	return scope.Close(h_netinterface);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetConnectionListGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_connection_list_t netconnectionlist;
	int flags = args[0]->Int32Value();
	int status = sigar_net_connection_list_get(inst->sigar, &netconnectionlist, flags);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_net_connection_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_netconnectionlist = Array::New(netconnectionlist.number);
	unsigned i;
	for (i=0; i<netconnectionlist.number; i++) {
		sigar_net_connection_t netconnection   = netconnectionlist.data[i];
		Handle<Object> h_netconnection = Object::New();
		h_netconnection->Set(v8u::Symbol("local_port"), Number::New(netconnection.local_port));
		h_netconnection->Set(v8u::Symbol("local_address"), node_sigar_net_address_to_string(&netconnection.local_address));
		h_netconnection->Set(v8u::Symbol("remote_port"), Number::New(netconnection.remote_port));
		h_netconnection->Set(v8u::Symbol("remote_address"), node_sigar_net_address_to_string(&netconnection.remote_address));
		h_netconnection->Set(v8u::Symbol("uid"), Number::New(netconnection.uid));
		h_netconnection->Set(v8u::Symbol("inode"), Number::New(netconnection.inode));
		h_netconnection->Set(v8u::Symbol("type"), Number::New(netconnection.type));
		h_netconnection->Set(v8u::Symbol("state"), Number::New(netconnection.state));
		h_netconnection->Set(v8u::Symbol("send_queue"), Number::New(netconnection.send_queue));
		h_netconnection->Set(v8u::Symbol("receive_queue"), Number::New(netconnection.receive_queue));
		h_netconnectionlist->Set(i, h_netconnection);
	}
	sigar_net_connection_list_destroy(inst->sigar, &netconnectionlist);
	return scope.Close(h_netconnectionlist);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetListenAddressGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_address_t netaddress;
	int port = args[0]->Int32Value();
	int status = sigar_net_listen_address_get(inst->sigar, port, &netaddress);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_net_listen_address_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<String> h_netaddress = node_sigar_net_address_to_string(&netaddress);
	return scope.Close(h_netaddress);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetStatGet) {
	if(args.Length() != 1) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_stat_t netstat;
	int flags = args[0]->Int32Value();
	int status = sigar_net_stat_get(inst->sigar, &netstat, flags);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_net_interface_stat_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_netstat = Object::New();
	Handle<Array> h_states = Array::New();
	unsigned i;
	for (i=0; i<SIGAR_TCP_UNKNOWN; i++) {
		h_states->Set(i, Number::New(netstat.tcp_states[i]));
	}
	h_netstat->Set(v8u::Symbol("tcp_states"), h_states);
	h_netstat->Set(v8u::Symbol("tcp_inbound_total"), Number::New(netstat.tcp_inbound_total));
	h_netstat->Set(v8u::Symbol("tcp_outbound_total"), Number::New(netstat.tcp_outbound_total));
	h_netstat->Set(v8u::Symbol("all_inbound_total"), Number::New(netstat.all_inbound_total));
	h_netstat->Set(v8u::Symbol("all_outbound_total"), Number::New(netstat.all_outbound_total));
	return scope.Close(h_netstat);
} V8_CB_END();

V8_CB(NodeSigar::SigarNetStatPortGet) {
	if(args.Length() != 3) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_net_stat_t netstatport;
	int flags = args[0]->Int32Value();

	sigar_net_address_t address;
	node_sigar_str2net_address(args[1]->ToString(), &address);
	unsigned long port = args[2]->Int32Value();
	int status = sigar_net_stat_port_get(inst->sigar, &netstatport, flags, &address, port);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_net_stat_port_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_netstatport = Object::New();
	Handle<Array> h_states = Array::New();
	unsigned i;
	for (i=0; i<SIGAR_TCP_UNKNOWN; i++) {
		h_states->Set(i, Number::New(netstatport.tcp_states[i]));
	}
	h_netstatport->Set(v8u::Symbol("tcp_states"), h_states);
	h_netstatport->Set(v8u::Symbol("tcp_inbound_total"), Number::New(netstatport.tcp_inbound_total));
	h_netstatport->Set(v8u::Symbol("tcp_outbound_total"), Number::New(netstatport.tcp_outbound_total));
	h_netstatport->Set(v8u::Symbol("all_inbound_total"), Number::New(netstatport.all_inbound_total));
	h_netstatport->Set(v8u::Symbol("all_outbound_total"), Number::New(netstatport.all_outbound_total));
	return scope.Close(h_netstatport);
} V8_CB_END();
// Testing, see './test/TodoTest.coffee' V8_CB_END();

V8_CB(NodeSigar::SigarTcpGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_tcp_t tcp;
	int status = sigar_tcp_get(inst->sigar, &tcp);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_tcp_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_tcp = Object::New();
	h_tcp->Set(v8u::Symbol("active_opens"), Number::New(tcp.active_opens));
	h_tcp->Set(v8u::Symbol("passive_opens"), Number::New(tcp.passive_opens));
	h_tcp->Set(v8u::Symbol("attempt_fails"), Number::New(tcp.attempt_fails));
	h_tcp->Set(v8u::Symbol("estab_resets"), Number::New(tcp.estab_resets));
	h_tcp->Set(v8u::Symbol("curr_estab"), Number::New(tcp.curr_estab));
	h_tcp->Set(v8u::Symbol("in_segs"), Number::New(tcp.in_segs));
	h_tcp->Set(v8u::Symbol("out_segs"), Number::New(tcp.out_segs));
	h_tcp->Set(v8u::Symbol("retrans_segs"), Number::New(tcp.retrans_segs));
	h_tcp->Set(v8u::Symbol("in_errs"), Number::New(tcp.in_errs));
	h_tcp->Set(v8u::Symbol("out_rsts"), Number::New(tcp.out_rsts));
	return scope.Close(h_tcp);
} V8_CB_END();

V8_CB(NodeSigar::SigarNfsClientV2Get) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_nfs_client_v2_t nfsclientv2;
	int status = sigar_nfs_client_v2_get(inst->sigar, &nfsclientv2);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_nfs_client_v2_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_nfsclientv2 = Object::New();
	h_nfsclientv2->Set(v8u::Symbol("null"), Number::New(nfsclientv2.null));
	h_nfsclientv2->Set(v8u::Symbol("getattr"), Number::New(nfsclientv2.getattr));
	h_nfsclientv2->Set(v8u::Symbol("setattr"), Number::New(nfsclientv2.setattr));
	h_nfsclientv2->Set(v8u::Symbol("root"), Number::New(nfsclientv2.root));
	h_nfsclientv2->Set(v8u::Symbol("lookup"), Number::New(nfsclientv2.lookup));
	h_nfsclientv2->Set(v8u::Symbol("readlink"), Number::New(nfsclientv2.readlink));
	h_nfsclientv2->Set(v8u::Symbol("read"), Number::New(nfsclientv2.read));
	h_nfsclientv2->Set(v8u::Symbol("writecache"), Number::New(nfsclientv2.writecache));
	h_nfsclientv2->Set(v8u::Symbol("write"), Number::New(nfsclientv2.write));
	h_nfsclientv2->Set(v8u::Symbol("create"), Number::New(nfsclientv2.create));
	h_nfsclientv2->Set(v8u::Symbol("remove"), Number::New(nfsclientv2.remove));
	h_nfsclientv2->Set(v8u::Symbol("rename"), Number::New(nfsclientv2.rename));
	h_nfsclientv2->Set(v8u::Symbol("link"), Number::New(nfsclientv2.link));
	h_nfsclientv2->Set(v8u::Symbol("symlink"), Number::New(nfsclientv2.symlink));
	h_nfsclientv2->Set(v8u::Symbol("mkdir"), Number::New(nfsclientv2.mkdir));
	h_nfsclientv2->Set(v8u::Symbol("rmdir"), Number::New(nfsclientv2.rmdir));
	h_nfsclientv2->Set(v8u::Symbol("readdir"), Number::New(nfsclientv2.readdir));
	h_nfsclientv2->Set(v8u::Symbol("fsstat"), Number::New(nfsclientv2.fsstat));
	return scope.Close(h_nfsclientv2);
} V8_CB_END();

V8_CB(NodeSigar::SigarNfsServerV2Get) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_nfs_server_v2_t nfsserverv2;
	int status = sigar_nfs_server_v2_get(inst->sigar, &nfsserverv2);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_nfs_server_v2_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	};
	Handle<Object> h_nfsserverv2 = Object::New();
	h_nfsserverv2->Set(v8u::Symbol("null"), Number::New(nfsserverv2.null));
	h_nfsserverv2->Set(v8u::Symbol("getattr"), Number::New(nfsserverv2.getattr));
	h_nfsserverv2->Set(v8u::Symbol("setattr"), Number::New(nfsserverv2.setattr));
	h_nfsserverv2->Set(v8u::Symbol("root"), Number::New(nfsserverv2.root));
	h_nfsserverv2->Set(v8u::Symbol("lookup"), Number::New(nfsserverv2.lookup));
	h_nfsserverv2->Set(v8u::Symbol("readlink"), Number::New(nfsserverv2.readlink));
	h_nfsserverv2->Set(v8u::Symbol("read"), Number::New(nfsserverv2.read));
	h_nfsserverv2->Set(v8u::Symbol("writecache"), Number::New(nfsserverv2.writecache));
	h_nfsserverv2->Set(v8u::Symbol("write"), Number::New(nfsserverv2.write));
	h_nfsserverv2->Set(v8u::Symbol("create"), Number::New(nfsserverv2.create));
	h_nfsserverv2->Set(v8u::Symbol("remove"), Number::New(nfsserverv2.remove));
	h_nfsserverv2->Set(v8u::Symbol("rename"), Number::New(nfsserverv2.rename));
	h_nfsserverv2->Set(v8u::Symbol("link"), Number::New(nfsserverv2.link));
	h_nfsserverv2->Set(v8u::Symbol("symlink"), Number::New(nfsserverv2.symlink));
	h_nfsserverv2->Set(v8u::Symbol("mkdir"), Number::New(nfsserverv2.mkdir));
	h_nfsserverv2->Set(v8u::Symbol("rmdir"), Number::New(nfsserverv2.rmdir));
	h_nfsserverv2->Set(v8u::Symbol("readdir"), Number::New(nfsserverv2.readdir));
	h_nfsserverv2->Set(v8u::Symbol("fsstat"), Number::New(nfsserverv2.fsstat));
	return scope.Close(h_nfsserverv2);
} V8_CB_END();

V8_CB(NodeSigar::SigarArpListGet) {
	if(args.Length() != 0) V8_STHROW(v8u::RangeErr("Invalid Argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	sigar_arp_list_t arplist;
	int status = sigar_arp_list_get(inst->sigar, &arplist);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_arp_list_get error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	Handle<Array> h_arplist = Array::New(arplist.number);
	unsigned i;
	for (i=0; i<arplist.number; i++) {
		sigar_arp_t arp = arplist.data[i];
		Handle<Object> h_arp = Object::New();
		h_arp->Set(v8u::Symbol("ifname"), String::New(arp.ifname));
		h_arp->Set(v8u::Symbol("type"), String::New(arp.type));
		h_arp->Set(v8u::Symbol("hwaddr"), node_sigar_net_address_to_string(&arp.hwaddr));
		h_arp->Set(v8u::Symbol("address"), node_sigar_net_address_to_string(&arp.address));
		h_arp->Set(v8u::Symbol("flags"), Number::New(arp.flags));
		h_arplist->Set(i, h_arp);
	}
	sigar_arp_list_destroy(inst->sigar, &arplist);
	return scope.Close(h_arplist);
} V8_CB_END();

V8_CB(NodeSigar::SigarRpcPing) {
	if(args.Length() != 4) V8_STHROW(v8u::TypeErr("Invalid argument"));
	V8_M_UNWRAP(NodeSigar, args.This());

	String::Utf8Value hostname(args[0]->ToString());
	int protocol = args[1]->Int32Value();
	//int protocol = SIGAR_NETCONN_TCP;
	unsigned long program = args[2]->IntegerValue();
	unsigned long version = args[3]->IntegerValue();
	int status = sigar_rpc_ping(*hostname, protocol, program, version);
	if(status != SIGAR_OK) {
		char msg[200];
		sprintf(msg, "sigar_rpc_ping error: %d (%s)", status, sigar_strerror(inst->sigar, status));
		V8_STHROW(v8u::TypeErr(msg));
	}
	return scope.Close(Boolean::New(true));
} V8_CB_END();

//SIGAR_DECLARE(char *) sigar_format_size(sigar_uint64_t size, char *buf);

NODE_DEF_MAIN() {
	NodeSigar::init(target);
	target->Set(v8u::Symbol("FSTYPE_UNKNOWN"), Number::New(SIGAR_FSTYPE_UNKNOWN));
	target->Set(v8u::Symbol("FSTYPE_NONE"), Number::New(SIGAR_FSTYPE_NONE));
	target->Set(v8u::Symbol("FSTYPE_LOCAL_DISK"), Number::New(SIGAR_FSTYPE_LOCAL_DISK));
	target->Set(v8u::Symbol("FSTYPE_NETWORK"), Number::New(SIGAR_FSTYPE_NETWORK));
	target->Set(v8u::Symbol("FSTYPE_RAM_DISK"), Number::New(SIGAR_FSTYPE_RAM_DISK));
	target->Set(v8u::Symbol("FSTYPE_CDROM"), Number::New(SIGAR_FSTYPE_CDROM));
	target->Set(v8u::Symbol("FSTYPE_SWAP"), Number::New(SIGAR_FSTYPE_SWAP));
	target->Set(v8u::Symbol("FSTYPE_MAX"), Number::New(SIGAR_FSTYPE_MAX));
} NODE_DEF_MAIN_END(sigar)
