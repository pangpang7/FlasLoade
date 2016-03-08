SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\Program Files (x86)\Intel\iCLS Client\;C:\Program Files\Intel\iCLS Client\;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\DAL;C:\Program Files (x86)\Intel\Intel(R) Management Engine Components\IPT;C:\Program Files (x86)\UltraEdit
SET CPU_TYPE=ARMCM0
SET CPU_VENDOR=ARM
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\test.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\startup_nuc122._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\example.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\example.axf" --i32combined --output ".\example.hex"
fromelf --vhx --32x1 -o .\example.bin .\example.axf
