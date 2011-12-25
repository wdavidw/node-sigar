#include <v8.h>
#include <node.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <errno.h>
#include "sigar.h"
#include "sigar_fileinfo.h"
#include "sigar_log.h"
#include "sigar_format.h"
#include "sigar_ptql.h"

using namespace node;
using namespace v8;

class NodeSigar: public ObjectWrap
{
private:
	sigar_t *sigar;
public:
	static void Initialize(Handle<Object> target)
	{
		HandleScope scope;
		Local<FunctionTemplate> t = FunctionTemplate::New(New);
		Local<FunctionTemplate> s_ct = Local<FunctionTemplate>::New(t);
		s_ct->InstanceTemplate()->SetInternalFieldCount(1);
		s_ct->SetClassName(String::NewSymbol("NodeSigar"));
		NODE_SET_PROTOTYPE_METHOD(s_ct, "mem", GetMem);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "swap", GetSwap);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "cpu", SigarCpuGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "cpuList", SigarCpuListGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "cpuInfo", SigarCpuInfoListGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "uptime", SigarUptimeGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "loadavg", SigarLoadavgGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "resourceLimit", SigarResourceLimitGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procList", SigarProcListGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procStat", SigarProcStatGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procMem", SigarProcMemGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procCred", SigarProcCredGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procTime", SigarProcTimeGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procCpu", SigarProcCpuGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procState", SigarProcStateGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procArgs", SigarProcArgsGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procEnv", SigarProcEnvGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procFd", SigarProcFdGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procExe", SigarProcExeGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "procModules", SigarProcModulesGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "threadCpu", SigarThreadCpuGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "fileSystemList", SigarFileSystemListGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "fileSystemUsage", SigarFileSystemUsageGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "fileSystemPing", SigarFileSystemPingGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "netInfo", SigarNetInfoGet);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "netRouteList", SigarNetRouteListGet);
		target->Set(String::NewSymbol("NodeSigar"), s_ct->GetFunction());
		target->Set(String::NewSymbol("FSTYPE_UNKNOWN"), Number::New(SIGAR_FSTYPE_UNKNOWN));
		target->Set(String::NewSymbol("FSTYPE_NONE"), Number::New(SIGAR_FSTYPE_NONE));
		target->Set(String::NewSymbol("FSTYPE_LOCAL_DISK"), Number::New(SIGAR_FSTYPE_LOCAL_DISK));
		target->Set(String::NewSymbol("FSTYPE_NETWORK"), Number::New(SIGAR_FSTYPE_NETWORK));
		target->Set(String::NewSymbol("FSTYPE_RAM_DISK"), Number::New(SIGAR_FSTYPE_RAM_DISK));
		target->Set(String::NewSymbol("FSTYPE_CDROM"), Number::New(SIGAR_FSTYPE_CDROM));
		target->Set(String::NewSymbol("FSTYPE_SWAP"), Number::New(SIGAR_FSTYPE_SWAP));
		target->Set(String::NewSymbol("FSTYPE_MAX"), Number::New(SIGAR_FSTYPE_MAX));
	}
	~NodeSigar()
	{
		sigar_close(this->sigar);
	}
	static Handle<Value> New(const Arguments& args)
	{
		HandleScope scope;
		NodeSigar* g = new NodeSigar();
		g->Wrap(args.This());
	    sigar_open(&(g->sigar));

		return args.This();
	}
//	static Handle<Object> MapSigarNetAddress(sigar_net_address_t& address)
//	{
//		Handle<Object> h_address = Object::New();
//
//		sigar_net_address_to_string(sigar_t *sigar, *address, char *addr_str)
//
//		return h_address;

