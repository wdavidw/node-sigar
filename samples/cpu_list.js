var sigar = require('sigar');

var cpus = sigar().cpuList()
for(var i = 0; i < cpus.length; i++){
    console.log(cpus[i]);
}
