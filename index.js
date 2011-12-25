
var sigar = new require('./build/Release/sigar');

module.exports = function(){
	return new sigar.NodeSigar;
}
for(var k in sigar){
	if(k !== 'NodeSigar'){
		module.exports[k] = sigar[k];
	}
}