## Loading Kernel Module (linux)
Update Makefile to point to {module}.o

## Loading Kernel Module (linux)
sudo insmod {module}.ko

## Removing Kernel Module
sudo rmmod {module}.ko

## Read Active Modules
lsmod (/proc/modules)

## Reading Kernel Log Buffer
dmesg (tail /var/log/syslog | grep kernel)

## Clearing Kernel Log Buffer
sudo dmesg -c
