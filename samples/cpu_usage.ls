
Sigar = require '../build/Release/sigar' .init!
printf = require 'printf'

# replication of `sigar_cpu_perc_calculate`
# please refer to `sigar_format.c` for more info

prev = Sigar.cpu!
setInterval ->
	curr = Sigar.cpu!
	diff_user = curr.user - prev.user
	diff_sys  = curr.sys  - prev.sys
	diff_nice = curr.nice - prev.nice
	diff_idle = curr.idle - prev.idle
	diff_wait = curr.wait - prev.wait
	diff_irq = curr.irq - prev.irq
	diff_soft_irq = curr.soft_irq - prev.soft_irq
	diff_stolen = curr.stolen - prev.stolen

	diff_user = if diff_user < 0 then 0 else diff_user
	diff_sys  = if diff_sys  < 0 then 0 else diff_sys
	diff_nice = if diff_nice < 0 then 0 else diff_nice
	diff_idle = if diff_idle < 0 then 0 else diff_idle
	diff_wait = if diff_wait < 0 then 0 else diff_wait
	diff_irq = if diff_irq < 0 then 0 else diff_irq
	diff_soft_irq = if diff_soft_irq < 0 then 0 else diff_soft_irq
	diff_stolen = if diff_stolen < 0 then 0 else diff_stolen

	diff_total =
		diff_user + diff_sys + diff_nice + diff_idle +
		diff_wait + diff_irq + diff_soft_irq +
		diff_stolen

	perc = {
		user: diff_user / diff_total
		sys: diff_sys / diff_total
		nice: diff_nice / diff_total
		idle: diff_idle / diff_total
		wait: diff_wait / diff_total
		irq: diff_irq / diff_total
		soft_irq: diff_soft_irq / diff_total
		stolen: diff_stolen / diff_total
	}
	perc.combined = perc.user + perc.sys + perc.nice + perc.wait
	console.log printf "user: %0.2f sys: %0.2f nice: %0.2f idle: %0.2f", perc.user, perc.sys, perc.nice, perc.idle
	prev := curr
, 1000
