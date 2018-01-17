::set PATH=%PATH%;C:\Program Files (x86)\Freescale\eclipse\plugins\com.pemicro.debug.gdbjtag.pne.expansion_3.2.4.201710241511\win32
set PATH=%PATH%;C:\Program Files (x86)\Freescale\eclipse\plugins\com.pemicro.debug.gdbjtag.pne_3.2.4.201710241511\win32
set PATH=%PATH%;C:\Program Files (x86)\Freescale\Toolchain\bin

start arm-none-eabi-gdb.exe --command "gdbCommandFile.txt"
::%1
::"gdbCommandFile.txt"

TIMEOUT 3

start pegdbserver_console.exe -device=NXP_KL2x_KL25Z128M4 -startserver -serverport=7224 -singlesession -interface=OPENSDA



exit