//		Handle<Object> h_address = Object::New();
//		h_address->Set(String::NewSymbol("family"), Number::New(address.family));
//		h_address->Set(String::NewSymbol("addr_in"), Number::New(address.addr.in));
//		Handle<Array> h_address_in6 = Array::New();
//		h_address_in6->Set(0, Number::New(address.addr.in6[0]));
//		h_address_in6->Set(1, Number::New(address.addr.in6[1]));
//		h_address_in6->Set(2, Number::New(address.addr.in6[2]));
//		h_address_in6->Set(3, Number::New(address.addr.in6[3]));
//		h_address->Set(String::NewSymbol("addr_in6"), h_address_in6);
//		h_address->Set(String::NewSymbol("mac"), String::New(address.addr.mac));
//		return h_address;
//	}
	static Handle<Value> GetMem(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_mem_t mem;
	    int status = sigar_mem_get(g->sigar, &mem);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_mem_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
	    Handle<Object> h_mem = Object::New();
	    h_mem->Set(String::NewSymbol("ram"), Number::New(mem.ram));
	    h_mem->Set(String::NewSymbol("total"), Number::New(mem.total));
	    h_mem->Set(String::NewSymbol("used"), Number::New(mem.used));
	    h_mem->Set(String::NewSymbol("free"), Number::New(mem.free));
	    h_mem->Set(String::NewSymbol("actual_used"), Number::New(mem.actual_used));
	    h_mem->Set(String::NewSymbol("actual_free"), Number::New(mem.actual_free));
	    h_mem->Set(String::NewSymbol("used_percent"), Number::New(mem.used_percent));
	    h_mem->Set(String::NewSymbol("free_percent"), Number::New(mem.free_percent));
		return scope.Close(h_mem);
	}
	static Handle<Value> GetSwap(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_swap_t swap;
	    int status = sigar_swap_get(g->sigar, &swap);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_swap_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
	    Handle<Object> h_swap = Object::New();
	    h_swap->Set(String::NewSymbol("total"), Number::New(swap.total));
	    h_swap->Set(String::NewSymbol("used"), Number::New(swap.used));
	    h_swap->Set(String::NewSymbol("free"), Number::New(swap.free));
	    h_swap->Set(String::NewSymbol("page_in"), Number::New(swap.page_in));
	    h_swap->Set(String::NewSymbol("page_out"), Number::New(swap.page_out));
		return scope.Close(h_swap);
	}
	static Handle<Value> SigarCpuGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_cpu_t cpu;
	    int status = sigar_cpu_get(g->sigar, &cpu);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_cpu_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
        Handle<Object> h_cpu = Object::New();
        h_cpu->Set(String::NewSymbol("user"), Number::New(cpu.user));
        h_cpu->Set(String::NewSymbol("sys"), Number::New(cpu.sys));
        h_cpu->Set(String::NewSymbol("nice"), Number::New(cpu.nice));
        h_cpu->Set(String::NewSymbol("idle"), Number::New(cpu.idle));
        h_cpu->Set(String::NewSymbol("wait"), Number::New(cpu.wait));
        h_cpu->Set(String::NewSymbol("irq"), Number::New(cpu.irq));
        h_cpu->Set(String::NewSymbol("soft_irq"), Number::New(cpu.soft_irq));
        h_cpu->Set(String::NewSymbol("stolen"), Number::New(cpu.stolen));
        h_cpu->Set(String::NewSymbol("total"), Number::New(cpu.total));
		return scope.Close(h_cpu);
	}
	static Handle<Value> SigarCpuListGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_cpu_list_t cpulist;
	    unsigned i;
	    int status = sigar_cpu_list_get(g->sigar, &cpulist);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_cpu_list_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
	    Handle<Array> h_cpus = Array::New(cpulist.number);
	    for (i=0; i<cpulist.number; i++) {
	        sigar_cpu_t cpu = cpulist.data[i];
	        Handle<Object> h_cpu = Object::New();
	        h_cpu->Set(String::NewSymbol("user"), Number::New(cpu.user));
	        h_cpu->Set(String::NewSymbol("sys"), Number::New(cpu.sys));
	        h_cpu->Set(String::NewSymbol("nice"), Number::New(cpu.nice));
	        h_cpu->Set(String::NewSymbol("idle"), Number::New(cpu.idle));
	        h_cpu->Set(String::NewSymbol("wait"), Number::New(cpu.wait));
	        h_cpu->Set(String::NewSymbol("irq"), Number::New(cpu.irq));
	        h_cpu->Set(String::NewSymbol("soft_irq"), Number::New(cpu.soft_irq));
	        h_cpu->Set(String::NewSymbol("stolen"), Number::New(cpu.stolen));
	        h_cpu->Set(String::NewSymbol("total"), Number::New(cpu.total));
	        h_cpus->Set(i, h_cpu);
	    }
	    sigar_cpu_list_destroy(g->sigar, &cpulist);
		return scope.Close(h_cpus);
	}
	static Handle<Value> SigarCpuInfoListGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_cpu_info_list_t cpuinfo;
	    unsigned i;
	    int status = sigar_cpu_info_list_get(g->sigar, &cpuinfo);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_cpu_list_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
	    Handle<Array> h_cpus = Array::New(cpuinfo.number);
	    for (i=0; i<cpuinfo.number; i++) {
	        sigar_cpu_info_t cpu = cpuinfo.data[i];
	        Handle<Object> h_cpu = Object::New();
	        h_cpu->Set(String::NewSymbol("vendor"), String::New(cpu.vendor));
	        h_cpu->Set(String::NewSymbol("model"), String::New(cpu.model));
	        h_cpu->Set(String::NewSymbol("mhz"), Number::New(cpu.mhz));
	        h_cpu->Set(String::NewSymbol("mhz_max"), Number::New(cpu.mhz_max));
	        h_cpu->Set(String::NewSymbol("mhz_min"), Number::New(cpu.mhz_min));
	        h_cpu->Set(String::NewSymbol("cache_size"), Number::New(cpu.cache_size));
	        h_cpu->Set(String::NewSymbol("total_sockets"), Number::New(cpu.total_sockets));
	        h_cpu->Set(String::NewSymbol("total_cores"), Number::New(cpu.total_cores));
	        h_cpu->Set(String::NewSymbol("cores_per_socket"), Number::New(cpu.cores_per_socket));
	        h_cpus->Set(i, h_cpu);
	    }
	    sigar_cpu_info_list_destroy(g->sigar, &cpuinfo);
		return scope.Close(h_cpus);
	}
	static Handle<Value> SigarUptimeGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_uptime_t uptime;
	    int status = sigar_uptime_get(g->sigar, &uptime);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_uptime_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
		return scope.Close(Number::New(uptime.uptime));
	}
	static Handle<Value> SigarLoadavgGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_loadavg_t loadavg;
	    int status = sigar_loadavg_get(g->sigar, &loadavg);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_uptime_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
	    Handle<Array> h_loadavgs = Array::New(3);
	    h_loadavgs->Set(0, Number::New(loadavg.loadavg[0]));
	    h_loadavgs->Set(1, Number::New(loadavg.loadavg[1]));
	    h_loadavgs->Set(2, Number::New(loadavg.loadavg[2]));
		return scope.Close(h_loadavgs);
	}
	static Handle<Value> SigarResourceLimitGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_resource_limit_t resourcelimit;
	    int status = sigar_resource_limit_get(g->sigar, &resourcelimit);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_cpu_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
        Handle<Object> h_resourcelimit= Object::New();
        h_resourcelimit->Set(String::NewSymbol("cpu_cur"), Number::New(resourcelimit.cpu_cur));
        h_resourcelimit->Set(String::NewSymbol("cpu_max"), Number::New(resourcelimit.cpu_max));
        h_resourcelimit->Set(String::NewSymbol("file_size_cur"), Number::New(resourcelimit.file_size_cur));
        h_resourcelimit->Set(String::NewSymbol("file_size_max"), Number::New(resourcelimit.file_size_max));
        h_resourcelimit->Set(String::NewSymbol("pipe_size_cur"), Number::New(resourcelimit.pipe_size_cur));
        h_resourcelimit->Set(String::NewSymbol("pipe_size_max"), Number::New(resourcelimit.pipe_size_max));
        h_resourcelimit->Set(String::NewSymbol("data_cur"), Number::New(resourcelimit.data_cur));
        h_resourcelimit->Set(String::NewSymbol("data_max"), Number::New(resourcelimit.data_max));
        h_resourcelimit->Set(String::NewSymbol("stack_cur"), Number::New(resourcelimit.stack_cur));
        h_resourcelimit->Set(String::NewSymbol("stack_max"), Number::New(resourcelimit.stack_max));
        h_resourcelimit->Set(String::NewSymbol("core_cur"), Number::New(resourcelimit.core_cur));
        h_resourcelimit->Set(String::NewSymbol("core_max"), Number::New(resourcelimit.core_max));
        h_resourcelimit->Set(String::NewSymbol("memory_cur"), Number::New(resourcelimit.memory_cur));
        h_resourcelimit->Set(String::NewSymbol("memory_max"), Number::New(resourcelimit.memory_max));
        h_resourcelimit->Set(String::NewSymbol("processes_cur"), Number::New(resourcelimit.processes_cur));
        h_resourcelimit->Set(String::NewSymbol("processes_max"), Number::New(resourcelimit.processes_max));
        h_resourcelimit->Set(String::NewSymbol("open_files_cur"), Number::New(resourcelimit.open_files_cur));
        h_resourcelimit->Set(String::NewSymbol("open_files_max"), Number::New(resourcelimit.open_files_max));
        h_resourcelimit->Set(String::NewSymbol("virtual_memory_cur"), Number::New(resourcelimit.virtual_memory_cur));
        h_resourcelimit->Set(String::NewSymbol("virtual_memory_max"), Number::New(resourcelimit.virtual_memory_max));
		return scope.Close(h_resourcelimit);
	}
	static Handle<Value> SigarProcListGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_list_t proclist;
	    unsigned i;
	    int status = sigar_proc_list_get(g->sigar, &proclist);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_list_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
	    Handle<Array> h_proclist = Array::New(proclist.number);
	    for (i=0; i<proclist.number; i++) {
	    	sigar_pid_t pid = proclist.data[i];
	    	h_proclist->Set(i, Number::New(pid));
	    }
	    sigar_proc_list_destroy(g->sigar, &proclist);
		return scope.Close(h_proclist);
	}
	static Handle<Value> SigarProcStatGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_stat_t procstat;
		int status = sigar_proc_stat_get(g->sigar, &procstat);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
        Handle<Object> h_procstat = Object::New();
        h_procstat->Set(String::NewSymbol("total"), Number::New(procstat.total));
        h_procstat->Set(String::NewSymbol("sleeping"), Number::New(procstat.sleeping));
        h_procstat->Set(String::NewSymbol("running"), Number::New(procstat.running));
        h_procstat->Set(String::NewSymbol("zombie"), Number::New(procstat.zombie));
        h_procstat->Set(String::NewSymbol("stopped"), Number::New(procstat.stopped));
        h_procstat->Set(String::NewSymbol("idle"), Number::New(procstat.idle));
        h_procstat->Set(String::NewSymbol("threads"), Number::New(procstat.threads));
		return scope.Close(h_procstat);
	}
	static Handle<Value> SigarProcMemGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_mem_t procmem;
	    int status = sigar_proc_mem_get(g->sigar, args[0]->IntegerValue(), &procmem);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
        Handle<Object> h_procmem = Object::New();
        h_procmem->Set(String::NewSymbol("size"), Number::New(procmem.size));
        h_procmem->Set(String::NewSymbol("resident"), Number::New(procmem.resident));
        h_procmem->Set(String::NewSymbol("share"), Number::New(procmem.share));
        h_procmem->Set(String::NewSymbol("minor_faults"), Number::New(procmem.minor_faults));
        h_procmem->Set(String::NewSymbol("major_faults"), Number::New(procmem.major_faults));
        h_procmem->Set(String::NewSymbol("page_faults"), Number::New(procmem.page_faults));
		return scope.Close(h_procmem);
	}
	static Handle<Value> SigarProcCredGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_cred_t proccred;
	    int status = sigar_proc_cred_get(g->sigar, args[0]->IntegerValue(), &proccred);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
        Handle<Object> h_proccred = Object::New();
        h_proccred->Set(String::NewSymbol("uid"), Number::New(proccred.uid));
        h_proccred->Set(String::NewSymbol("gid"), Number::New(proccred.gid));
        h_proccred->Set(String::NewSymbol("euid"), Number::New(proccred.euid));
        h_proccred->Set(String::NewSymbol("egid"), Number::New(proccred.egid));
		return scope.Close(h_proccred);
	}
	static Handle<Value> SigarProcTimeGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_time_t proctime;
	    int status = sigar_proc_time_get(g->sigar, args[0]->IntegerValue(), &proctime);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
        Handle<Object> h_proctime = Object::New();
        h_proctime->Set(String::NewSymbol("start_time"), Number::New(proctime.start_time));
        h_proctime->Set(String::NewSymbol("user"), Number::New(proctime.user));
        h_proctime->Set(String::NewSymbol("sys"), Number::New(proctime.sys));
        h_proctime->Set(String::NewSymbol("total"), Number::New(proctime.total));
		return scope.Close(h_proctime);
	}
	static Handle<Value> SigarProcCpuGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_cpu_t proccpu;
	    int status = sigar_proc_cpu_get(g->sigar, args[0]->IntegerValue(), &proccpu);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
        Handle<Object> h_proccpu = Object::New();
        h_proccpu->Set(String::NewSymbol("start_time"), Number::New(proccpu.start_time));
        h_proccpu->Set(String::NewSymbol("user"), Number::New(proccpu.user));
        h_proccpu->Set(String::NewSymbol("sys"), Number::New(proccpu.sys));
        h_proccpu->Set(String::NewSymbol("total"), Number::New(proccpu.total));
        h_proccpu->Set(String::NewSymbol("last_time"), Number::New(proccpu.last_time));
        h_proccpu->Set(String::NewSymbol("percent"), Number::New(proccpu.percent));
		return scope.Close(h_proccpu);
	}
	static Handle<Value> SigarProcStateGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_state_t procstate;
	    int status = sigar_proc_state_get(g->sigar, args[0]->IntegerValue(), &procstate);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_stat_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    };
        // TODO: state is probably not handled correctly,
	    // eg, moving the next 2 lines after h_procstate creation will add extra characters
        char state[0];
        state[0] = procstate.state;
        Handle<Object> h_procstate = Object::New();
        h_procstate->Set(String::NewSymbol("name"), String::New(procstate.name));
        h_procstate->Set(String::NewSymbol("state"), String::New(state));
        h_procstate->Set(String::NewSymbol("ppid"), Number::New(procstate.ppid));
        h_procstate->Set(String::NewSymbol("tty"), Number::New(procstate.tty));
        h_procstate->Set(String::NewSymbol("priority"), Number::New(procstate.priority));
        h_procstate->Set(String::NewSymbol("nice"), Number::New(procstate.nice));
        h_procstate->Set(String::NewSymbol("processor"), Number::New(procstate.processor));
        h_procstate->Set(String::NewSymbol("threads"), Number::New(procstate.threads));
		return scope.Close(h_procstate);
	}
	static Handle<Value> SigarProcArgsGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_args_t procargs;
	    unsigned i;
	    int status = sigar_proc_args_get(g->sigar, args[0]->IntegerValue(), &procargs);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_args_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
	    Handle<Array> h_procargs = Array::New(procargs.number);
	    for (i=0; i<procargs.number; i++) {
	    	char* arg = procargs.data[i];
	    	h_procargs->Set(i, String::New(arg));
	    }
	    sigar_proc_args_destroy(g->sigar, &procargs);
		return scope.Close(h_procargs);
	}
	static int SigarProcEnvGet_getter(void *data,
	                               const char *key, int klen,
	                               char *val, int vlen)
	{
		Handle<Object> d = *((Handle<Object>*)data);
		d->Set(String::New(key, klen), String::New(val, vlen));
	    return SIGAR_OK;
	}
	static Handle<Value> SigarProcEnvGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_env_t procenv;
		procenv.type = procenv.SIGAR_PROC_ENV_ALL;
		procenv.env_getter = NodeSigar::SigarProcEnvGet_getter;
		Handle<Object> data = Object::New();
		procenv.data = &data;
	    int status = sigar_proc_env_get(g->sigar, args[0]->IntegerValue(), &procenv);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_env_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
		return scope.Close(data);
	}
	static Handle<Value> SigarProcFdGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_fd_t procfd;
	    int status = sigar_proc_fd_get(g->sigar, args[0]->IntegerValue(), &procfd);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_fd_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
		// Note, return an object because only total but a comment
		// suggest they might add other properties
		Handle<Object> h_procfd = Object::New();
		h_procfd->Set(String::NewSymbol("total"), Number::New(procfd.total));
		return scope.Close(h_procfd);
	}
	static Handle<Value> SigarProcExeGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_exe_t procexe;
	    int status = sigar_proc_exe_get(g->sigar, args[0]->IntegerValue(), &procexe);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_exe_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    };
        Handle<Object> h_procexe = Object::New();
        h_procexe->Set(String::NewSymbol("name"), String::New(procexe.name));
        h_procexe->Set(String::NewSymbol("cwd"), String::New(procexe.cwd));
        h_procexe->Set(String::NewSymbol("root"), String::New(procexe.root));
		return scope.Close(h_procexe);
	}
	static int SigarProcModulesGet_getter(void *data,
	                               char *key, int klen)
	{
		Handle<Array> d = *((Handle<Array>*)data);
		// Not sure if index arg is meant to work
		d->Set(d->Length(), String::New(key, klen));
	    return SIGAR_OK;
	}
	static Handle<Value> SigarProcModulesGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_proc_modules_t procmodule;
		procmodule.module_getter = NodeSigar::SigarProcModulesGet_getter;
		Handle<Array> data = Array::New();
		procmodule.data = &data;
		int status = sigar_proc_modules_get(g->sigar, args[0]->IntegerValue(), &procmodule);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_proc_module_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
		return scope.Close(data);
	}
	static Handle<Value> SigarThreadCpuGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_thread_cpu_t threadcpu;
	    int status = sigar_thread_cpu_get(g->sigar, args[0]->IntegerValue(), &threadcpu);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_thread_cpu_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    };
        Handle<Object> h_threadcpu = Object::New();
        h_threadcpu->Set(String::NewSymbol("user"), Number::New(threadcpu.user));
        h_threadcpu->Set(String::NewSymbol("sys"), Number::New(threadcpu.sys));
        h_threadcpu->Set(String::NewSymbol("total"), Number::New(threadcpu.total));
		return scope.Close(h_threadcpu);
	}
	static Handle<Value> SigarFileSystemListGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_file_system_list_t filesystemlist;
	    unsigned i;
	    int status = sigar_file_system_list_get(g->sigar, &filesystemlist);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_file_system_list_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
	    Handle<Array> h_filesystemlist = Array::New(filesystemlist.number);
	    for (i=0; i<filesystemlist.number; i++) {
	    	sigar_file_system_t filesystem = filesystemlist.data[i];
	        Handle<Object> h_filesystem = Object::New();
	    	h_filesystem->Set(String::NewSymbol("dir_name"), String::New(filesystem.dir_name));
	    	h_filesystem->Set(String::NewSymbol("dev_name"), String::New(filesystem.dev_name));
	    	h_filesystem->Set(String::NewSymbol("type_name"), String::New(filesystem.type_name));
	    	h_filesystem->Set(String::NewSymbol("sys_type_name"), String::New(filesystem.sys_type_name));
	    	h_filesystem->Set(String::NewSymbol("options"), String::New(filesystem.options));
	    	h_filesystem->Set(String::NewSymbol("type"), Number::New(filesystem.type));
	    	h_filesystem->Set(String::NewSymbol("flags"), Number::New(filesystem.flags));
	    	h_filesystemlist->Set(i, h_filesystem);
	    }
	    sigar_file_system_list_destroy(g->sigar, &filesystemlist);
		return scope.Close(h_filesystemlist);
	}
	static Handle<Value> SigarFileSystemUsageGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 1){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_file_system_usage_t filesystemusage;
	    String::Utf8Value dirname(args[0]->ToString());
	    int status = sigar_file_system_usage_get(g->sigar,  *dirname, &filesystemusage);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_file_system_usage_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    };
        Handle<Object> h_filesystemusage = Object::New();
        Handle<Object> h_diskusage= Object::New();
        sigar_disk_usage_t diskusage = filesystemusage.disk;
        h_diskusage->Set(String::NewSymbol("reads"), Number::New(diskusage.reads));
        h_diskusage->Set(String::NewSymbol("writes"), Number::New(diskusage.writes));
        h_diskusage->Set(String::NewSymbol("write_bytes"), Number::New(diskusage.write_bytes));
        h_diskusage->Set(String::NewSymbol("read_bytes"), Number::New(diskusage.read_bytes));
        h_diskusage->Set(String::NewSymbol("rtime"), Number::New(diskusage.rtime));
        h_diskusage->Set(String::NewSymbol("wtime"), Number::New(diskusage.wtime));
        h_diskusage->Set(String::NewSymbol("qtime"), Number::New(diskusage.qtime));
        h_diskusage->Set(String::NewSymbol("time"), Number::New(diskusage.time));
        h_diskusage->Set(String::NewSymbol("snaptime"), Number::New(diskusage.snaptime));
        h_diskusage->Set(String::NewSymbol("service_time"), Number::New(diskusage.service_time));
        h_diskusage->Set(String::NewSymbol("queue"), Number::New(diskusage.queue));
        h_filesystemusage->Set(String::NewSymbol("disk"), h_diskusage);
        h_filesystemusage->Set(String::NewSymbol("use_percent"), Number::New(filesystemusage.use_percent));
        h_filesystemusage->Set(String::NewSymbol("total"), Number::New(filesystemusage.total));
        h_filesystemusage->Set(String::NewSymbol("free"), Number::New(filesystemusage.free));
        h_filesystemusage->Set(String::NewSymbol("used"), Number::New(filesystemusage.used));
        h_filesystemusage->Set(String::NewSymbol("avail"), Number::New(filesystemusage.avail));
        h_filesystemusage->Set(String::NewSymbol("files"), Number::New(filesystemusage.files));
        h_filesystemusage->Set(String::NewSymbol("free_files"), Number::New(filesystemusage.free_files));
		return scope.Close(h_filesystemusage);
	}
	static Handle<Value> SigarFileSystemPingGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_file_system_t filesystem;
	    int status = sigar_file_system_ping(g->sigar, &filesystem);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_file_system_ping error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    };
        Handle<Object> h_filesystem = Object::New();
        //printf("TEST %s | %u \n", filesystem.dir_name, filesystem.dev_name);
    	h_filesystem->Set(String::NewSymbol("dir_name"), String::New(filesystem.dir_name));
    	h_filesystem->Set(String::NewSymbol("dev_name"), String::New(filesystem.dev_name));
    	h_filesystem->Set(String::NewSymbol("type_name"), String::New(filesystem.type_name));
    	h_filesystem->Set(String::NewSymbol("sys_type_name"), String::New(filesystem.sys_type_name));
    	h_filesystem->Set(String::NewSymbol("options"), String::New(filesystem.options));
    	h_filesystem->Set(String::NewSymbol("type"), Number::New(filesystem.type));
    	h_filesystem->Set(String::NewSymbol("flags"), Number::New(filesystem.flags));
		return scope.Close(h_filesystem);
	}
	static Handle<Value> SigarNetInfoGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_net_info_t netinfo;
	    int status = sigar_net_info_get(g->sigar, &netinfo);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_file_system_ping error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    };
        Handle<Object> h_netinfo = Object::New();
    	h_netinfo->Set(String::NewSymbol("default_gateway"), String::New(netinfo.default_gateway));
    	h_netinfo->Set(String::NewSymbol("default_gateway_interface"), String::New(netinfo.default_gateway_interface));
    	h_netinfo->Set(String::NewSymbol("host_name"), String::New(netinfo.host_name));
    	h_netinfo->Set(String::NewSymbol("domain_name"), String::New(netinfo.domain_name));
    	h_netinfo->Set(String::NewSymbol("primary_dns"), String::New(netinfo.primary_dns));
    	h_netinfo->Set(String::NewSymbol("secondary_dns"), String::New(netinfo.secondary_dns));
		return scope.Close(h_netinfo);
	}
	static Handle<String> nodesigar_net_address_to_string(sigar_net_address_t *address)
	{
	    char addr_str[SIGAR_INET6_ADDRSTRLEN];
	    sigar_net_address_to_string(NULL, address, addr_str);
	    return String::New(addr_str);
	}
	static Handle<Value> SigarNetRouteListGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_net_route_list_t netroutelist;
	    unsigned i;
	    int status = sigar_net_route_list_get(g->sigar, &netroutelist);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_net_route_list_get error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    }
	    Handle<Array> h_netroutelist = Array::New(netroutelist.number);
	    for (i=0; i<netroutelist.number; i++) {
	    	sigar_net_route_t netroute = netroutelist.data[i];
	        Handle<Object> h_netroute = Object::New();

	        //char* h_destination;
//	        char h_destination[SIGAR_INET6_ADDRSTRLEN];
//	        sigar_net_address_t destination = netroute.destination;
//	        sigar_net_address_to_string(NULL, &destination, h_destination);
//	        h_netroute->Set(String::NewSymbol("destination"), String::New(h_destination));

//			sigar_net_address_t destination = netroute.destination;
//	        Handle<String> toto = nodesigar_net_address_to_string(&destination);
//	        h_netroute->Set(String::NewSymbol("destination"), toto);

	        sigar_net_address_t destination = netroute.destination;
	        Handle<Object> h_destination = Object::New();
	        h_destination->Set(String::NewSymbol("family"), Number::New(destination.family));
	        h_destination->Set(String::NewSymbol("in"), Number::New(destination.addr.in));
	        Handle<Array> h_destination_in6 = Array::New();
	        h_destination_in6->Set(0, Number::New(destination.addr.in6[0]));
	        h_destination_in6->Set(1, Number::New(destination.addr.in6[1]));
	        h_destination_in6->Set(2, Number::New(destination.addr.in6[2]));
	        h_destination_in6->Set(3, Number::New(destination.addr.in6[3]));
	        h_destination->Set(String::NewSymbol("in6"), h_destination_in6);
	        h_netroute->Set(String::NewSymbol("destination"), h_destination);

//	        char* gateway;
//			sigar_net_address_to_string(g->sigar, &netroute.gateway, gateway);
//	        h_netroute->Set(String::NewSymbol("gateway"), String::New(gateway));

	        sigar_net_address_t gateway = netroute.gateway;
	        Handle<Object> h_gateway = Object::New();
	        h_gateway->Set(String::NewSymbol("family"), Number::New(gateway.family));
	        h_gateway->Set(String::NewSymbol("in"), Number::New(gateway.addr.in));
	        Handle<Array> h_gateway_in6 = Array::New();
	        h_gateway_in6->Set(0, Number::New(gateway.addr.in6[0]));
	        h_gateway_in6->Set(1, Number::New(gateway.addr.in6[1]));
	        h_gateway_in6->Set(2, Number::New(gateway.addr.in6[2]));
	        h_gateway_in6->Set(3, Number::New(gateway.addr.in6[3]));
	        h_gateway->Set(String::NewSymbol("in6"), h_gateway_in6);
	        h_netroute->Set(String::NewSymbol("gateway"), h_gateway);

//	        char* mask;
//			sigar_net_address_to_string(g->sigar, &netroute.mask, mask);
//	        h_netroute->Set(String::NewSymbol("mask"), String::New(mask));

	        sigar_net_address_t mask = netroute.mask;
	        Handle<Object> h_mask = Object::New();
	        h_mask->Set(String::NewSymbol("family"), Number::New(mask.family));
	        h_mask->Set(String::NewSymbol("in"), Number::New(mask.addr.in));
	        Handle<Array> h_mask_in6= Array::New();
	        h_mask_in6->Set(0, Number::New(mask.addr.in6[0]));
	        h_mask_in6->Set(1, Number::New(mask.addr.in6[1]));
	        h_mask_in6->Set(2, Number::New(mask.addr.in6[2]));
	        h_mask_in6->Set(3, Number::New(mask.addr.in6[3]));
	        h_mask->Set(String::NewSymbol("in6"), h_mask_in6);
	        h_netroute->Set(String::NewSymbol("mask"), h_mask);

	        h_netroute->Set(String::NewSymbol("flags"), Number::New(netroute.flags));
	        h_netroute->Set(String::NewSymbol("refcnt"), Number::New(netroute.refcnt));
	        h_netroute->Set(String::NewSymbol("use"), Number::New(netroute.use));
	        h_netroute->Set(String::NewSymbol("metric"), Number::New(netroute.metric));
	        h_netroute->Set(String::NewSymbol("mtu"), Number::New(netroute.mtu));
	        h_netroute->Set(String::NewSymbol("window"), Number::New(netroute.window));
	        h_netroute->Set(String::NewSymbol("irtt"), Number::New(netroute.irtt));
	        h_netroute->Set(String::NewSymbol("ifname"), String::New(netroute.ifname));
	        h_netroutelist->Set(i, h_netroute);
	    }
	    sigar_net_route_list_destroy(g->sigar, &netroutelist);
		return scope.Close(h_netroutelist);
	}
	static Handle<Value> SigarNetInterfaceConfigGet(const Arguments& args)
	{
		HandleScope scope;
		if(args.Length() != 0){
			return ThrowException(Exception::TypeError(String::New("Invalid argument")));
		}
		NodeSigar* g = ObjectWrap::Unwrap<NodeSigar>(args.This());
		sigar_net_interface_config_t netinterface;
	    String::Utf8Value name(args[0]->ToString());
	    int status = sigar_net_interface_config_get(g->sigar, *name, &netinterface);
	    if (status != SIGAR_OK) {
	        char msg[200];
	        sprintf(msg, "sigar_file_system_ping error: %d (%s)", status, sigar_strerror(g->sigar, status));
			return ThrowException(Exception::TypeError(String::New(msg)));
	    };
        Handle<Object> h_netinterface = Object::New();
        h_netinterface->Set(String::NewSymbol("name"), String::New(netinterface.name));
        h_netinterface->Set(String::NewSymbol("type"), String::New(netinterface.type));
        h_netinterface->Set(String::NewSymbol("description"), String::New(netinterface.description));

        char* hwaddr;
		sigar_net_address_to_string(g->sigar, &netinterface.hwaddr, hwaddr);
		h_netinterface->Set(String::NewSymbol("hwaddr"), String::New(hwaddr));
        char* address;
		sigar_net_address_to_string(g->sigar, &netinterface.address, address);
		h_netinterface->Set(String::NewSymbol("address"), String::New(address));
        char* destination;
		sigar_net_address_to_string(g->sigar, &netinterface.destination, destination);
		h_netinterface->Set(String::NewSymbol("destination"), String::New(destination));
        char* broadcast;
		sigar_net_address_to_string(g->sigar, &netinterface.broadcast, broadcast);
		h_netinterface->Set(String::NewSymbol("broadcast"), String::New(broadcast));
        char* address6;
		sigar_net_address_to_string(g->sigar, &netinterface.address6, address6);
		h_netinterface->Set(String::NewSymbol("address6"), String::New(address6));

//    	h_netinterface->Set(String::NewSymbol("hwaddr"), NodeSigar::MapSigarNetAddress(netinterface.hwaddr));
//    	h_netinterface->Set(String::NewSymbol("address"), NodeSigar::MapSigarNetAddress(netinterface.address));
//    	h_netinterface->Set(String::NewSymbol("destination"), NodeSigar::MapSigarNetAddress(netinterface.destination));
//    	h_netinterface->Set(String::NewSymbol("broadcast"), NodeSigar::MapSigarNetAddress(netinterface.broadcast));
//    	h_netinterface->Set(String::NewSymbol("netmask"), NodeSigar::MapSigarNetAddress(netinterface.netmask));
//    	h_netinterface->Set(String::NewSymbol("address6"), NodeSigar::MapSigarNetAddress(netinterface.address6));
        h_netinterface->Set(String::NewSymbol("prefix6_length"), Number::New(netinterface.prefix6_length));
        h_netinterface->Set(String::NewSymbol("scope6"), Number::New(netinterface.scope6));
        h_netinterface->Set(String::NewSymbol("flags"), Number::New(netinterface.flags));
        h_netinterface->Set(String::NewSymbol("mtu"), Number::New(netinterface.mtu));
        h_netinterface->Set(String::NewSymbol("metric"), Number::New(netinterface.metric));
        h_netinterface->Set(String::NewSymbol("tx_queue_len"), Number::New(netinterface.tx_queue_len));
		return scope.Close(h_netinterface);
	}
};

extern "C" void
init(v8::Handle<v8::Object> target)
{
    v8::HandleScope scope;
    NodeSigar::Initialize(target);
}
