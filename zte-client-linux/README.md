# gxgk.bbs.zte.linux
## English Introduction
### Use under general *Linux* distrubutions
Just `make` (`gcc` as `cc` while `g++` as `cxx` by default).
### Use under *OpenWRT*
You may need a crossing-platform compiler to create corresponding link files. Recommanded means are as follows:
#### For Alcatel-Lucent Shanghai Bell Residential Gateway 100A (RG100A)
It is recommanded to use `opwnwrt-gcc-rg100a` as `cc`, `openwrt-g++-rg100a` as `cxx` and then execute `make PF=rg100a`
#### For Huawei HG255d
It is recommanded to use `opwnwrt-gcc-hg255d` as `cc`, `openwrt-g++-hg255d` as `cxx` and then execute `make PF=hg255d`
