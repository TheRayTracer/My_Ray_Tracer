my_ray_tracer.exe -input ..\scenes\reflective_teapot.txt           -size 512 512 -output ..\results\reflective_teapot_no_bounce.bmp -shadows -bounces 0 -weight 0.01

my_ray_tracer.exe -input ..\scenes\reflective_teapot.txt           -size 512 512 -output ..\results\reflective_teapot.bmp           -shadows -bounces 3 -weight 0.01
my_ray_tracer.exe -input ..\scenes\reflective_teapot_and_floor.txt -size 512 512 -output ..\results\reflective_teapot_and_floor.bmp -shadows -bounces 3 -weight 0.01
my_ray_tracer.exe -input ..\scenes\glossy_teapot.txt               -size 512 512 -output ..\results\glossy_teapot.bmp               -shadows -bounces 3 -weight 0.01 -samples 128
my_ray_tracer.exe -input ..\scenes\glossy_teapot_and_floor.txt     -size 512 512 -output ..\results\glossy_teapot_and_floor.bmp     -shadows -bounces 1 -weight 0.01 -samples 128

my_ray_tracer.exe -input ..\scenes\teapot_on_wood_floor.txt        -size 512 512 -output ..\results\teapot_on_wood_floor.bmp        -shadows -bounces 1 -weight 0.01

pause