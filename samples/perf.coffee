
sigar = require '..'

l = 1*1000*1000

###
i = 0
time = (new Date).getTime()
while i++ < l
	sigar()
console.log Math.round(l / (Date.now() - time) * 1000), ' per seconds'
###

i = 0
s = sigar()
time = (new Date).getTime()
while i++ < l
	s.swap()
console.log Math.round(l / (Date.now() - time) * 1000), ' per seconds'

i = 0
s = sigar()
time = (new Date).getTime()
while i++ < l
	s.cpu()
console.log Math.round(l / (Date.now() - time) * 1000), ' per seconds'

i = 0
s = sigar()
time = (new Date).getTime()
while i++ < l
	s.cpuList()
console.log Math.round(l / (Date.now() - time) * 1000), ' per seconds'

i = 0
s = sigar()
time = (new Date).getTime()
while i++ < l
	s.cpuInfoList()
console.log Math.round(l / (Date.now() - time) * 1000), ' per seconds'

i = 0
s = sigar()
time = (new Date).getTime()
while i++ < l
	s.loadavg()
console.log Math.round(l / (Date.now() - time) * 1000), ' per seconds'

i = 0
s = sigar()
time = (new Date).getTime()
while i++ < l
	s.resourceLimit()
console.log Math.round(l / (Date.now() - time) * 1000), ' per seconds'

i = 0
s = sigar()
time = (new Date).getTime()
while i++ < l
	s.procList()
console.log Math.round(l / (Date.now() - time) * 1000), ' per seconds'

i = 0
s = sigar()
time = (new Date).getTime()
while i++ < l
	s.procStat()
console.log Math.round(l / (Date.now() - time) * 1000), ' per seconds'
