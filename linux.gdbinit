set disassembly-flavor intel
add-symbol-file util.so -o 0x7DB000000000
add-symbol-file base.so -o 0x7DB100000000
add-symbol-file renderer_opengl.so -o 0x7DB200000000
add-symbol-file wayland.so -o 0x7DB300000000
